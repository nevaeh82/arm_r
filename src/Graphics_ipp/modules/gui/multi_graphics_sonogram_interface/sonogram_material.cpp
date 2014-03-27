#include "../../../stdafx.h"
#include "sonogram_material.h"
#include "../../../modules\gui\multi_graphics\mg_fastlab_style.h"//цветовой стиль по умолчанию
#include "../../../modules/log/log.h"
#ifdef OMEGA
#include "third_party/crashrpt/include/CrashRpt.h"
#endif // OMEGA
#include <QPainter>
#include <QColor>
#include <qt_windows.h>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <tbb\tbb.h>


//#define PROFILE_SONOGRAM_RENDERING

#ifdef PROFILE_SONOGRAM_RENDERING
#include "ittnotify.h"
#pragma comment(lib, "libittnotify.lib")
#else
#define __itt_pause();
#define __itt_resume();
#endif

#define VERY_SMALL_NUMBER 0.000000001;//9 знаков
const int Sonogram_AntiScarPixelX = 2;

Q_DECLARE_METATYPE(FFTLine_32f);
Q_DECLARE_METATYPE(FFTLine);

SonogramMaterial::SonogramMaterial(SonogramGraphicsRenderThread* renderThread,QGraphicsItem *parent) : Q_MG_Material(parent,100,100,QImage::Format_Indexed8,true,renderThread)
{
	qRegisterMetaType<FFTLine_32f>();
	qRegisterMetaType<FFTLine>();
	GlobalCache.setMaxCost(3000);
	key_kf_x_ = 1;
	key_kf_y_ = 1;
	sonogram_step_ = 32768;

    externalbuf = NULL;
	preBuildIm = NULL;
	pDst = NULL;
	
	prev_preBuildImgSZ = 0;
	prev_externalBufSZ = 0;
	prev_pDstSz = 0;

	PaletteGlobalKf = 0;
	PaletteGlobalMaximum = 0;
	PaletteGlobalMinimum = 0;	

	calculatePallateThread = NULL;
	LoadColorTableFromFile(":/Q_MultiGraphics/Resources/default.p8bit");
	prevKfY_ = 0;

}

SonogramMaterial::~SonogramMaterial()
{
    delete RenderThread;
	delete calculatePallateThread;
	_aligned_free(preBuildIm);
	ippFree(pDst);
	ippFree(externalbuf);
}

void SonogramMaterial::ExtMaterialReInit()
{
	if (prevKfY_ != kf.y())
	{
		QMutexLocker mlock(&ElementsSpectrumCache_mutex_);
		ElementsSpectrumCache_mean_.clear();
		ElementsSpectrumCache_max_.clear();

		previusSended_YStart_ = -777;
		previusSended_YEnd_ = -777;
		prevKfY_  = kf.y();
	}	
}

void SonogramMaterial::SelfClear()
{
	QMutexLocker mlock(&ElementsSpectrumCache_mutex_);
	ElementsSpectrumCache_mean_.clear();
	ElementsSpectrumCache_max_.clear();
}


bool SonogramMaterial::PrepareForViewPortChange( qreal key_kf_x, qreal key_kf_y,double StartPixelX, double EndPixelX, double StartPixelY,double EndPixelY )
{
	if ((PaletteGlobalKf == 0/*палитра еще считается*/) || (dataFile_ == NULL) || (dataFile_->SampleCount()-sonogram_step_ < 10)) return false;

	elementYStartByCurrentScreen_ = StartPixelY/ElementH;
	elementYEndByCurrentScreen_ =  (EndPixelY/ElementH)+1;
	
	key_kf_x_                   = ((sonogram_step_)/MaterialSize.width())+VERY_SMALL_NUMBER;
	key_kf_y_                   = ((total_lines_/MaterialSize.height()))+VERY_SMALL_NUMBER;
	round_key_kf_               = qRound64(key_kf_y_);

	return true;
}

bool SonogramMaterial::ExtViewPortChange( Q_MG_MaterialElement* NewEl,unsigned __int64 elXpos,unsigned __int64 elYpos )
{
	return true;
}

void SonogramMaterial::ExtAfterViewPortChange()
{
	calcMaxMeanSpectrumsAndSendSignal(true);
}


bool SonogramMaterial::calcMaxMeanSpectrumsAndSendSignal( bool full_screen_calc )
{
	if (full_screen_calc)
	{
		if ((!ElementsSpectrumCache_mean_.contains(elementYStartByCurrentScreen_)) || (!ElementsSpectrumCache_mean_.contains(elementYEndByCurrentScreen_-1))) return false;
		if ((!ElementsSpectrumCache_max_.contains(elementYStartByCurrentScreen_)) || (!ElementsSpectrumCache_max_.contains(elementYEndByCurrentScreen_-1))) return false;
		if ((elementYStartByCurrentScreen_ == previusSended_YStart_) && (elementYEndByCurrentScreen_ == previusSended_YEnd_)) return true;
		previusSended_YStart_ = elementYStartByCurrentScreen_;
		previusSended_YEnd_ = elementYEndByCurrentScreen_;
	}
		

	//for (int a = 0;a < ElementsSpectrumCache_mean_.count();a++)	qDebug() << "AfterViewPortChange: Mean ElY="<<ElementsSpectrumCache_mean_.keys()[a];
	//for (int a = 0;a < ElementsSpectrumCache_max_.count();a++)	qDebug() << "AfterViewPortChange: Max  ElY="<<ElementsSpectrumCache_max_.keys()[a];

	FFTLine_32f TotalMean (new Storage<Ipp32f>(sonogram_step_));
	TotalMean->WriteComplete((size_t)sonogram_step_);
	memset(*TotalMean,0,TotalMean->AvailReadBytes());

	FFTLine TotalMax (new Storage<Ipp16s>(sonogram_step_));
	TotalMax->WriteComplete((size_t)sonogram_step_);
	ippsSet_16s(-250,*TotalMax,sonogram_step_);
	bool not_emit_signal = true;//на тот случай когда ну ваще нихрена не добавилось и нулевой ненадо отсылать спектр

	{//QMutexLocker 
		QMutexLocker mlock(&ElementsSpectrumCache_mutex_);
		Ipp32f div_param = 0;
		for (int i = elementYStartByCurrentScreen_;i < elementYEndByCurrentScreen_;i++ )
		{
			//среднее
			FFTLine_32f teml_line = ElementsSpectrumCache_mean_.value(i);
			if (teml_line != NULL)
			{
				div_param++;
				ippsAdd_32f_I(*teml_line,	*TotalMean, sonogram_step_);
				not_emit_signal = false;
			}
			else
			{
				if (full_screen_calc)
				{
					qDebug() << " !!!!!! WARNING !!!!!!  SonogramMaterial::ExtAfterViewPortChange() teml_line == NULL, нет кэшериванных данных для рассчета среднего спектра по элементу" << i; 
				}
				break;
			}
						
			//максимальное
			FFTLine teml_line2 = ElementsSpectrumCache_max_.value(i);
			if (teml_line2 != NULL ) 
			{
				ippsMaxEvery_16s_I(*teml_line2,*TotalMax,sonogram_step_);
				not_emit_signal = false;
			}
			else 
			{
				if (full_screen_calc)
				{
					qDebug() << " !!!!!! WARNING !!!!!!  SonogramMaterial::ExtAfterViewPortChange() teml_line == NULL, нет кэшериванных данных для рассчета максимального спектра по элементу" << i; 
				}
				break;
			}
		}//for (int i = elementYStartByCurrentScreen_;i < elementYEndByCurrentScreen_;i++ )

		if (div_param > 1) ippsDivC_32f_I(div_param,*TotalMean,sonogram_step_);
	}//QMutexLocker 

	if (!not_emit_signal) emit ScreenSpectrumsChanged(TotalMean,TotalMax);
	return true;
}

bool SonogramMaterial::setData( const FileType& dataFile, int fftOrder, QByteArray color_level_data, bool updateAfterSet, SIZE_T& MaximumRamOfSystem )
{
    LSCOPE("SonogramMaterial");

	PaletteGlobalKf = 0;//сразу поставили палитру в 0
	if ((fftOrder < 10) || (fftOrder >= 20)) return false;
	if (!dataFile) return false;
	fftOrder_ = fftOrder;

	unsigned __int64 items_count = dataFile->SampleCount();
	
	unsigned int sonogram_step = 1 << fftOrder;

	if (items_count-sonogram_step <= 10) return false;//меньше 10 линий то выход
	
	dataFile_ = dataFile;
	
	FirstPaint = updateAfterSet;
	NeedMaterialUpdateAfterMoov = true;

	if ((sonogram_step != sonogram_step_))	sonogram_step_ = sonogram_step;

	total_lines_ = items_count-sonogram_step;

	GlobalCache.clear();
	
	int optimalCachSize = 0;

	if (MaximumRamOfSystem == 0)
	{
		MEMORYSTATUS memstat;
		ZeroMemory(&memstat,sizeof(MEMORYSTATUS));
		memstat.dwLength = sizeof(MEMORYSTATUS);
		GlobalMemoryStatus(&memstat);
		LTRACE_S(QString("Amount of physical memory currently available: %1 MB").arg(memstat.dwAvailPhys/1e6, 0, 'f', 3));

		MaximumRamOfSystem = memstat.dwAvailPhys-(SIZE_T)(1024*1024*300);//300 МБ
		LTRACE_S(QString("Initial maximum RAM: %1 MB").arg(MaximumRamOfSystem/1e6, 0, 'f', 3));

#ifndef _WIN64
		MaximumRamOfSystem = static_cast<SIZE_T>(std::min(double(MaximumRamOfSystem),1.5*1024*1024*1024)/3.0);
#endif
	}
	
	optimalCachSize = MaximumRamOfSystem/(sonogram_step*6);
	LTRACE_S(QString("Optimal cache size: %1 pts.").arg(optimalCachSize));

	if (optimalCachSize < 1000)
    {
        LOG("Ошибка: недостаточно памяти для сонограммы");
        return false;
    }
	GlobalCache.setMaxCost(optimalCachSize);

	if (color_level_data.size() != 12) //2+2+8
	{
		//расчет палитры, прогоняем весь файл (части файла)
		CreateAndStartCalculatePaletteThread();
	}
	else
	{
		LoadColorLevelsFromData(color_level_data);
		ViewPortChange();
	}
	
	
	//update();
	return true;
}


void SonogramMaterial::LoadColorTableFromFile( QString Fname )
{
	colorTable.clear();
	colorTable.resize(256);
	colorTable.fill(0);

	if (Fname.isEmpty()) return;

	QFile fl(Fname);
	if (!fl.open(QIODevice::ReadOnly))
	{
		//emit sendCommand("mg_InfoWidget","AddText","ВНИМАНИЕ: Файл палитры ненайден");
		QMessageBox::critical(NULL,"Ошибка","Сонограмма: Файл палитры ненайден");
		return;
	}

	if (fl.read((char*)colorTable.data(),256*4) != 256*4) return;
	fl.close();
	return;
}

void SonogramMaterial::LoadColorLevelsFromData( QByteArray in_data )
{
	if (in_data.size() != 12) return;
	memcpy((char*)&PaletteGlobalMaximum,&in_data.constData()[0], sizeof(short));
	memcpy((char*)&PaletteGlobalMinimum,&in_data.constData()[2], sizeof(short));
	memcpy((char*)&PaletteGlobalKf,&in_data.constData()[4], sizeof(double));
}


INT64 SonogramMaterial::DrawCalculatedLineToPreBuildImage( INT64 StartPixelPerLine, INT64 EndPixelPerLine, Storage<Ipp16s> &store, int preBuildImW, int Y )
{
	if (EndPixelPerLine > sonogram_step_) { /*DebugBreak();*/	EndPixelPerLine = sonogram_step_;}

	__int64 cb = StartPixelPerLine;
	if (cb < 0) cb = 0;
	if (cb >= (__int64 )store.AvailRead()) cb  = (__int64 )store.AvailRead();
	__int64 ce = EndPixelPerLine;
	if (ce < 0) cb = 0;
	if (ce >= (__int64 )store.AvailRead()) ce  = (__int64 )store.AvailRead();

	if (cb-StartPixelPerLine < 0) cb = StartPixelPerLine;
	if (ce-StartPixelPerLine >= preBuildImW) ce = preBuildImW+StartPixelPerLine-1;

	const Ipp16s* data_ptr = store.const_data();  //  не надо вызывать data() в теле цикла, если хочется оптимизации - const_data()

	for (__int64 x = cb; x < ce; x++)
	{
		int realX = x-StartPixelPerLine;

		Ipp16s val = data_ptr[x];

		if (val < -240) _ASSERT(false);
		if (val > 240) _ASSERT(false);

 		uchar colorIndex = 0;
		if (val < PaletteGlobalMinimum)	colorIndex = 0;
		else 
			if (val > PaletteGlobalMaximum) colorIndex = 255;
			else
				colorIndex = qAbs(val-PaletteGlobalMinimum)*PaletteGlobalKf;

		*(uchar*)&preBuildIm[realX+(preBuildImW /*preBuildImBytePerLine*/ *Y)] = colorIndex;//(uchar)(val/1.88235f)+(uchar)128;
	}
	return EndPixelPerLine;
}


bool SonogramMaterial::getFFTInfoLists( Q_MG_MaterialElement* el,PresentFFTList& presentList, Storage<__int64>& missedFFT)
{
	presentList.presentFFT.clear();
	presentList.pixelFrom = 0;
	missedFFT.Clear();
	
	INT64 StartLine, EndLine;
	INT64 StartPixelPerLine, EndPixelPerLine;
	int preBuildImW, preBuildImH;
	double interleaver_step;
	if ( !calculateDrawVariables(el,StartLine,EndLine,StartPixelPerLine,EndPixelPerLine,preBuildImW,preBuildImH,interleaver_step) )
		return false;
	bool Enable_interleaver = (interleaver_step > 0);

	missedFFT.PrepareForWrite(preBuildImH);

	for (int i = 0; i < preBuildImH;i++)
	{
		__int64 Fpos;
		if (!InverseVAxis)	{	if (Enable_interleaver)	Fpos = StartLine+i*interleaver_step;else Fpos = StartLine+i;}
		else				{	if (Enable_interleaver)	Fpos = StartLine+(qAbs(EndLine-StartLine)-1)-i*interleaver_step;else Fpos = StartLine+(preBuildImH-1)-i;}

		if (Fpos < 0) {	presentList.pixelFrom++;continue;	}
		if (dataFile_ == NULL) return false;
		if (Fpos > dataFile_->SampleCount() - sonogram_step_) {	break;	}

		FFTLine* storePointer;
		{//QMutexLocker
			QMutexLocker locker(&GlobalCacheMutex);
			storePointer = GlobalCache[Fpos];
		}//QMutexLocker
		if (storePointer == NULL) {	missedFFT[missedFFT.AvailRead()] = Fpos; missedFFT.WriteComplete(1);	}
		else
		{
			FFTLineStruct str;
			str.FilePos = Fpos;
			str.FFT_ptr = *storePointer;
			presentList.presentFFT.append(str);
		}
	}
	return true;
}

bool SonogramMaterial::calculateDrawVariables( Q_MG_MaterialElement* el, INT64& StartLine, INT64& EndLine, INT64& StartPixelPerLine, INT64& EndPixelPerLine, int& preBuildImW, int& preBuildImH, double& interleaver_step )
{
	double ElCoordsX = el->elX*ElementW;
	double ElCoordsY = el->elY*ElementH;

	double Not_or_InverseDraw = 0;
	if (!InverseVAxis) Not_or_InverseDraw = (double)(ElCoordsY);
	else Not_or_InverseDraw = (MaterialSize.height()-(double)(ElCoordsY+ElementH));

	if (!InverseVAxis) {	if (Not_or_InverseDraw > MaterialSize.height())	return false;	}
	else {	if (Not_or_InverseDraw < -100) return false; }

	StartLine =  (Not_or_InverseDraw*key_kf_y_);
	EndLine =  (Not_or_InverseDraw*key_kf_y_)+(ElementH*key_kf_y_);
	if (StartLine > EndLine) {INT64 t = StartLine;StartLine = EndLine;EndLine = t;}
	StartLine -= Sonogram_AntiScarPixelX;
	EndLine += Sonogram_AntiScarPixelX;

	StartPixelPerLine = (ElCoordsX*key_kf_x_);
	EndPixelPerLine = ((ElCoordsX+ElementW)*key_kf_x_);
	if (StartPixelPerLine > EndPixelPerLine) {INT64 t = StartPixelPerLine;StartPixelPerLine = EndPixelPerLine;EndPixelPerLine = t;}
	StartPixelPerLine -= Sonogram_AntiScarPixelX;
	EndPixelPerLine += Sonogram_AntiScarPixelX;

	preBuildImH = qAbs(EndLine-StartLine);
	if (preBuildImH == 0) return false;
	preBuildImW = qAbs(EndPixelPerLine-StartPixelPerLine);
	if (preBuildImW == 0) return false;

	interleaver_step = 0;//не используется
	if (preBuildImH/ElementH > 5) //5FFT на пиксель
	{
		interleaver_step = (preBuildImH/ElementH)/2;//4 - interleaver value
		//qDebug() << "interleaver_step =" << interleaver_step;
		preBuildImH = preBuildImH/interleaver_step;
	}
	return true;
}


//#pragma optimize("", off)
bool SonogramMaterial::CalcParallelFFT( QVector< QPair<__int64,FFTLine*> >& out, Storage<__int64>& in, Q_MG_RenderThreadClass* breaker )
{
	//tbb::tick_count t0 = tbb::tick_count::now();

	QMutex Mtx;
	out.clear();
    out.resize(static_cast<int>(in.AvailRead()));
	QPair<__int64,FFTLine*> pp;
	pp.first = -1;pp.second = NULL;
	out.fill(pp);

	const int optimal_thread_count		= QThread::idealThreadCount();
	const int lines_per_one_iteration	= static_cast<int>(in.AvailRead()) / optimal_thread_count;
	const int last_lines				= static_cast<int>(in.AvailRead()) % optimal_thread_count;
	
	dsp::WindowPolicy<dsp::wtBlackman>  apply_window((size_t)sonogram_step_);
	bool returnFromTBB = false;

	out.detach();
    

	tbb::parallel_for( tbb::blocked_range<int>(0,optimal_thread_count), [&,breaker](const tbb::blocked_range<int>& r)
	{
		dsp::ComplexFastFourierTransform<>	fft(fftOrder_);
		dsp::MakeSpectrum<>                 make_spectrum(sonogram_step_);

		__int64 PrevSeekPos = -1;

		Storage<Ipp32fc>  file_data_buffer(sonogram_step_);
		Storage<Ipp32fc>  fft_source_buffer(sonogram_step_);
		Storage<Ipp32fc>  fft_result_buffer(sonogram_step_);
		fft_source_buffer.WriteComplete(static_cast<size_t>(sonogram_step_));


		//посути цикла не должно быть
		for(int thread_id = r.begin();thread_id < r.end();thread_id++)
		{
			const int end = (thread_id+1)*lines_per_one_iteration + ((thread_id == optimal_thread_count-1) ? last_lines : 0);

			for (int i = thread_id*lines_per_one_iteration;i < end; i++)
			{

				if ((breaker->thread_status() != 0) || (static_cast<size_t>(i) >= in.AvailRead())) return;

				__int64 Fpos = in[i];

				FFTLine* store = new FFTLine(new Storage<Ipp16s>(sonogram_step_));

				fft_result_buffer.Clear();

				if(PrevSeekPos >= 0)//не первый заход
				{
					if(Fpos-PrevSeekPos < 0u || Fpos-PrevSeekPos > static_cast<qint64>(file_data_buffer.AvailRead()))
						file_data_buffer.Clear();
					else
						file_data_buffer.RemoveFirst(static_cast<size_t>(Fpos-PrevSeekPos));
				}

				if(file_data_buffer.AvailRead() < sonogram_step_)
				{

					{
						QMutexLocker locker(&Mtx);
						if(!dataFile_->Seek(Fpos+file_data_buffer.AvailRead()))
						{
						    delete store;
							continue;
						}

						if(!dataFile_->Read(file_data_buffer.PrepareForWrite((size_t)sonogram_step_), sonogram_step_))
						{
						    delete store;
							continue;
						}
					}


					file_data_buffer.WriteComplete((size_t)sonogram_step_);
				}

				PrevSeekPos = Fpos;

				apply_window(file_data_buffer, fft_source_buffer);

				if (breaker->thread_status() != 0) {tbb::task::self().cancel_group_execution();delete store;return;}

				if (!(fft)( fft_source_buffer, fft_source_buffer.AvailRead(), fft_result_buffer.PrepareForWrite( (size_t)sonogram_step_ )) ) qDebug() << "fft = FALSE";
				fft_result_buffer.WriteComplete((size_t)sonogram_step_);

				make_spectrum(  fft_result_buffer, fft_result_buffer.AvailRead(), store->get()->PrepareForWrite((size_t)sonogram_step_),NULL);
				store->get()->WriteComplete((size_t)sonogram_step_);

				out[i].first = Fpos;
				out[i].second = store;

			}
		}

		if (breaker->thread_status() != 0) return;
		returnFromTBB = true;
	});
	
	//tbb::tick_count t1 = tbb::tick_count::now();
	//qDebug() << "calcFFT time=" << (t1-t0).seconds()*1000 << " msecs Count =" << in.AvailRead();
	return returnFromTBB;
}
//#pragma optimize("", on)

//#pragma optimize("", off)
bool SonogramMaterial::BuildImageByConveerLine( ConveerLine* val )
{
	//tbb::tick_count t0 = tbb::tick_count::now();

	//qDebug() << "RenderThread: PREPARE    X=" << NewEl->elX << "  Y=" << NewEl->elY;;

 	val->el->img->setColorTable(colorTable);

	INT64 StartLine, EndLine;
	INT64 StartPixelPerLine, EndPixelPerLine;
	int preBuildImW, preBuildImH;
	double interleaver_step;
	if ( !calculateDrawVariables(val->el,StartLine,EndLine,StartPixelPerLine,EndPixelPerLine,preBuildImW,preBuildImH,interleaver_step) ) return true;
	
	int preBuildImSz = preBuildImW*preBuildImH;
	if ( preBuildImSz != prev_preBuildImgSZ)
	{
		prev_preBuildImgSZ = preBuildImSz;
		_aligned_free(preBuildIm);
		preBuildIm = (char*)_aligned_malloc(preBuildImSz,32);		
		_ASSERT(preBuildIm);
	}
	memset(preBuildIm,0,preBuildImSz);

	const int optimal_thread_count		= QThread::idealThreadCount();
	const int lines_per_one_iteration	= preBuildImH / optimal_thread_count;
	const int last_lines				= preBuildImH % optimal_thread_count;

	val->compliteFFTList.presentFFT.detach();

	ElementsSpectrumCache_mutex_.lock();
	bool AlreadyHave_spectrums = ElementsSpectrumCache_mean_.contains(val->el->elY);
	ElementsSpectrumCache_mutex_.unlock();
	
	QMutex localByThread_mean_vector_mutrex;
	QVector<Storage<Ipp32f>*> localByThread_sum_vector;
	QVector<Storage<Ipp16s>*> localByThread_max_vector;

	FFTLine_32f el_spectrum_mean(new Storage<Ipp32f>(sonogram_step_));
	el_spectrum_mean->WriteComplete((size_t)sonogram_step_);
	memset(*el_spectrum_mean,0,el_spectrum_mean->AvailReadBytes());
	FFTLine el_spectrum_max(new Storage<Ipp16s>(sonogram_step_));
	el_spectrum_max->WriteComplete((size_t)sonogram_step_);
	ippsSet_16s(-250,*el_spectrum_max,sonogram_step_);
	
	
	
	tbb::parallel_for( tbb::blocked_range<int>(0,optimal_thread_count), [&](const tbb::blocked_range<int>& r)
	{
		//посути цикла не должно быть
		for(int thread_id = r.begin();thread_id < r.end();thread_id++)
		{
			const int end = (thread_id+1)*lines_per_one_iteration + ((thread_id == optimal_thread_count-1) ? last_lines : 0);

			Storage<Ipp32f>* localByThread_mean = NULL;
			Storage<Ipp16s>* localByThread_max = NULL;
			if (!AlreadyHave_spectrums)
			{
				localByThread_mean = new Storage<Ipp32f>(sonogram_step_);
				localByThread_mean->WriteComplete((size_t)sonogram_step_);
				memset(*localByThread_mean,0,localByThread_mean->AvailReadBytes());

				localByThread_max = new Storage<Ipp16s>(sonogram_step_);
				localByThread_max->WriteComplete((size_t)sonogram_step_);
				ippsSet_16s(-250,*localByThread_max,sonogram_step_);
				{
					QMutexLocker mlock(&localByThread_mean_vector_mutrex);
					localByThread_sum_vector.append(localByThread_mean);
					localByThread_max_vector.append(localByThread_max);
				}
			}
			
			
			for (int i = thread_id*lines_per_one_iteration;i < end; i++)
			{
				if (i < val->compliteFFTList.pixelFrom) continue;
				if (i-val->compliteFFTList.pixelFrom >= val->compliteFFTList.presentFFT.count()) break;
				
				FFTLine fftLine = val->compliteFFTList.presentFFT.at(i-val->compliteFFTList.pixelFrom).FFT_ptr;
				//"рисуем" полученную линию в preBuildIm
				EndPixelPerLine = DrawCalculatedLineToPreBuildImage(StartPixelPerLine, EndPixelPerLine, *fftLine, preBuildImW, i);
				
				if (!AlreadyHave_spectrums)
				{
					//рассчитываем сумма спектров для элемента
					Ipp32f* buff = new Ipp32f[sonogram_step_];
					ippsConvert_16s32f_Sfs(*fftLine,buff,sonogram_step_,0);
					ippsAdd_32f_I(buff,	*localByThread_mean, sonogram_step_);
					delete[] buff;
					
					//рассчитываем максимальный спектр
					ippsMaxEvery_16s_I(*fftLine,*localByThread_max,sonogram_step_);
				}
			}
		}//*/
	});//*/

	if (!AlreadyHave_spectrums)
	{
		for (int a = 0;a < localByThread_sum_vector.count();a++)
		{
			Storage<Ipp32f>* localByThread_mean = localByThread_sum_vector.at(a);
			ippsAdd_32f_I(*localByThread_mean, *el_spectrum_mean, sonogram_step_);
			delete localByThread_mean;
		}
		Ipp32f div_param = val->compliteFFTList.presentFFT.count() - val->compliteFFTList.pixelFrom;
		ippsDivC_32f_I(div_param,*el_spectrum_mean,sonogram_step_);

		for (int a = 0;a < localByThread_max_vector.count();a++)
		{
			Storage<Ipp16s>* localByThread_max = localByThread_max_vector.at(a);
			ippsMaxEvery_16s_I(*localByThread_max,*el_spectrum_max,sonogram_step_);
			delete localByThread_max;
		}


		{
			QMutexLocker mlock(&ElementsSpectrumCache_mutex_);
			ElementsSpectrumCache_mean_.insert(val->el->elY,el_spectrum_mean);
			ElementsSpectrumCache_max_.insert(val->el->elY,el_spectrum_max);
		}
		calcMaxMeanSpectrumsAndSendSignal(false);
	}
	
	
	/*QImage DebugImage(preBuildImW,preBuildImH,QImage::Format_Indexed8);
	DebugImage.setColorTable(colorTable);
	memcpy(DebugImage.bits(),preBuildIm,preBuildImSz);
	DebugImage.save("DebugPix\\DebugImage ElID_"+QString::number(val->el->ElementIndex_64)+" Y_"+QString::number(val->el->elY)+"  X_"+QString::number(val->el->elX)+".png");//*/

	double xFactor = 1.0/key_kf_x_;
	double yFactor = 1.0/key_kf_y_;
	if (xFactor == 0) xFactor = 1;
	DoubleIppiRect src_roi;
	int interpolation = IPPI_INTER_CUBIC2P_B05C03;
	
	src_roi.x = Sonogram_AntiScarPixelX;
	src_roi.width = preBuildImW-(Sonogram_AntiScarPixelX*2);

	src_roi.y = Sonogram_AntiScarPixelX;
	src_roi.height = preBuildImH-(Sonogram_AntiScarPixelX*2);
		
	IppiRect roundSrcRoi = {src_roi.x,src_roi.y,src_roi.width,src_roi.height};

	IppiSize src_sz;
	src_sz.width = preBuildImW;
	src_sz.height = preBuildImH;
	IppiRect dst_roi={0,0,ElementH,ElementW};

	xFactor = (double)ElementW/src_roi.width;
	yFactor = (double)ElementH/src_roi.height;
	if ((xFactor < 1) && (yFactor < 1)) interpolation = IPPI_INTER_SUPER;

	int external_bufSZ;
	ippiResizeGetBufSize( roundSrcRoi, dst_roi, 1, interpolation, &external_bufSZ );
	if (external_bufSZ != prev_externalBufSZ)
	{
		prev_externalBufSZ = external_bufSZ;
		ippsFree( externalbuf );
		externalbuf = ippsMalloc_8u(external_bufSZ);
	}
	
	int pDstSz = dst_roi.width*dst_roi.height;
	if (prev_pDstSz != pDstSz)
	{
		prev_pDstSz = pDstSz;
		ippsFree ( pDst );
		pDst = ippsMalloc_8u(pDstSz);
		_ASSERT(pDst);
	}

	double xShift = -((double)src_roi.x*xFactor);
	double yShift = -((double)src_roi.y*yFactor);
	ippiResizeSqrPixel_8u_C1R((Ipp8u*)preBuildIm, src_sz, src_sz.width, roundSrcRoi, pDst, dst_roi.width, dst_roi, xFactor, yFactor, xShift, yShift, interpolation,externalbuf);


	//копирование из внутреннего буфера во внешний Img
	memcpy(val->el->img->bits(),pDst,pDstSz);

	//tbb::tick_count t1 = tbb::tick_count::now();
	//qDebug() << "Render element time=" << (t1-t0).seconds()*1000 << "msec";
	return false;
}

void SonogramMaterial::CalcPallateProgressChanged( int perc )
{
	sendCommand("mg_InfoWidget","PrintText","Открытие файла, расчёт палитры");
	sendCommand("mg_InfoWidget","SetProgress",perc);
}

void SonogramMaterial::CalcPallateFinished( bool Ok )
{
	if (!Ok)
	{
		PaletteGlobalKf = 0;
		sendCommand("mg_InfoWidget","PrintText",tr("Ошибка при рассчете уровней сигнала (палитра не посчиталась)"));
		return;
	}
	sendCommand("mg_InfoWidget","SetProgress",100);
	sendCommand("mg_InfoWidget","PrintText","");
	ViewPortChange();
	
	QByteArray signalVal;
	signalVal.append((char*)&PaletteGlobalMaximum,sizeof(short));
	signalVal.append((char*)&PaletteGlobalMinimum,sizeof(short));
	signalVal.append((char*)&PaletteGlobalKf,sizeof(double));
	emit CalcPallateFinished_signal(signalVal);
}

void SonogramMaterial::CreateAndStartCalculatePaletteThread()
{
	PaletteGlobalKf = 0;
	delete calculatePallateThread;
	calculatePallateThread = new CalculatePallateThreadClass(this);
	calculatePallateThread->StartCalculate(this);
}




//#pragma optimize("", on)


//////////////////////////////////////////////////////////////////////////
//	SonogramGraphicsRenderThread
//////////////////////////////////////////////////////////////////////////


SonogramGraphicsRenderThread::SonogramGraphicsRenderThread() : Q_MG_RenderThreadClass()
{
	Put_To_Conveer_Queue_node = NULL;
	inConveerEl = 0;
	int ElementsPerLine = 10;
	optimalCountOnRenderSession_ = ElementsPerLine*2;
	total_in_on_this_list_ = 0;
	total_out_from_this_conveer_ = 0;
}

SonogramGraphicsRenderThread::~SonogramGraphicsRenderThread()
{
	FinishHim_wait();
}


void SonogramGraphicsRenderThread::addData( Storage<Q_MG_MaterialElement*>& in_list )
{
	//MakePauseAndDo([&]()
	//{
		if (in_list.AvailRead() == 0)//нехуй добавлять несмотря на изменение ViewPorta
		{
			Start_Wake();
			return;
		}
		
		

        QVector<Q_MG_MaterialElement*> vect(static_cast<int>(in_list.AvailRead()));
		memcpy(vect.data(),in_list.const_data(),in_list.AvailReadBytes());
	
		QMutexLocker mlock(&threadQueue_mutex_);	
		threadQueue_ << vect;
		total_in_on_this_list_ += vect.count();

		//сортировка элементов в порядке нахождения на экране
		std::sort(&threadQueue_.data()[0],&threadQueue_.data()[threadQueue_.count()],
			[&](const Q_MG_MaterialElement* a, const Q_MG_MaterialElement* b) -> bool
			{
				bool A = parentMaterial->ElementIsOnScreen(a);
				bool B = parentMaterial->ElementIsOnScreen(b);
				double cya = a->elY;
				double cyb = b->elY;
				if (A && B) return (cya < cyb);
				if (A) cya *= -1;
				if (B) cyb *= -1;
				return (cya < cyb);
			}
		);//*/

		debugOut("SonogramGraphicsRenderThread::addData "+QString::number(in_list.AvailRead()));
		Start_Wake();
	//});
}

void SonogramGraphicsRenderThread::ClearQueue()
{
	MakePauseAndDo([&]()
	{
		this->threadQueue_.clear();
	});
}



void SonogramGraphicsRenderThread::RenderData()
{
	{
		QMutexLocker queue_lock(&threadQueue_mutex_);
		int count = threadQueue_.count();
		total_in_on_this_list_ = count;
		if (count == 0) return;
	}

	if (Put_To_Conveer_Queue_node != NULL) delete Put_To_Conveer_Queue_node;
    
    __itt_resume();

	tbb::flow::graph conveer;
	total_out_from_this_conveer_ = 0;
	inConveerEl = 0;
	Put_To_Conveer_Queue_node = new tbb::flow::queue_node<ConveerLine>(conveer);
	tbb::flow::function_node< ConveerLine, ConveerLine > CalcFFT_node (conveer,tbb::flow::serial,functor_CalcFFT);
	tbb::flow::queue_node<ConveerLine> ToDraw_Queue_node(conveer);
	tbb::flow::function_node< ConveerLine, ConveerLine > DrawImage_node (conveer,tbb::flow::serial,functor_DrawImage);

	tbb::flow::make_edge( *Put_To_Conveer_Queue_node, CalcFFT_node );
	tbb::flow::make_edge( CalcFFT_node, ToDraw_Queue_node);
	tbb::flow::make_edge( ToDraw_Queue_node, DrawImage_node);

	PutFromRenderQueue_N_ElementsOnConveer();

	conveer.wait_for_all();

    __itt_pause();

	//qDebug() << "FINISHRED";
	delete Put_To_Conveer_Queue_node;
	Put_To_Conveer_Queue_node = NULL;
}


void SonogramGraphicsRenderThread::ElementDropFromConveer( const ConveerLine& val)
{
	inConveerEl = inConveerEl-1;
	total_out_from_this_conveer_++;
	if ((val.ctrl_renderThreadOK) && (val.ctrl_calcThreadOK))
	{
		emit ElementReady(val.el,false);
		//подкидываем след. элемент в топку
		PutFromRenderQueue_N_ElementsOnConveer();
	}
	else
	{
		if (val.ctrl_thread->thread_status() == 1)//была пауза причиной остановки
		{
			//возвращаем "свалившиеся с ленты" недорендеренные обратно в очередь
			QMutexLocker queue_lock(&threadQueue_mutex_);
			threadQueue_.append(val.el);
			return;
		}

		//забиваем болт, ибо это фаталити
	}
}

int SonogramGraphicsRenderThread::PutFromRenderQueue_N_ElementsOnConveer( int N )
{
	QMutexLocker queue_lock(&threadQueue_mutex_);

	int Ostatoc = optimalCountOnRenderSession_ - inConveerEl;
	if ((Ostatoc < 0) || (Put_To_Conveer_Queue_node == NULL)) return threadQueue_.count();
	
	int RenderSessionCounts;
	if (threadQueue_.count() == 0) return 0;
	RenderSessionCounts = qMin((int)threadQueue_.count(),Ostatoc);
	
	
	if (NeedBreaked())
	{
		debugOut("NeedBreaked - return");
		return threadQueue_.count();
	}
	
	for (int i = 0;i < RenderSessionCounts;i++)
	{
		ConveerLine cline;
		cline.el = threadQueue_[0];
		threadQueue_.pop_front ();
		if (!UseFilterIfOn(cline.el)) continue;
	
		cline.ctrl_thread = GetThreadPointer();
		cline.ctrl_calcThreadOK = false;
		cline.ctrl_renderThreadOK = false;
		Put_To_Conveer_Queue_node->try_put(cline);
		inConveerEl = inConveerEl+1;
		if (NeedBreaked()) break;
	}

	return threadQueue_.count();
}

int SonogramGraphicsRenderThread::GetCurrentProgressPercent()
{
	if (total_in_on_this_list_ == 0) return 1;
	int retVal = (total_out_from_this_conveer_*100)/total_in_on_this_list_;
	return retVal;
}

void SonogramGraphicsRenderThread::setParent( SonogramMaterial* in_parent )
{
	parentMaterial = in_parent;

	functor_CalcFFT.InitClass(in_parent,&parentMaterial->GlobalCache,&parentMaterial->GlobalCacheMutex);
	functor_DrawImage.InitClass(in_parent,this);
}

int SonogramGraphicsRenderThread::ElementsInQueue()
{
	QMutexLocker mlock(&threadQueue_mutex_);	
	return threadQueue_.count();
}



//////////////////////////////////////////////////////////////////////////
//	functorClassCalcFFT
//////////////////////////////////////////////////////////////////////////
ConveerLine functorClassCalcFFT::operator()( ConveerLine val )
{
#define RETURN(B)		{val.ctrl_calcThreadOK = B;/*if (B) qDebug() << "C:  calculateFFT OK X=" << val.el->elX << " Y=" << val.el->elY;else  qDebug() << "C:  calculateFFT BREAKED X=" << val.el->elX << " Y=" << val.el->elY;*/ return val;}
#define BREAK_FUNCTOR	if (val.ctrl_thread->thread_status() != 0) {val.ctrl_calcThreadOK = false;break;}
#define RETURN_FUNCTOR	if (val.ctrl_thread->thread_status() != 0) RETURN(false)

	val.ctrl_calcThreadOK = false;
	if ((!parentMaterial_) || (!GCache_)) return val;
	if (val.ctrl_thread->thread_status() != 0) return val;
		
	Storage<__int64> missedList;
	PresentFFTList presentList;

	//проверили наличие/отсутсвие необходимых линий в кэше. Запонив присутствующие сразу в "ленту"
	if (!parentMaterial_->getFFTInfoLists(val.el,val.compliteFFTList,missedList)) RETURN(false)
	RETURN_FUNCTOR
	if (missedList.AvailRead() == 0) { /*debugOut("FFT Cached");*/ RETURN(true)}
	
	QVector< QPair<__int64,FFTLine*> > outData;
	bool FFTresult = parentMaterial_->CalcParallelFFT(outData,missedList,val.ctrl_thread);
	
	//заносим теперь все это в КЭШ залочив предварительно, и также дополняем val.compliteFFTList
	GCacheLocker_->lock();
	for (int i = 0;i < outData.count();i++)
	{
		if (outData[i].first < 0) continue;
		GCache_->insert(outData[i].first,outData[i].second);
		FFTLineStruct str;
		str.FilePos = outData[i].first;
		str.FFT_ptr = *outData[i].second;
		val.compliteFFTList.presentFFT.append(str);
	}
	GCacheLocker_->unlock();

	if (!FFTresult) RETURN(false);

	RETURN_FUNCTOR
	//сортировка элементов в пиксельном порядке для ускорения/упрощения рендера
	std::sort(&val.compliteFFTList.presentFFT.data()[0],&val.compliteFFTList.presentFFT.data()[val.compliteFFTList.presentFFT.count()],
		[&](const FFTLineStruct a, const FFTLineStruct b) -> bool
	{
		return (a.FilePos < b.FilePos);
	});
	
	RETURN(true);

#undef RETURN_FUNCTOR
#undef BREAK_FUNCTOR
#undef RETURN
}

functorClassCalcFFT::functorClassCalcFFT()
{
	parentMaterial_ = NULL;
	GCache_ = NULL;
	GCacheLocker_ = NULL;
}

functorClassCalcFFT::functorClassCalcFFT( const functorClassCalcFFT& val )
{
	GCache_ = val.GCache_;
	parentMaterial_ = val.parentMaterial_;
	GCacheLocker_ = val.GCacheLocker_;
}

void functorClassCalcFFT::InitClass( SonogramMaterial* parent, QCache<__int64,FFTLine>* GCache, QMutex* GCacheLocker)
{
	parentMaterial_ = parent;
	GCache_ = GCache;
	GCacheLocker_ = GCacheLocker;
}

functorClassCalcFFT& functorClassCalcFFT::operator=( functorClassCalcFFT& val )
{
	parentMaterial_ = val.parentMaterial_;
	GCache_ = val.GCache_;
	GCacheLocker_ = val.GCacheLocker_;
	return *this;
}



//////////////////////////////////////////////////////////////////////////
//	functorClassDrawImage
//////////////////////////////////////////////////////////////////////////

ConveerLine functorClassDrawImage::operator()( ConveerLine val )
{
#define BREAK_FUNCTOR if ((*val.ctrl_thread == 1) || (*val.ctrl_thread == 2)) {val.ctrl_renderThreadOK = false;break;}

	val.ctrl_renderThreadOK = false;
	if ((renderThread_ == NULL) || (parentMaterial_ == NULL)) return val;
	
	if ((val.ctrl_calcThreadOK) &&  (val.ctrl_thread->thread_status() == 0))
	{
		parentMaterial_->BuildImageByConveerLine(&val);
		val.ctrl_renderThreadOK = true;
	}

	if (val.ctrl_renderThreadOK) val.el->elementIsReady_ = true;
	renderThread_->ElementDropFromConveer(val);
	return val;

#undef BREAK_FUNCTOR
}



functorClassDrawImage::functorClassDrawImage(): QObject()
{
	renderThread_ = NULL;
}

functorClassDrawImage::functorClassDrawImage( const functorClassDrawImage& val ): QObject()
{
	renderThread_ = val.renderThread_;
	parentMaterial_ = val.parentMaterial_; 
}

functorClassDrawImage& functorClassDrawImage::operator=( functorClassDrawImage& val )
{
	renderThread_ = val.renderThread_;
	parentMaterial_ = val.parentMaterial_;
	return *this;
}

void functorClassDrawImage::InitClass(SonogramMaterial* parent, SonogramGraphicsRenderThread* renderThread )
{
	renderThread_ = renderThread;
	parentMaterial_ = parent;
}


CalculatePallateThreadClass::CalculatePallateThreadClass( QObject *parent /*= NULL*/ ):QThread(parent)
{
	thread_finish_status_ = 0;
	parentMaterial_ = NULL;
}

CalculatePallateThreadClass::~CalculatePallateThreadClass()
{
	FinishHim_wait();
}

void CalculatePallateThreadClass::FinishHim_wait()
{
	if (!isRunning()) return;
	thread_finish_status_ = 2;
	wait();
}

void CalculatePallateThreadClass::StartCalculate( SonogramMaterial* parentMaterial )
{
	FinishHim_wait();
	if (parentMaterial_ != parentMaterial)
	{
		parentMaterial_ = parentMaterial;
		disconnect(SIGNAL(CalcProgress(int)));
		disconnect(SIGNAL(CalcFinished(bool)));
		connect(this,SIGNAL(CalcFinished(bool)),parentMaterial_,SLOT(CalcPallateFinished(bool)));
		connect(this,SIGNAL(CalcProgress(int)),parentMaterial_,SLOT(CalcPallateProgressChanged(int)));
	}
	start();
}

void CalculatePallateThreadClass::run()
{
#ifdef OMEGA
    CrThreadAutoInstallHelper crash_rpt_helper;
#endif // OMEGA
	curProgressPerc_ = 0;
	const int TotalLinesSelectFromFile = 200;
	int LocalFFTOrder = parentMaterial_->fftOrder_-2;
	unsigned int LocalSonogramStep = 1 << LocalFFTOrder;

	dsp::ComplexFastFourierTransform<>  fft(LocalFFTOrder);
	dsp::WindowPolicy<dsp::wtBlackman>  apply_window((size_t)LocalSonogramStep);
	dsp::MakeSpectrum<>                 make_spectrum(LocalSonogramStep);
	Storage<Ipp32fc>	file_data_buffer(LocalSonogramStep);
	Storage<Ipp32fc>	fft_source_buffer(LocalSonogramStep);
	Storage<Ipp32fc>	fft_result_buffer(LocalSonogramStep);
	Storage<Ipp16s>		store(LocalSonogramStep);

    static_assert(TotalLinesSelectFromFile > 0, "Total lines must be positive");

	unsigned __int64 step                   = parentMaterial_->dataFile_->SampleCount()/TotalLinesSelectFromFile;
	unsigned __int64 file_position          = 0;

	parentMaterial_->PaletteGlobalMaximum   = -240;
	parentMaterial_->PaletteGlobalMinimum   = 240;

	Storage<Ipp16s> tmpmean(TotalLinesSelectFromFile);
	tmpmean.WriteComplete(TotalLinesSelectFromFile);

	curProgressPerc_    = 1;
	emit CalcProgress( curProgressPerc_ );//всегда 1 эмитится как начало процесса

	int prevPerc        = curProgressPerc_;//чтобы не эмитилось одинаковый % несколько раз подряд

	for (int i = 0; i < TotalLinesSelectFromFile;i++)
	{
		if (thread_finish_status_ == 2) { emit CalcFinished( false );return; }
		store.Clear();
		file_data_buffer.Clear();
		fft_result_buffer.Clear();
		fft_source_buffer.Clear();

		if(!parentMaterial_->dataFile_->Seek(file_position)) continue;
		if(!parentMaterial_->dataFile_->Read(file_data_buffer.PrepareForWrite((size_t)LocalSonogramStep), LocalSonogramStep)) continue;

		file_data_buffer.WriteComplete((size_t)LocalSonogramStep);

		apply_window(file_data_buffer, fft_source_buffer.PrepareForWrite((size_t)LocalSonogramStep));
		fft_source_buffer.WriteComplete((size_t)LocalSonogramStep);

		if (!(fft)( fft_source_buffer, fft_source_buffer.AvailRead(), fft_result_buffer.PrepareForWrite( (size_t)LocalSonogramStep)) ) qDebug() << "fft = FALSE";
		fft_result_buffer.WriteComplete((size_t)LocalSonogramStep);

		make_spectrum(  fft_result_buffer, fft_result_buffer.AvailRead(), store.PrepareForWrite((size_t)LocalSonogramStep),NULL);
		store.WriteComplete((size_t)LocalSonogramStep);


		short maxv;short minv;
		ippsMinMax_16s(store,LocalSonogramStep,&minv,&maxv);

		if ((maxv <  240) && (parentMaterial_->PaletteGlobalMaximum < maxv)) parentMaterial_->PaletteGlobalMaximum = maxv;
		if ((minv > -240) && (parentMaterial_->PaletteGlobalMinimum > minv)) parentMaterial_->PaletteGlobalMinimum = minv;

		ippsMean_16s_Sfs(store,LocalSonogramStep,&tmpmean[i],ippAlgHintFast);//ippAlgHintAccurate

		if (thread_finish_status_ == 2) { emit CalcFinished( false );return; }

		file_position += step;
		curProgressPerc_ = i*100/TotalLinesSelectFromFile;

		if (prevPerc == curProgressPerc_) continue;
		prevPerc = curProgressPerc_;
		emit CalcProgress( curProgressPerc_ );
	}

	short PaletteGlobalMinMaxDiff = qAbs(parentMaterial_->PaletteGlobalMaximum-parentMaterial_->PaletteGlobalMinimum);
	parentMaterial_->PaletteGlobalKf = 255.0f/(float)PaletteGlobalMinMaxDiff;

	curProgressPerc_ = 100;
	emit CalcProgress( curProgressPerc_ );
	if (thread_finish_status_ == 2) { emit CalcFinished( false );return; }
	else emit CalcFinished( true );
}


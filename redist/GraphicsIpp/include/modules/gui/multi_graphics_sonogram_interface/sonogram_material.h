#ifndef SONOGRAM_MATERIAL_H
#define SONOGRAM_MATERIAL_H

#include "../../../modules/gui/multi_graphics/multi_graphics.h"
#include "../../../modules/core/storage_inl.h"
#include "../../../modules/filesystem/filesystem.h"

#include <ipp.h>
#include <tbb\flow_graph.h>
#include <QPointer>

class QFile;
class QGraphicsItem;
class Q_MG_Style;



namespace dsp
{

	class DontCheckIPPResult
	{
	public:
		void CheckIPPResult(IppStatus)
		{
			return;
		}
	};

	class DebugBreakOnIPPError
	{
	public:
		void CheckIPPResult(IppStatus ipps)
		{
			if(ipps != ippStsNoErr)
				__debugbreak();
			return;
		}
	};

#ifdef _DEBUG
	typedef DebugBreakOnIPPError DefaultIPPErrorCheckingPolicy;
#else
	typedef DontCheckIPPResult DefaultIPPErrorCheckingPolicy;
#endif // _DEBUG

    enum WindowTypes: int
    {
        wtRectangular       = 0,
        wtBlackman          = 1,
        wtBlackmanHarris    = 2,
        wtChebyshev         = 3,
    };

    template<WindowTypes type>
    class WindowPolicy;

    template<>
    class WindowPolicy<wtRectangular>
    {
        WindowPolicy(size_t length): length_(length) {}

        void operator()(Ipp32fc* in_out_data)
        {
            _DBG_POINTER(in_out_data);
            return;
        }

        void operator()(const Ipp32fc* in_data, Ipp32fc* out_data)
        {
            _DBG_POINTER(in_data);
            _DBG_POINTER(out_data);
            return;
        }

    private:
        size_t length_;
    };

    class WindowPolicyBase
    {
    public:
        size_t length() const { return window_coeffs_.AvailRead(); }

        void operator()(Ipp32fc* in_out_data)
        {
            _DBG_POINTER(in_out_data);
            _DBG_IPP_STATUS(ippsMul_32f32fc_I(window_coeffs_, in_out_data, static_cast<int>(length())));
            return;
        }

        void operator()(const Ipp32fc* in_data, Ipp32fc* out_data)
        {
            _DBG_POINTER(in_data);
            _DBG_POINTER(out_data);
            _DBG_IPP_STATUS(ippsMul_32f32fc(window_coeffs_, in_data, out_data, static_cast<int>(length())));
            return;
        }
    protected:
        Storage<Ipp32f>     window_coeffs_;
    };

    template<>
    class WindowPolicy<wtBlackman>: public WindowPolicyBase
    {
    public:
        WindowPolicy(size_t length)
        {
            Ipp32f* window = window_coeffs_.PrepareForWrite(length);
            _DBG_POINTER(window);

            _DBG_IPP_STATUS(ippsSet_32f(1.0f, window, static_cast<int>(length)));
            _DBG_IPP_STATUS(ippsWinBlackmanStd_32f_I(window, static_cast<int>(length)));
            window_coeffs_.WriteComplete(length);
        }
    };

    template<>
    class WindowPolicy<wtBlackmanHarris>: public WindowPolicyBase
    {
    public:
        WindowPolicy(size_t length)
        {
            Ipp32f* window = window_coeffs_.PrepareForWrite(length);
            _DBG_POINTER(window);

            float k = 2.0f * IPP_PI / length;
            for(size_t i = 0; i < length; i++)
                window[i] = 0.35875 - 0.48829*cos(i*k) + 0.14128*cos(i*2*k) - 0.01168*cos(3*i*k);

            window_coeffs_.WriteComplete(length);
        }
    };

    template<>
    class WindowPolicy<wtChebyshev>: public WindowPolicyBase
    {
    public:
        WindowPolicy(size_t length, double winr)
        {
            Ipp32f* window = window_coeffs_.PrepareForWrite(length);
            _DBG_POINTER(window);

            long double x0,tetta,tettas,tettai,R,r;
            int N,M,m,i;
            M=length/2.0;
            N=2*M+1;
            r=powl(10,winr/-20.);
            x0=cosh(acosh(1/r)/(2.0*M));
            tettas=2*IPP_PI/N;
            for(i=0;i<=M;i++)
            {
                R=0;
                tettai=tettas*i;
                for(m=1;m<M;m++)
                {
                    tetta=tettas*m;
                    R+=Tx(2*M,x0*cos(tetta/2.))*cos(m*tettai);
                }
                if(i>0)
                    window[M-i] =(1+2*r*R)/N;
                if(M+i < static_cast<int>(length))
                    window[M+i] =(1+2*r*R)/N;
            }

            window_coeffs_.WriteComplete(length);
        }

    private:
        static long double acosh(long double x)
        {
            long double result;

            if (x <= 1.0)
                result = 0.0;
            else if (x > 1.0e10)
                result = log((long double)2)+log(x);
            else
                result = log(x+sqrt((x-1.0)*(x+1.0)));
            return result;
        }

        static long double Tx(int n,long double arg)
        {
            if(fabs(arg)<=1) return cos(n*acos(arg));
            else return cosh(n*acosh(arg));
        }
    };

	template<class IPPErrorCheckingPolicy = DefaultIPPErrorCheckingPolicy>
	class ComplexFastFourierTransform: public IPPErrorCheckingPolicy
	{
	public:
		ComplexFastFourierTransform(int order): IPPErrorCheckingPolicy(), order_(order), temp_storage_(get_size())
		{
			CheckIPPResult(ippsFFTInitAlloc_C_32fc(&fft_spec_, order_, IPP_FFT_DIV_FWD_BY_N, ippAlgHintFast));
		}

		ComplexFastFourierTransform(const ComplexFastFourierTransform& src): IPPErrorCheckingPolicy(src), order_(src.order_), temp_storage_(get_size())
		{
			CheckIPPResult(ippsFFTInitAlloc_C_32fc(&fft_spec_, order_, IPP_FFT_DIV_FWD_BY_N, ippAlgHintFast));
		}

		bool operator()(const Ipp32fc* in_buffer, size_t in_size, Ipp32fc* out_buffer)
		{
			if(in_size != get_size())
                return false;

			temp_storage_.WriteData( in_buffer, in_size );

			Ipp32fc* temp_buffer        = temp_storage_;
			_DBG_POINTER(temp_buffer);

			const int half_size     = get_size()/2;

			CheckIPPResult(ippsFFTFwd_CToC_32fc_I(temp_buffer, fft_spec_, NULL));
			CheckIPPResult(ippsCopy_32fc(&(((Ipp32fc*)temp_buffer)[half_size]), out_buffer, half_size));
			CheckIPPResult(ippsCopy_32fc((Ipp32fc*)temp_buffer, &out_buffer[half_size], half_size));
			
			temp_storage_.RemoveFirst( in_size );
			return true;
		}

		~ComplexFastFourierTransform()
		{
			CheckIPPResult(ippsFFTFree_C_32fc(fft_spec_));
		}

		int get_size() const { return 1 << order_; }

	private:
		IppsFFTSpec_C_32fc*	fft_spec_;
		int                 order_;
		Storage<Ipp32fc>    temp_storage_;
	};

	template<class IPPErrorCheckingPolicy = DefaultIPPErrorCheckingPolicy>
	class MakeSpectrum: public IPPErrorCheckingPolicy
	{
	public:
        MakeSpectrum(int DefaultSize):
            temp_buffer_(DefaultSize),
            temp_out_buffer_(DefaultSize)
        {
        }
		void operator()(const Ipp32fc* in_buffer, size_t in_size, Ipp16s* out_buffer_16s,  Ipp32f* out_buffer_32f)
		{
            CheckIPPResult(ippsMagnitude_32fc(in_buffer, temp_out_buffer_.PrepareForWrite(in_size), static_cast<int>(in_size)));
            CheckIPPResult(ippsThreshold_LT_32f_I(temp_out_buffer_.data(), static_cast<int>(in_size), 1e-12f));
			

            CheckIPPResult(ippsLog10_32f_A11(temp_out_buffer_.const_data(), temp_buffer_.PrepareForWrite(in_size), static_cast<int>(in_size)));
			CheckIPPResult(ippsMulC_32f(temp_buffer_.const_data(), 20.0f, temp_out_buffer_.PrepareForWrite(in_size), static_cast<int>(in_size)));

			if (out_buffer_32f != NULL)	memcpy(out_buffer_32f,temp_out_buffer_.const_data(),in_size);

            CheckIPPResult(ippsConvert_32f16s_Sfs(temp_out_buffer_.const_data(),out_buffer_16s,static_cast<int>(in_size),ippRndNear,0));
		}
    private:

        Storage<Ipp32f> temp_buffer_;
		Storage<Ipp32f> temp_out_buffer_;

	};

} //namespace dsp

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


struct DoubleIppiRect
{
	double x;
	double y;
	double width;
	double height;
	DoubleIppiRect()													{	x=0;y=0;width=0;height=0;	};
	DoubleIppiRect(double _x,double _y,double _width,double _height)	{	x=_x;y=_y;width=_width;height=_height;	};
};


typedef std::shared_ptr<Storage<Ipp16s>> FFTLine;
typedef std::shared_ptr<Storage<Ipp32f>> FFTLine_32f;

struct FFTLineStruct
{
	__int64 FilePos;//он же индекс Кэша
	FFTLine FFT_ptr;
};



struct PresentFFTList
{
	int pixelFrom;//с которого пикселя содержит список (включительно = pixelFrom)
	QVector<FFTLineStruct> presentFFT;
};




struct ConveerLine 
{
	Q_MG_MaterialElement* el;
	PresentFFTList compliteFFTList;
	
	//флаги управления конвеером
    Q_MG_RenderThreadClass *ctrl_thread;
	bool ctrl_calcThreadOK;
	bool ctrl_renderThreadOK;
};



class SonogramGraphicsRenderThread;
class SonogramMaterial;



class CalculatePallateThreadClass : public QThread
{
	Q_OBJECT

public:
	CalculatePallateThreadClass( QObject *parent = NULL);
	virtual ~CalculatePallateThreadClass();

	void	StartCalculate(SonogramMaterial* parentMaterial);
	int		GetCurrentProgressPercent() {return curProgressPerc_;};

signals:
	void CalcProgress(int perc);
	void CalcFinished(bool Ok);

protected:
	void	FinishHim_wait();	//возвращается только при полной остановке
	
private:
	//упрпавление
	void	run();
	SonogramMaterial* parentMaterial_;
	QAtomicInt curProgressPerc_;
	
	QAtomicInt thread_finish_status_;//0- play  1-none 2- finish

	DISALLOW_COPY_AND_ASSIGN(CalculatePallateThreadClass);
};



class SonogramMaterial : public Q_MG_Material
{
	Q_OBJECT
friend SonogramGraphicsRenderThread;
friend CalculatePallateThreadClass;
public:
    typedef std::shared_ptr<filesystem::ReaderInterface>    FileType;

	SonogramMaterial(SonogramGraphicsRenderThread* renderThread, QGraphicsItem *parent = NULL);
	~SonogramMaterial();

    bool setData(const FileType& dataFile, int fftOrder, QByteArray color_level_data, bool updateAfterSet, SIZE_T& MaximumRamOfSystem); //если cacheSz 0 то произойдет рассчет кэш и вернется в cacheSz, если не 0 то установиться размер что подали

	virtual void ExtMaterialReInit();
	virtual void SelfClear();
	virtual bool ExtViewPortChange(Q_MG_MaterialElement* NewEl,unsigned __int64 elXpos,unsigned __int64 elYpos);

	INT64 DrawCalculatedLineToPreBuildImage( INT64 StartPixelPerLine, INT64 EndPixelPerLine, Storage<Ipp16s> &store, int preBuildImW, int Y);

	virtual bool PrepareForViewPortChange(qreal key_kf_x, qreal key_kf_y, double StartPixelX, double EndPixelX, double StartPixelY,double EndPixelY);
	virtual void ExtAfterViewPortChange();


	//функции вызываемые потоком рендера
	bool BuildImageByConveerLine(ConveerLine* val);
	bool getFFTInfoLists(Q_MG_MaterialElement* el,PresentFFTList& presentList, Storage<__int64>& missedFFT);//проверяет есть ли данные для элемента в кэш и заполняет 2 списка;возврашает удачность произошедшего
	inline bool calculateDrawVariables(Q_MG_MaterialElement* el, INT64& StartLine, INT64& EndLine, INT64& StartPixelPerLine, INT64& EndPixelPerLine, int& preBuildImW, int& preBuildImH, double& interleaver_step);
    bool CalcParallelFFT( QVector< QPair<__int64,FFTLine*> >& out, Storage<__int64>& in, Q_MG_RenderThreadClass* breaker);//если true то досчитало если false то выкинуло по break

	
	

signals:
	void ScreenSpectrumsChanged(FFTLine_32f mean_spectrum, FFTLine max_spectrum);
	void CalcPallateFinished_signal(QByteArray levels_data);

public slots:
	void CalcPallateProgressChanged(int perc);
	void CalcPallateFinished(bool Ok);


private:
	void CreateAndStartCalculatePaletteThread();
	QPointer<CalculatePallateThreadClass> calculatePallateThread;
	
	

	void		LoadColorTableFromFile(QString Fname);//загрузка цветовой схемы
	void		LoadColorLevelsFromData(QByteArray in_data);

	QMutex GlobalCacheMutex;
	QCache <__int64, FFTLine>	GlobalCache;

	FileType                dataFile_;
	int						fftOrder_;
	unsigned int            sonogram_step_;
	double                  total_lines_;//всего линий на файл


	//для функций на текущий рендер
	qreal                   key_kf_x_;
	qreal                   key_kf_y_;
	unsigned __int64        round_key_kf_;//y

	//работа с памятью для скейла, чтобы не выделять ее постоянно
	int                     prev_preBuildImgSZ;
	int                     prev_externalBufSZ;
	int                     prev_pDstSz;
	Ipp8u*                  externalbuf;
	char*                   preBuildIm;
	Ipp8u*                  pDst;


	//функционал для рассчеты палитры, заполняется потоком расчета палитры
	QVector<QRgb>			colorTable;
 	short					PaletteGlobalMaximum;
	short					PaletteGlobalMinimum;	
	double					PaletteGlobalKf;


	//хранилище средних и максимальных спектров за элемент и функциональные переменные
	QMap<__int64,FFTLine_32f>	ElementsSpectrumCache_mean_;	//хранилище средних спектров за элемент
	QMap<__int64,FFTLine>		ElementsSpectrumCache_max_;		//хранилище максимальных спектров за элемент
	QMutex						ElementsSpectrumCache_mutex_;
	__int64						elementYStartByCurrentScreen_;
	__int64						elementYEndByCurrentScreen_;
	__int64						previusSended_YStart_;
	__int64						previusSended_YEnd_;
	double						prevKfY_;//чтобы эмитить если только меняется Y
	bool						calcMaxMeanSpectrumsAndSendSignal(bool full_screen_calc);

	DISALLOW_COPY_AND_ASSIGN(SonogramMaterial);
	
};






class functorClassCalcFFT
{
public:
	functorClassCalcFFT();
	functorClassCalcFFT(const functorClassCalcFFT& copy);
	~functorClassCalcFFT() {};

	ConveerLine operator()(ConveerLine);
	functorClassCalcFFT& operator=(functorClassCalcFFT& val);

	void InitClass(SonogramMaterial* parent, QCache<__int64, FFTLine>* GCache, QMutex* GCacheLocker);

private:

	QCache<__int64, FFTLine>* GCache_;
	QMutex* GCacheLocker_;
	SonogramMaterial* parentMaterial_;

};


class functorClassDrawImage: public QObject
{
	Q_OBJECT
public:
	functorClassDrawImage();
	functorClassDrawImage(const functorClassDrawImage& copy);
	~functorClassDrawImage() {};


	ConveerLine operator()(ConveerLine);
	functorClassDrawImage& operator=(functorClassDrawImage& val);

	void InitClass(SonogramMaterial* parent, SonogramGraphicsRenderThread* renderThread);

private:
	SonogramGraphicsRenderThread* renderThread_;
	SonogramMaterial* parentMaterial_;
};










class SonogramGraphicsRenderThread: public Q_MG_RenderThreadClass
{
	Q_OBJECT
public:
	SonogramGraphicsRenderThread();
	~SonogramGraphicsRenderThread();

	void setParent(SonogramMaterial* in_parent);

	virtual void    RenderData();
	virtual void	addData(Storage<Q_MG_MaterialElement*>& in_list);
	virtual void	ClearQueue();//возвращатся по выполненею операции очистки (ждет паузу, потом чистит)
	virtual int		GetCurrentProgressPercent();

	virtual int		ElementsInQueue();
	void ElementDropFromConveer(const ConveerLine&);

private:
	functorClassCalcFFT		functor_CalcFFT;
	functorClassDrawImage	functor_DrawImage;

	int PutFromRenderQueue_N_ElementsOnConveer(int N = 0);//возвращает сколько элементов осталось в очереди после выкладки, если N = 0,количество будет выбрано автоматически от optimalCountOnRenderSession_

	//рабочие данные
	SonogramMaterial* parentMaterial;
	QVector<Q_MG_MaterialElement*>	 threadQueue_;
	QMutex		 threadQueue_mutex_;

	tbb::flow::queue_node<ConveerLine>* Put_To_Conveer_Queue_node;
	QAtomicInt inConveerEl;
	int optimalCountOnRenderSession_;

	int total_in_on_this_list_;
	int total_out_from_this_conveer_;

	DISALLOW_COPY_AND_ASSIGN(SonogramGraphicsRenderThread);
};



#endif // SONOGRAM_MATERIAL_H

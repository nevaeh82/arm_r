#include "../../../stdafx.h"
#include "components_relation.h"

ComponentsRelation::ComponentsRelation( SonogramInterface* sonogram, Q_MG_SpectrumInterface* spectrum, QObject* parent /*= NULL*/ ) : QObject(parent)
{
	Q_ASSERT_X(sonogram,"ComponentsRelation","Wrong params for class constructor sonogram pointer == NULL");
	Q_ASSERT_X(spectrum,"ComponentsRelation","Wrong params for class constructor spectrum pointer == NULL");
	 
	sonogram_ = sonogram;
	spectrum_ = spectrum;
	spectrum_->SetAutoscaleY(true);
	multy_select_on_ = false;

	spectrum_->ClearAllSelections();
	sonogram_->ClearAllSelections();
	spectrum_->ClearAllMarkers();
	sonogram_->ClearAllMarkers();

	spectrum_->setRelationStatus(true);
	sonogram_->setRelationStatus(true);
	
	spectrum_->getBaseEngineClass()->RelationBaseClass_ = sonogram_->getBaseEngineClass();

	spectrumShowMeanHold_ = true;
	spectrumShowMaxHold_ = true;

	prev_sended_start_hz_on_spectrum_view_chenge_ = 0;
	prev_sended_end_hz_on_spectrum_view_chenge_ = 0;
	
	connect(sonogram_,SIGNAL(viewPortChanged(double,double,double,double)),this,SLOT(my_SonogramViewPortChanged(double,double,double,double)));
	connect(sonogram_,SIGNAL(SonogramScreenSpectrumsChanged(FFTLine_32f,FFTLine)),this,SLOT(my_SonogramScreenSpectrumsChanged(FFTLine_32f,FFTLine)));
	connect(spectrum_,SIGNAL(viewPortChanged(double,double,double,double)),this,SLOT(my_SpectrumViewPortChanged(double,double,double,double)));
	
	connect(sonogram_,SIGNAL(MarkersChanged(QVector<double>,bool)),this,SLOT(my_SonogramMarkerChange(QVector<double>,bool)));
	connect(spectrum_,SIGNAL(MarkersChanged(QVector<double>,bool)),this,SLOT(my_SpectrumMarkerChange(QVector<double>,bool)));

	connect(sonogram_,SIGNAL(SelectionChangedTo(double,double,double,double)),this,SLOT(my_SonogramSelectionChangeTo(double,double,double,double)));
	connect(spectrum_,SIGNAL(SelectionChangedTo(double,double,double,double)),this,SLOT(my_SpectrumSelectionChangeTo(double,double,double,double)));

	connect(sonogram_,SIGNAL(selectionFinished(double,double,double,double)),this,SLOT(my_SonorgamSelectionFinished(double,double,double,double)));
	connect(spectrum_,SIGNAL(SelectionFinished(double,double,double,double)),this,SLOT(my_SpectrumSelectionFinished(double,double,double,double)));

	connect(sonogram_,SIGNAL(DeselectFinished()),this,SLOT(my_SonorgamDeselectionFinished()));
	connect(spectrum_,SIGNAL(DeselectFinished()),this,SLOT(my_SpectrumDeselectionFinished()));
	
	connect(sonogram_,SIGNAL(SelectionCleared()),this,SLOT(my_SonogramSelectionCleared()));
	connect(spectrum_,SIGNAL(SelectionCleared()),this,SLOT(my_SpectrumSelectionCleared()));
}

ComponentsRelation::~ComponentsRelation()
{
	if (spectrum_)
	{
		spectrum_->setRelationStatus(false);
		spectrum_->getBaseEngineClass()->RelationBaseClass_ = nullptr;
	}
	if (sonogram_) sonogram_->setRelationStatus(false);
}

bool ComponentsRelation::Setup( const FileType& reader, int fft_Order, QByteArray color_level_data /*= QByteArray()*/, QString sectrumVLabel )
{
	sectrumVLabel_ = sectrumVLabel;
	if (spectrum_->isActivated()) spectrum_->Reset();
	bool retVal = sonogram_->Setup(reader, fft_Order, color_level_data);
	return retVal;
}

void ComponentsRelation::my_SonogramViewPortChanged( double start_hz, double start_smpl, double end_hz, double end_smpl )
{
	//qDebug() << "SonogramViewPortChanged  start_hz:" << start_hz << "  start_smpl:" << start_smpl << "  end_hz:" << end_hz << "  end_smpl:" << end_smpl;
	spectrum_->SetViewport(start_hz,0,end_hz,0);

	double a,b;
	spectrum_->GetCurrentViewport(prev_sended_start_hz_on_spectrum_view_chenge_,a,prev_sended_end_hz_on_spectrum_view_chenge_,b);
	//int diff = spectrum_->getBaseEngineClass()->Grid_Layer->getBorderHightWidth().x()-sonogram_->getBaseEngineClass()->Grid_Layer->getBorderHightWidth().x();
	//spectrum_->MoveScreenByPixels(QPoint(diff+2,0),false);
}

void ComponentsRelation::my_SpectrumViewPortChanged( double start_hz, double start_Y, double end_hz, double end_Y )
{
	if ((prev_sended_start_hz_on_spectrum_view_chenge_ == start_hz) && (prev_sended_end_hz_on_spectrum_view_chenge_ == end_hz))//значит что спектр скейлился только по Y и нехрен слать ничего в сонограмму
		return;
	
	prev_sended_start_hz_on_spectrum_view_chenge_ = start_hz;
	prev_sended_end_hz_on_spectrum_view_chenge_ = end_hz;
	//qDebug() << "SpectrumViewPortChanged  start_hz:" << start_hz << "  start_smpl:" << start_Y << "  end_hz:" << end_hz << "  end_smpl:" << end_Y;
	sonogram_->SetViewport(start_hz,0,end_hz,0);
	//int diff = sonogram_->getBaseEngineClass()->Grid_Layer->getBorderHightWidth().x()-spectrum_->getBaseEngineClass()->Grid_Layer->getBorderHightWidth().x();
	//sonogram_->MoveScreenByPixels(QPoint(diff,0),false);
}

void ComponentsRelation::my_SonogramScreenSpectrumsChanged( FFTLine_32f mean_spectrum, FFTLine max_spectrum )
{
	if (max_spectrum == NULL) return;
	if (mean_spectrum == NULL) return;

	cur_mean_spectrum_ = mean_spectrum;
	cur_max_spectrum_ = max_spectrum;

	Ipp32f* buff_1 = NULL;
	size_t sz_1 = 0;
	Ipp32f* buff_2 = NULL;
	size_t sz_2 = 0;

	if (spectrumShowMeanHold_)
	{
		buff_1 = *mean_spectrum;
		sz_1 = mean_spectrum->AvailRead();
	}
	
	if (spectrumShowMaxHold_)
	{
		if (buff_1 == NULL)
		{
			sz_1 = max_spectrum->AvailRead();
			buff_1 = new Ipp32f[sz_1];
            ippsConvert_16s32f(*max_spectrum, buff_1, static_cast<int>(sz_1));
		}
		else
		{
			sz_2 = max_spectrum->AvailRead();
			buff_2 = new Ipp32f[sz_2];
			ippsConvert_16s32f(*max_spectrum, buff_2, static_cast<int>(sz_2));
		}
	}
	
	
	if ((buff_1 == NULL) || (sz_1 == 0)) Q_ASSERT_X(buff_1,"ComponentsRelation","Try export empty data to spectrum");
	if (spectrum_->isActivated()) spectrum_->PermanentDataSetup(buff_1,buff_2);
	else
	{
		//объект не инициализирован
		spectrum_->Setup(true,sonogram_->GetCurrentBandwidth_hz(), sectrumVLabel_, buff_1, sz_1, buff_2, sz_2);
		spectrum_->getBaseEngineClass()->AllCopyMatrixByGridPublic();

		double a,b,c,d;
		sonogram_->GetCurrentViewport(a,b,c,d);
		spectrum_->SetViewport(a,0,c,0);
	}
	
	if (spectrumShowMaxHold_)
	{
		if (buff_2 == NULL)	delete[] buff_1;
		else delete[] buff_2;
	}
}

void ComponentsRelation::Reset()
{
	spectrum_->Reset();
	sonogram_->Reset();
	cur_mean_spectrum_ = NULL;
	cur_max_spectrum_ = NULL;
	prev_sended_start_hz_on_spectrum_view_chenge_ = 0;
	prev_sended_end_hz_on_spectrum_view_chenge_ = 0;
}

void ComponentsRelation::SpectrumShowOnlyMaxHold()
{
	if ((spectrumShowMeanHold_ == false) && (spectrumShowMaxHold_ == true)) return;
	spectrumShowMeanHold_ = false;
	spectrumShowMaxHold_ = true;
	my_SonogramScreenSpectrumsChanged( cur_mean_spectrum_, cur_max_spectrum_);
}

void ComponentsRelation::SpectrumShowOnlyMeanHold()
{
	if ((spectrumShowMeanHold_ == true) && (spectrumShowMaxHold_ == false)) return;
	spectrumShowMeanHold_ = true;
	spectrumShowMaxHold_ = false;
	my_SonogramScreenSpectrumsChanged( cur_mean_spectrum_, cur_max_spectrum_);
}

void ComponentsRelation::SpectrumShowAllHolds()
{
	if ((spectrumShowMeanHold_ == true) && (spectrumShowMaxHold_ == true)) return;
	spectrumShowMeanHold_ = true;
	spectrumShowMaxHold_ = true;
	my_SonogramScreenSpectrumsChanged( cur_mean_spectrum_, cur_max_spectrum_);
}

void ComponentsRelation::my_SonogramMarkerChange( QVector<double> Mark,bool isVertical )
{
	if (isVertical)	spectrum_->SetMarkers(Mark,isVertical,true);
}

void ComponentsRelation::my_SpectrumMarkerChange( QVector<double> Mark,bool isVertical )
{
	if (isVertical)	sonogram_->SetMarkers(Mark,isVertical,true);
}

void ComponentsRelation::my_SonogramSelectionChangeTo( double x1, double y1, double x2, double y2 )
{
	if (multy_select_on_)
	{
		if (spectrum_->GetAllHorSelections().size() > sonogram_->GetAllHorSelections().size()+sonogram_->GetAllRectSelections().size())
			spectrum_->RemoveLastSelection();
	}
	
	spectrum_->SetSelection(x1,y1,x2,y2,true);
}

void ComponentsRelation::my_SpectrumSelectionChangeTo( double x1, double y1, double x2, double y2 )
{
	int oldtype = sonogram_->curSelectionType();
	sonogram_->SetCurSelectionType(1);
	if (multy_select_on_)
	{
		if (sonogram_->GetAllHorSelections().size()+sonogram_->GetAllRectSelections().size() > spectrum_->GetAllHorSelections().size())
			sonogram_->RemoveLastSelection();
	}
	sonogram_->SetSelection(x1,y1,x2,y2,true);
	sonogram_->SetCurSelectionType(oldtype);
}

void ComponentsRelation::my_SpectrumSelectionCleared()
{
	sonogram_->ClearAllSelections(true);
}

void ComponentsRelation::my_SonogramSelectionCleared()
{
	spectrum_->ClearAllSelections(true);
}

void ComponentsRelation::SetZeroFrequencyHz( double val )
{
	spectrum_->SetZeroFrequencyHz(val);
	sonogram_->SetVirtualOffsetX(val);
}

void ComponentsRelation::SetMultySelectEnabled( bool enabled )
{
	sonogram_->SetMultySelectEnabled(enabled);
	spectrum_->SetMultySelectEnabled(enabled);
	multy_select_on_ = enabled;
}

void ComponentsRelation::SetButtonCtrlClickIsSingle( bool is_single /*= true*/ )
{
	sonogram_->SetButtonCtrlClickIsSingle(is_single);
	spectrum_->SetButtonCtrlClickIsSingle(is_single);
}

void ComponentsRelation::my_SpectrumSelectionFinished( double x1, double y1, double x2, double y2 )
{
	if (multy_select_on_)
	{
		sonogram_->ClearAllSelections(true);
		QList<QPointF> pp = spectrum_->GetAllHorSelections();
		int oldtype = sonogram_->curSelectionType();
		sonogram_->SetCurSelectionType(1);
		for (int i = 0;i < pp.size();i++)
			sonogram_->SetSelection(pp[i].x(),0,pp[i].y(),0,true);	
		sonogram_->SetCurSelectionType(oldtype);
	}
}

void ComponentsRelation::my_SonorgamSelectionFinished( double x1, double y1, double x2, double y2 )
{
	if (multy_select_on_)
	{
		spectrum_->ClearAllSelections(true);
		QList<QRectF> rr = sonogram_->GetAllRectSelections();
		QList<QPointF> pp = sonogram_->GetAllHorSelections();
		for (int i = 0;i < pp.size();i++)
			spectrum_->SetSelection(pp[i].x(),0,pp[i].y(),0,true);	
		for (int i = 0;i < rr.size();i++)
			spectrum_->SetSelection(rr[i].x(),0,rr[i].right(),0,true);
	}
}

void ComponentsRelation::my_SonorgamDeselectionFinished()
{
	spectrum_->ClearAllSelections(true);
	QList<QRectF> rr = sonogram_->GetAllRectSelections();
	QList<QPointF> pp = sonogram_->GetAllHorSelections();
	for (int i = 0;i < pp.size();i++)
		spectrum_->SetSelection(pp[i].x(),0,pp[i].y(),0,true);	
	for (int i = 0;i < rr.size();i++)
		spectrum_->SetSelection(rr[i].x(),0,rr[i].right(),0,true);
}

void ComponentsRelation::my_SpectrumDeselectionFinished()
{
	sonogram_->ClearAllSelections(true);
	QList<QPointF> pp = spectrum_->GetAllHorSelections();
	int oldtype = sonogram_->curSelectionType();
	sonogram_->SetCurSelectionType(1);
	for (int i = 0;i < pp.size();i++)
		sonogram_->SetSelection(pp[i].x(),0,pp[i].y(),0,true);	
	sonogram_->SetCurSelectionType(oldtype);
}

#include "../../../stdafx.h"
#include "spectrum_material.h"
#include "../../../modules/gui/multi_graphics/mg_fastlab_style.h"//цветовой стиль по умолчанию

SpectrumMaterial::SpectrumMaterial(QGraphicsItem *parent): Q_MG_Material(parent)
{
	GlobalCacheC.setMaxCost(10000);
	DataIsInline = false;

	FilledSpectrum = false;
	find_current_cache_cluster_ = 0;
	CurCachCluster = NULL;
	key_kf_x_ = 0;
	round_key_kf_ = 0;
	StartPixelX_ = 0;
	EndPixelX_ = 0;
	StartPixelY_ = 0;
    EndPixelY_ = 0;
}

void SpectrumMaterial::ExtMaterialReInit()
{
	ClearPP();
}

bool SpectrumMaterial::setData( MaterialFloatData &_mat, bool updateAfterSet)
{
//    qDebug() << this->isVisible();
	if (DataMemoryStore.Copy(_mat))
	{
		DataIsInline = false;
		FirstPaint = updateAfterSet;
		NeedMaterialUpdateAfterMoov = true;
		GlobalCacheC.clear();

//        this->paint();
        update();
		return true;
	}
	else return false;
}

bool SpectrumMaterial::setInlineData( float* spectrum,unsigned int array_size )
{
	if ((spectrum == NULL) || (array_size == 0)) return false;
	DataIsInline = true;
	DataMemoryStore.values = spectrum;
	DataMemoryStore.PointCount = array_size;
	FirstPaint = true;
	NeedMaterialUpdateAfterMoov = true;
	GlobalCacheC.clear();
	update();
	return true;
}

bool SpectrumMaterial::PrepareForViewPortChange( qreal key_kf_x,qreal key_kf_y, double StartPixelX, double EndPixelX, double StartPixelY,double EndPixelY)
{
	if (!DataMemoryStore.IsValid()) return false;

	StartPixelX_ = StartPixelX;
	EndPixelX_ = EndPixelX;
	StartPixelY_ = StartPixelY;
	EndPixelY_ = EndPixelY;

	key_kf_x_ = key_kf_x;
	round_key_kf_ = qRound64(key_kf_x_);

	if ((PointItemList.AvailRead() > 0) && (key_kf_x_ > 0.1)) ClearPP();
	if (PointItemList.AvailRead() > 200) ClearPP();

	find_current_cache_cluster_ = false;

	CurCachCluster = NULL;
	CurCachCluster = GlobalCacheC[round_key_kf_];
	
	if (CurCachCluster == NULL)//ненашли кэш
	{
		CurCachCluster = new CacheCluster();
		CurCachCluster->kf = round_key_kf_;
		CurCachCluster->ElListCach.setMaxCost(100000);
		find_current_cache_cluster_ = false;
	}
	else find_current_cache_cluster_ = true;


	return true;
}

bool SpectrumMaterial::ExtViewPortChange(Q_MG_MaterialElement* NewEl,unsigned __int64 elXpos,unsigned __int64 elYpos)
{
	bool NeedHide = true;

	double ElCoordsX = elXpos*ElementW;
// 	double ElCoordsY = elYpos*ElementH;

	//определяем с каторого и по какой отсчет рисовать
	__int64 MassStartPos = qRound64(ElCoordsX*key_kf_x_)-1;
	__int64 MassEndPos = qRound64((elXpos+1)*ElementW*key_kf_x_)+2;
	if (MassStartPos < 0) MassStartPos = 0;
	if (MassEndPos >= DataMemoryStore.PointCount) MassEndPos = DataMemoryStore.PointCount-1;

	CacheElement* OneMomentCurCachElement = NULL;
	__int64 fromC = MassStartPos;
	qreal cur_PX = 0;
	qreal next_PX = 0;
	qreal min_ValY = 2000000000000000;//MinY
	qreal max_ValY =-2000000000000000;//MaxY
	bool DrawMinMax = false;

	qreal prevXPixel = 0;
	qreal prevMaxY = 0;
	qreal prevMinY = 0;

	
	QPolygon *spectrum_fill = NULL;
	//spectrum_fill = new QPolygon(ElementW);
	//spectrum_fill->fill(QPoint(-1,ElementH+10));
	
	
	for (__int64 f = MassStartPos;f < MassEndPos;f++)
	{
		cur_PX = f/key_kf_x_;

		if (key_kf_x_ > 4)
		{
			OneMomentCurCachElement = CurCachCluster->ElListCach[f];
			if (OneMomentCurCachElement != NULL)//нашли кэш
			{
				//проверка не пропущен ли пиксель со сраным дробным kf и не дробным QCach
				if ((__int64)(prevXPixel)+1 != (__int64)(cur_PX))
				{
					if (DrawLineInElement(NewEl,prevXPixel+1,prevMinY,prevXPixel+1,prevMaxY,spectrum_fill)) NeedHide = false;
				}

				double p1y = fast_GetYPixelByValue(OneMomentCurCachElement->minVal);
				double p2y = fast_GetYPixelByValue(OneMomentCurCachElement->maxVal);

				if (DrawLineInElement(NewEl,cur_PX,p1y,cur_PX,p2y,spectrum_fill)) NeedHide = false;
				f=OneMomentCurCachElement->ToC;

				//и дорисовать добавочную линию
				if (!NeedHide)
				{
					next_PX = (f+1)/key_kf_x_;
					double p3y = fast_GetYPixelByValue(DataMemoryStore[f+1]);
					if (((p3y > p1y)  && (p3y > p2y)) || ((p3y < p1y) && (p3y < p2y)))
					{
						if (qAbs(p3y-p1y) > qAbs(p3y-p2y)) DrawLineInElement(NewEl,cur_PX,p2y,next_PX,p3y,spectrum_fill);
						else DrawLineInElement(NewEl,cur_PX,p1y,next_PX,p3y,spectrum_fill);
					}
				}

				prevXPixel = cur_PX;
				prevMaxY = p1y;
				prevMinY = p2y;
				min_ValY = 2000000000000000;//MinY
				max_ValY =-2000000000000000;//MaxY
				fromC = f+1;
				DrawMinMax = false;
				continue;
			}
		}

		next_PX = (f+1)/key_kf_x_;
		double val1y = DataMemoryStore[f];
		double val2y = DataMemoryStore[f+1];

		if ((__int64)(cur_PX) == (__int64)(next_PX))
		{
			DrawMinMax = true;
			if (val1y > val2y)
			{
				min_ValY = qMin(min_ValY,val2y);
				max_ValY = qMax(max_ValY,val1y);
			}
			else
			{
				min_ValY = qMin(min_ValY,val1y);
				max_ValY = qMax(max_ValY,val2y);
			}
		}
		else
		{		
			double p1y = 0;double p2y = 0;

			if (DrawMinMax)
			{
				p1y = fast_GetYPixelByValue(min_ValY);
				p2y = fast_GetYPixelByValue(max_ValY);
				if (DrawLineInElement(NewEl,cur_PX,p1y,cur_PX,p2y,spectrum_fill)) NeedHide = false;
				if (key_kf_x_ > 4)
				{
					OneMomentCurCachElement = new CacheElement(fromC,f,min_ValY,max_ValY);
					CurCachCluster->ElListCach.insert(fromC,OneMomentCurCachElement);
				}
			}
			p1y = fast_GetYPixelByValue(DataMemoryStore[f]);
			p2y = fast_GetYPixelByValue(DataMemoryStore[f+1]);
			if (DrawLineInElement(NewEl,cur_PX,p1y,next_PX,p2y,spectrum_fill)) NeedHide = false;

			min_ValY = 2000000000000000;//MinY
			max_ValY =-2000000000000000;//MaxY
			fromC = f+1;
			DrawMinMax = false;
		}
	}//for (__int64 f = MassStartPos;f < MassEndPos;f++)



	/*
	spectrum_fill->prepend(QPoint(-1,ElementH+1));
	
	int PrevY = spectrum_fill->point(0).y();
	for (int z = 1;z < ElementW;z++)
	{
		int curY = spectrum_fill->point(z).y();
		if (curY == ElementH+10) spectrum_fill->setPoint(z,z,PrevY);
		else PrevY = curY;
	}

	spectrum_fill->append(QPoint(ElementW+1,ElementH+1));


	QPainter pnt(NewEl->img);
	//pnt.setPen(settings->Material_GrafColor);
	pnt.setPen(QColor(255,255,0));
	pnt.setBrush(settings->Material_GrafColor);
	pnt.drawPolygon(*spectrum_fill);

	delete spectrum_fill;*/

	return NeedHide;
}

void SpectrumMaterial::ExtAfterViewPortChange()
{
	if (!find_current_cache_cluster_) GlobalCacheC.insert(round_key_kf_,CurCachCluster);
	
	//рисование отсчетиков
	if (key_kf_x_ <= 0.1)
	{
		__int64 MassStartPos = qRound64(StartPixelX_*key_kf_x_);
		__int64 MassEndPos = qRound64(EndPixelX_*key_kf_x_)+1;
		if (MassStartPos < 0) MassStartPos = 0;
		if (MassEndPos > DataMemoryStore.PointCount) MassEndPos = DataMemoryStore.PointCount;

		Q_MG_PointItem** ElPointItem = PointItemList.PrepareForWrite(50);
		unsigned __int64 appendedEl = 0;
		double ppsize = 8;

		for (__int64 f = MassStartPos;f < MassEndPos;f++)
		{
			bool Found = false;
			for (size_t g=0;g < PointItemList.AvailRead();g++)
				if (PointItemList[g]->PointIndex == f)
				{ Found = true;break; }
				if (Found) continue;

				qreal cur_PX = f/key_kf_x_;
				qreal p1y = fast_GetYPixelByValue(DataMemoryStore[f]);
				Q_MG_PointItem* pp = new Q_MG_PointItem(7,7,this,settings);
				pp->setPos(cur_PX-(int)(ppsize/2),p1y-(int)(ppsize/2));
				pp->setToolTip(tr("Значения отсчета\nЧастота:  ")+Interpret(GetValueByPixelOnMaterial(cur_PX,0).x(),false,false)+tr("\nУровень: ")+Interpret(DataMemoryStore[f],true,false));
				pp->setZValue(2);
				pp->PointIndex = f;

				if (ElList.AvailWrite() <= 0) {ElPointItem = PointItemList.PrepareForWrite(50);appendedEl = 0;}
                if(ElPointItem)
                {
                    ElPointItem[appendedEl] = pp;
                    PointItemList.WriteComplete(1);
                }
				appendedEl++;
		}
	}

}

void SpectrumMaterial::ClearPP()
{
	for (size_t i = 0;i < PointItemList.AvailRead();i++)
	{
		scene()->removeItem(PointItemList[i]);	
		delete PointItemList[i];
	}
	PointItemList.Clear();
}

float* SpectrumMaterial::GetMaterialDataArrayByPixels( double globP1x,double globP2x,size_t *outLen )
{
	if (!DataMemoryStore.IsValid()) return NULL;

	if (globP1x > globP2x) { double t = globP1x;globP1x = globP2x;globP2x = t;}

	double StartPixelX = (pos().x()*-1)+globP1x;
	double EndPixelX = (pos().x()*-1)+globP2x;

	qreal key_kf = kf.x()*Hor_kf_inc;

	__int64 MassStartPos = (StartPixelX*key_kf)+1;
	__int64 MassEndPos = (EndPixelX*key_kf);

	if ((MassStartPos > DataMemoryStore.PointCount) || (MassEndPos < 0)) DebugBreak();

	if (MassStartPos < 0) MassStartPos = 0;
	if (MassEndPos >= DataMemoryStore.PointCount) MassEndPos = DataMemoryStore.PointCount-1;

	size_t tt = (MassEndPos-MassStartPos)+1;
	if (tt > DataMemoryStore.PointCount) tt = DataMemoryStore.PointCount;
	if (tt <= 0)
	{
		*outLen = 0;
		return NULL;
	}
	//qDebug() << MassStartPos << " -> " << MassEndPos;
	*outLen = tt;
	return &DataMemoryStore.values[MassStartPos];
}

void SpectrumMaterial::SetFillSpectrum( bool fill /*= true*/ )
{
	FilledSpectrum = fill;
	update();
}


bool SpectrumMaterial::DrawLineInElement( Q_MG_MaterialElement* el,double p1x,double p1y,double p2x,double p2y, QPolygon* spectrum_fill )
{
	double StartPixelX = el->pos().x(); 
	double EndPixelX = StartPixelX+ElementW;
	double StartPixelY = el->pos().y(); 
	double EndPixelY = StartPixelY+ElementH;

	unsigned int color = settings->Material_GrafColor_1.rgb();
	switch (MaterialID)
	{
		case 1: color = settings->Material_GrafColor_1.rgb();break;
		case 2: color = settings->Material_GrafColor_2.rgb();break;
		case 3: color = settings->Material_GrafColor_3.rgb();break;
	}

	uchar *sLine;
	int bytesPerLine = el->img->bytesPerLine();
	uchar* iBits = el->img->bits();

	bool Invert = false;
	bool retValPaint = false;
	double Opty_Y = (p2y-p1y);
	double Opty_X = (p2x-p1x);


	bool FilledPoly = true;
	if (spectrum_fill == NULL) FilledPoly = false;


	if (qAbs(Opty_X) < qAbs(Opty_Y)) Invert = true;
	if (!Invert)
	{
		double stp = p1x;
		double enp = p2x;
		if (stp>enp) {double a=enp;enp=stp;stp=a;}

		if (stp < StartPixelX) stp = StartPixelX;
		if (enp > EndPixelX) enp = EndPixelX;

		if (Opty_X+p1y == 0) return retValPaint;

		for (double x = stp;x < enp;x++)
		{
			double y = (x-p1x)*Opty_Y/Opty_X+p1y;

			int localX = (int)(x-StartPixelX);
			int localY = (int)(y-StartPixelY);

			if (y > EndPixelY)
			{
				//if (FilledPoly)
				//	spectrum_fill->setPoint(localX,localX,ElementH);
				continue;
			}
			if (y < StartPixelY)
			{
				//if (FilledPoly)
				//	spectrum_fill->setPoint(localX,localX,0);
				continue;
			}

			//int localX = (int)(x-StartPixelX);

			if ((localX < 0) || (localY < 0) || (localX >= ElementW) ||  (localY >= ElementH)) break;


			if (FilledPoly)
				spectrum_fill->setPoint(localX,localX,localY);

			//el->img->setPixel(localX,localY,color);
			sLine = (uchar *)(iBits+localY*bytesPerLine);
			((uint *)sLine)[localX] = color;
			retValPaint = true;
		}
	}
	else
	{
		double stp = p1y;
		double enp = p2y;
		if (stp>enp) {double a=enp;enp=stp;stp=a;}

		if (stp < StartPixelY) stp = StartPixelY;
		if (enp > EndPixelY) enp = EndPixelY;

		if (Opty_Y+p1x == 0) return retValPaint;

		for (double y = stp;y < enp;y++)
		{
			double x = Opty_X*(y-p1y)/Opty_Y+p1x;

			int localX = (int)(x-StartPixelX);
			int localY = (int)(y-StartPixelY);

			if (x > EndPixelX)
			{
				//if (FilledPoly)
				//	spectrum_fill->setPoint(localX,localX,ElementH);
				continue;
			}
			if (x < StartPixelX)
			{
				//if (FilledPoly)
				//	spectrum_fill->setPoint(localX,localX,ElementH);
				continue;
			}


			//int localY = (int)(y-StartPixelY);
			if ((localX < 0) || (localY < 0) || (localX >= ElementW) ||  (localY >= ElementH)) break;

			//el->img->setPixel(localX,localY,color);
			if (FilledPoly)
				spectrum_fill->setPoint(localX,localX,localY);

			sLine = (uchar *)(iBits+localY*bytesPerLine);
			((uint *)sLine)[localX] = color;
			retValPaint = true;

		}
	}
	return retValPaint;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
Q_MG_PointItem::Q_MG_PointItem( int _szX,int _szY,SpectrumMaterial *parent,Q_MG_Style* _settings ) : QGraphicsObject(parent)
{
	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::LeftButton);
	setFlag(QGraphicsItem::ItemIsFocusable,false);
	setFlag(QGraphicsItem::ItemIsMovable,true);
	setFlag(QGraphicsItem::ItemIsSelectable,true);
	szX = _szX;szY = _szY;
	settings = _settings;
	setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	parent_ = parent;
}

void Q_MG_PointItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
	QColor pen_color = settings->Material_BorderPointColor_1;
	QColor fill_color = settings->Material_FillPointColor_1;
	switch (parent_->MaterialID)
	{
		case 1: pen_color = settings->Material_BorderPointColor_1;fill_color = settings->Material_FillPointColor_1;break;
		case 2: pen_color = settings->Material_BorderPointColor_2;fill_color = settings->Material_FillPointColor_2;break;
		case 3: pen_color = settings->Material_BorderPointColor_3;fill_color = settings->Material_FillPointColor_3;break;
	}

	painter->setPen(pen_color);
	painter->setBrush(fill_color);
	painter->drawRect(QRectF(0,0,szX,szY));
}

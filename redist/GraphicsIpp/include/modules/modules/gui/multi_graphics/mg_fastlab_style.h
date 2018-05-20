#ifndef Q_MG_FASTLAB_STYLE_H
#define Q_MG_FASTLAB_STYLE_H

#include "mg_settings.h"
#include <QColor>
class QSettings;

class Q_MG_Style : public Q_MG_Settings
{
public:
	Q_MG_Style()
	{
		BackGroundBrush_BeginColor =			QColor(0,0,0);
		BackGroundBrush_MidColor =				QColor(0,0,0);
		BackGroundBrush_EndColor =				QColor(0,0,0);

		BackGrid_SubLinesColor =				QColor(0,40,0);
		BackGrid_BaseLinesColor =				QColor(0,60,0);
		BackGrid_HightLinesColor =				QColor(0,60,0);

		GridBorder_LineColor =					QColor(0,110,110);
		GridBorder_BeginColor =					QColor(0, 0, 0);
		GridBorder_MidColor =					QColor(0, 0, 0);
		GridBorder_EndColor =					QColor(0, 0, 0);
		GridText_Color =						QColor(155,255,255);
		GridConer_Color =						QColor(0, 0, 0);

		MouseCursor_Color =						QColor(200,200,200);
		MouseCursor_GlowColor =					QColor(255,255,255,100);
		MouseCursor_MiniCursorColor =			QColor(250,250,250);
		MouseCursor_TextColor =					QColor(220,255,255);

		SelectionFillColor =					QColor(150,150,255,100);
		SelectionBorderColor =					QColor(0,190,255);
		DeselectionFillColor =					QColor(255,50,50,100);
		DeselectionBorderColor =				QColor(255,50,50);

		StaticAreaFillColor =					QColor(255,100,0,50);
		DeStaticAreaFillColor =					QColor(255,100,0,50);


		TextDecorate_MidLineColor0 =			QColor(0,190,255);
		TextDecorate_MidLineColor1 =			QColor(200,255,255);
		TextDecorate_MidLineColor2 =			QColor(255,50,50);
		TextDecorate_MidLine_TextFillColor =	QColor(0,100,100,100);
		TextDecorate_MidLine_TextBorderColor =	QColor(200,255,255,170);
		TextDecorate_MidLine_TextColor =		QColor(255,255,255);
		TextDecorate_AdditionLinesColor =		QColor(100,190,255);
		TextDecorate_BaseLineColor0 =			QColor(0,190,255);
		TextDecorate_BaseLineColor1 =			QColor(200,255,255);
		TextDecorate_BaseLineColor2 =			QColor(255,50,50);
		TextDecorate_BaseFillColor0 =			QColor(0,0,150,100);
		TextDecorate_BaseFillColor1 =			QColor(0,100,100,100);
		TextDecorate_BaseFillColor2 =			QColor(50,0,0,100);
		TextDecorate_BaseTextColor =			QColor(255,255,255);

		Marker_Color =							QColor(200,255,255);
		Marker_GlowColor =						QColor(0,255,255,100);
		Marker_TextFillColor =					QColor(0,100,100,100);
		Marker_TextColor =						QColor(255,255,255);

		Material_GrafColor_1 =					QColor(50,200,50);
		Material_GrafColor_2 =					QColor(50,200,200);
		Material_GrafColor_3 =					QColor(80,80,220);
		Material_BorderPointColor_1 =			QColor(0,255,0);
		Material_BorderPointColor_2 =			QColor(0,255,255);
		Material_BorderPointColor_3 =			QColor(0,0,255);
		Material_FillPointColor_1 =				QColor(0,255,0,100);
		Material_FillPointColor_2 =				QColor(50,255,255,100);
		Material_FillPointColor_3 =				QColor(0,0,255,100);

		InfoWidget_Back_BeginColor =			QColor(0,100,100,150);
		InfoWidget_Back_MidColor =				QColor(0,0,0,200);
		InfoWidget_Border_Color =				GridBorder_LineColor;
		InfoWidget_Text_Color =					QColor(100,250,250);
		InfoWidget_ProgressBar_EmptyFillColor = QColor(0,100,100);
		InfoWidget_ProgressBar_FullFillColor =  QColor(0,200,200);
		InfoWidget_ProgressBar_HightColor =		QColor(0,255,255);

		ToolTipStyleSheet = "";//{ background: qlineargradient(spread:reflect, x1:0.5, y1:0, x2:0.5, y2:0.5, stop:0 rgba(0, 100, 100, 255), stop:0.5 rgba(0, 0, 0, 255), stop:1 rgba(0, 0, 0, 255)); color: rgb(220,230,230); border: 1px solid ; border-color: rgb(0, 130, 130); }";

		StrangeCross_LineColor =				QColor(0,200,150);
		StrangeCross_TextFillColor =			QColor(0,0,0,100);
		StrangeCross_TextColor =				QColor(210,250,250);



		loadSettings();
	}
	~Q_MG_Style() {};
};




#endif //Q_MG_FASTLAB_STYLE_H

#ifndef Q_MG_SETTINGS_H
#define Q_MG_SETTINGS_H

#include <QColor>
class QSettings;

class Q_MG_Settings 
{

public:
	Q_MG_Settings(QSettings* _settings = NULL);
	~Q_MG_Settings() {};

	void loadSettings();
	void saveSettings(bool AutoSyncIni = true);

	QColor BackGroundBrush_BeginColor;
	QColor BackGroundBrush_MidColor;
	QColor BackGroundBrush_EndColor;

	QColor BackGrid_SubLinesColor;
	QColor BackGrid_BaseLinesColor;
	QColor BackGrid_HightLinesColor;

	QColor GridBorder_LineColor;
	QColor GridBorder_BeginColor;
	QColor GridBorder_MidColor;
	QColor GridBorder_EndColor;
	QColor GridText_Color;
	QColor GridConer_Color;

	QColor MouseCursor_Color;
	QColor MouseCursor_GlowColor;
	QColor MouseCursor_MiniCursorColor;
	QColor MouseCursor_TextColor;

	QColor SelectionFillColor;
	QColor SelectionBorderColor;
	QColor DeselectionFillColor;
	QColor DeselectionBorderColor;

	QColor StaticAreaFillColor;
	QColor DeStaticAreaFillColor;

	//0-выделение 1-маркеры , 2 - deselect
	QColor TextDecorate_MidLineColor0;
	QColor TextDecorate_MidLineColor1;
	QColor TextDecorate_MidLineColor2;
	QColor TextDecorate_MidLine_TextFillColor;
	QColor TextDecorate_MidLine_TextBorderColor;
	QColor TextDecorate_MidLine_TextColor;
	QColor TextDecorate_AdditionLinesColor;
	QColor TextDecorate_BaseLineColor0;
	QColor TextDecorate_BaseLineColor1;
	QColor TextDecorate_BaseLineColor2;
	QColor TextDecorate_BaseFillColor0;
	QColor TextDecorate_BaseFillColor1;
	QColor TextDecorate_BaseFillColor2;
	QColor TextDecorate_BaseTextColor;

	QColor Marker_Color;
	QColor Marker_GlowColor;
	QColor Marker_TextFillColor;
	QColor Marker_TextColor;

	QColor Material_GrafColor_1;
	QColor Material_GrafColor_2;
	QColor Material_GrafColor_3;
	QColor Material_BorderPointColor_1;
	QColor Material_BorderPointColor_2;
	QColor Material_BorderPointColor_3;
	QColor Material_FillPointColor_1;
	QColor Material_FillPointColor_2;
	QColor Material_FillPointColor_3;


	QColor InfoWidget_Back_BeginColor;
	QColor InfoWidget_Back_MidColor;
	QColor InfoWidget_Text_Color;
	QColor InfoWidget_Border_Color;
	QColor InfoWidget_ProgressBar_EmptyFillColor;
	QColor InfoWidget_ProgressBar_FullFillColor;
	QColor InfoWidget_ProgressBar_HightColor;

	QString ToolTipStyleSheet;

	QColor StrangeCross_LineColor;
	QColor StrangeCross_TextColor;
	QColor StrangeCross_TextFillColor;

private:
	QSettings* settings;

};


#endif //Q_MG_SETTINGS_H

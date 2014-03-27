#include "../../../stdafx.h"
#include "mg_settings.h"
#include <QSettings>

Q_MG_Settings::Q_MG_Settings( QSettings* _settings )
{
	settings = _settings;
}

void Q_MG_Settings::saveSettings(bool AutoSyncIni)
{
	if (settings == NULL) {qCritical("Can't save settings. File not open.");return;}
	settings->beginGroup("Q_MG_Settings");
	settings->setValue("BackGroundBrush_BeginColor",BackGroundBrush_BeginColor);
	settings->setValue("BackGroundBrush_MidColor",BackGroundBrush_MidColor);
	settings->setValue("BackGroundBrush_EndColor",BackGroundBrush_EndColor);
	
	settings->setValue("BackGrid_SubLinesColor",BackGrid_SubLinesColor);
	settings->setValue("BackGrid_BaseLinesColor",BackGrid_BaseLinesColor);
	settings->setValue("BackGrid_HightLinesColor",BackGrid_HightLinesColor);
	
	settings->setValue("GridBorder_LineColor",GridBorder_LineColor);
	settings->setValue("GridBorder_BeginColor",GridBorder_BeginColor);
	settings->setValue("GridBorder_MidColor",GridBorder_MidColor);
	settings->setValue("GridBorder_EndColor",GridBorder_EndColor);
	settings->setValue("GridText_Color",GridText_Color);
	settings->setValue("GridConer_Color",GridConer_Color);
	
	settings->setValue("MouseCursor_Color",MouseCursor_Color);
	settings->setValue("MouseCursor_GlowColor",MouseCursor_GlowColor);
	settings->setValue("MouseCursor_MiniCursorColor",MouseCursor_MiniCursorColor);
	settings->setValue("MouseCursor_TextColor",MouseCursor_TextColor);

	settings->setValue("SelectionFillColor",SelectionFillColor);
	settings->setValue("SelectionBorderColor",SelectionBorderColor);
	settings->setValue("DeselectionFillColor",DeselectionFillColor);
	settings->setValue("DeselectionBorderColor",DeselectionBorderColor);

	settings->setValue("StaticAreaFillColor", StaticAreaFillColor);
	settings->setValue("DeStaticAreaFillColor", DeStaticAreaFillColor);
	
	settings->setValue("TextDecorate_MidLineColor0",TextDecorate_MidLineColor0);
	settings->setValue("TextDecorate_MidLineColor1",TextDecorate_MidLineColor1);
	settings->setValue("TextDecorate_MidLineColor2",TextDecorate_MidLineColor2);
	settings->setValue("TextDecorate_MidLine_TextFillColor",TextDecorate_MidLine_TextFillColor);
	settings->setValue("TextDecorate_MidLine_TextBorderColor",TextDecorate_MidLine_TextBorderColor);
	settings->setValue("TextDecorate_MidLine_TextColor",TextDecorate_MidLine_TextColor);
	settings->setValue("TextDecorate_AdditionLinesColor",TextDecorate_AdditionLinesColor);
	settings->setValue("TextDecorate_BaseLineColor0",TextDecorate_BaseLineColor0);
	settings->setValue("TextDecorate_BaseLineColor1",TextDecorate_BaseLineColor1);
	settings->setValue("TextDecorate_BaseLineColor2",TextDecorate_BaseLineColor2);
	settings->setValue("TextDecorate_BaseFillColor0",TextDecorate_BaseFillColor0);
	settings->setValue("TextDecorate_BaseFillColor1",TextDecorate_BaseFillColor1);
	settings->setValue("TextDecorate_BaseFillColor2",TextDecorate_BaseFillColor2);
	settings->setValue("TextDecorate_BaseTextColor",TextDecorate_BaseTextColor);
	
	settings->setValue("Marker_Color",Marker_Color);
	settings->setValue("Marker_GlowColor",Marker_GlowColor);
	settings->setValue("Marker_TextFillColor",Marker_TextFillColor);
	settings->setValue("Marker_TextColor",Marker_TextColor);
	
	settings->setValue("Material_GrafColor_1",Material_GrafColor_1);
	settings->setValue("Material_GrafColor_2",Material_GrafColor_2);
	settings->setValue("Material_GrafColor_3",Material_GrafColor_3);
	settings->setValue("Material_BorderPointColor_1",Material_BorderPointColor_1);
	settings->setValue("Material_BorderPointColor_2",Material_BorderPointColor_2);
	settings->setValue("Material_BorderPointColor_3",Material_BorderPointColor_2);
	settings->setValue("Material_FillPointColor_1",Material_FillPointColor_1);
	settings->setValue("Material_FillPointColor_2",Material_FillPointColor_2);
	settings->setValue("Material_FillPointColor_3",Material_FillPointColor_3);
	settings->setValue("InfoWidget_Back_BeginColor",InfoWidget_Back_BeginColor);
	settings->setValue("InfoWidget_Back_MidColor",InfoWidget_Back_MidColor);
	settings->setValue("InfoWidget_Text_Color",InfoWidget_Text_Color);
	settings->setValue("InfoWidget_Border_Color",InfoWidget_Border_Color);
	settings->setValue("InfoWidget_ProgressBar_EmptyFillColor",InfoWidget_ProgressBar_EmptyFillColor);
	settings->setValue("InfoWidget_ProgressBar_FullFillColor",InfoWidget_ProgressBar_FullFillColor);
	settings->setValue("InfoWidget_ProgressBar_HightColor",InfoWidget_ProgressBar_HightColor);

	settings->setValue("ToolTipStyleSheet",ToolTipStyleSheet);
	settings->setValue("StrangeCross_LineColor",StrangeCross_LineColor);
	settings->setValue("StrangeCross_TextFillColor",StrangeCross_TextFillColor);
	settings->setValue("StrangeCross_TextColor",StrangeCross_TextColor);
	settings->endGroup();
	if (AutoSyncIni) settings->sync();
}

void Q_MG_Settings::loadSettings()
{
	if (settings == NULL) return;
	settings->beginGroup("Q_MG_Settings");
		
	BackGroundBrush_BeginColor 				= settings->value("BackGroundBrush_BeginColor",BackGroundBrush_BeginColor).value<QColor>();
	BackGroundBrush_MidColor 				= settings->value("BackGroundBrush_MidColor",BackGroundBrush_MidColor).value<QColor>();
	BackGroundBrush_EndColor 				= settings->value("BackGroundBrush_EndColor",BackGroundBrush_EndColor).value<QColor>();		

	BackGrid_SubLinesColor 					= settings->value("BackGrid_SubLinesColor",BackGrid_SubLinesColor).value<QColor>();	
	BackGrid_BaseLinesColor					= settings->value("BackGrid_BaseLinesColor",BackGrid_BaseLinesColor).value<QColor>();
	BackGrid_HightLinesColor 				= settings->value("BackGrid_HightLinesColor",BackGrid_HightLinesColor).value<QColor>();

	GridBorder_LineColor 					= settings->value("GridBorder_LineColor",GridBorder_LineColor).value<QColor>();
	GridBorder_BeginColor 					= settings->value("GridBorder_BeginColor",GridBorder_BeginColor).value<QColor>();
	GridBorder_MidColor						= settings->value("GridBorder_MidColor",GridBorder_MidColor).value<QColor>();
	GridBorder_EndColor						= settings->value("GridBorder_EndColor",GridBorder_EndColor).value<QColor>();
	GridText_Color							= settings->value("GridText_Color",GridText_Color).value<QColor>();
	GridConer_Color							= settings->value("GridConer_Color",GridConer_Color).value<QColor>();

	MouseCursor_Color						= settings->value("MouseCursor_Color",MouseCursor_Color).value<QColor>();
	MouseCursor_GlowColor					= settings->value("MouseCursor_GlowColor",MouseCursor_GlowColor).value<QColor>();
	MouseCursor_MiniCursorColor				= settings->value("MouseCursor_MiniCursorColor",MouseCursor_MiniCursorColor).value<QColor>();
	MouseCursor_TextColor					= settings->value("MouseCursor_TextColor",MouseCursor_TextColor).value<QColor>();

	SelectionFillColor						= settings->value("SelectionFillColor",SelectionFillColor).value<QColor>();
	SelectionBorderColor					= settings->value("SelectionBorderColor",SelectionBorderColor).value<QColor>();
	DeselectionFillColor					= settings->value("DeselectionFillColor",DeselectionFillColor).value<QColor>();
	DeselectionBorderColor					= settings->value("DeselectionBorderColor",DeselectionBorderColor).value<QColor>();

	StaticAreaFillColor						= settings->value("StaticAreaFillColor", StaticAreaFillColor).value<QColor>();
	DeStaticAreaFillColor					= settings->value("DeStaticAreaFillColor", DeStaticAreaFillColor).value<QColor>();

	TextDecorate_MidLineColor0				= settings->value("TextDecorate_MidLineColor0",TextDecorate_MidLineColor0).value<QColor>();
	TextDecorate_MidLineColor1				= settings->value("TextDecorate_MidLineColor1",TextDecorate_MidLineColor1).value<QColor>();
	TextDecorate_MidLineColor2				= settings->value("TextDecorate_MidLineColor2",TextDecorate_MidLineColor2).value<QColor>();
	TextDecorate_MidLine_TextFillColor		= settings->value("TextDecorate_MidLine_TextFillColor",TextDecorate_MidLine_TextFillColor).value<QColor>();
	TextDecorate_MidLine_TextBorderColor	= settings->value("TextDecorate_MidLine_TextBorderColor",TextDecorate_MidLine_TextBorderColor).value<QColor>();
	TextDecorate_MidLine_TextColor			= settings->value("TextDecorate_MidLine_TextColor",TextDecorate_MidLine_TextColor).value<QColor>();
	TextDecorate_AdditionLinesColor			= settings->value("TextDecorate_AdditionLinesColor",TextDecorate_AdditionLinesColor).value<QColor>();
	TextDecorate_BaseLineColor0				= settings->value("TextDecorate_BaseLineColor0",TextDecorate_BaseLineColor0).value<QColor>();
	TextDecorate_BaseLineColor1				= settings->value("TextDecorate_BaseLineColor1",TextDecorate_BaseLineColor1).value<QColor>();
	TextDecorate_BaseLineColor2				= settings->value("TextDecorate_BaseLineColor2",TextDecorate_BaseLineColor2).value<QColor>();
	TextDecorate_BaseFillColor0				= settings->value("TextDecorate_BaseFillColor0",TextDecorate_BaseFillColor0).value<QColor>();
	TextDecorate_BaseFillColor1				= settings->value("TextDecorate_BaseFillColor1",TextDecorate_BaseFillColor1).value<QColor>();
	TextDecorate_BaseFillColor2				= settings->value("TextDecorate_BaseFillColor2",TextDecorate_BaseFillColor2).value<QColor>();
	TextDecorate_BaseTextColor				= settings->value("TextDecorate_BaseTextColor",TextDecorate_BaseTextColor).value<QColor>();

	Marker_Color							= settings->value("Marker_Color",Marker_Color).value<QColor>();
	Marker_GlowColor						= settings->value("Marker_GlowColor",Marker_GlowColor).value<QColor>();
	Marker_TextFillColor					= settings->value("Marker_TextFillColor",Marker_TextFillColor).value<QColor>();
	Marker_TextColor						= settings->value("Marker_TextColor",Marker_TextColor).value<QColor>();

	Material_GrafColor_1					= settings->value("Material_GrafColor_1",Material_GrafColor_1).value<QColor>();
	Material_GrafColor_2					= settings->value("Material_GrafColor_1",Material_GrafColor_2).value<QColor>();
	Material_GrafColor_3					= settings->value("Material_GrafColor_1",Material_GrafColor_3).value<QColor>();
	Material_BorderPointColor_1				= settings->value("Material_BorderPointColor_1",Material_BorderPointColor_1).value<QColor>();
	Material_BorderPointColor_2				= settings->value("Material_BorderPointColor_2",Material_BorderPointColor_2).value<QColor>();
	Material_BorderPointColor_3				= settings->value("Material_BorderPointColor_3",Material_BorderPointColor_3).value<QColor>();
	Material_FillPointColor_1					= settings->value("Material_FillPointColor_1",Material_FillPointColor_1).value<QColor>();
	Material_FillPointColor_2					= settings->value("Material_FillPointColor_2",Material_FillPointColor_2).value<QColor>();
	Material_FillPointColor_3					= settings->value("Material_FillPointColor_3",Material_FillPointColor_3).value<QColor>();
	
	InfoWidget_Back_BeginColor				= settings->value("InfoWidget_Back_BeginColor",InfoWidget_Back_BeginColor).value<QColor>();
	InfoWidget_Back_MidColor				= settings->value("InfoWidget_Back_MidColor",InfoWidget_Back_MidColor).value<QColor>();
	InfoWidget_Text_Color 					= settings->value("InfoWidget_Text_Color",InfoWidget_Text_Color).value<QColor>();
	InfoWidget_Border_Color 				= settings->value("InfoWidget_Border_Color",InfoWidget_Border_Color).value<QColor>();

	ToolTipStyleSheet 						= settings->value("ToolTipStyleSheet",ToolTipStyleSheet).toString();

	StrangeCross_LineColor 					= settings->value("StrangeCross_LineColor", StrangeCross_LineColor).value<QColor>();
	StrangeCross_TextFillColor				= settings->value("StrangeCross_TextFillColor",StrangeCross_TextFillColor).value<QColor>();
	StrangeCross_TextColor 					= settings->value("StrangeCross_TextColor", StrangeCross_TextColor).value<QColor>();

	InfoWidget_ProgressBar_EmptyFillColor	= settings->value("InfoWidget_ProgressBar_EmptyFillColor", StrangeCross_LineColor).value<QColor>();
	InfoWidget_ProgressBar_FullFillColor	= settings->value("InfoWidget_ProgressBar_FullFillColor", StrangeCross_LineColor).value<QColor>();
	InfoWidget_ProgressBar_HightColor		= settings->value("InfoWidget_ProgressBar_HightColor", StrangeCross_LineColor).value<QColor>();

	settings->endGroup();
}


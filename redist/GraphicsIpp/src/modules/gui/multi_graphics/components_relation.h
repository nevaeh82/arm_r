#ifndef COMPONENTSRELATION_H
#define COMPONENTSRELATION_H

#include <QObject>
#include "../../../modules/gui/multi_graphics_sonogram_interface/sonogram_interface.h"
#include "../../../modules/gui/multi_graphics_spectrum_interface/mg_spectrum_interface.h"
#include "../../../modules/filesystem/filesystem.h"


class ComponentsRelation : public QObject
{
	Q_OBJECT
public:
	typedef SonogramMaterial::FileType  FileType;

	ComponentsRelation(SonogramInterface* sonogram, Q_MG_SpectrumInterface* spectrum, QObject* parent = NULL);
	~ComponentsRelation();

	bool Setup(const FileType&, int fft_Order, QByteArray color_level_data = QByteArray(), QString sectrumVLabel = tr("дБ"));
	void Reset();
	void SetZeroFrequencyHz(double val);

	void SpectrumShowOnlyMaxHold();
	void SpectrumShowOnlyMeanHold();
	void SpectrumShowAllHolds();

	//выделение
	void SetMultySelectEnabled(bool enabled);//при вызове зачистит все текущее выделение
	void SetButtonCtrlClickIsSingle(bool is_single = true);//по умолчанию с зажатым Ctrl делается множественное выделение (false), с установкой флага все меняется на оборот, без Ctrl multy с Ctrl single
	//////////////////////////////////////////////////////////////////////////

private:
	QPointer<SonogramInterface> sonogram_;
	QPointer<Q_MG_SpectrumInterface> spectrum_;
	QString sectrumVLabel_;
	bool spectrumShowMaxHold_;
	bool spectrumShowMeanHold_;
	bool multy_select_on_;//false

	FFTLine_32f cur_mean_spectrum_;
	FFTLine		cur_max_spectrum_;

	double	prev_sended_start_hz_on_spectrum_view_chenge_;//чтобы не отправлять в сонограмму одни и теже значения когда спектр скейлится по Y
	double	prev_sended_end_hz_on_spectrum_view_chenge_;//чтобы не отправлять в сонограмму одни и теже значения когда спектр скейлится по Y

private slots:
	void my_SonogramViewPortChanged(double start_hz, double start_smpl, double end_hz, double end_smpl);
	void my_SpectrumViewPortChanged(double start_hz, double start_Y, double end_hz, double end_Y);
	void my_SonogramScreenSpectrumsChanged(FFTLine_32f mean_spectrum, FFTLine max_spectrum);

	void my_SonogramMarkerChange(QVector<double> Mark,bool isVertical);
	void my_SpectrumMarkerChange(QVector<double> Mark,bool isVertical);

	void my_SonogramSelectionChangeTo(double x1, double y1, double x2, double y2);
	void my_SpectrumSelectionChangeTo(double x1, double y1, double x2, double y2);

	void my_SpectrumSelectionFinished(double x1, double y1, double x2, double y2);
	void my_SonorgamSelectionFinished(double x1, double y1, double x2, double y2);
	void my_SonorgamDeselectionFinished();
	void my_SpectrumDeselectionFinished();

	void my_SpectrumSelectionCleared();
	void my_SonogramSelectionCleared();
};


#endif //ComponentsRelation_H

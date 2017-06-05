#ifndef RECORDSIGNALSETTINGS_H
#define RECORDSIGNALSETTINGS_H

#include <QWidget>

namespace Ui {
class RecordSignalSettings;
}

class RecordSignalSettings : public QWidget
{
	Q_OBJECT

public:
	explicit RecordSignalSettings(QWidget *parent = 0);
	~RecordSignalSettings();

	struct RecordSignalSettingsData
	{
		bool screenshot;
		bool record;
	};

private:
	Ui::RecordSignalSettings *ui;

private slots:
	void saveSettings();
	void init();
};

#endif // RECORDSIGNALSETTINGS_H

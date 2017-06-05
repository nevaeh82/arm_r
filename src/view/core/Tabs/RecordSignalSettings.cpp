#include "RecordSignalSettings.h"
#include "ui_RecordSignalSettings.h"

#include <QSettings>

RecordSignalSettings::RecordSignalSettings(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::RecordSignalSettings)
{
	ui->setupUi(this);

	connect(ui->cbScreenshot, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
	connect(ui->cbRecord, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));

	connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
	connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(close()));
}

RecordSignalSettings::~RecordSignalSettings()
{
	delete ui;
}

void RecordSignalSettings::init()
{
	QSettings aSettings("./Tabs/record.ini", QSettings::IniFormat);
	aSettings.setIniCodec("UTF-8");

	ui->cbScreenshot->setChecked(aSettings.value("Common/screenshot", true).toBool());
	ui->cbRecord->setChecked(aSettings.value("Common/record", false).toBool());
}

void RecordSignalSettings::saveSettings()
{
	QSettings aSettings("./Tabs/record.ini", QSettings::IniFormat);
	aSettings.setIniCodec("UTF-8");

	aSettings.setValue("Common/screenshot", ui->cbScreenshot->isChecked());
	aSettings.setValue("Common/record", ui->cbRecord->isChecked());
}

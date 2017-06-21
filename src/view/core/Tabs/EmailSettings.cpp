#include "EmailSettings.h"
#include "ui_EmailSettings.h"

EmailSettings::EmailSettings(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::EmailSettings)
{
	ui->setupUi(this);

	init();

	connect(ui->pbOk, SIGNAL(clicked(bool)), this, SLOT(close()));
	connect(ui->pbCancel, SIGNAL(clicked(bool)), this, SLOT(close()));

	connect(ui->pbOk, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
	connect(ui->pbCancel, SIGNAL(clicked(bool)), this, SLOT(init()));

}

EmailSettings::~EmailSettings()
{
	delete ui;
}

SMSComPortDialog *EmailSettings::getSmsDialog()
{
	return ui->smsWidget;
}

void EmailSettings::init()
{
	QSettings aSettings("./Tabs/mail.ini", QSettings::IniFormat);
	aSettings.setIniCodec("UTF-8");

	m_isSend=aSettings.value("Common/isSend").toBool();
	m_isMoving = aSettings.value("Common/isMoving").toBool();
	m_isStanding = aSettings.value("Common/isStanding").toBool();
	m_isUnknown = aSettings.value("Common/isUnknown").toBool();
	m_isDopler = aSettings.value("Common/isDopler").toBool();

	m_localSettings.host = aSettings.value("Connection/host").toString();
	m_localSettings.port = aSettings.value("Connection/port").toInt();
	m_localSettings.connetionType = aSettings.value("Connection/connectionType").toInt();

	m_localSettings.userName = aSettings.value("Auth/user").toString();
	m_localSettings.passwd = aSettings.value("Auth/password").toString();
	m_localSettings.authType = aSettings.value("Auth/authMethod").toInt();

	m_localMailList = aSettings.value("Address/addressList").toString().split(",");

	QSettings aSettingsRemote("./Tabs/remotemail.ini", QSettings::IniFormat);
	aSettingsRemote.setIniCodec("UTF-8");

	m_remoteSettings.host = aSettingsRemote.value("Connection/host").toString();
	m_remoteSettings.port = aSettingsRemote.value("Connection/port").toInt();
	m_remoteSettings.connetionType = aSettingsRemote.value("Connection/connectionType").toInt();

	m_remoteSettings.userName = aSettingsRemote.value("Auth/user").toString();
	m_remoteSettings.passwd = aSettingsRemote.value("Auth/password").toString();
	m_remoteSettings.authType = aSettingsRemote.value("Auth/authMethod").toInt();

	m_remoteMailList = aSettingsRemote.value("Address/addressList").toString().split(",");

	initView();

	ui->smsWidget->slotClose();
}

void EmailSettings::saveSettings()
{
	QSettings aSettings("./Tabs/mail.ini", QSettings::IniFormat);
	aSettings.setIniCodec("UTF-8");

	aSettings.setValue("Common/isSend", ui->cbEnable->isChecked());
	aSettings.setValue("Common/isMoving", ui->cbMoving->isChecked());
	aSettings.setValue("Common/isStanding", ui->cbStanding->isChecked());
	aSettings.setValue("Common/isDopler", ui->cbDopler->isChecked());
	aSettings.setValue("Common/isUnknown", ui->cbUnknown->isChecked());

	aSettings.setValue("Connection/host", ui->leRemoteHost_2->text());
	aSettings.setValue("Connection/port", ui->sbRemotePort_2->value());
	aSettings.setValue("Connection/connectionType", ui->cbRemoteType_2->currentIndex());

	aSettings.setValue("Auth/user", ui->leRemoteUser_2->text());
	aSettings.setValue("Auth/password", ui->leRemotePassword_2->text());
	aSettings.setValue("Auth/authMethod", ui->leRemoteAuthPlain_2->currentIndex());

	aSettings.setValue("Address/addressList", ui->leLocalMails->text());

	QSettings aSettingsRemote("./Tabs/remotemail.ini", QSettings::IniFormat);
	aSettingsRemote.setIniCodec("UTF-8");

	aSettingsRemote.setValue("Connection/host", ui->leRemoteHost->text());
	aSettingsRemote.setValue("Connection/port", ui->sbRemotePort->value());
	aSettingsRemote.setValue("Connection/connectionType", ui->cbRemoteType->currentIndex());

	aSettingsRemote.setValue("Auth/user", ui->leRemoteUser->text());
	aSettingsRemote.setValue("Auth/password", ui->leRemotePassword->text());
	aSettingsRemote.setValue("Auth/authMethod", ui->leRemoteAuthPlain->currentIndex());

	aSettingsRemote.setValue("Address/addressList", ui->leRemoteMails->text());

	init();

	ui->smsWidget->slotAccept();

	emit signalMailSettingsUpdate();
}

void EmailSettings::initView()
{
	ui->cbEnable->setChecked(m_isSend);
	ui->cbUnknown->setChecked(m_isUnknown);
	ui->cbStanding->setChecked(m_isStanding);
	ui->cbMoving->setChecked(m_isMoving);
	ui->cbDopler->setChecked(m_isDopler);

	ui->leRemoteHost_2->setText(m_localSettings.host);
	ui->sbRemotePort_2->setValue(m_localSettings.port);
	ui->cbRemoteType_2->setCurrentIndex(m_localSettings.connetionType);

	ui->leRemoteUser_2->setText(m_localSettings.userName);
	ui->leRemotePassword_2->setText(m_localSettings.passwd);
	ui->leRemoteAuthPlain_2->setCurrentIndex(m_localSettings.authType);

	ui->leLocalMails->setText(m_localMailList.join(","));

	// --

	ui->leRemoteHost->setText(m_remoteSettings.host);
	ui->sbRemotePort->setValue(m_remoteSettings.port);
	ui->cbRemoteType->setCurrentIndex(m_remoteSettings.connetionType);

	ui->leRemoteUser->setText(m_remoteSettings.userName);
	ui->leRemotePassword->setText(m_remoteSettings.passwd);
	ui->leRemoteAuthPlain->setCurrentIndex(m_remoteSettings.authType);

	ui->leRemoteMails->setText(m_remoteMailList.join(","));

}

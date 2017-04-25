#ifndef EMAILSETTINGS_H
#define EMAILSETTINGS_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class EmailSettings;
}

struct MailSettings
{
	QString host;
	int port;
	int connetionType;

	QString userName;
	QString passwd;
	int authType;
};


class EmailSettings : public QWidget
{
	Q_OBJECT

public:
	explicit EmailSettings(QWidget *parent = 0);
	~EmailSettings();

	bool isSend() {return m_isSend;}
	bool isStanding() {return m_isStanding;}
	bool isMoving() {return m_isMoving;}
	bool isUnknown() {return m_isUnknown;}

	QStringList remoteMailList() {return m_remoteMailList;}
	QStringList localMailList() {return m_localMailList;}

	MailSettings remoteMailSettings() {return m_remoteSettings;}
	MailSettings localMailSettings() {return m_localSettings;}

private:
	Ui::EmailSettings *ui;

	MailSettings m_localSettings;
	MailSettings m_remoteSettings;

	QStringList m_localMailList;
	QStringList m_remoteMailList;

	bool m_isSend;
	bool m_isStanding;
	bool m_isUnknown;
	bool m_isMoving;

private slots:
	void init();
	void saveSettings();

	void initView();

signals:
	void signalMailSettingsUpdate();
};

#endif // EMAILSETTINGS_H

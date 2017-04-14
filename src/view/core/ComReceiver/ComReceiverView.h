#ifndef COMRECEIVERVIEW_H
#define COMRECEIVERVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QTimer>

namespace Ui {
class ComReceiverView;
}

class ComReceiverView : public QWidget
{
	Q_OBJECT

public:
	explicit ComReceiverView(QWidget *parent = 0);
	~ComReceiverView();

	bool isNewPrm() const;

	void setPrmData(int freq);
	void setPrmDataLong(int freq, int att, int filter, int temerature, int input);

	void setPdData(int power);
	void getBoards(QList<QString> &boards);

private:
	Ui::ComReceiverView *ui;

	QString m_redStyle;
	QString m_greenStyle;

	QTimer* m_prmTimer;
	QTimer* m_pdTimer;


public slots:
	void setComConnectState(bool on);
	void setKtrConnectState(bool on);

	void onSetPrmFreq();
	void onSetPrmAtt();
	void onSetPrmFilter();

	void onSetPdFreq();
	void onSetPdAtt();

private slots:
	void onConnectCom(bool on);
	void onConnectKtr(bool on);

	void onPrmTimeout();
	void onPdTimeout();

	void onNewPrmChecked(int isChecked);

	void onConnectionMode();

	void onAddBoard();

	void onRemoveBoard();

	void onComDisconnect();
signals:
	void signalComConnect(QString, bool);
	void signalKtrConnect(QString, bool);

	void signalSetPrmFreq(int);
	void signalSetPrmAtt(int);
	void signalSetPrmFilter(int);

	void signalSetPdFreq(int);
	void signalSetPdAtt(int);
};

#endif // COMRECEIVERVIEW_H

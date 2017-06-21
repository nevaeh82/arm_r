#include "ComReceiverView.h"
#include "ui_ComReceiverView.h"

#define LINE_TIMEOUT 10000

ComReceiverView::ComReceiverView(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ComReceiverView)
{
	ui->setupUi(this);
	connect(ui->pbComConnect, SIGNAL(toggled(bool)), this, SLOT(onConnectCom(bool)));
	connect(ui->pbKtrConnect, SIGNAL(toggled(bool)), this, SLOT(onConnectKtr(bool)));

	connect(ui->pbUpPrmFreq, SIGNAL(clicked(bool)), this, SLOT(onSetPrmFreq()));
	connect(ui->pbUpPrmAtt, SIGNAL(clicked(bool)), this, SLOT(onSetPrmAtt()));
	connect(ui->pbUpPrmFilter, SIGNAL(clicked(bool)), this, SLOT(onSetPrmFilter()));

	connect(ui->pbPdFreq, SIGNAL(clicked(bool)), this, SLOT(onSetPdFreq()));
	connect(ui->pbPdAtt, SIGNAL(clicked(bool)), this, SLOT(onSetPdAtt()));

	connect(ui->isNewPrm, SIGNAL(stateChanged(int)), this, SLOT(onNewPrmChecked(int)));

	connect(ui->radioButtonCom, SIGNAL(clicked(bool)), this, SLOT(onConnectionMode()));
	connect(ui->radioButtonKtr, SIGNAL(clicked(bool)), this, SLOT(onConnectionMode()));

	connect(ui->addBoardPb, SIGNAL(clicked(bool)), this, SLOT(onAddBoard()));
	connect(ui->removeBoardPb, SIGNAL(clicked(bool)), this, SLOT(onRemoveBoard()));

//	ui->leAnswerAtt->setVisible(false);
//	ui->leAnswerFilter->setVisible(false);

	ui->groupBoxAddBoard->setVisible(false);

	//background-color: rgba(255, 0, 0, 150);
	//font: 75 10pt "MS Shell Dlg 2";

	m_redStyle = QString("QLineEdit{ background-color: rgba(255, 0, 0, 150); \
													  font: 75 10pt \"MS Shell Dlg 2\";}" );
	m_greenStyle = QString("QLineEdit{ background-color: rgba(0, 255, 0, 150); \
													  font: 75 10pt \"MS Shell Dlg 2\";}" );


	m_prmTimer = new QTimer(this);
	m_prmTimer->setInterval(LINE_TIMEOUT);

	m_pdTimer = new QTimer(this);
	m_pdTimer->setInterval(LINE_TIMEOUT);

	connect(m_prmTimer, SIGNAL(timeout()), this, SLOT(onPrmTimeout()));
	connect(m_pdTimer, SIGNAL(timeout()), this, SLOT(onPdTimeout()));

	this->adjustSize();
}

ComReceiverView::~ComReceiverView()
{
	delete ui;
}

bool ComReceiverView::isNewPrm() const
{
	return ui->isNewPrm->isChecked();
}

void ComReceiverView::setPrmData(int freq)
{
	setPrmDataLong(freq, 0,0,0,0);

	ui->isNewPrm->setChecked(false);
}

void ComReceiverView::setPrmDataLong(int freq, int att, int filter, int temerature, int input)
{
	ui->leAnswerFreq->setText(QString("%1 mhz").arg(freq));
	ui->leAnswerAtt->setText(QString("%1 db").arg(att));
	if(filter == 0) {
		ui->leAnswerFilter->setText(ui->prmFilter->itemText(0));
	} else if(filter == 1 || filter == 2) {
		ui->leAnswerFilter->setText(ui->prmFilter->itemText(filter-1));
	}
	ui->lblTempVal->setText(QString("%1 C").arg(temerature));
	ui->lblInputNumVal->setText(QString("%1 input").arg(input));

	ui->leAnswerAtt->setStyleSheet(m_greenStyle);
	ui->leAnswerFilter->setStyleSheet(m_greenStyle);
	ui->leAnswerFreq->setStyleSheet(m_greenStyle);

	if(temerature > 0) {
		ui->isNewPrm->setChecked(true);
	}

	m_prmTimer->start();
}

void ComReceiverView::setPdData(int value)
{
	double pow = 15 + ((value - 1670) / 19.82);
	pow = ((int)(pow * 10)) / 10.0;

	ui->leAnswerPDpower->setText( QString("%1 dBm").arg(pow) );
	ui->leAnswerPDpower->setStyleSheet(m_greenStyle);

	m_pdTimer->start();
}

void ComReceiverView::setComConnectState(bool on)
{
	if(!on) {
		ui->pbComConnect->blockSignals(true);
		ui->pbComConnect->setChecked(false);
		ui->lblConnectState->setText("Com disconnected");
		ui->pbComConnect->setText("Connect");
		ui->pbComConnect->blockSignals(false);
	} else {
		ui->lblConnectState->setText("Com Connected");
		ui->pbComConnect->setText("DISConnect");
	}
}

void ComReceiverView::setKtrConnectState(bool on)
{
	if(!on) {
		ui->pbComConnect->blockSignals(true);
		ui->pbComConnect->setChecked(false);
		ui->lblConnectState->setText("KTP disconnected");
		ui->pbKtrConnect->setText("Connect");
		ui->pbComConnect->blockSignals(false);
	} else {
		ui->lblConnectState->setText("KTP Connected");
		ui->pbKtrConnect->setText("DISConnect");
	}
}

void ComReceiverView::onSetPrmFreq()
{
	emit signalSetPrmFreq(ui->prmFreq->value());
}

void ComReceiverView::onSetPrmAtt()
{
	emit signalSetPrmAtt(ui->prmAtt->value());
}

void ComReceiverView::onSetPrmFilter()
{
	int value = ui->prmFilter->currentIndex();
	if(value) {
		value += 1;
	}

	emit signalSetPrmFilter(value);
}

void ComReceiverView::onSetPdFreq()
{
	emit signalSetPdFreq(ui->pdFreq->value());
}

void ComReceiverView::onSetPdAtt()
{
	emit signalSetPdAtt(ui->pdAtt->value());
}

void ComReceiverView::onConnectCom(bool on)
{
	emit signalComConnect(QString("COM%1").arg(ui->sbComPort->value()), on);
}

void ComReceiverView::onConnectKtr(bool on)
{

	emit signalKtrConnect(ui->ktrIpFrame->text(), on);
}

void ComReceiverView::onPrmTimeout()
{
	ui->leAnswerAtt->setStyleSheet(m_redStyle);
	ui->leAnswerFilter->setStyleSheet(m_redStyle);
	ui->leAnswerFreq->setStyleSheet(m_redStyle);

	m_prmTimer->stop();
}

void ComReceiverView::onPdTimeout()
{
	ui->leAnswerPDpower->setStyleSheet(m_redStyle);

	m_pdTimer->stop();
}

void ComReceiverView::onNewPrmChecked(int isChecked)
{
	if(isChecked == Qt::Checked) {
		ui->leAnswerAtt->setVisible(true);
		ui->leAnswerFilter->setVisible(true);
		ui->lblInputNum->setVisible(true);
		ui->lblInputNumVal->setVisible(true);
		ui->lblTemp->setVisible(true);
		ui->lblTempVal->setVisible(true);
	} else {
		ui->leAnswerAtt->setVisible(false);
		ui->leAnswerFilter->setVisible(false);
		ui->lblInputNum->setVisible(false);
		ui->lblInputNumVal->setVisible(false);
		ui->lblTemp->setVisible(false);
		ui->lblTempVal->setVisible(false);
	}

	this->adjustSize();
}

void ComReceiverView::onConnectionMode()
{
	if(ui->radioButtonCom->isChecked()) {
		ui->groupBoxCom->setVisible(true);
		ui->groupBoxAddBoard->setVisible(false);

//		ui->leAnswerAtt->setVisible(true);
//		ui->leAnswerAtt->setVisible(true);
//		ui->leAnswerAtt->setVisible(true);
//		ui->leAnswerPDpower->setVisible(true);
	} else {
		ui->groupBoxCom->setVisible(false);
		ui->groupBoxAddBoard->setVisible(true);

//		ui->leAnswerAtt->setVisible(false);
//		ui->leAnswerAtt->setVisible(false);
//		ui->leAnswerAtt->setVisible(false);
//		ui->leAnswerPDpower->setVisible(false);
	}

	this->adjustSize();
}

void ComReceiverView::onAddBoard()
{
	QString board = ui->addBoardLe->text();

	if(board.isEmpty()) {
		return;
	}

	board.append("," + QString::number(ui->sbDevice->value()));

	if( board.isEmpty() || !ui->addBoardListWidget->findItems(board, Qt::MatchExactly).isEmpty() ) {
		return;
	}

	ui->addBoardListWidget->addItem( board );
	ui->addBoardListWidget->setCurrentRow(ui->addBoardListWidget->count()-1);
}

void ComReceiverView::onRemoveBoard()
{
	int currentRow = ui->addBoardListWidget->currentRow();

	if (currentRow >= 0) {
		QListWidgetItem* item = ui->addBoardListWidget->takeItem(currentRow);
		delete item;
	}
}

void ComReceiverView::onComDisconnect()
{
	setComConnectState(false);
}

void ComReceiverView::getBoards(QList<QString> &boards)
{
	for(int i = 0; i<ui->addBoardListWidget->count(); i++) {
		boards.append( ui->addBoardListWidget->item(i)->text() );
	}
}

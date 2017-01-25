#include "AnalysisResultWidget.h"

#include "ui_AnalysisResultWidget.h"

#include <QLabel>

#define COLUMN_COUNT 6

AnalysisResultWidget::AnalysisResultWidget(QWidget *parent):
	QWidget(parent),
	ui(new Ui::AnalysisResultWidget),
	m_currentIndex(-1)
{
	ui->setupUi(this);

	connect(ui->pbContinue, SIGNAL(clicked(bool)), this, SLOT(slotContinue(bool)));
	connect(ui->pbClose, SIGNAL(clicked(bool)), this, SLOT(slotClose(bool)));

	connect(ui->pbAdd, SIGNAL(clicked(bool)), this, SLOT(addSignal()));

	connect(ui->tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(onCellClicked(int,int)));
	connect(ui->tableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onItemClicked(QTableWidgetItem*)));
	connect(ui->tableWidget, SIGNAL(itemEntered(QTableWidgetItem*)), this, SLOT(onItemClicked(QTableWidgetItem*)));
	connect(ui->tableWidget, SIGNAL(itemActivated(QTableWidgetItem*)), this, SLOT(onItemClicked(QTableWidgetItem*)));
	connect(ui->tableWidget, SIGNAL(activated(QModelIndex)), this, SLOT(onItemActivated(QModelIndex)));
	connect(ui->tableWidget, SIGNAL(pressed(QModelIndex)), this, SLOT(onItemActivated(QModelIndex)));
	connect(ui->tableWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(onItemActivated(QModelIndex)));
}

AnalysisResultWidget::~AnalysisResultWidget()
{
}

void AnalysisResultWidget::setAnalysisData(const RdsProtobuf::ServerMessage_OneShotData_AnalysisData &adata)
{
	QString txtDetected;
	int tst = adata.signal_size();
	m_currentIndex = adata.detector_index();

	createSections(tst);

	if(!adata.success()) {
		ui->errorLbl->setText( QString::fromStdString(adata.error()) );
//	createSections(5);
//		addRange(0, 100,
//					102);
//		addRange(1, 104,
//					108);

//		addRange(2, 98,
//					99);

		ui->tableWidget->setVisible(false);
		ui->errorLbl->setVisible(true);
		ui->pbAdd->setVisible(false);
	} else {
		ui->tableWidget->setVisible(true);
		ui->errorLbl->setVisible(false);
		ui->pbAdd->setVisible(true);
	}

	for(int i = 0; i < adata.signal_size(); i++) {
		txtDetected.append( QString("Detected signal #%1 : \r\n").arg(i) );
		txtDetected.append( QString("	Range start %1  end %2 \r\n").arg( adata.signal(i).range().start() ).
							arg( adata.signal(i).range().end() ) );

		QString systemStr;
		QString moduleStr;
		QString typeStr;
		QString infoStr;

		//        if( msg.signal(i).has_parameters() ) {
		//            RdsProtobuf::Signal_SignalParameters param = msg.signal(i).signal().parameters();
		//            txtDetected.append( QString("	Signal System %1 \r\n").arg( QString::fromStdString(param.system()) ) );

		//            systemStr = QString::fromStdString(param.system());

		//            txtDetected.append( QString("	Signal Modulation %1 \r\n").arg( QString::fromStdString(param.modulation()) ) );

		//            moduleStr = QString::fromStdString(param.modulation());

		//            txtDetected.append(  QString("	Signal Type %1\r\n").arg( QString::fromStdString(param.type()) ) );

		//            typeStr = QString::fromStdString(param.type());
		//        }

		if( adata.signal(i).has_system() ) {
			systemStr = QString::fromStdString(adata.signal(i).system());
		}

		if( adata.signal(i).has_modulation() ) {
			moduleStr = QString::fromStdString(adata.signal(i).modulation());
		}

		if( adata.signal(i).has_estimation_probability() ) {
			typeStr = QString("%1%").arg(adata.signal(i).estimation_probability());
		}

		if( adata.signal(i).has_more_info() ) {
			txtDetected.append( QString("	Signal Info %1\r\n").arg( QString::fromStdString(adata.signal(i).more_info()) ) );

			infoStr = QString::fromStdString(adata.signal(i).more_info());
		}

		addRange(i, adata.signal(i).range().start(),
					adata.signal(i).range().end());

		addSignalInfo(i,
					  systemStr,
					  moduleStr,
					  typeStr,
					  infoStr);
	}

	emit analysisReady(m_currentIndex);

	this->show();
}


void AnalysisResultWidget::createSections(int count)
{
	ui->tableWidget->clear();

	ui->tableWidget->setRowCount(count);
	ui->tableWidget->setColumnCount(COLUMN_COUNT);

	QStringList labels;
	labels << tr("Range start");
	labels << tr("Range end");
	labels << tr("Signal System");
	labels << tr("Signal Modulation");
	labels << tr("Signal probability");
	labels << tr("Signal Info");
	ui->tableWidget->setHorizontalHeaderLabels(labels);

	for(int i = 0; i<ui->tableWidget->rowCount(); i++) {
		//ui->tableWidget->setRowHeight(i, 300);
	}

	for(int i = 0; i<ui->tableWidget->columnCount(); i++) {
		if(i == 5) {
			ui->tableWidget->setColumnWidth(i, 900);
		}
		if(i == 2 || i == 3 || i == 4) {
			ui->tableWidget->setColumnWidth(i, 100);
		}
	}

}

void AnalysisResultWidget::addRange(int i, double start, double end) {
	QDoubleSpinBox* sb1 = new QDoubleSpinBox(this);
	QDoubleSpinBox* sb2 = new QDoubleSpinBox(this);
	sb1->setMinimum(10);
	sb1->setMaximum(8000);

	sb2->setMinimum(10);
	sb2->setMaximum(8000);

	sb1->setValue(start);
	sb2->setValue(end);

	ui->tableWidget->setCellWidget(i, 0, sb1);
	ui->tableWidget->setCellWidget(i, 1, sb2);
}

void AnalysisResultWidget::addSignalInfo(const int i,
										 const QString& system,
										 const QString& modulation,
										 const QString& type,
										 const QString& info)
{
	ui->tableWidget->setItem(i, 2, new QTableWidgetItem( system ) );
	ui->tableWidget->setItem(i, 3, new QTableWidgetItem( modulation ) );
	ui->tableWidget->setItem(i, 4, new QTableWidgetItem( type ) );
	ui->tableWidget->setItem(i, 5, new QTableWidgetItem( info ) );
}

void AnalysisResultWidget::addSignal()
{
	if( ui->tableWidget->selectedRanges().size() <= 0 ) {
		return;
	}

	int i = ui->tableWidget->selectedRanges().first().topRow();

	QDoubleSpinBox* sb = (QDoubleSpinBox*)(ui->tableWidget->cellWidget(i, 0));
	double start = sb->value();

	sb = (QDoubleSpinBox*)(ui->tableWidget->cellWidget(i, 1));
	double end = sb->value();

	emit onAddToList(start, end);
}

void AnalysisResultWidget::slotContinue(bool)
{
	ui->tableWidget->clear();
	emit signalContinue(m_currentIndex, true);

	m_currentIndex = -1;

}

void AnalysisResultWidget::slotClose(bool)
{
	ui->tableWidget->clear();
	emit signalClose();

	m_currentIndex = -1;
}

void AnalysisResultWidget::onCellClicked(int row, int col)
{
	int t1 = row;
	int t2 = col;

	double start;
	double end;

	QDoubleSpinBox* sb = dynamic_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(row, 0));
	if(!sb) {
		return;
	}


	start = sb->value();

	sb = dynamic_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget(row, 1));

	if(!sb) {
		return;
	}

	end = sb->value();

	emit analysisSelection(m_currentIndex, start, end);
}

void AnalysisResultWidget::onItemClicked(QTableWidgetItem *itm)
{
	int t=0;
	t = itm->row();
	int k = 0;

	onCellClicked(itm->row(), itm->column());
}

void AnalysisResultWidget::onItemActivated(QModelIndex ind)
{
	int t=0;
	t = ind.row();
	int k = 0;

	onCellClicked(ind.row(), ind.column());
}

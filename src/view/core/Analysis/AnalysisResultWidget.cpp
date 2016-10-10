#include "AnalysisResultWidget.h"

#include "ui_AnalysisResultWidget.h"

#include <QLabel>

#define COLUMN_COUNT 6

AnalysisResultWidget::AnalysisResultWidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::AnalysisResultWidget)
{
	ui->setupUi(this);

    connect(ui->pbContinue, SIGNAL(clicked(bool)), this, SIGNAL(signalContinue(bool)));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(addSignal()));
}

AnalysisResultWidget::~AnalysisResultWidget()
{
}

void AnalysisResultWidget::setAnalysisData(RdsProtobuf::AnalysisDetected msg)
{
    QString txtDetected;
    int tst = msg.signal_size();

    createSections(tst);

    for(int i = 0; i < msg.signal_size(); i++) {
        txtDetected.append( QString("Detected signal #%1 : \r\n").arg(i) );
        txtDetected.append( QString("	Range start %1  end %2 \r\n").arg( msg.signal(i).signal().range().start() ).
                                                                      arg( msg.signal(i).signal().range().end() ) );

        QString systemStr;
        QString moduleStr;
        QString typeStr;
        QString infoStr;

        if( msg.signal(i).signal().has_parameters() ) {
            RdsProtobuf::Signal_SignalParameters param = msg.signal(i).signal().parameters();
            txtDetected.append( QString("	Signal System %1 \r\n").arg( QString::fromStdString(param.system()) ) );

            systemStr = QString::fromStdString(param.system());

            txtDetected.append( QString("	Signal Modulation %1 \r\n").arg( QString::fromStdString(param.modulation()) ) );

            moduleStr = QString::fromStdString(param.modulation());

            txtDetected.append(  QString("	Signal Type %1\r\n").arg( QString::fromStdString(param.type()) ) );

            typeStr = QString::fromStdString(param.type());
        }

        if( msg.signal(i).signal().has_info() ) {
            txtDetected.append( QString("	Signal Info %1\r\n").arg( QString::fromStdString(msg.signal(i).signal().info()) ) );

            infoStr = QString::fromStdString(msg.signal(i).signal().info());
        }

        addRange(i, msg.signal(i).signal().range().start(),
                 msg.signal(i).signal().range().end());

        addSignalInfo(i,
                      systemStr,
                      moduleStr,
                      typeStr,
                      infoStr);
    }
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
    labels << tr("Signal Type");
    labels << tr("Signal Info");
    ui->tableWidget->setHorizontalHeaderLabels(labels);

    for(int i = 0; i<ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->setRowHeight(i, 300);
    }

    for(int i = 0; i<ui->tableWidget->columnCount(); i++) {
        if(i == 5) {
            ui->tableWidget->setColumnWidth(i, 300);
        }
        if(i == 2 || i == 3 || i == 4) {
            ui->tableWidget->setColumnWidth(i, 1);
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

//    double start = (QDoubleSpinBox*)(ui->tableWidget->cellWidget(i, 0))->value();
//    double end = (QDoubleSpinBox*)(ui->tableWidget->cellWidget(i, 1))->value();

//    emit onAddToList(start, end);
}

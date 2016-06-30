#pragma once

#include <QWidget>
#include <QDialog>
#include <QByteArray>
#include <QScrollBar>

namespace Ui {
class SolverSetupWidget;
}

class SolverSetupWidget : public QDialog
{
	Q_OBJECT

public:
	explicit SolverSetupWidget(QWidget *parent = 0);
	~SolverSetupWidget();

	void setSolverVersion(const QString &version);

private:
	Ui::SolverSetupWidget *ui;

public:
	void appendSolverResult(const QString& log);

signals:
	void onSendSignal();
	void onGetVersion();
};

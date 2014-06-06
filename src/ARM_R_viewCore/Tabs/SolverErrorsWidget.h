#ifndef LOGERRORSDIALOG_H
#define LOGERRORSDIALOG_H

#include <QDialog>

namespace Ui {
class SolverErrorsWidget;
}

class SolverErrorsWidget : public QDialog
{
	Q_OBJECT

public:
	explicit SolverErrorsWidget(QWidget *parent = 0);
	~SolverErrorsWidget();

	void appendSolverError(const QString& error);

private:
	Ui::SolverErrorsWidget *ui;
};

#endif // LOGERRORSDIALOG_H

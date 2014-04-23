#ifndef SOLVERRESULTWIDGET_H
#define SOLVERRESULTWIDGET_H

#include <QWidget>
#include <QByteArray>

namespace Ui {
class SolverResultWidget;
}

class SolverResultWidget : public QWidget
{
	Q_OBJECT

public:
	explicit SolverResultWidget(QWidget *parent = 0);
	~SolverResultWidget();

private:
	Ui::SolverResultWidget *ui;

public:
	void appendSolverResult(QByteArray& data);
};

#endif // SOLVERRESULTWIDGET_H

#ifndef BIN_HEX_LINE_EDIT_H
#define BIN_HEX_LINE_EDIT_H

#include <QWidget>
#include <QLineEdit>
#include <functional>
namespace gui
{

class BinHexLineEdit : public QFrame
{
	Q_OBJECT
public:
	template<class fT>
	BinHexLineEdit(QWidget *parent, fT set_func = nullptr);
	~BinHexLineEdit();

	enum WorkMode
	{
		mode_Hex = 0,
		mode_Bin,
		mode_Dec
	};

	void setWorkMode(WorkMode workMode);
	void setText(QString text);
	
private:
	QLineEdit	line_editor_;
	QPushButton miniButton_;
	WorkMode workMode_;
	std::function<void (QLineEdit* editor)> settings_func_;

private slots:
	void miniButtonClicked_slot();

};


class BinHexLineEditValidator: public QValidator
{
	Q_OBJECT
public:
	BinHexLineEditValidator ( BinHexLineEdit::WorkMode workMode, QObject *parent = nullptr): QValidator(parent) { workMode_ = workMode;};
	virtual QValidator::State	validate ( QString & input, int & pos ) const;
private:
	BinHexLineEdit::WorkMode workMode_;
};





//////////////////////////////////////////////////////////////////////////
//реализация конструктора

template<class fT>
BinHexLineEdit::BinHexLineEdit(QWidget *parent,fT set_func): QFrame(parent)
{
	workMode_ = mode_Hex;
	setFrameShadow(QFrame::Plain);
	setFrameShape(QFrame::StyledPanel);
	QHBoxLayout* hl = new QHBoxLayout();
	setLayout(hl);
	hl->setMargin(0);
	hl->setSpacing(0);
	hl->addWidget(&line_editor_);
	hl->addWidget(&miniButton_);
	line_editor_.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	line_editor_.setFrame(false);
	line_editor_.setValidator(new BinHexLineEditValidator(workMode_,&line_editor_));
	miniButton_.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
	miniButton_.setFixedWidth(20);
	miniButton_.setToolTip("Сменить режим отображения данных");
	miniButton_.setText("FF");
	miniButton_.setFocusPolicy(Qt::NoFocus);
	connect(&miniButton_,SIGNAL(clicked()),this,SLOT(miniButtonClicked_slot()));
	if (settings_func_) settings_func_(&line_editor_);
	
	
	
}



}
#endif // BIN_HEX_LINE_EDIT_H

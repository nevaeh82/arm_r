#ifndef Q_BUTTONLIST_H
#define Q_BUTTONLIST_H

#include <QObject>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPointer>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QButtonGroup>
#include <QCheckBox>

namespace gui
{



//Группа это 1 ряд из кнопок
class ButtonList : public QScrollArea
{
	Q_OBJECT
public:
	ButtonList(QWidget *parent, int buttons_in_group, bool right_mark_enabled = false);
	~ButtonList();

	bool			SetButtonsIcons				(QIcon i_b1,QIcon i_b2,QIcon i_b3,QIcon i_b4);		//устанавливает иконки на все кнопки (до создания)
	void			SetIconForButton			(int group_index, int button,QIcon ico);					//на созданную кнопку 
	void			SetButtonsToolTip			(QString t1, QString t2, QString t3, QString t4);	//устанавливает подсказку на все кнопки
	void			SetToolTipForButton			(int group_index, int button,QString tooltip);		//устанавливает подсказку на созданную кнопку кнопку
	int				AddGroup					(QString Text, QVariant data = QVariant(), bool auto_select = false);								//возвращает порядковый номер\индекс, добавляет новую группу кнопок
	QString			GetGroupLabel				(int group_index) const;
	void			SetGroupLabel				(int group_index,QString Text);
	void			SetGroupButtonEnabled		(int group_index, int button, bool enabled);
	void			SetGroupLabelLeftAligment	(bool enable);//false
	void			SetChackable				(bool enabled);//false
	void			SetCheckState				(int group_index, Qt::CheckState state, bool enable_signal = false);
	Qt::CheckState	GetCheckState				(int group_index);//Qt::Unckecked
	void			SetSpacing					(int spacing);//10
	void			SetCheckToolTip				(QString tooltip);//""
	void			SetCheckTristate			(bool enable_tristate);//false
	void			SetGroupVisible				(int group_index, bool visible);//true
	QStringList     GetGroupsNames				();//список имен, порядок соответсвует номерам групп
		
		
	bool	DeleteGroup(int group_index);

	int		GetCurrentSelectedGroup();
	void	SetCurrentSelectedGroup(int group,bool emit_signal = true);
	int		GetGroupsCount();
	void	SetSelectionEnabled(bool aa);//ВКЛ. ВЫКЛ. функции выборки (залипания) - по умолчанию - выкл.
	void	SetVisibleGroup(int group, bool vis);
	void	ClearAll();
	void	SetNumbersVisible(bool visible);
	QVariant GetDataByGroupIndex(int GroupIndex) const;
	

signals:
	void SelectionChanged(int GroupIndex);//GroupIndex < 0 - ничего не выбрано
	void ButtonPressed(int GroupIndex, int Button);
	void CheckChange(int GroupIndex, Qt::CheckState state);

//////////////////////////////////////////////////////////////////////////
//Далее PRIVATE
//////////////////////////////////////////////////////////////////////////

private:

	struct GroupRecord 
	{
		QPointer<QHBoxLayout>	interanlLayout;
		QPointer<QLabel>		Label;
		QPointer<QCheckBox>		check_box;
		QPointer<QButtonGroup>	Bg;
		QPointer<QLabel>		right_mark;
		QPointer<QFrame>		Line;
		QVariant				data;
	};

	QIcon ib1,ib2,ib3,ib4;
	QString checktooltip_;
	QString tooltip1, tooltip2, tooltip3, tooltip4;
	QPointer<QVBoxLayout> MainLay;
	QPointer<QWidget> WidgetContents;
	QPointer<QLabel> EmptyList;
	QPointer<QFrame> ButLine;

	QList<GroupRecord*> ListOfRecords;
	int					buttons_in_group_;
	bool				numbers_visible_;
	bool				group_label_left_aligment_;
	bool				chackable_;
	bool				enable_tristate_;


	void RecountLabels();
	void RecountSelected();
	
	void connectChecker(QCheckBox* ch);
	void disconnectChecker(QCheckBox* ch);

	bool		is_selection_enabled_;
	int			CurSel;
	QTimer		timer_ensure_;
	QPixmap		right_mark_pixmap_;
	bool		right_mark_enabled_;

private slots:
	void GButtonClicked(int id);
	void SomeChecked_slot(int state);
	void timer_ensure_timeout();

};



}//namespace gui

#endif // Q_BUTTONLIST_H

#include <stdafx.h>
#include "button_list.h"
#include <QScrollBar>

namespace gui
{


ButtonList::ButtonList(QWidget *parent, int buttons_in_group, bool right_mark_enabled) : QScrollArea(parent)
{
 	if (parent->layout() != NULL)
 	{
 		parent->layout()->addWidget(this);
 	}
	right_mark_enabled_ = right_mark_enabled;
	group_label_left_aligment_ = false;
	chackable_ = false;
	timer_ensure_.setInterval(100);
	timer_ensure_.setSingleShot(true);
	connect(&timer_ensure_,SIGNAL(timeout()),this,SLOT(timer_ensure_timeout()));
	if (buttons_in_group < 1) buttons_in_group = 1;
	buttons_in_group_ = buttons_in_group;
	enable_tristate_ = false;
	setWidgetResizable(true);
	setFrameShape(QFrame::NoFrame);
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	//setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	//verticalScrollBar()->setTracking(false);
	//verticalScrollBar()->setSingleStep(1);
	//verticalScrollBar()->setInvertedControls(false);
	//verticalScrollBar()->setInvertedAppearance(true);

	WidgetContents = new QWidget();
	WidgetContents->setObjectName("WidgetContent");
	if (right_mark_enabled_)
	{
		WidgetContents->setStyleSheet(" 	QWidget#WidgetContent { 	background-color: rgb(254, 254, 254);	}");    //  feature workaround на вс€кий случай, 255 может быть не белый
	}
	
		
	MainLay = new QVBoxLayout();
	if (right_mark_enabled_)
	{
		MainLay->setContentsMargins(0,0,0,0);
		right_mark_pixmap_.load(":/button_list/Resources/coner.png");
	}
	else
		MainLay->setContentsMargins(0,0,5,0);
	MainLay->setSpacing(10);

	WidgetContents->setLayout(MainLay);
	QFrame* line = new QFrame(WidgetContents);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
	MainLay->addWidget(line);

	QFont font;
	font.setStyleStrategy(QFont::PreferAntialias);
	EmptyList = new QLabel(WidgetContents);
	EmptyList->setFont(font);
	EmptyList->setText(QString::fromLocal8Bit("«аписи отсутствуют"));
	EmptyList->setAlignment(Qt::AlignCenter);
	MainLay->addWidget(EmptyList);

	ButLine = new QFrame(WidgetContents);
	ButLine->setFrameShape(QFrame::HLine);
	ButLine->setFrameShadow(QFrame::Sunken);
	MainLay->addWidget(ButLine);

	setWidget(WidgetContents);

	MainLay->addSpacerItem(new QSpacerItem(30,30,QSizePolicy::Expanding,QSizePolicy::Expanding));

	is_selection_enabled_ = false;
	CurSel = -1;
	numbers_visible_ = true;

}

ButtonList::~ButtonList()
{
	while (ListOfRecords.count() > 0)
	{
		GroupRecord* Rec = ListOfRecords[0];
		delete Rec->Bg;
		delete Rec;
		ListOfRecords.removeAt(0);
	}
}

void ButtonList::SetSpacing( int spacing )
{
	MainLay->setSpacing(spacing);
}


bool ButtonList::SetButtonsIcons( QIcon i_b1,QIcon i_b2,QIcon i_b3,QIcon i_b4 )
{
	ib1 = i_b1;
	ib2 = i_b2;
	ib3 = i_b3;
	ib4 = i_b4;
	return false;
}

void ButtonList::GButtonClicked( int id )
{
	QButtonGroup *sen = qobject_cast<QButtonGroup *>(sender());

	for (int i = 0;i < ListOfRecords.count();i++)
		if (ListOfRecords[i]->Bg == sen)
		{
			bool need_signal = true;
			if ((id == 0) && (is_selection_enabled_))
			{
				if (CurSel == i)
				{
					need_signal = false;
					ListOfRecords[i]->Bg->button(0)->setChecked(true);
				}
				else
				{
					CurSel = i;
					RecountSelected();
					emit SelectionChanged(CurSel);
				}
			}

			if (need_signal)
			{
				emit ButtonPressed(i,id);
			}
			break;
		}
}

int ButtonList::AddGroup( QString Text, QVariant data, bool auto_select )
{
	EmptyList->hide();
	ButLine->hide();

	//создание секции
	GroupRecord* Rec = new GroupRecord();
	Rec->data = data;
	Rec->interanlLayout = new QHBoxLayout();
	if (chackable_)
	{
		Rec->interanlLayout->setContentsMargins(4,0,0,2);
	}
	else
		Rec->interanlLayout->setContentsMargins(0,0,0,2);
	Rec->interanlLayout->setSpacing(0);
	Rec->Line = new QFrame(WidgetContents);
	Rec->Line->setFrameShape(QFrame::HLine);
	Rec->Line->setFrameShadow(QFrame::Sunken);

	if (MainLay->count()-1)
	{
		QLayoutItem *li = MainLay->takeAt(MainLay->count()-1);
		delete li;
	}

	MainLay->addItem(Rec->interanlLayout);
	MainLay->addWidget(Rec->Line);
	MainLay->addSpacerItem(new QSpacerItem(30,30,QSizePolicy::Expanding,QSizePolicy::Expanding));

	QFont font;
	font.setPointSize(12);
	font.setBold(false);
	font.setWeight(75);
	font.setStyleStrategy(QFont::PreferAntialias);
	Rec->Label = new QLabel(WidgetContents);
	Rec->Label->setFont(font);
	Rec->Label->setScaledContents(false);
	Rec->Label->setAlignment(Qt::AlignCenter);
	Rec->Label->setText(QString::number(ListOfRecords.count()+1));
	Rec->Label->setMinimumSize(35,30);
	Rec->Label->setMaximumSize(35,30);
	if (numbers_visible_) { Rec->Label->setVisible(true);Rec->Label->show();}
	else Rec->Label->setVisible(false);
	Rec->interanlLayout->addWidget(Rec->Label);


	if (chackable_)
	{
		Rec->check_box = new QCheckBox(WidgetContents);
		Rec->check_box->setText("");
		Rec->check_box->setMaximumSize(QSize(16,30));
		Rec->check_box->setMinimumSize(QSize(16,30));
		Rec->check_box->setToolTip(checktooltip_);
		Rec->check_box->setTristate(enable_tristate_);
		Rec->check_box->show();
		connectChecker(Rec->check_box);
		Rec->interanlLayout->addWidget(Rec->check_box);
	}
	

	QSize IconSize = QSize(24,24);

	QFont font1;
	font1.setPointSize(10);
	font1.setStyleStrategy(QFont::PreferAntialias);

	Rec->Bg = new QButtonGroup();
	Rec->Bg->setExclusive(false);

	QPushButton* b = new QPushButton(WidgetContents);
	if (group_label_left_aligment_)
		b->setStyleSheet("text-align: left;padding-left: 15px;");
	QFontMetrics fmt(font1);
	int ggg = fmt.width(Text)+10;
	b->setMinimumSize(ggg,30);
	b->setIconSize(IconSize);
	b->setIcon(ib1);
	b->setText(Text);
	b->setFont(font1);
	b->setCheckable(is_selection_enabled_);
	b->setToolTip(tooltip1);
	b->show();
	Rec->interanlLayout->addWidget(b);
	Rec->Bg->addButton(b,0);

	if (buttons_in_group_ > 1)
	{
		QPushButton* b2 = new QPushButton(WidgetContents);
		b2->setMinimumSize(30,30);
		b2->setMaximumSize(30,30);
		b2->setIconSize(IconSize);
		b2->setIcon(ib2);
		b2->setToolTip(tooltip2);
		b2->show();
		Rec->interanlLayout->addWidget(b2);
		Rec->Bg->addButton(b2,1);
	}

	if (buttons_in_group_ > 2)
	{
		QPushButton* b3 = new QPushButton(WidgetContents);
		b3->setMinimumSize(30,30);
		b3->setMaximumSize(30,30);
		b3->setIconSize(IconSize);
		b3->setIcon(ib3);
		b3->setToolTip(tooltip3);
		b3->show();
		Rec->interanlLayout->addWidget(b3);
		Rec->Bg->addButton(b3,2);
	}


	if (buttons_in_group_ > 3)
	{
		QPushButton* b4 = new QPushButton(WidgetContents);
		b4->setMinimumSize(30,30);
		b4->setMaximumSize(30,30);
		b4->setIconSize(IconSize);
		b4->setIcon(ib4);
		b4->setToolTip(tooltip4);
		b4->show();
		Rec->interanlLayout->addWidget(b4);
		Rec->Bg->addButton(b4,3);
	}
	
	if (right_mark_enabled_)
	{
		Rec->right_mark = new QLabel(WidgetContents);
		Rec->right_mark->setText("");
		Rec->right_mark->setMaximumSize(QSize(15,30));
		Rec->right_mark->setMinimumSize(QSize(15,30));
		Rec->right_mark->show();
		Rec->interanlLayout->addWidget(Rec->right_mark);
	}


	connect(Rec->Bg,SIGNAL(buttonClicked(int)),this,SLOT(GButtonClicked(int)));

	ListOfRecords.append(Rec);
	if (auto_select)
	{
		CurSel = ListOfRecords.count()-1;
		RecountSelected();
		emit SelectionChanged(CurSel);
	}
	return ListOfRecords.count()-1;
}

bool ButtonList::DeleteGroup( int Index )
{
	if ((ListOfRecords.count() <= Index) || (Index < 0)) return false;

	GroupRecord* Rec = ListOfRecords[Index];
	QList<QAbstractButton *> bl = Rec->Bg->buttons();
	
	while (bl.count() > 0)
	{
		QPushButton* b = qobject_cast<QPushButton*>(bl[0]);
		delete b;
		bl.removeAt(0);
	}

	MainLay->removeWidget(Rec->Line);
	MainLay->removeItem(Rec->interanlLayout);
	delete	Rec->right_mark;
	delete	Rec->check_box;
	delete	Rec->Bg;
	delete	Rec->Label;
	delete	Rec->Line;
	delete	Rec->interanlLayout;
	delete	Rec;
	ListOfRecords.removeAt(Index);
	RecountLabels();

 	if (ListOfRecords.count() == 0)
 	{
 		EmptyList->show();ButLine->show();		
 	}
	if (CurSel == Index)
	{
		CurSel = -1;
		RecountSelected();
		emit SelectionChanged(CurSel);
	}

	return true;
}

void ButtonList::ClearAll()
{
	for (int i = 0 ; i < ListOfRecords.count();i++)
	{
		GroupRecord* Rec = ListOfRecords[i];
		QList<QAbstractButton *> bl = Rec->Bg->buttons();

		while (bl.count() > 0)
		{
			QPushButton* b = qobject_cast<QPushButton*>(bl[0]);
			delete b;
			bl.removeAt(0);
		}
		
		MainLay->removeWidget(Rec->Line);
		MainLay->removeItem(Rec->interanlLayout);
		delete	Rec->right_mark;
		delete	Rec->check_box;
		delete	Rec->Bg;
		delete	Rec->Label;
		delete	Rec->Line;
		delete	Rec->interanlLayout;
		delete	Rec;
	}

	bool need_signal = false;
	if (CurSel > -1) need_signal = true;
	CurSel = -1;
	if (need_signal) emit SelectionChanged(CurSel);
	ListOfRecords.clear();
	EmptyList->show();
	ButLine->show();
}

void ButtonList::RecountLabels()
{
	for (int i = 0;i < ListOfRecords.count();i++)
	{
		GroupRecord* Rec = ListOfRecords[i];
		Rec->Label->setText(QString::number(i+1));
	}
}

void ButtonList::SetSelectionEnabled( bool aa )
{
	is_selection_enabled_ = aa;
	for (int i = 0;i < ListOfRecords.count();i++)
	{
		QPushButton* Pb = qobject_cast<QPushButton*>(ListOfRecords[i]->Bg->button(0));
		Pb->setCheckable(is_selection_enabled_);
		Pb->setChecked(false);
	}
	RecountSelected();
}

void ButtonList::RecountSelected()
{
	if (!is_selection_enabled_) return;
	if (CurSel >= ListOfRecords.count())
	{
		CurSel = -1;
		emit SelectionChanged(CurSel);
	}
	
	for (int i = 0;i < ListOfRecords.count();i++)
	{
		QPushButton* Pb = qobject_cast<QPushButton*>(ListOfRecords[i]->Bg->button(0));
		QPointer<QLabel> right_mark = ListOfRecords[i]->right_mark;
		if (i == CurSel)
		{
			QFont fnt = Pb->font();
			fnt.setBold(true);
			fnt.setUnderline(true);
			Pb->setFont(fnt);
			Pb->setChecked(true);
			if (right_mark)
			{
				right_mark->setPixmap(right_mark_pixmap_);
			}
			timer_ensure_.start();
		}
		else
		{
			if (right_mark)
				right_mark->clear();
			
			QFont fnt = Pb->font();
			fnt.setBold(false);
			fnt.setUnderline(false);
			Pb->setFont(fnt);
			Pb->setChecked(false);
		}
	}
}

int ButtonList::GetCurrentSelectedGroup()
{
	if (CurSel >= ListOfRecords.count()) 
	{
		CurSel = -1;
		emit SelectionChanged(CurSel);
	}
	return CurSel;
}

void ButtonList::SetVisibleGroup( int group, bool vis )
{
	if ((group >= ListOfRecords.count()) || (group < 0)) return;
	QPushButton* Pb1 = qobject_cast<QPushButton*>(ListOfRecords[group]->Bg->button(1));
	QPushButton* Pb2 = qobject_cast<QPushButton*>(ListOfRecords[group]->Bg->button(2));
	QPushButton* Pb3 = qobject_cast<QPushButton*>(ListOfRecords[group]->Bg->button(3));
	Pb1->setVisible(vis);
	Pb2->setVisible(vis);
	Pb3->setVisible(vis);
}


int ButtonList::GetGroupsCount()
{
	return ListOfRecords.count();
}

void ButtonList::SetCurrentSelectedGroup( int group_index,bool emit_signal )
{
	if ((group_index >= ListOfRecords.count())) return; //(group_index < 0) здесь может быть
	
	CurSel = group_index;
	RecountSelected();
	if (emit_signal) emit SelectionChanged(CurSel);
	if (group_index >= 0)	emit ButtonPressed(group_index,0);
}

void ButtonList::SetButtonsToolTip( QString t1, QString t2, QString t3, QString t4 )
{
	tooltip1 = t1;
	tooltip2 = t2;
	tooltip3 = t3;
	tooltip4 = t4;
}

void ButtonList::SetNumbersVisible( bool visible )
{
	numbers_visible_ = visible;
}

QVariant ButtonList::GetDataByGroupIndex(int group_index) const
{
	if ((group_index >= ListOfRecords.size())  || (group_index < 0)) return QVariant();
	return ListOfRecords.at(group_index)->data;
}

void ButtonList::SetIconForButton( int group_index, int Button, QIcon ico )
{
	if ((group_index >= ListOfRecords.size()) || (group_index < 0)) return;
	if (Button >= buttons_in_group_) return;
	ListOfRecords.at(group_index)->Bg->button(Button)->setIcon(ico);
}

void ButtonList::timer_ensure_timeout()
{
	if ((CurSel >= 0) && (CurSel < ListOfRecords.size()))
		ensureWidgetVisible(ListOfRecords[CurSel]->Bg->button(0));
}

QString ButtonList::GetGroupLabel( int group_index ) const
{
	if ((group_index  >= ListOfRecords.size()) || (group_index < 0)) return QString();
	return ListOfRecords.at(group_index)->Bg->button(0)->text();
}

void ButtonList::SetGroupLabel( int group_index,QString Text )
{
	if ((group_index  >= ListOfRecords.size()) || (group_index < 0)) return;
	return ListOfRecords.at(group_index)->Bg->button(0)->setText(Text);
}

Qt::CheckState ButtonList::GetCheckState( int group_index )
{
	if (!chackable_) return Qt::Unchecked;
	if ((group_index  >= ListOfRecords.size()) || (group_index < 0)) return Qt::Unchecked;
	if (ListOfRecords.at(group_index)->check_box != nullptr)
		return ListOfRecords.at(group_index)->check_box->checkState();
	return Qt::Unchecked;
}

void ButtonList::SetCheckState( int group_index, Qt::CheckState state, bool enable_signal /*= false*/ )
{
	if (!chackable_) return;
	if ((group_index  >= ListOfRecords.size()) || (group_index < 0)) return;
	if (ListOfRecords.at(group_index)->check_box != nullptr)
	{
		if (!enable_signal)
			disconnectChecker(ListOfRecords.at(group_index)->check_box);
		
		if (state == Qt::PartiallyChecked)
			if (!enable_tristate_) state = Qt::Checked;
						
		ListOfRecords.at(group_index)->check_box->setCheckState(state);
		
		if (!enable_signal)
			connectChecker(ListOfRecords.at(group_index)->check_box);
	}
}

void ButtonList::SetChackable( bool enabled )
{
	chackable_ = enabled;
}

void ButtonList::SetGroupLabelLeftAligment( bool enable )
{
	group_label_left_aligment_ = enable;
}

void ButtonList::SomeChecked_slot( int state )
{
	QCheckBox *sen = qobject_cast<QCheckBox*>(sender());

	for (int i = 0;i < ListOfRecords.count();i++)
	{
		if (ListOfRecords[i]->check_box == sen)
		{
			emit CheckChange(i,ListOfRecords[i]->check_box->checkState());
		}
	}
}

void ButtonList::connectChecker( QCheckBox* ch )
{
	connect(ch,SIGNAL(stateChanged(int)), this, SLOT(SomeChecked_slot(int)));
}

void ButtonList::disconnectChecker( QCheckBox* ch )
{
	disconnect(ch,SIGNAL(stateChanged(int)), this, SLOT(SomeChecked_slot(int)));
}

void ButtonList::SetCheckToolTip( QString tooltip )
{
	checktooltip_ = tooltip;
}

void ButtonList::SetCheckTristate( bool enable_tristate )
{
	enable_tristate_ = enable_tristate;
}

void ButtonList::SetGroupButtonEnabled( int group_index, int button, bool enabled )
{
	if ((group_index  >= ListOfRecords.size()) || (group_index < 0)) return;
	if ((button < 0) || (button >= buttons_in_group_)) return;
	ListOfRecords[group_index]->Bg->button(button)->setEnabled(enabled);
}

void ButtonList::SetToolTipForButton( int group_index, int button,QString tooltip )
{
	if ((group_index  >= ListOfRecords.size()) || (group_index < 0)) return;
	if ((button < 0) || (button >= buttons_in_group_)) return;
	ListOfRecords[group_index]->Bg->button(button)->setToolTip(tooltip);
}

void ButtonList::SetGroupVisible( int group_index, bool visible )
{
	if ((group_index  >= ListOfRecords.size()) || (group_index < 0)) return;
	if (ListOfRecords[group_index]->Bg->button(0)->isVisible() == visible) return;

	for (int i = 0 ; i < buttons_in_group_;i++)
		ListOfRecords[group_index]->Bg->button(i)->setVisible(visible);
	if (chackable_)
		ListOfRecords[group_index]->check_box->setVisible(visible);
	if (numbers_visible_)
		ListOfRecords[group_index]->Label->setVisible(visible);
	if (right_mark_enabled_)
		ListOfRecords[group_index]->right_mark->setVisible(visible);

	ListOfRecords[group_index]->Line->setVisible(visible);
	QMargins marg = ListOfRecords[group_index]->interanlLayout->contentsMargins();
	if (!visible)
		marg.setBottom(0);
	else
		marg.setBottom(2);
	ListOfRecords[group_index]->interanlLayout->setContentsMargins(marg);	
}

QStringList ButtonList::GetGroupsNames()
{
	QStringList names;
	for (int i=0;i < ListOfRecords.size();i++)
		names.append(ListOfRecords[i]->Bg->button(0)->text());
	return names;
}


}//namespace gui
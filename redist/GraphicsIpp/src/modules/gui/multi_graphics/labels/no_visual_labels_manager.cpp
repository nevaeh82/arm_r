#include "../../../../StdAfx.h"
#include "no_visual_labels_manager.h"
#include "../../../../modules/gui/multi_graphics/multi_graphics.h"

NoVisualLabelsManager::NoVisualLabelsManager(Q_MG_MouseCursor *parent): QObject(parent)
{
	graphics_parent_ = parent;
	text_drawer_.setTextColor(QColor(100,250,250));
	text_drawer_.setSqureBorderColor(QColor(0,250,250));
	text_drawer_.setSqureBackgroundColor(QColor(0,0,0,150));
	prev_kf_x_ = 0;
	prev_trans_x_ = 0;
	timer_recalc_unions_.setSingleShot(true);
	timer_recalc_unions_.setInterval(1800);
	connect(&timer_recalc_unions_,SIGNAL(timeout()),this,SLOT(Timer_recalc_unions_timeout_slot()));
	union_text_ = tr("Signals:");
	show_hints_ = true;
}

NoVisualLabelsManager::~NoVisualLabelsManager()
{
	//clearLabels();
}

void NoVisualLabelsManager::addLabel( double pos, QString text, QString name, bool immediately_update /*= false*/, QColor label_text_color )
{
	gui::mg1::LabelInstrumentItem* new_item = new gui::mg1::LabelInstrumentItem(&text_drawer_,pos,text,name,QFont(),label_text_color);
	new_item->setParentItem(graphics_parent_);
	new_item->setVisible(false);
	//connect(new_item,SIGNAL(ItemClicked(QString,doubleScene)),this,SLOT(ItemClicked_slot(QString,doubleScene)));
	labels_list_.append(new_item);

	if (immediately_update) Timer_recalc_unions_timeout_slot();
	else					timer_recalc_unions_.start();//при добалении всегда рестартуем таймер обновления (ибо тогда первая сотня влезать долго будет)
}

void NoVisualLabelsManager::clearLabels()
{
	timer_recalc_unions_.stop();
	clearUnions();
	while (labels_list_.size() > 0)
	{
		delete labels_list_.takeLast();
	}
}

void NoVisualLabelsManager::Timer_recalc_unions_timeout_slot()
{
	if (timer_recalc_unions_.isActive()) timer_recalc_unions_.stop();
	UpdateLabelsGeomentry(true);
}

void NoVisualLabelsManager::ReCalcUnions()
{
	#ifdef SHOWTIMERS
	tbb::tick_count t0 = tbb::tick_count::now();	
	#endif

	QList<gui::mg1::LabelInstrumentItem*> visibleList = labels_list_;
	visibleList.detach();

	for (int i = 0;i< labels_list_.count();i++)
	{
		gui::mg1::LabelInstrumentItem* label_instrument_target_item = labels_list_.at(i);
		if (!label_instrument_target_item->isVisible()) continue;
		if (visibleList.count() == 0) break;

		//for (int b = i+1;b < labels_list_.count();b++)
		for (int b = 0;b < visibleList.count();b++)
		{
			if (b < 0) b = 0;
			if (b == visibleList.count()) break;
			gui::mg1::LabelInstrumentItem* label_instrument_check_item = visibleList.at(b);
			//LabelInstrumentItem* label_instrument_check_item = labels_list_.at(b);
			if (label_instrument_target_item == label_instrument_check_item) continue;//судя по алгоритму такого ваще не должно быть
			if (!label_instrument_check_item->isVisible()) continue;
			
			//проверяем столкновение элементов на предмет создания объеденения
			if (fastItemsIntersect(label_instrument_target_item,label_instrument_check_item))
			{
				gui::mg1::LabelInstrumentUnion* cur_target_union = CreateNewUnion();
				cur_target_union->addToUnion(label_instrument_check_item);
				cur_target_union->addToUnion(label_instrument_target_item);
				updateUnionPosition(cur_target_union);

				label_instrument_target_item->setVisible(false);
				label_instrument_check_item->setVisible(false);
				visibleList.removeOne(label_instrument_target_item);
				visibleList.removeOne(label_instrument_check_item);
				b-=2;

				//далее в цикле с самого начала проверяем кого теперь из свободных пересекает созданное объеденение а также уже созданные ранее объеденения
				bool free_items_restart_search = true;
				while (free_items_restart_search)
				{
					free_items_restart_search = false;
					for (int z = 0;z< visibleList.count();z++)
					{
						gui::mg1::LabelInstrumentItem* free_target_item = visibleList.at(z);
						if (!free_target_item->isVisible()) continue;											
						if (fastItemsIntersect(cur_target_union,free_target_item))
						{
							cur_target_union->addToUnion(free_target_item);
							free_target_item->setVisible(false);
							visibleList.removeAt(z);
							b-=1;
							updateUnionPosition(cur_target_union);
							free_items_restart_search = true;
							break;
						}
					}

					if (free_items_restart_search) continue;

					for (int u = 0;u< labels_unions_list_.count();u++)
					{
						gui::mg1::LabelInstrumentUnion* target_union = labels_unions_list_.at(u);
						if (target_union == cur_target_union) continue;
						if (fastItemsIntersect(cur_target_union,target_union))
						{
							cur_target_union->addToUnion(target_union);
							labels_unions_list_.removeOne(target_union);
							delete target_union;
							updateUnionPosition(cur_target_union);
							free_items_restart_search = true;
							break;
						}
					}
				}//while (free_items_restart_search)

			}
		}//for (int b = i+1;b < labels_list_.count();b++)

	} //for (int i = 0;i< labels_list_.count();i++)


	//установка хинтов (экономит время)
	for (int u = 0;u< labels_unions_list_.count();u++)
		labels_unions_list_.at(u)->setHint();
	
	//проверка
	/*{
		int total = 0;
		for (int u = 0;u< labels_unions_list_.count();u++)
		{
			total += labels_unions_list_.at(u)->getChilds().count();
			labels_unions_list_.at(u)->setHint();
		}
		qDebug() << "total" << total;
	}
	//*/


	#ifdef SHOWTIMERS
	tbb::tick_count t1 = tbb::tick_count::now();
	qDebug() << "ReCalcUnions time=" << (t1-t0).seconds()*1000 << " msecs";//*/
	#endif
}

gui::mg1::LabelInstrumentUnion* NoVisualLabelsManager::findInUnionsList( gui::mg1::LabelInstrumentItem* itm )
{
	for (int i = 0; i < labels_unions_list_.size();i++)
	{
		QList<gui::mg1::LabelInstrumentItem*>	zzz = labels_unions_list_.at(i)->getChilds();
		int index = zzz.indexOf(itm);
		if (index < 0) return nullptr;
		return labels_unions_list_.at(i);
	}
	return nullptr;
}

void NoVisualLabelsManager::clearUnions()
{
	while (labels_unions_list_.size() > 0)
	{
		delete labels_unions_list_.takeLast();
	}
}

void NoVisualLabelsManager::updateUnionPosition( gui::mg1::LabelInstrumentUnion* un )
{
	int central_pixel_pos = graphics_parent_->GetPixelByValue(un->getCentralScenePos(),false);//getConvertor()->SceneToPixels(un->getCentralScenePos(),Qt::Horizontal).value();
	int y_pos = un->getLastItem()->y();
	un->setPos(central_pixel_pos - un->boundingRect().width()/2,y_pos);
}

void NoVisualLabelsManager::UpdateLabelsGeomentry( bool update_unions )
{
	int pixel_y = 10;
	if (labels_list_.count() == 0) return;
#ifdef SHOWTIMERS
	tbb::tick_count t0 = tbb::tick_count::now();	
#endif

	if (update_unions)
	{
		clearUnions();
#ifdef SHOWTIMERS
		tbb::tick_count t2 = tbb::tick_count::now();	
#endif

		//расставляем как есть
		for (int i = 0;i< labels_list_.count();i++)
		{
			gui::mg1::LabelInstrumentItem* l_item = labels_list_.at(i);
			l_item->setNormalMode();
			l_item->setVisible(true);
			int center_pixel_x = graphics_parent_->GetPixelByValue(l_item->getValueScenePos(),false);//getConvertor()->SceneToPixels(l_item->getValueScenePos(), Qt::Horizontal).value();
			l_item->setPos(center_pixel_x - l_item->boundingRect().width()/2 , pixel_y);
		}

		bool needCalcUnions = false;
		//проверяем пересечения как элементов и минимизируем необходимые
		for (int i = 0;i< labels_list_.count();i++)
		{
			gui::mg1::LabelInstrumentItem* label_instrument_target_item = labels_list_.at(i);
			for (int b = i+1;b < labels_list_.count();b++)
			{
				gui::mg1::LabelInstrumentItem* label_instrument_check_item = labels_list_.at(b);
				if (label_instrument_target_item == label_instrument_check_item) continue;//судя по алгоритму такого ваще не должно быть
				//проверяем столкновение
				if (fastItemsIntersect(label_instrument_target_item,label_instrument_check_item))
				{
					if (label_instrument_target_item->isInMinimalMode() && label_instrument_check_item->isInMinimalMode())
					{
						//оба уже свернутых и пересекаются, в дальнеих действиях смысла нет
						needCalcUnions = true;
						continue;
					}

					//уменьшаем первый элемент
					label_instrument_target_item->setMinimalMode();
					//проверяем продолжается ли столкновение
					if (fastItemsIntersect(label_instrument_target_item,label_instrument_check_item))
					{
						//уменьшаем второй элемент
						label_instrument_check_item->setMinimalMode();
						//проверяем продолжается ли столкновение
						if (fastItemsIntersect(label_instrument_target_item,label_instrument_check_item))
							needCalcUnions = true;
					}
				}
			}
		}

#ifdef SHOWTIMERS
		tbb::tick_count t3 = tbb::tick_count::now();
		qDebug() << "start updateLabelsGeomentry time=" << (t3-t2).seconds()*1000 << " msecs";
#endif

		if (needCalcUnions) ReCalcUnions();
	}
	else
	{
		//только позиции
		for (int i = 0;i< labels_list_.count();i++)
		{
			gui::mg1::LabelInstrumentItem* l_item = labels_list_.at(i);
			if (!l_item->isVisible()) continue;
			int center_pixel_x = graphics_parent_->GetPixelByValue(l_item->getValueScenePos(),false);//getConvertor()->SceneToPixels(l_item->getValueScenePos(), Qt::Horizontal).value();
			l_item->setPos(center_pixel_x - l_item->boundingRect().width()/2 , pixel_y);
		}

		for (int i = 0;i< labels_unions_list_.count();i++)
		{
			updateUnionPosition(labels_unions_list_.at(i));
		}

	}


#ifdef SHOWTIMERS
	tbb::tick_count t1 = tbb::tick_count::now();
	qDebug() << "UpdateLabelsGeomentry time=" << (t1-t0).seconds()*1000 << " msecs";
#endif
}

gui::mg1::LabelInstrumentUnion* NoVisualLabelsManager::CreateNewUnion()
{
	gui::mg1::LabelInstrumentUnion* new_union = new gui::mg1::LabelInstrumentUnion(&text_drawer_,union_text_,QFont(),show_hints_);
	new_union->setParentItem(graphics_parent_);
	labels_unions_list_.append(new_union);
	return new_union;
}

bool NoVisualLabelsManager::StartTimerForUpdate()
{
	if (labels_list_.count() == 0) return false;

	if (timer_recalc_unions_.isActive())
	{
		timer_recalc_unions_.start();
		return true;
	}

	if (labels_list_.count() > 150)
	{
		timer_recalc_unions_.start();
		return true;
	}
	else
		UpdateLabelsGeomentry(true);

	return false;
}


bool NoVisualLabelsManager::fastItemsIntersect( QGraphicsItem* first_item,QGraphicsItem* second_item )
{
	QRectF fi_r = first_item->boundingRect();
	fi_r.moveTo(first_item->pos());
	QRectF si_r = second_item->boundingRect();
	si_r.moveTo(second_item->pos());

	if (fi_r.intersects(si_r)) return true;
	else return false;
}

void NoVisualLabelsManager::redraw()
{

	if ((prev_trans_x_ == graphics_parent_->getCurTrans().x()) && (prev_kf_x_ == graphics_parent_->getCurKf().x())) 
		return;	//нихера не изменилось
	
	if (prev_kf_x_ == graphics_parent_->getCurKf().x())
	{
//        qDebug() << "REDRAW!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
		//масштаб по x не менялся
		prev_trans_x_ = graphics_parent_->getCurTrans().x();
		UpdateLabelsGeomentry(false);
	}
	else
	{
//        qDebug() << "REDRAW222222222222222222222222222222222222222!";
		prev_kf_x_ = graphics_parent_->getCurKf().x();
		if (StartTimerForUpdate())
			UpdateLabelsGeomentry(false);
	}
}

void NoVisualLabelsManager::changeLabelText( QString name, QString new_text, bool immediately_update/* = false */)
{
	gui::mg1::LabelInstrumentItem* tmp = findItemByName(name);
	if (tmp == nullptr) return;
	tmp->changeText(new_text);
	if (immediately_update) Timer_recalc_unions_timeout_slot();
	else					timer_recalc_unions_.start();
}

void NoVisualLabelsManager::changeLabelPosition( QString name, double new_pos, bool immediately_update/* = false */)
{
	gui::mg1::LabelInstrumentItem* tmp = findItemByName(name);
	if (tmp == nullptr) return;
	tmp->changePos(new_pos);
	if (immediately_update) Timer_recalc_unions_timeout_slot();
	else					timer_recalc_unions_.start();
}

void NoVisualLabelsManager::removeLabel( QString name, bool immediately_update /*= false*/ )
{
	gui::mg1::LabelInstrumentItem* tmp = findItemByName(name);
	if (tmp == nullptr) return;
	labels_list_.removeOne(tmp);
	delete tmp;
	if (immediately_update) Timer_recalc_unions_timeout_slot();
	else					timer_recalc_unions_.start();
}

gui::mg1::LabelInstrumentItem* NoVisualLabelsManager::findItemByName( QString name )
{
	for (int i = 0;i < labels_list_.count();i++)
	{
		if (labels_list_[i]->getName() == name)
			return labels_list_[i];
	}
	return nullptr;
}

void NoVisualLabelsManager::setUnionsText( QString union_text )
{
	union_text_  = union_text;
	Timer_recalc_unions_timeout_slot();
}

void NoVisualLabelsManager::setUnionsShowHints( bool show_hints )
{
	show_hints_ = show_hints;
	for (int u = 0;u< labels_unions_list_.count();u++)
		labels_unions_list_.at(u)->setShowHints( show_hints );
}



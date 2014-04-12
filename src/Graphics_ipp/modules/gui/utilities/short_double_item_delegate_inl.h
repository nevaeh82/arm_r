#ifndef _GUI_UTILITIES_SHORT_DOUBLE_ITEM_DELEGATE_H_
#define _GUI_UTILITIES_SHORT_DOUBLE_ITEM_DELEGATE_H_

namespace utilities {

class ShortDoubleItemDelegate: public QStyledItemDelegate
{
public:
    ShortDoubleItemDelegate(int length = 0, QObject* parent = nullptr): QStyledItemDelegate(parent), length_(length) {}

    virtual QString displayText(const QVariant& value, const QLocale&) const
    {        
        return QString("%1").arg(value.toDouble(), 0, 'f', length_);
    }
    
private:
    int length_;
};

}  //  namespace utilities

#endif
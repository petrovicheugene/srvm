//========================================================
#include "ZActiveStyledItemDelegate.h"
//========================================================
ZActiveStyledItemDelegate::ZActiveStyledItemDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{

}
//========================================================
void	ZActiveStyledItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QStyleOptionViewItem newOption(option);

    newOption.state =  newOption.state | QStyle::State_Active;
    QStyledItemDelegate::paint(painter, newOption, index);
}
//========================================================

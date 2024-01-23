//========================================================
#include "X_ActiveStyledItemDelegate.h"
//========================================================
X_ActiveStyledItemDelegate::X_ActiveStyledItemDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{

}
//========================================================
void	X_ActiveStyledItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QStyleOptionViewItem newOption(option);

    newOption.state =  newOption.state | QStyle::State_Active;
    QStyledItemDelegate::paint(painter, newOption, index);
}
//========================================================

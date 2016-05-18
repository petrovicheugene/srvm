//========================================================
#ifndef ZACTIVESTYLEDITEMDELEGATE_H
#define ZACTIVESTYLEDITEMDELEGATE_H
//========================================================
//========================================================
#include <QStyledItemDelegate>
//========================================================
class ZActiveStyledItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ZActiveStyledItemDelegate(QWidget *parent = 0);
    virtual void	paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

};
//========================================================
#endif // ZACTIVESTYLEDITEMDELEGATE_H

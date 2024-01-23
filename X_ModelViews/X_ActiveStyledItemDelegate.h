//========================================================
#ifndef X_ACTIVESTYLEDITEMDELEGATE_H
#define X_ACTIVESTYLEDITEMDELEGATE_H
//========================================================
//========================================================
#include <QStyledItemDelegate>
//========================================================
class X_ActiveStyledItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit X_ActiveStyledItemDelegate(QWidget *parent = 0);
    virtual void	paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

};
//========================================================
#endif // X_ACTIVESTYLEDITEMDELEGATE_H

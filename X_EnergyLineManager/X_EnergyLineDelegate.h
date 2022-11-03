//======================================================================
#ifndef X_ENERGYLINEDELEGATE_H
#define X_ENERGYLINEDELEGATE_H
//======================================================================
#include <QStyledItemDelegate>
#include "X_General.h"

//======================================================================
//======================================================================
class X_EnergyLineDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit X_EnergyLineDelegate(QObject *parent = nullptr);

    virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual QSize	sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

signals:


public slots:


private:
    // VARS
    bool xv_preventMouseReleaseHandle;
    // FUNCS
    bool editorEvent ( QEvent * event,
                       QAbstractItemModel * model,
                       const QStyleOptionViewItem & option,
                       const QModelIndex & index );

    bool eventFilter(QObject *object, QEvent *event);


};
//======================================================================
#endif // X_ENERGYLINEDELEGATE_H

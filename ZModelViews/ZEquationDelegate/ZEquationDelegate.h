//=======================================================
#ifndef ZEQUATIONDELEGATE_H
#define ZEQUATIONDELEGATE_H
//=======================================================
#include <QStyledItemDelegate>
//=======================================================

//=======================================================
class ZEquationDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ZEquationDelegate(QObject *parent = 0);

    virtual QWidget* createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	setEditorData ( QWidget * editor, const QModelIndex & index ) const;
    virtual void	setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
    virtual QSize	sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;


private slots:

       // void zh_onDialogAccept();

signals:

public slots:
};
//=======================================================
#endif // ZEQUATIONDELEGATE_H

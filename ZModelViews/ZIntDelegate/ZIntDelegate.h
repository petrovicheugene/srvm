//====================================
#ifndef ZINTDELEGATE_H
#define ZINTDELEGATE_H
//====================================
#include <QStyledItemDelegate>
//====================================
class ZIntDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ZIntDelegate(QObject *parent = 0);

    QWidget * createEditor(QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	setEditorData(QWidget * editor, const QModelIndex & index) const;
    virtual void	setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
    virtual QSize	sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;


public slots:

    void zp_setEditorMinMax(qint64 min, qint64 max) ;

private:

    // VARS
    qint64 zv_editorMin;
    qint64 zv_editorMax;

    //funcs

};
//====================================
#endif // ZINTDELEGATE_H

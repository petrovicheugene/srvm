//=====================================================
#ifndef X_STRINGDELEGATE_H
#define X_STRINGDELEGATE_H
//=====================================================
#include <QStyledItemDelegate>
//=====================================================


//=====================================================
class X_StringDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit X_StringDelegate(QObject *parent = 0);

    virtual QWidget * createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void	paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void	setEditorData(QWidget* editor, const QModelIndex& index) const;
    virtual void	setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    virtual QSize	sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void	updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    //☺void mf_setFillEmptiness(bool flag);

private:
    //bool mv_fillEmptinessFlag;

signals:

public slots:

};
//=====================================================
#endif // X_STRINGDELEGATE_H

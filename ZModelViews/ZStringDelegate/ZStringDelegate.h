//=====================================================
#ifndef ZSTRINGDELEGATE_H
#define ZSTRINGDELEGATE_H
//=====================================================
#include <QStyledItemDelegate>
//=====================================================


//=====================================================
class ZStringDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ZStringDelegate(bool fillEmptinessFlag = false, QObject *parent = 0);

    virtual QWidget * createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void	paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void	setEditorData(QWidget* editor, const QModelIndex& index) const;
    virtual void	setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    virtual QSize	sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void	updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    void mf_setFillEmptiness(bool flag);

private:
    bool mv_fillEmptinessFlag;

signals:

public slots:

};
//=====================================================
#endif // ZSTRINGDELEGATE_H

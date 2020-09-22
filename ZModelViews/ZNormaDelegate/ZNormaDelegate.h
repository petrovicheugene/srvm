//=======================================================
#ifndef ZNORMADELEGATE_H
#define ZNORMADELEGATE_H
//=======================================================
#include <QStyledItemDelegate>
//=======================================================
class ZNormaDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ZNormaDelegate(QObject *parent = 0);

    virtual QWidget* createEditor ( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    virtual void	paint ( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    virtual void	setEditorData ( QWidget* editor, const QModelIndex& index ) const;
    virtual void	setModelData ( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const;
    virtual QSize sizeHint ( const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    virtual void	updateEditorGeometry ( QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

signals:

    void zg_currentCalibrationWindowName(const QString& windowName) const;

public slots:


};
//=======================================================
#endif // ZNORMADELEGATE_H

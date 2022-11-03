//=======================================================
#ifndef X_NORMADELEGATE_H
#define X_NORMADELEGATE_H
//=======================================================
#include <QStyledItemDelegate>
//=======================================================
class X_NormaDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit X_NormaDelegate(QObject *parent = 0);

    virtual QWidget* createEditor ( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    virtual void	paint ( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    virtual void	setEditorData ( QWidget* editor, const QModelIndex& index ) const;
    virtual void	setModelData ( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const;
    virtual QSize sizeHint ( const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    virtual void	updateEditorGeometry ( QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

signals:

    void xg_currentCalibrationWindowName(const QString& windowName) const;

public slots:


};
//=======================================================
#endif // X_NORMADELEGATE_H

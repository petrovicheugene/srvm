//==================================================
#ifndef X_WINDOWTYPECOMBOBOXDELEGATE_H
#define X_WINDOWTYPECOMBOBOXDELEGATE_H
//==================================================
#include <QStyledItemDelegate>
#include <QStringList>
//==================================================
class X_WindowTypeComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit X_WindowTypeComboBoxDelegate(QObject *parent = 0);

    virtual QWidget * createEditor(QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	setEditorData(QWidget * editor, const QModelIndex & index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:


signals:


public slots:

private:

 };
//==================================================
#endif // X_WINDOWTYPECOMBOBOXDELEGATE_H

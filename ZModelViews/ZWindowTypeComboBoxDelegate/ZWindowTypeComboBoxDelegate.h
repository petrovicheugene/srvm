//==================================================
#ifndef ZWINDOWTYPECOMBOBOXDELEGATE_H
#define ZWINDOWTYPECOMBOBOXDELEGATE_H
//==================================================
#include <QStyledItemDelegate>
#include <QStringList>
//==================================================
class ZWindowTypeComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ZWindowTypeComboBoxDelegate(QObject *parent = 0);

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
#endif // ZWINDOWTYPECOMBOBOXDELEGATE_H

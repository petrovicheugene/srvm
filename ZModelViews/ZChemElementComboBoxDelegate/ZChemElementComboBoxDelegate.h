//==================================================
#ifndef ZCOMBOBOXDELEGATE_H
#define ZCOMBOBOXDELEGATE_H
//==================================================
#include <QStyledItemDelegate>
#include <QStringList>
//==================================================
class ZChemElementComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ZChemElementComboBoxDelegate(QObject *parent = 0);

    virtual QWidget * createEditor(QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	setEditorData(QWidget * editor, const QModelIndex & index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:


signals:

    void zg_requestChemElementList(QStringList&) const;

public slots:

private:

    // VARS
    QString zv_notDefinedString;
    QString zv_notDefinedDisplayString;
};
//==================================================
#endif // ZCOMBOBOXDELEGATE_H

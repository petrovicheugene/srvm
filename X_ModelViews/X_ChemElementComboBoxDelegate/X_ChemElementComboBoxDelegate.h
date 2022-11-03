//==================================================
#ifndef X_COMBOBOXDELEGATE_H
#define X_COMBOBOXDELEGATE_H
//==================================================
#include <QStyledItemDelegate>
#include <QStringList>
//==================================================
class X_ChemElementComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit X_ChemElementComboBoxDelegate(QObject *parent = 0);

    virtual QWidget * createEditor(QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	setEditorData(QWidget * editor, const QModelIndex & index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:


signals:

    void xg_requestChemElementList(QStringList&) const;

public slots:

private:

    // VARS
    QString xv_notDefinedDisplayString;
};
//==================================================
#endif // X_COMBOBOXDELEGATE_H

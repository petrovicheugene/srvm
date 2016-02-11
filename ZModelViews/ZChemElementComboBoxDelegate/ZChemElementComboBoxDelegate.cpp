//==================================================
#include "ZChemElementComboBoxDelegate.h"
#include "globalVariables.h"
#include <QComboBox>
#include <QStyle>
#include <QApplication>
//==================================================
ZChemElementComboBoxDelegate::ZChemElementComboBoxDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    zv_notDefinedDisplayString = tr("Not defined");
}
//==================================================
QWidget* ZChemElementComboBoxDelegate::createEditor(QWidget* parent,
                                         const QStyleOptionViewItem & option,
                                         const QModelIndex & index ) const
{
    QComboBox* editor = new QComboBox(parent);
    QStringList chemElementList;
    emit zg_requestChemElementList(chemElementList);
    editor->addItem(zv_notDefinedDisplayString);

    QVariant vData = index.model()->data(index, Qt::DisplayRole);
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
    {
        QString currentChemElement = vData.toString();
        if(!chemElementList.contains(currentChemElement) && currentChemElement != glDefaultChemElementString)
        {
            editor->addItem(currentChemElement);
        }
    }

    editor->addItems(chemElementList);
    return editor;
}
//================================================
void	ZChemElementComboBoxDelegate::setEditorData(QWidget * editor,
                                                    const QModelIndex & index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QVariant vData = index.model()->data(index, Qt::DisplayRole);
    if(!vData.isValid() || vData.isNull() || !vData.canConvert<QString>())
    {
        comboBox->setCurrentIndex(0);
        return;
    }

    QString currentChemElement = vData.toString() ;
    int comboBoxIndex = comboBox->findText(currentChemElement);
    if(comboBoxIndex < 0)
    {
        comboBoxIndex = 0;
    }
    comboBox->setCurrentIndex(comboBoxIndex);
}
//================================================
void ZChemElementComboBoxDelegate::setModelData(QWidget *editor,
                                                QAbstractItemModel *model,
                                                const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString currentText = comboBox->currentText();
    if(currentText == zv_notDefinedDisplayString)
    {
        currentText = glDefaultChemElementString;
    }
     model->setData(index, currentText, Qt::EditRole);
}
//================================================
void ZChemElementComboBoxDelegate::updateEditorGeometry(QWidget *editor,
                                                        const QStyleOptionViewItem &option,
                                                        const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
//================================================
void ZChemElementComboBoxDelegate::paint(QPainter *painter,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    QVariant vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || vData.isNull() || !vData.canConvert<QString>())
    {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }
    QStyleOptionViewItemV4 myOption = option;

    QString dataString = vData.toString();
    if(dataString == glDefaultChemElementString)
    {
        myOption.text = zv_notDefinedDisplayString;
    }
    else
    {
        myOption.text = dataString;
    }
    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
}
//================================================

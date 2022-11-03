//==================================================
#include "X_ChemElementComboBoxDelegate.h"
#include "X_General.h"
#include <QComboBox>
#include <QStyle>
#include <QApplication>
//==================================================
X_ChemElementComboBoxDelegate::X_ChemElementComboBoxDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    xv_notDefinedDisplayString = tr("Not defined");
}
//==================================================
QWidget* X_ChemElementComboBoxDelegate::createEditor(QWidget* parent,
                                         const QStyleOptionViewItem & option,
                                         const QModelIndex & index ) const
{
    QComboBox* editor = new QComboBox(parent);
    QStringList chemElementList;
    emit xg_requestChemElementList(chemElementList);
    editor->addItem(xv_notDefinedDisplayString);

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
void	X_ChemElementComboBoxDelegate::setEditorData(QWidget * editor,
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
void X_ChemElementComboBoxDelegate::setModelData(QWidget *editor,
                                                QAbstractItemModel *model,
                                                const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString currentText = comboBox->currentText();
    if(currentText == xv_notDefinedDisplayString)
    {
        currentText = glDefaultChemElementString;
    }
     model->setData(index, currentText, Qt::EditRole);
}
//================================================
void X_ChemElementComboBoxDelegate::updateEditorGeometry(QWidget *editor,
                                                        const QStyleOptionViewItem &option,
                                                        const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
//================================================
void X_ChemElementComboBoxDelegate::paint(QPainter *painter,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    QStyleOptionViewItem newOption(option);
    initStyleOption(&newOption, index);
    newOption.state =  newOption.state | QStyle::State_Active;

//    QStyleOptionViewItemV4 myOption
//            = option;
//    newOption.state =  newOption.state | QStyle::State_Active;


    QVariant vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || vData.isNull() || !vData.canConvert<QString>())
    {
        QStyledItemDelegate::paint(painter, newOption, index);
        return;
    }
     QString dataString = vData.toString();
    if(dataString == glDefaultChemElementString)
    {
        newOption.text = xv_notDefinedDisplayString;
    }
    else
    {
        newOption.text = dataString;
    }
    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &newOption, painter);
}
//================================================

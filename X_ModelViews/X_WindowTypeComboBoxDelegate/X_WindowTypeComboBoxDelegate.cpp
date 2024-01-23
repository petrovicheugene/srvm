//==================================================
#include "X_WindowTypeComboBoxDelegate.h"
#include "X_CalibrationWindow.h"
#include <QComboBox>
#include <QStyle>
#include <QApplication>
//==================================================
X_WindowTypeComboBoxDelegate::X_WindowTypeComboBoxDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}
//==================================================
QWidget* X_WindowTypeComboBoxDelegate::createEditor(QWidget* parent,
                                                   const QStyleOptionViewItem & option,
                                                   const QModelIndex & index ) const
{
    QComboBox* editor = new QComboBox(parent);

    QStringList typeNameList;
    typeNameList << X_CalibrationWindow::xp_displayTypeName(X_CalibrationWindow::WT_NOT_DEFINED);
    //typeNameList << X_CalibrationWindow::xp_typeName(X_CalibrationWindow::WT_BASE_PEAK);
    typeNameList << X_CalibrationWindow::xp_displayTypeName(X_CalibrationWindow::WT_PEAK);
    typeNameList << X_CalibrationWindow::xp_displayTypeName(X_CalibrationWindow::WT_COHERENT);
    typeNameList << X_CalibrationWindow::xp_displayTypeName(X_CalibrationWindow::WT_INCOHERENT);

    editor->addItems(typeNameList);
    return editor;
}
//================================================
void	X_WindowTypeComboBoxDelegate::setEditorData(QWidget * editor,
                                                   const QModelIndex & index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QVariant vData = index.model()->data(index, Qt::DisplayRole);
    if(!vData.isValid() || vData.isNull() || !vData.canConvert<QString>())
    {
        comboBox->setCurrentIndex(0);
        return;
    }

    QString currentWindowType = vData.toString() ;
    int comboBoxIndex = comboBox->findText(currentWindowType);
    if(comboBoxIndex < 0)
    {
        comboBoxIndex = 0;
    }
    comboBox->setCurrentIndex(comboBoxIndex);
}
//================================================
void X_WindowTypeComboBoxDelegate::setModelData(QWidget *editor,
                                               QAbstractItemModel *model,
                                               const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString currentText = comboBox->currentText();
    model->setData(index, QVariant(currentText), Qt::EditRole);
}
//================================================
void X_WindowTypeComboBoxDelegate::updateEditorGeometry(QWidget *editor,
                                                       const QStyleOptionViewItem &option,
                                                       const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
//================================================
void X_WindowTypeComboBoxDelegate::paint(QPainter *painter,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    QVariant vData = index.data(Qt::DisplayRole);
    if(!vData.isValid() || vData.isNull() || !vData.canConvert<QString>())
    {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }
    QStyleOptionViewItem myOption = option;
    myOption.state =  myOption.state | QStyle::State_Active;

    QString dataString = vData.toString();
    myOption.text = dataString;
    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
}
//================================================


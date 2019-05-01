//==================================================
#include "ZWindowTypeComboBoxDelegate.h"
#include "ZCalibrationWindow.h"
#include <QComboBox>
#include <QStyle>
#include <QApplication>
//==================================================
ZWindowTypeComboBoxDelegate::ZWindowTypeComboBoxDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}
//==================================================
QWidget* ZWindowTypeComboBoxDelegate::createEditor(QWidget* parent,
                                                   const QStyleOptionViewItem & option,
                                                   const QModelIndex & index ) const
{
    QComboBox* editor = new QComboBox(parent);

    QStringList typeNameList;
    typeNameList << ZCalibrationWindow::zp_displayTypeName(ZCalibrationWindow::WT_NOT_DEFINED);
    //typeNameList << ZCalibrationWindow::zp_typeName(ZCalibrationWindow::WT_BASE_PEAK);
    typeNameList << ZCalibrationWindow::zp_displayTypeName(ZCalibrationWindow::WT_PEAK);
    typeNameList << ZCalibrationWindow::zp_displayTypeName(ZCalibrationWindow::WT_COHERENT);
    typeNameList << ZCalibrationWindow::zp_displayTypeName(ZCalibrationWindow::WT_INCOHERENT);

    editor->addItems(typeNameList);
    return editor;
}
//================================================
void	ZWindowTypeComboBoxDelegate::setEditorData(QWidget * editor,
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
void ZWindowTypeComboBoxDelegate::setModelData(QWidget *editor,
                                               QAbstractItemModel *model,
                                               const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString currentText = comboBox->currentText();
    model->setData(index, QVariant(currentText), Qt::EditRole);
}
//================================================
void ZWindowTypeComboBoxDelegate::updateEditorGeometry(QWidget *editor,
                                                       const QStyleOptionViewItem &option,
                                                       const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
//================================================
void ZWindowTypeComboBoxDelegate::paint(QPainter *painter,
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


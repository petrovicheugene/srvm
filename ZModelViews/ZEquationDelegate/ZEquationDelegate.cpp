//======================================================================
#include "ZEquationDelegate.h"
#include "ZEquationChooseDialog.h"
#include "ZEquationSettingsData.h"
#include "globalVariables.h"
#include <QStyle>
#include <QApplication>

//======================================================================
ZEquationDelegate::ZEquationDelegate(QObject* parent) : QStyledItemDelegate(parent)
{
    zv_editorChildDialogOpen = false;
}
//======================================================================
QWidget* ZEquationDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    ZEquationChooseDialog* dialog = new ZEquationChooseDialog();
    dialog->setAttribute(Qt::WA_DeleteOnClose);



    return dialog;
}
//======================================================================
void	ZEquationDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItemV4 newOption(option);
    newOption.state =  newOption.state | QStyle::State_Active;

    QStyledItemDelegate::paint(painter, newOption, index);
}
//======================================================================
void	ZEquationDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if(!index.isValid())
    {
        return;
    }

    ZEquationChooseDialog* dialog = qobject_cast<ZEquationChooseDialog*>(editor);
    if(!dialog)
    {
        return;
    }

    QVariant vSettings = index.data(Qt::EditRole);
    if(!vSettings.isValid() || vSettings.isNull() || !vSettings.canConvert<ZEquationSettingsData>())
    {
        return;
    }

    dialog->zp_setEquationSettings(vSettings.value<ZEquationSettingsData>());

    //QStyledItemDelegate::setEditorData(editor, index);
}
//======================================================================
void	ZEquationDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    ZEquationChooseDialog* dialog = qobject_cast<ZEquationChooseDialog*>(editor);
    if(!dialog)
    {
        return;
    }

    if(dialog->result() == QDialog::Rejected) // zp_accepted())
    {
        return;
    }

    ZEquationSettingsData settings = dialog->zp_equationSettings();
    model->setData(index, QVariant::fromValue(settings));
}
//======================================================================
QSize ZEquationDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}
//======================================================================
void	ZEquationDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    ZEquationChooseDialog* dialog = qobject_cast<ZEquationChooseDialog*>(editor);
    if(!dialog)
    {
        return;
    }

    int deltaX = QApplication::style()->pixelMetric(QStyle::PM_MdiSubWindowFrameWidth);
    int deltaY = QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);

    QPoint globalEditorPoint = option.widget->mapToGlobal(option.rect.bottomRight());

    globalEditorPoint.setX(globalEditorPoint.x() + deltaX);
    globalEditorPoint.setY(globalEditorPoint.y() + deltaY);

    dialog->move(globalEditorPoint);

    // QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}
//======================================================================
void ZEquationDelegate::zh_onEditorChildDialogOpen(bool dialogOpen)
{
    zv_editorChildDialogOpen = dialogOpen;
}
//======================================================================

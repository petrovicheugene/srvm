//======================================================================
#include "X_EquationDelegate.h"
#include "X_EquationChooseDialog.h"
#include "X_EquationSettingsData.h"
#include "X_General.h"
#include <QStyle>
#include <QApplication>

//======================================================================
X_EquationDelegate::X_EquationDelegate(QObject* parent) : QStyledItemDelegate(parent)
{
    xv_editorChildDialogOpen = false;
}
//======================================================================
QWidget* X_EquationDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    X_EquationChooseDialog* dialog = new X_EquationChooseDialog();
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(this, &X_EquationDelegate::xg_currentCalibrationWindowName,
            dialog, &X_EquationChooseDialog::xg_currentCalibrationWindowName);

    return dialog;
}
//======================================================================
void	X_EquationDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem newOption(option);
    newOption.state =  newOption.state | QStyle::State_Active;

    QStyledItemDelegate::paint(painter, newOption, index);
}
//======================================================================
void	X_EquationDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if(!index.isValid())
    {
        return;
    }

    X_EquationChooseDialog* dialog = qobject_cast<X_EquationChooseDialog*>(editor);
    if(!dialog)
    {
        return;
    }

    QVariant vSettings = index.data(Qt::EditRole);
    if(!vSettings.isValid() || vSettings.isNull() || !vSettings.canConvert<X_EquationSettingsData>())
    {
        return;
    }

    dialog->xp_setEquationSettings(vSettings.value<X_EquationSettingsData>());

    //QStyledItemDelegate::setEditorData(editor, index);
}
//======================================================================
void	X_EquationDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    X_EquationChooseDialog* dialog = qobject_cast<X_EquationChooseDialog*>(editor);
    if(!dialog)
    {
        return;
    }

    if(dialog->result() == QDialog::Rejected) // xp_accepted())
    {
        return;
    }

    X_EquationSettingsData settings = dialog->xp_equationSettings();
    model->setData(index, QVariant::fromValue(settings));
}
//======================================================================
QSize X_EquationDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}
//======================================================================
void	X_EquationDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    X_EquationChooseDialog* dialog = qobject_cast<X_EquationChooseDialog*>(editor);
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
void X_EquationDelegate::zh_onEditorChildDialogOpen(bool dialogOpen)
{
    xv_editorChildDialogOpen = dialogOpen;
}
//======================================================================

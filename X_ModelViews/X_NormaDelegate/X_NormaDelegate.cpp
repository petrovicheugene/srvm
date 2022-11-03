//=======================================================
#include "X_NormaDelegate.h"
#include "X_NormaSettingsDialog.h"
#include "X_General.h"
#include <QStyle>
#include <QApplication>

//=======================================================
X_NormaDelegate::X_NormaDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}
//=======================================================
QWidget* X_NormaDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    X_NormaSettingsDialog* dialog = new X_NormaSettingsDialog();
    connect(this,
            &X_NormaDelegate::xg_currentCalibrationWindowName,
            dialog,
            &X_NormaSettingsDialog::xp_insertVariable);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowFlag(Qt::WindowStaysOnTopHint);

    return dialog;
}
//=======================================================
void	X_NormaDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem newOption(option);
    newOption.state =  newOption.state | QStyle::State_Active;

    QStyledItemDelegate::paint(painter, newOption, index);
}
//=======================================================
void	X_NormaDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if(!index.isValid())
    {
        return;
    }

    X_NormaSettingsDialog* dialog = qobject_cast<X_NormaSettingsDialog*>(editor);
    if(!dialog)
    {
        return;
    }

    QVariant vSettings = index.data(Qt::EditRole);
    if(!vSettings.isValid() || vSettings.isNull() || !vSettings.canConvert<X_NormaSettingsData>())
    {
        return;
    }

    dialog->xp_setNormaSettings(vSettings.value<X_NormaSettingsData>());
}
//=======================================================
void	X_NormaDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    X_NormaSettingsDialog* dialog = qobject_cast<X_NormaSettingsDialog*>(editor);
    if(!dialog)
    {
        return;
    }

    if(dialog->result() == QDialog::Rejected)
    {
        return;
    }

    X_NormaSettingsData settings = dialog->xp_normaSettings();
    model->setData(index, QVariant::fromValue(settings));
}
//=======================================================
QSize X_NormaDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}
//=======================================================
void	X_NormaDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    X_NormaSettingsDialog* dialog = qobject_cast<X_NormaSettingsDialog*>(editor);
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
    dialog->activateWindow();

}
//=======================================================


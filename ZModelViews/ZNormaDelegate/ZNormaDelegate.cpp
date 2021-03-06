//=======================================================
#include "ZNormaDelegate.h"
#include "ZNormaSettingsDialog.h"
#include "ZGeneral.h"
#include <QStyle>
#include <QApplication>

//=======================================================
ZNormaDelegate::ZNormaDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}
//=======================================================
QWidget* ZNormaDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    ZNormaSettingsDialog* dialog = new ZNormaSettingsDialog();
    connect(this,
            &ZNormaDelegate::zg_currentCalibrationWindowName,
            dialog,
            &ZNormaSettingsDialog::zp_insertVariable);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowFlag(Qt::WindowStaysOnTopHint);

    return dialog;
}
//=======================================================
void	ZNormaDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem newOption(option);
    newOption.state =  newOption.state | QStyle::State_Active;

    QStyledItemDelegate::paint(painter, newOption, index);
}
//=======================================================
void	ZNormaDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if(!index.isValid())
    {
        return;
    }

    ZNormaSettingsDialog* dialog = qobject_cast<ZNormaSettingsDialog*>(editor);
    if(!dialog)
    {
        return;
    }

    QVariant vSettings = index.data(Qt::EditRole);
    if(!vSettings.isValid() || vSettings.isNull() || !vSettings.canConvert<ZNormaSettingsData>())
    {
        return;
    }

    dialog->zp_setNormaSettings(vSettings.value<ZNormaSettingsData>());
}
//=======================================================
void	ZNormaDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    ZNormaSettingsDialog* dialog = qobject_cast<ZNormaSettingsDialog*>(editor);
    if(!dialog)
    {
        return;
    }

    if(dialog->result() == QDialog::Rejected)
    {
        return;
    }

    ZNormaSettingsData settings = dialog->zp_normaSettings();
    model->setData(index, QVariant::fromValue(settings));
}
//=======================================================
QSize ZNormaDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}
//=======================================================
void	ZNormaDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    ZNormaSettingsDialog* dialog = qobject_cast<ZNormaSettingsDialog*>(editor);
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


//=======================================================
#include "ZNormaDelegate.h"
#include "ZNormaSettingsDialog.h"
#include "globalVariables.h"
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
    dialog->setAttribute(Qt::WA_DeleteOnClose);
//    connect(dialog, &ZNormaSettingsDialog::accepted,
//            this, &ZNormaDelegate::zh_onDialogAccept);
    connect(dialog, &ZNormaSettingsDialog::rejected,
            dialog, &ZNormaSettingsDialog::close);

    return dialog;
}
//=======================================================
void	ZNormaDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);
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

    if(!dialog->zp_accepted())
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


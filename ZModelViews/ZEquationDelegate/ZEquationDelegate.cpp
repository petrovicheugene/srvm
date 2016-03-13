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

}
//======================================================================
QWidget* ZEquationDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    ZEquationChooseDialog* dialog = new ZEquationChooseDialog();
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(dialog, &ZEquationChooseDialog::accepted,
            dialog, &ZEquationChooseDialog::close);

    connect(dialog, &ZEquationChooseDialog::rejected,
            dialog, &ZEquationChooseDialog::close);

    return dialog;
}
//======================================================================
void	ZEquationDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);
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

    if(!dialog->zp_accepted())
    {
        return;
    }

    ZEquationSettingsData settings = dialog->zp_equationSettings();
    model->setData(index, QVariant::fromValue(settings));

    //    QStyledItemDelegate::setModelData(editor, model, index);
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
    dialog->activateWindow();

    // QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}
//======================================================================
//void ZEquationDelegate::zh_onDialogAccept()
//{
//#ifdef DBG
//    qDebug() <<  "DIALOG ACCEPTED";
//#endif

//    ZEquationChooseDialog* dialog = qobject_cast<ZEquationChooseDialog*>(sender());
//    if(dialog)
//    {
//        dialog->close();
//    }
//}
//======================================================================

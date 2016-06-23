//=====================================================
#include "ZStringDelegate.h"
#include <QLineEdit>
#include <QDebug>
#include <QPainter>
//=====================================================
ZStringDelegate::ZStringDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    //mv_fillEmptinessFlag = fillEmptinessFlag;
}
//=====================================================
QWidget* ZStringDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    //QLineEdit* editor = new QLineEdit(parent);
    //return editor;
    return QStyledItemDelegate::createEditor(parent, option, index);
}
//=====================================================
void	ZStringDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem newOption(option);
    newOption.state =  newOption.state | QStyle::State_Active;

    QStyledItemDelegate::paint(painter, option, index);
}
//=====================================================
void	ZStringDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    // ZStringDelegate::setEditorData(editor, index);
    if(!index.isValid())
    {
        return;
    }

    QLineEdit* lineEditor = qobject_cast<QLineEdit*>(editor);
    if(lineEditor == 0)
    {
        return;
    }

    QVariant vData = index.data();
    if(!vData.isValid() || vData.isNull() || !vData.canConvert<QString>())
    {
        return;
    }

    QString str = vData.toString();
//    if(mv_fillEmptinessFlag && str == "---")
//    {
//        str = "";
//    }

    lineEditor->setText(str);
}
//=====================================================
void	ZStringDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
//    if(mv_fillEmptinessFlag)
//    {
//        QLineEdit* lineEditor = qobject_cast<QLineEdit*>(editor);
//        if(lineEditor != 0 )
//        {
//            if(lineEditor->text().isEmpty())
//            {
//                lineEditor->setText("---");
//            }
//        }
//    }

    QStyledItemDelegate::setModelData(editor, model, index);
}
//=====================================================
QSize ZStringDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}
//=====================================================
void	ZStringDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}
//=====================================================
//void ZStringDelegate::mf_setFillEmptiness(bool flag)
//{
//    mv_fillEmptinessFlag = flag;
//}
//=====================================================

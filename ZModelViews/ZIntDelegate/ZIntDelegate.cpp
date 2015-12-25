//====================================
#include "ZIntDelegate.h"
#include <QIntValidator>
#include "QSpinBox"
//====================================
ZIntDelegate::ZIntDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    zv_editorMin = std::numeric_limits<long>::min();
    zv_editorMax = std::numeric_limits<long>::max();
}
//================================================
QWidget* ZIntDelegate::createEditor(QWidget* parent,
                                    const QStyleOptionViewItem & option,
                                    const QModelIndex & index ) const
{
    //    QWidget* editor = QStyledItemDelegate::createEditor(parent, option, index);
    //    QLineEdit* lineEditEditor = qobject_cast<QLineEdit*>(editor);
    //    if( lineEditEditor )
    //    {
    //        QIntValidator* validator = new QIntValidator(lineEditEditor);
    //        lineEditEditor->setValidator(validator);
    //    }

    //    return editor;
    QSpinBox* editor = new QSpinBox(parent);
    //    QIntValidator* validator = new QIntValidator(lineEditEditor);
    //    editor->setValidator(validator);
    editor->setMinimum(zv_editorMin);
    editor->setMaximum(zv_editorMax);
    return editor;

}
//================================================
void	ZIntDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    //    QVariant vEditorData = index.model()->data(index);
    //    QLineEdit* lineEditEditor = qobject_cast<QLineEdit*>(editor);
    //    if(!vEditorData.isValid() || !vEditorData.canConvert<QString>() || !lineEditEditor)
    //    {
    //        QStyledItemDelegate::setEditorData(editor, index);
    //    }

    //    lineEditEditor->setText(vEditorData.toString());

    QSpinBox* intEditor = qobject_cast<QSpinBox*>(editor);
    QVariant vEditorData = index.model()->data(index);
    if(!vEditorData.isValid() || !vEditorData.canConvert<int>() || !intEditor)
    {
        QStyledItemDelegate::setEditorData(editor, index);
    }

    intEditor->setValue(vEditorData.toInt());
}
//================================================
void	ZIntDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
    if(model == 0 || !index.isValid())
    {
        return;
    }
    QSpinBox* intEditor = qobject_cast<QSpinBox*>(editor);
    if(!intEditor)
    {
        return;
    }

    model->setData(index, QVariant(intEditor->value()));
}
//================================================
QSize	ZIntDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    return QStyledItemDelegate::sizeHint (option, index);
}
//================================================
void	ZIntDelegate::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QStyledItemDelegate::updateEditorGeometry (editor, option, index);
}
//================================================
void ZIntDelegate::zp_setEditorMinMax(qint64 min, qint64 max)
{
    if(min < max || min < std::numeric_limits<long>::min() || max > std::numeric_limits<long>::max())
    {
        zv_editorMin = min;
        zv_editorMax = max;
    }
}
//================================================

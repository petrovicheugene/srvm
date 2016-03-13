//====================================
#include "ZChannelNumberDelegate.h"
#include <QIntValidator>
#include "QSpinBox"
//====================================
ZChannelNumberDelegate::ZChannelNumberDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
   zv_editorMin = std::numeric_limits<long>::min();
   zv_editorMax = std::numeric_limits<long>::max();
   zv_currentModelIndex = QModelIndex();
   zv_immediatellyValueChangeFlag = true;
}
//================================================
QWidget* ZChannelNumberDelegate::createEditor(QWidget* parent,
                                              const QStyleOptionViewItem & option,
                                              const QModelIndex& index ) const
{
   QSpinBox* editor = new QSpinBox(parent);
   QModelIndex* pCurrentModelIndex = const_cast<QModelIndex*>(&zv_currentModelIndex);
   *pCurrentModelIndex = index;
   bool minMaxSetRes = false;

   if(index.column() == 2)
   {
      QModelIndex nextIndex = index.model()->index(index.row(), 3);
      if(nextIndex.isValid())
      {
         QVariant vData = nextIndex.data();
         if(vData.isValid() && vData.canConvert<int>())
         {
            bool ok;
            int nextChannelValue = vData.toInt(&ok);
            if(ok)
            {
               editor->setMinimum(zv_editorMin);
               editor->setMaximum(nextChannelValue);
               minMaxSetRes = true;
            }
         }
      }
   }
   else if(index.column() == 3)
   {
      QModelIndex nextIndex = index.model()->index(index.row(), 2);
      if(nextIndex.isValid())
      {
         QVariant vData = nextIndex.data();
         if(vData.isValid() && vData.canConvert<int>())
         {
            bool ok;
            int nextChannelValue = vData.toInt(&ok);
            if(ok)
            {
               editor->setMinimum(nextChannelValue);
               editor->setMaximum(zv_editorMax);
               minMaxSetRes = true;
            }
         }
      }
   }

   if(!minMaxSetRes)
   {
      editor->setMinimum(zv_editorMin);
      editor->setMaximum(zv_editorMax);
   }

   if(zv_immediatellyValueChangeFlag)
   {
       ZChannelNumberDelegate* pThis = const_cast<ZChannelNumberDelegate*>(this);
       connect(editor, SIGNAL(valueChanged(int)), pThis, SLOT(zh_editorValueChanged(int)));
   }
   return editor;
}
//================================================
void	ZChannelNumberDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
   QSpinBox* intEditor = qobject_cast<QSpinBox*>(editor);
   QVariant vEditorData = index.model()->data(index);
   if(!vEditorData.isValid() || !vEditorData.canConvert<int>() || !intEditor)
   {
      QStyledItemDelegate::setEditorData(editor, index);
   }

   intEditor->setValue(vEditorData.toInt());
}
//================================================
void	ZChannelNumberDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
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
QSize	ZChannelNumberDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
   return QStyledItemDelegate::sizeHint (option, index);
}
//================================================
void	ZChannelNumberDelegate::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
   QStyledItemDelegate::updateEditorGeometry (editor, option, index);
}
//================================================
void ZChannelNumberDelegate::zp_setImmediatellyValueChange(bool flag)
{
    zv_immediatellyValueChangeFlag = flag;
}
//================================================
void ZChannelNumberDelegate::zp_setEditorMinMax(qint64 min, qint64 max)
{
   if(min < max || min < std::numeric_limits<long>::min() || max > std::numeric_limits<long>::max())
   {
      zv_editorMin = min;
      zv_editorMax = max;
   }
}
//================================================
void ZChannelNumberDelegate::zh_editorValueChanged(int value)
{
   QWidget* editor = qobject_cast<QWidget*>(sender());
   if(editor == 0 || !zv_currentModelIndex.isValid())
   {
      return;
   }
   QAbstractItemModel* model = const_cast<QAbstractItemModel*>(zv_currentModelIndex.model());
   setModelData (editor, model, zv_currentModelIndex);
}
//================================================

//====================================
#include "X_ChannelNumberDelegate.h"
#include <QIntValidator>
#include "QSpinBox"
//====================================
X_ChannelNumberDelegate::X_ChannelNumberDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
   xv_editorMin = std::numeric_limits<long>::min();
   xv_editorMax = std::numeric_limits<long>::max();
   xv_currentModelIndex = QModelIndex();
   xv_immediatellyValueChangeFlag = true;
}
//================================================
QWidget* X_ChannelNumberDelegate::createEditor(QWidget* parent,
                                              const QStyleOptionViewItem & option,
                                              const QModelIndex& index ) const
{
   QSpinBox* editor = new QSpinBox(parent);
   QModelIndex* pCurrentModelIndex = const_cast<QModelIndex*>(&xv_currentModelIndex);
   *pCurrentModelIndex = index;
   bool minMaxSetRes = false;

   if(index.column() == 3)
   {
      QModelIndex nextIndex = index.model()->index(index.row(), 4);
      if(nextIndex.isValid())
      {
         QVariant vData = nextIndex.data();
         if(vData.isValid() && vData.canConvert<int>())
         {
            bool ok;
            int nextChannelValue = vData.toInt(&ok);
            if(ok)
            {
               editor->setMinimum(xv_editorMin);
               editor->setMaximum(nextChannelValue);
               minMaxSetRes = true;
            }
         }
      }
   }
   else if(index.column() == 4)
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
               editor->setMinimum(nextChannelValue);
               editor->setMaximum(xv_editorMax);
               minMaxSetRes = true;
            }
         }
      }
   }

   if(!minMaxSetRes)
   {
      editor->setMinimum(xv_editorMin);
      editor->setMaximum(xv_editorMax);
   }

   if(xv_immediatellyValueChangeFlag)
   {
       X_ChannelNumberDelegate* pThis = const_cast<X_ChannelNumberDelegate*>(this);
       connect(editor, SIGNAL(valueChanged(int)), pThis, SLOT(xh_editorValueChanged(int)));
   }
   return editor;
}
//=================================================================
void	X_ChannelNumberDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QStyleOptionViewItem newOption(option);
    newOption.state =  newOption.state | QStyle::State_Active;
    QStyledItemDelegate::paint(painter, newOption, index);
}
//================================================
void	X_ChannelNumberDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
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
void	X_ChannelNumberDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
   if(model == nullptr || !index.isValid())
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
QSize	X_ChannelNumberDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
   return QStyledItemDelegate::sizeHint (option, index);
}
//================================================
void	X_ChannelNumberDelegate::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
   QStyledItemDelegate::updateEditorGeometry (editor, option, index);
}
//================================================
void X_ChannelNumberDelegate::xp_setImmediatellyValueChange(bool flag)
{
    xv_immediatellyValueChangeFlag = flag;
}
//================================================
void X_ChannelNumberDelegate::xp_setEditorMinMax(qint64 min, qint64 max)
{
   if(min < max || min < std::numeric_limits<long>::min() || max > std::numeric_limits<long>::max())
   {
      xv_editorMin = min;
      xv_editorMax = max;
   }
}
//================================================
void X_ChannelNumberDelegate::xh_editorValueChanged(int value)
{
   QWidget* editor = qobject_cast<QWidget*>(sender());
   if(editor == 0 || !xv_currentModelIndex.isValid())
   {
      return;
   }
   QAbstractItemModel* model = const_cast<QAbstractItemModel*>(xv_currentModelIndex.model());
   setModelData (editor, model, xv_currentModelIndex);
}
//================================================

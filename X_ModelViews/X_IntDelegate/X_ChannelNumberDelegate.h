//====================================
#ifndef X_INTDELEGATE_H
#define X_INTDELEGATE_H
//====================================
#include <QStyledItemDelegate>
//====================================
class X_ChannelNumberDelegate : public QStyledItemDelegate
{
   Q_OBJECT
public:
   explicit X_ChannelNumberDelegate(QObject *parent = nullptr);

   QWidget * createEditor(QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
   virtual void	paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
   virtual void	setEditorData(QWidget * editor, const QModelIndex & index) const;
   virtual void	setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
   virtual QSize	sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
   virtual void	updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

   void xp_setImmediatellyValueChange(bool);

public slots:

   void xp_setEditorMinMax(qint64 min, qint64 max) ;

private:

   // VARS
   qint64 xv_editorMin;
   qint64 xv_editorMax;
   QModelIndex xv_currentModelIndex;
   bool xv_immediatellyValueChangeFlag;
   //funcs

private slots:

   void xh_editorValueChanged(int value);

};
//====================================
#endif // X_INTDELEGATE_H

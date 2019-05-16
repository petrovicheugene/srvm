//====================================
#ifndef ZINTDELEGATE_H
#define ZINTDELEGATE_H
//====================================
#include <QStyledItemDelegate>
//====================================
class ZChannelNumberDelegate : public QStyledItemDelegate
{
   Q_OBJECT
public:
   explicit ZChannelNumberDelegate(QObject *parent = nullptr);

   QWidget * createEditor(QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
   virtual void	paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
   virtual void	setEditorData(QWidget * editor, const QModelIndex & index) const;
   virtual void	setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
   virtual QSize	sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
   virtual void	updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

   void zp_setImmediatellyValueChange(bool);

public slots:

   void zp_setEditorMinMax(qint64 min, qint64 max) ;

private:

   // VARS
   qint64 zv_editorMin;
   qint64 zv_editorMax;
   QModelIndex zv_currentModelIndex;
   bool zv_immediatellyValueChangeFlag;
   //funcs

private slots:

   void zh_editorValueChanged(int value);

};
//====================================
#endif // ZINTDELEGATE_H

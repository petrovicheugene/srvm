//======================================================
#ifndef X_WINDOWGRAPHICSITEM_H
#define X_WINDOWGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>
#include "X_GraphicsItemUserTypes.h"
#include <QColor>
#include <QPair>
//======================================================
class X_CalibrationWindow;
//======================================================
class X_WindowGraphicsItem : public QGraphicsItem
{
public:
   explicit X_WindowGraphicsItem(const X_CalibrationWindow*,
                                QColor color,
                                QGraphicsItem * parent = 0);
   // overrided
   QRectF boundingRect() const override;
   void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
   QPainterPath shape() const override;
   int type() const override;

   // FUNCS
   static void xp_setCurrentWindowColor(QColor);
   static void xp_setCurrentWindowId(qint64);
   static bool xp_setTopAndButtonMargins(qreal top, qreal bottom);
   static bool xp_setAlphaChannelValue(quint8 alpha);

   qint64 xp_windowId() const;
   void xp_setLeftRightMargins(qreal, qreal);
   void xp_updateCurrentWindow(bool);
   void xp_updateItem();

   void xp_setVisible(bool visible);

protected:

   // FUNCS
   void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
   void zh_recalcShapeAndBoundingRect();

private:

   // VARS
   QColor xv_color;
   QColor xv_lineColor;
   QColor xv_brushColor;
   qint64 xv_windowId;
   qreal xv_leftMargin;
   qreal xv_rightMargin;

   static qreal xv_topMargin;
   static qreal xv_bottomMargin;
   static qint64 xv_currentWindowId;
   static quint8 xv_alphaCannelValue;

   static QColor xv_currentWindowColor;
   QPainterPath xv_shape;
   QPainterPath xv_paintRect;
   QPainterPath xv_marginsPainterPath;
   QRectF xv_boundingRect;


   // FUNCS

};
//======================================================
#endif // X_WINDOWGRAPHICSITEM_H

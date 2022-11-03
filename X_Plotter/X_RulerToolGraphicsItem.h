//======================================================
#ifndef X_ROLERTOOLGRAPHICSITEM_H
#define X_ROLERTOOLGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>
#include "X_GraphicsItemUserTypes.h"
//======================================================
class X_TextGraphicsItem;
//======================================================
class X_RulerToolGraphicsItem: public QGraphicsItem
{
public:
    explicit X_RulerToolGraphicsItem(QGraphicsItem * parent = 0);
    ~X_RulerToolGraphicsItem();
    // overrided
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const override;
    int type() const override;

    void xp_setProperties(QPointF startPosition,
                          QPointF endPosition, QRectF viewPortSceneRect, QSize viewportSize, const QString &text);
    void xp_updateItem();

protected:

    void zh_recalcShapeAndBoundingRect();

private:

    // VARS
    QGraphicsTextItem* xv_textItem;

    QPointF xv_startPosition;
    QPointF xv_endPosition;
    QRectF xv_viewportSceneRect;
    QSize xv_viewportSize;
    QString xv_text;

    QRectF xv_boundingRect;
    QPainterPath xv_shape;

    QPainterPath xv_linePainterPath;

    // FUNCS
    double zh_convertPixelsToSceneWidth(int pixelCount) const;
    double zh_convertPixelsToSceneHeight(int pixelCount) const;


};
//======================================================
#endif // X_ROLERTOOLGRAPHICSITEM_H

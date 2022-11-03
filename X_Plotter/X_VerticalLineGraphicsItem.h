//======================================================
#ifndef X_VERTICALLINEGRAPHICSITEM_H
#define X_VERTICALLINEGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>
#include "X_GraphicsItemUserTypes.h"
#include <QColor>
//======================================================
class X_EnergyCalibrationLine;
//======================================================
class X_VerticalLineGraphicsItem : public QGraphicsItem
{
public:
    explicit X_VerticalLineGraphicsItem(X_EnergyCalibrationLine* energyCalibrationLine,
                                       QGraphicsItem * parent = 0);
    // overrided
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const override;
    int type() const override;

    void xp_updateItem();
    static bool xp_setTopAndButtonMargins(double top, double bottom);



protected:

    void zh_recalcShapeAndBoundingRect();

private:

    //VARS
    static double xv_topMargin;
    static double xv_bottomMargin;

    X_EnergyCalibrationLine* xv_energyCalibrationLine;

    QRectF xv_boundingRect;
    QPainterPath xv_shape;
    QPainterPath xv_linePainterPath;

};
//======================================================
#endif // X_VERTICALLINEGRAPHICSITEM_H

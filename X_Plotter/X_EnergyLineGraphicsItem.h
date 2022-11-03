//======================================================
#ifndef X_ENERGYLINEGRAPHICSITEM_H
#define X_ENERGYLINEGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>
#include "X_GraphicsItemUserTypes.h"
#include <QColor>
//======================================================
class X_TextGraphicsItem;
//======================================================
class X_EnergyLineGraphicsItem : public QGraphicsItem
{
public:
    explicit X_EnergyLineGraphicsItem(const QString& chemicalElementSymbol,
                                     const QString &name,
                                     int heightPercent,
                                     double energyValue,
                                     QGraphicsItem * parent = 0);
    ~X_EnergyLineGraphicsItem();
    // overrided
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const override;
    int type() const override;

    void xp_updateItem();
    static bool xp_setTopAndButtonMargins(double top, double bottom);
    static void xp_setFontPixelSize(int pixelSize);

    void xp_setHeightPercent(int percent);
    void xp_setXPosition(double xPos);
    QString xp_chemicalElementSymbol() const;
    QString xp_lineName() const;
    void xp_setColor(QColor color);

    double xp_energyValue() const;

protected:

    void zh_recalcShapeAndBoundingRect();

private:

    //VARS
    static double xv_topMargin;
    static double xv_bottomMargin;
    static int xv_fontPixelSize;
    X_TextGraphicsItem* xv_textItem;

    double xv_xPosition;
    int xv_heightPercent;
    QString xv_chemicalElementSymbol;
    QString xv_lineName;
    QColor xv_color;
    double xv_energyValue;

    QRectF xv_boundingRect;
    QPainterPath xv_shape;
    QPainterPath xv_linePainterPath;

};
//======================================================
#endif // X_ENERGYLINEGRAPHICSITEM_H

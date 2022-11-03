//====================================================
#include "X_ChartPointOptions.h"
#include "X_ChartPointGraphicsItem.h"
#include <math.h>
//====================================================
X_ChartPointOptions::X_ChartPointOptions()
{
    xv_pointType = PT_ROUND;
    xv_pointSize = 3;
    xv_outLineWidth = 1;

    xv_currentPointColor = QColor(Qt::red);
    xv_outLineColor = QColor(Qt::black);
    xv_pointColor = QColor(Qt::green);

    xv_horizontalRulerScaleValue = 0.001;
    xv_horizontalRulerMarkPrecision = 3;
    xv_horizontalRulerLabelString = QString();

    xv_verticalRulerScaleValue = 0.001;
    xv_verticalRulerMarkPrecision = 3;
    xv_verticalRulerLabelString = QString();

    zh_recalcShapeAndBoundingRect();
}
//====================================================
X_ChartPointOptions::PointType X_ChartPointOptions::xp_pointType() const
{
    return xv_pointType;
}
//====================================================
bool X_ChartPointOptions::xp_setPointType(X_ChartPointOptions::PointType type)
{
    if(xv_pointType == type)
    {
        return false;
    }

    xv_pointType = type;
    zh_recalcShapeAndBoundingRect();
    zh_updateItems();
    return true;
}
//====================================================
int X_ChartPointOptions::xp_pointPixelSize() const
{
    return xv_pointSize;
}
//====================================================
bool X_ChartPointOptions::xp_setPointPixelSize(int pointSize)
{
    if(xv_pointSize == pointSize)
    {
        return false;
    }

    xv_pointSize = pointSize;
    zh_recalcShapeAndBoundingRect();
    zh_updateItems();
    return true;
}
//====================================================
int X_ChartPointOptions::xp_outLineWidth() const
{
    return xv_outLineWidth;
}
//====================================================
bool X_ChartPointOptions::xp_setOutLineWidth(int outLineWidth)
{
    if(xv_outLineWidth == outLineWidth)
    {
        return false;
    }

    xv_outLineWidth = outLineWidth;
    zh_updateItems();
    return true;
}
//====================================================
QColor X_ChartPointOptions::xp_currentPointColor() const
{
    return xv_currentPointColor;
}
//====================================================
bool X_ChartPointOptions::xp_setCurrentPointColor(QColor currentPointColor)
{
    if(xv_currentPointColor == currentPointColor)
    {
        return false;
    }

    xv_currentPointColor = currentPointColor;
    zh_updateItems();
    return true;
}
//====================================================
QColor X_ChartPointOptions::xp_outLineColor() const
{
    return xv_outLineColor;
}
//====================================================
bool X_ChartPointOptions::xp_setOutLineColor(QColor outLineColor)
{
    if(xv_outLineColor == outLineColor)
    {
        return false;
    }

    xv_outLineColor = outLineColor;
    zh_updateItems();
    return true;
}
//====================================================
QColor X_ChartPointOptions::xp_pointColor() const
{
    return xv_pointColor;
}
//====================================================
bool X_ChartPointOptions::xp_setPointColor(QColor pointColor)
{
    if(xv_pointColor == pointColor)
    {
        return false;
    }

    xv_pointColor = pointColor;
    zh_updateItems();
    return true;
}
//====================================================
qreal X_ChartPointOptions::xp_rulerScaleValue(Qt::Orientation orientation) const
{
    if(orientation == Qt::Horizontal)
    {
        return xv_horizontalRulerScaleValue;
    }
    else
    {
        return xv_verticalRulerScaleValue;
    }
}
//====================================================
bool X_ChartPointOptions::xp_setRulerScaleValue(Qt::Orientation orientation, qreal scaleValue)
{
    if(orientation == Qt::Horizontal)
    {
         if(xv_horizontalRulerScaleValue - scaleValue == 0.0)
         {
             return false;
         }
         xv_horizontalRulerScaleValue = scaleValue;
    }
    else
    {
        if(xv_verticalRulerScaleValue - scaleValue == 0.0)
        {
            return false;
        }
        xv_verticalRulerScaleValue = scaleValue;
    }
    return  true;
}
//====================================================
QString X_ChartPointOptions::xp_rulerLabelString(Qt::Orientation orientation) const
{
    if(orientation == Qt::Horizontal)
    {
        return xv_horizontalRulerLabelString;
    }
    else
    {
        return xv_verticalRulerLabelString;
    }
}
//====================================================
bool X_ChartPointOptions::xp_setRulerLabelString(Qt::Orientation orientation, const QString& rulerLabelString)
{
    if(orientation == Qt::Horizontal)
    {
        if(xv_horizontalRulerLabelString == rulerLabelString)
        {
            return false;
        }
        xv_horizontalRulerLabelString = rulerLabelString;
    }
    else
    {
        if(xv_verticalRulerLabelString == rulerLabelString)
        {
            return false;
        }
        xv_verticalRulerLabelString = rulerLabelString;
    }
    return true;
}
//====================================================
int X_ChartPointOptions::xp_rulerMarkPrecision(Qt::Orientation orientation) const
{
    if(orientation == Qt::Horizontal)
    {
        return xv_horizontalRulerMarkPrecision;
    }
    else
    {
        return xv_verticalRulerMarkPrecision;
    }
}
//====================================================
bool X_ChartPointOptions::xp_setRulerMarkPrecision(Qt::Orientation orientation, int precision)
{
    if(orientation == Qt::Horizontal)
    {
        if(xv_horizontalRulerMarkPrecision == precision)
        {
            return false;
        }
        xv_horizontalRulerMarkPrecision = precision;
    }
    else
    {
        if(xv_verticalRulerMarkPrecision == precision)
        {
            return false;
        }
        xv_verticalRulerMarkPrecision = precision;
    }
    return true;
}
//====================================================
void X_ChartPointOptions::zh_recalcShapeAndBoundingRect()
{
    // shape
    qreal pointHalfSize = (qreal)xv_pointSize / 2.0;
    QRectF rect = QRectF(-1*pointHalfSize, -1*pointHalfSize,
                         (qreal)xv_pointSize, (qreal)xv_pointSize);

    QPainterPath newShape;
    QPolygonF polygon;
    qreal a;
    switch(xv_pointType)
    {
    case PT_SQUARE:
        newShape.addRect(rect);
        break;
    case PT_TRIANGLE:
        a = pointHalfSize * 3.0 / sqrt(3.0);
        polygon.append(QPointF(0.0, -1*pointHalfSize));
        polygon.append(QPointF(-1*a/2, pointHalfSize/2));
        polygon.append(QPointF(a/2, pointHalfSize/2));
        polygon.append(QPointF(0.0, -1*pointHalfSize));

        newShape.addPolygon(polygon);
        break;
    case PT_ROUND:
        newShape.addEllipse(rect);
        break;
    default:
        newShape.addEllipse(rect);
    }

    xv_shape = newShape;
    xv_boundingRect = rect;
}
//====================================================
void X_ChartPointOptions::zh_updateItems() const
{
    foreach(X_ChartPointGraphicsItem* item, xv_seriesPointItemList)
    {
        item->xp_setNewBoundingRect(xv_boundingRect);
        item->update();
    }
}
//====================================================

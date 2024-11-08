//======================================================
#include "X_SpectrumGraphicsItem.h"
#include "X_AbstractSpectrum.h"
#include "X_GraphicsItemUserTypes.h"
#include "X_PlotterDefaultVariables.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QtMath>
//======================================================
// STATIC
//======================================================
QColor X_SpectrumGraphicsItem::xv_currentColor = QColor(Qt::red);
qint64 X_SpectrumGraphicsItem::xv_currentSpectrumId = -1;
//======================================================
// END STATIC
//======================================================
X_SpectrumGraphicsItem::X_SpectrumGraphicsItem(const X_AbstractSpectrum *spectrum,
                                               double boundingRectTopFactor,
                                               double distortionFactor,
                                               double distortionCorrectionFactor,
                                               QGraphicsItem *parent) : QGraphicsItem(parent)
{
    if(boundingRectTopFactor < 1)
    {
        xv_boundingRectTopFactor = 1;
    }
    else if(boundingRectTopFactor > 2)
    {
        xv_boundingRectTopFactor = 2;
    }
    else
    {
        xv_boundingRectTopFactor = boundingRectTopFactor;
    }

    if(distortionFactor > 1.0)
    {
        xv_distortionFactor = 1.0;
    }
    else if(distortionFactor < 0.1)
    {
        xv_distortionFactor = 0.1;
    }
    else
    {
        xv_distortionFactor = distortionFactor;
    }

    xv_distortionCorrectionFactor = distortionCorrectionFactor;

    GraphicsItemFlags flags = this->flags();
    flags |= ItemIsFocusable | ItemIsSelectable;
    setFlags(flags);
    setZValue(gl_defaultSpectrumZValue);

    xp_updateSpectrumData(spectrum);
    xp_updateSpectrum(spectrum->xp_isSpectrumVisible());
}
//======================================================
QRectF X_SpectrumGraphicsItem::boundingRect() const
{
    return xv_boundingRect;
}
//======================================================
void X_SpectrumGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();

    QPen pen(QBrush(xv_paintColor), xv_lineWidth);
    pen.setCosmetic(true);
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    painter->drawPath(xv_spectrumPainterPath);

    painter->restore();
}
//======================================================
QPainterPath X_SpectrumGraphicsItem::shape() const
{
    return xv_spectrumPainterPath;
}
//======================================================
int X_SpectrumGraphicsItem::type() const
{
    return SpectrumItemType;
}
//======================================================
void X_SpectrumGraphicsItem::xp_setCurrentSpectrumColor(QColor color)
{
    xv_currentColor = color;
}
//======================================================
void X_SpectrumGraphicsItem::xp_setCurrentSpectrumId(qint64 id)
{
    xv_currentSpectrumId = id;
}
//======================================================
qint64 X_SpectrumGraphicsItem::xp_currentSpectrumId()
{
    return xv_currentSpectrumId;
}
//======================================================
void X_SpectrumGraphicsItem::xp_setSpectrumData(const QList<quint32>& data)
{
    xv_spectrumData = data;
    // painter path calculation
    xh_calculatePainterPath();
    update(xv_boundingRect);
}
//======================================================
qint64 X_SpectrumGraphicsItem::xp_spectrumId()
{
    return xv_spectrumId;
}
//======================================================
void X_SpectrumGraphicsItem::xp_updateSpectrumData(const X_AbstractSpectrum *spectrum)
{
    xv_spectrumData = spectrum->xp_spectrumData();
    xv_color = spectrum->xp_color();
    xv_spectrumId = spectrum->xp_spectrumId();

    // painter path calculation
    xh_calculatePainterPath();
    if(xv_spectrumId != xv_currentSpectrumId)
    {
        setVisible(spectrum->xp_isSpectrumVisible());
    }

    update(xv_boundingRect);
}
//======================================================
void X_SpectrumGraphicsItem::xh_calculatePainterPath()
{
    prepareGeometryChange();
    QPointF point(0.0, 0.0);
    QPainterPath newShape;
    newShape.moveTo(point);

    double scValue = 0.0;
    for(int i = 0; i < xv_spectrumData.count(); i++)
    {
        scValue = xh_sceneDistortionPositionForValue((double)(xv_spectrumData.value(i)));
        point = QPointF((double)i,  scValue*-1);
        newShape.lineTo(point);
        point = QPointF((double)(i+1),  scValue*-1);
        newShape.lineTo(point);
    }

    // last point
    point = QPointF((double)(xv_spectrumData.count() - 1),  0);
    newShape.lineTo(point);
    xv_spectrumPainterPath = QPainterPath();
    xv_spectrumPainterPath = newShape;
    xv_boundingRect = xv_spectrumPainterPath.boundingRect();
    // rect top and bottom extension
    xv_boundingRect.setTop(xv_boundingRect.top()*xv_boundingRectTopFactor);
    xv_boundingRect.setBottom(xv_boundingRect.bottom()+5);
}
//======================================================
double X_SpectrumGraphicsItem::xh_sceneDistortionPositionForValue(double value)
{
    if(xv_distortionFactor == 1 || xv_distortionCorrectionFactor == 0)
    {
        return value;
    }

    double distortedValue = pow(value, xv_distortionFactor) * xv_distortionCorrectionFactor;
    return distortedValue;
}
//======================================================
void X_SpectrumGraphicsItem::xp_setDistortion(double distortionFactor, double distortionCorrectionFactor)
{
    if(distortionFactor > 1.0)
    {
        xv_distortionFactor = 1.0;
    }
    else if(distortionFactor < 0.1)
    {
        xv_distortionFactor = 0.1;
    }
    else
    {
        xv_distortionFactor = distortionFactor;
    }

    xv_distortionCorrectionFactor = distortionCorrectionFactor;

    xh_calculatePainterPath();
    update(xv_boundingRect);
}
//======================================================
bool X_SpectrumGraphicsItem::xp_isSpectrumCurrent()
{
    return xv_currentSpectrumId == xv_spectrumId;
}
//======================================================
void X_SpectrumGraphicsItem::xp_updateSpectrum(bool visible)
{
    if(xv_currentSpectrumId == xv_spectrumId)
    {
        setZValue(gl_currentSpectrumZValue);
        xv_paintColor = xv_currentColor;
        setVisible(true);
    }
    else
    {
        setZValue(gl_defaultSpectrumZValue);
        xv_paintColor = xv_color;
        setVisible(visible);
    }

    update();
}
//======================================================
void X_SpectrumGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}
//======================================================
void X_SpectrumGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsItem::mousePressEvent(event);
}
//======================================================
void X_SpectrumGraphicsItem::focusInEvent(QFocusEvent *event)
{
    QGraphicsItem::focusInEvent(event);
}
//======================================================
void X_SpectrumGraphicsItem::focusOutEvent(QFocusEvent *event)
{
    QGraphicsItem::focusOutEvent(event);
}
//======================================================
QColor X_SpectrumGraphicsItem::xp_spectrumColor() const
{
    return xv_color;
}
//======================================================
double X_SpectrumGraphicsItem::xp_lineWidth() const
{
    return xv_lineWidth;
}
//======================================================
void X_SpectrumGraphicsItem::xp_setLineWidth(double width)
{
    xv_lineWidth = width;
    xp_updateSpectrum(isVisible());
}
//======================================================

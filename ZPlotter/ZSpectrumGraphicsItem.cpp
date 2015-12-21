//======================================================
#include "ZSpectrumGraphicsItem.h"
#include "ZAbstractSpectrum.h"
#include "globalVariables.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QtMath>
//======================================================
ZSpectrumGraphicsItem::ZSpectrumGraphicsItem(const ZAbstractSpectrum *spectrum,
                                             qreal boundingRectTopFactor,
                                             qreal distortionFactor,
                                             qreal distortionCorrectionFactor,
                                             QGraphicsItem *parent) : QGraphicsItem(parent)
{
    if(boundingRectTopFactor < 1)
    {
        zv_boundingRectTopFactor = 1;
    }
    else if(boundingRectTopFactor > 2)
    {
        zv_boundingRectTopFactor = 2;
    }
    else
    {
        zv_boundingRectTopFactor = boundingRectTopFactor;
    }

    if(distortionFactor > 1.0)
    {
        zv_distortionFactor = 1.0;
    }
    else if(distortionFactor < 0.1)
    {
        zv_distortionFactor = 0.1;
    }
    else
    {
        zv_distortionFactor = distortionFactor;
    }

    zv_distortionCorrectionFactor = distortionCorrectionFactor;

    GraphicsItemFlags flags = this->flags();
    flags |= ItemIsFocusable | ItemIsSelectable;
    setFlags(flags);
    zp_updateSpectrumData(spectrum);
}
//======================================================
QRectF ZSpectrumGraphicsItem::boundingRect() const
{
    return zv_boundingRect;
}
//======================================================
void ZSpectrumGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    QPen pen(QBrush(zv_color),1);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->drawPath(zv_spectrumPainterPath);
    painter->restore();
}
//======================================================
QPainterPath ZSpectrumGraphicsItem::shape() const
{
    return zv_spectrumPainterPath;
}
//======================================================
int	 ZSpectrumGraphicsItem::type() const
{
    return SpectrumItemType;
}
//======================================================
void ZSpectrumGraphicsItem::zp_setSpectrumData(const QList<int>& data)
{
    zv_spectrumData = data;
    // painter path calculation
    zh_calculatePainterPath();
    update(zv_boundingRect);
}
//======================================================
qint64 ZSpectrumGraphicsItem::zp_spectrumId()
{
    return zv_spectrumId;
}
//======================================================
void ZSpectrumGraphicsItem::zp_updateSpectrumData(const ZAbstractSpectrum *spectrum)
{
    zv_spectrumData = spectrum->zp_spectrumData();
    zv_color = spectrum->zp_color();
    zv_spectrumId = spectrum->zp_spectrumId();

    // painter path calculation
    zh_calculatePainterPath();
    setVisible(spectrum->zp_isSpectrumVisible());
    update(zv_boundingRect);
}
//======================================================
void ZSpectrumGraphicsItem::zh_calculatePainterPath()
{
    QPointF point(0.0, 0.0);
    QPainterPath newShape;
    newShape.moveTo(point);

    qreal scValue = 0.0;
    for(int i = 0; i < zv_spectrumData.count(); i++)
    {
        scValue = zh_sceneDistortionPositionForValue((qreal)(zv_spectrumData.value(i)));
        point = QPointF((qreal)i,  scValue*-1);
        newShape.lineTo(point);
        point = QPointF((qreal)(i+1),  scValue*-1);
        newShape.lineTo(point);
    }

    // last point
    point = QPointF((qreal)(zv_spectrumData.count() - 1),  0);
    newShape.lineTo(point);
    zv_spectrumPainterPath = newShape;
    zv_boundingRect = zv_spectrumPainterPath.boundingRect();
    // rect top and bottom extension
    zv_boundingRect.setTop(zv_boundingRect.top()*zv_boundingRectTopFactor);
    zv_boundingRect.setBottom(zv_boundingRect.bottom()+5);
}
//======================================================
qreal ZSpectrumGraphicsItem::zh_sceneDistortionPositionForValue(qreal value)
{
    if(zv_distortionFactor == 1 || zv_distortionCorrectionFactor == 0)
    {
        return value;
    }

    qreal distortedValue = pow(value, zv_distortionFactor) * zv_distortionCorrectionFactor;
    return distortedValue;
}
//======================================================
void ZSpectrumGraphicsItem::zp_setDistortion(qreal distortionFactor, qreal distortionCorrectionFactor)
{
    if(distortionFactor > 1.0)
    {
        zv_distortionFactor = 1.0;
    }
    else if(distortionFactor < 0.1)
    {
        zv_distortionFactor = 0.1;
    }
    else
    {
        zv_distortionFactor = distortionFactor;
    }

    zv_distortionCorrectionFactor = distortionCorrectionFactor;

    zh_calculatePainterPath();
    update(zv_boundingRect);
}
//======================================================
void ZSpectrumGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
#ifdef DBG
    qDebug() << "RELEASE";
#endif
    QGraphicsItem::mouseReleaseEvent(event);
}
//======================================================
void ZSpectrumGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
#ifdef DBG
    qDebug() << "PRESS";
#endif
    QGraphicsItem::mousePressEvent(event);
}
//======================================================
void ZSpectrumGraphicsItem::focusInEvent(QFocusEvent *event)
{
#ifdef DBG
    qDebug() << "FOCUS IN";
#endif
    QGraphicsItem::focusInEvent(event);
}
//======================================================
void ZSpectrumGraphicsItem::focusOutEvent(QFocusEvent *event)
{
#ifdef DBG
    qDebug() << "FOCUS OUT";
#endif
    QGraphicsItem::focusOutEvent(event);
}
//======================================================

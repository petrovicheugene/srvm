//=======================================================
#include "X_PrimitivePlot.h"
#include <QPaintEvent>
#include <QStylePainter>
//=======================================================
X_PrimitivePlot::X_PrimitivePlot(QWidget *parent) : QFrame(parent)
{
    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);
    xv_paintStyle = PS_STAIRS_POLYGON;
    xv_antialiasingFlag = false;
}
//=======================================================
void X_PrimitivePlot::xp_paintData(const X_SpectrumPaintData &paintData)
{
    xh_recalcPaintData(paintData);
    repaint();
}
//=======================================================
void X_PrimitivePlot::xp_setPaintStyle(PaintStyle style)
{
    xv_paintStyle = style;
}
//=======================================================
void X_PrimitivePlot::xp_setPenColor(QColor color)
{
    xv_penColor = color;
}
//=======================================================
void X_PrimitivePlot::xp_setBrushColor(QColor color)
{
    xv_brushColor = color;
}
//=======================================================
void X_PrimitivePlot::xp_setAntialiasingFlag(bool flag)
{
    xv_antialiasingFlag = flag;
}
//=======================================================
void X_PrimitivePlot::paintEvent(QPaintEvent * event)
{
    QStylePainter painter(this);
    painter.setViewport(1, 1, this->width() - 2, this->height() - 2);
    painter.setWindow(0, (this->height() - 2) * (-1), this->width() - 2, this->height() - 2);
    painter.setRenderHint(QPainter::Antialiasing, xv_antialiasingFlag);


    if(xv_paintStyle == PS_DOTS)
    {
        xh_paintDots(&painter);
    }
    else if(xv_paintStyle == PS_STAIRS)
    {
        xh_paintStairs(&painter);
    }
    else if(xv_paintStyle == PS_POLYGON)
    {
        xh_paintPlygon(&painter);
    }
    else if(xv_paintStyle == PS_STAIRS_POLYGON)
    {
        xh_paintStairsPolygon(&painter);
    }
    else
    {
        xh_paintStraight(&painter);
    }
}
//=======================================================
void X_PrimitivePlot::xh_paintDots(QStylePainter* painter)
{

}
//=======================================================
void X_PrimitivePlot::xh_paintPlygon(QStylePainter* painter)
{
    //QPolygon polygon(xv_paintData.spectrumData);
    painter->save();
    painter->setPen(QPen(QBrush(xv_penColor), 1));
    painter->setBrush(QBrush(xv_brushColor));

    QPolygon polygon;
    foreach(QPoint point, xv_chartData)
    {
        polygon << point;
    }
    painter->drawPolygon(polygon);
    painter->restore();
}
//=======================================================
void X_PrimitivePlot::xh_paintStairsPolygon(QStylePainter* painter)
{
    painter->save();
    painter->setPen(QPen(QBrush(xv_penColor), 1));
    painter->setBrush(QBrush(xv_brushColor));

    //painter->setRenderHint(QPainter::Antialiasing);

    QPolygon polygon;
    // start point
    polygon << QPoint(0,xv_verticalShift * -1);
    for(int i = 0; i < xv_chartData.count() - 1; i++)
    {
        polygon << xv_chartData.value(i);
        polygon << QPoint(xv_chartData.value(i+1).x(), xv_chartData.value(i).y());
    }
    polygon << xv_chartData.last();
    // finish points
    polygon << QPoint(xv_chartData.last().x(), 0);

    painter->drawPolygon(polygon);
    painter->restore();
}
//=======================================================
void X_PrimitivePlot::xh_paintStairs(QStylePainter* painter)
{

}
//=======================================================
void X_PrimitivePlot::xh_paintStraight(QStylePainter* painter)
{

}
//=======================================================
void X_PrimitivePlot::xh_recalcPaintData(const X_SpectrumPaintData &paintData)
{
    // log Coordinates
    double channelPerPixel = (double)paintData.maxChannel / (double)(this->width() - 2);
    if(channelPerPixel <= 0)
    {
        channelPerPixel = 1;
    }

    double intensityPerPixel = (double)paintData.maxIntensity / (double)(this->height() - 2 - xv_verticalShift);
    if(intensityPerPixel <= 0)
    {
        intensityPerPixel = 1;
    }
    xv_chartData.clear();

    for(int i = 0; i < paintData.spectrumData.count(); i++)
    {
        int x = qRound((double)i / channelPerPixel);
        int y = -1 * qRound(((double)(paintData.spectrumData.value(i)) / intensityPerPixel) + xv_verticalShift);
        xv_chartData.append(QPoint(x,y));
    }
}
//=======================================================

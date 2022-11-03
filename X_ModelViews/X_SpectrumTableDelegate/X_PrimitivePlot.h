//=======================================================
#ifndef X_PRIMITIVEPLOT_H
#define X_PRIMITIVEPLOT_H
//=======================================================
#include <QFrame>
#include "X_SpectrumPaintData.h"
//=======================================================
class QStylePainter;
//=======================================================
class X_PrimitivePlot : public QFrame
{
    Q_OBJECT
public:
    explicit X_PrimitivePlot(QWidget *parent = 0);

    enum PaintStyle {PS_STRIGHT,
                     PS_STAIRS,
                     PS_DOTS,
                     PS_POLYGON,
                     PS_STAIRS_POLYGON};

    void xp_paintData(const X_SpectrumPaintData& paintData);
    void xp_setPaintStyle(PaintStyle);

    void xp_setPenColor(QColor);
    void xp_setBrushColor(QColor);
    void xp_setAntialiasingFlag(bool);

protected:
    // FUNCS
    void paintEvent(QPaintEvent * event);

private:

    // VARS
    PaintStyle xv_paintStyle;
    QColor xv_penColor;
    QColor xv_brushColor;
    const int xv_verticalShift = 1;
    bool xv_antialiasingFlag;
    // recalced paint data
    QVector<QPoint> xv_chartData;

    // FUNCS
    void zh_paintDots(QStylePainter*);
    void zh_paintPlygon(QStylePainter*);
    void zh_paintStairs(QStylePainter*);
    void zh_paintStairsPolygon(QStylePainter*);
    void zh_paintStraight(QStylePainter*);

    void zh_recalcPaintData(const X_SpectrumPaintData &paintData);

signals:

public slots:
};
//=======================================================
#endif // X_PRIMITIVEPLOT_H

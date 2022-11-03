//============================================
#ifndef X_SERIESPOINTOPTIONS_H
#define X_SERIESPOINTOPTIONS_H
//============================================
#include <QList>
#include <QRectF>
#include <QColor>
#include <QPainterPath>
//============================================
class X_ChartPointGraphicsItem;
//============================================
class X_ChartPointOptions
{
    friend class X_ChartPointGraphicsItem;
public:
    enum PointType{PT_ROUND,
                   PT_SQUARE,
                   PT_TRIANGLE
                  };

    X_ChartPointOptions();

    X_ChartPointOptions::PointType xp_pointType() const;
    bool xp_setPointType(X_ChartPointOptions::PointType);

    int xp_pointPixelSize() const;
    bool xp_setPointPixelSize(int);

    int xp_outLineWidth() const;
    bool xp_setOutLineWidth(int);

    QColor xp_currentPointColor() const;
    bool xp_setCurrentPointColor(QColor);

    QColor xp_outLineColor() const;
    bool xp_setOutLineColor(QColor);

    QColor xp_pointColor() const;
    bool xp_setPointColor(QColor);

    qreal xp_rulerScaleValue(Qt::Orientation) const;
    bool xp_setRulerScaleValue(Qt::Orientation, qreal);

    QString xp_rulerLabelString(Qt::Orientation) const;
    bool xp_setRulerLabelString(Qt::Orientation, const QString&);

    int xp_rulerMarkPrecision(Qt::Orientation) const;
    bool xp_setRulerMarkPrecision(Qt::Orientation, int);

private:

    // VARS
    QList<X_ChartPointGraphicsItem*> xv_seriesPointItemList;
    QRectF xv_boundingRect;
    QPainterPath xv_shape;

    qreal xv_horizontalRulerScaleValue;
    QString xv_horizontalRulerLabelString;
    int xv_horizontalRulerMarkPrecision;

    qreal xv_verticalRulerScaleValue;
    QString xv_verticalRulerLabelString;
    int xv_verticalRulerMarkPrecision;


    X_ChartPointOptions::PointType xv_pointType;
    int xv_pointSize;
    int xv_outLineWidth;
    QColor xv_currentPointColor;
    QColor xv_outLineColor;
    QColor xv_pointColor;

    // FUNCS
    void zh_recalcShapeAndBoundingRect();

    void zh_updateItems() const;

};
//============================================
#endif // X_SERIESPOINTOPTIONS_H

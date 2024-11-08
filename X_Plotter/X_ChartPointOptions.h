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

    double xp_pointPixelSize() const;
    bool xp_setPointPixelSize(double);

    double xp_outLineWidth() const;
    bool xp_setOutLineWidth(double);

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

    void xp_recalcShapeAndBoundingRect();
    void xp_updateItems() const;

    double xp_defaultPointSize() {return 3.0;};
    QRectF xp_boundingRect(bool checked) const;
    QPainterPath xp_shape(bool checked) const;

    QRectF xp_boundingRect(Qt::CheckState checkState) const;
    QPainterPath xp_shape(Qt::CheckState checkState) const;

private:

    // VARS
    QList<X_ChartPointGraphicsItem*> xv_seriesPointItemList;
    QRectF xv_checkedBoundingRect;
    QPainterPath xv_checkedShape;
    QRectF xv_uncheckedBoundingRect;
    QPainterPath xv_uncheckedShape;

    qreal xv_horizontalRulerScaleValue = 0.001;
    QString xv_horizontalRulerLabelString;
    double xv_horizontalRulerMarkPrecision = 3.0;

    qreal xv_verticalRulerScaleValue = 0.001;
    QString xv_verticalRulerLabelString;
    double xv_verticalRulerMarkPrecision = 3.0;


    X_ChartPointOptions::PointType xv_pointType = PT_ROUND;

    double xv_pointSize = 3.0;
    double xv_outLineWidth = 1.0;
    QColor xv_currentPointColor = QColor(Qt::red);
    QColor xv_outLineColor = QColor(Qt::black);
    QColor xv_pointColor = QColor(Qt::green);

    // FUNCS

    QPainterPath xh_createObliqueCross(const QRectF& rect);

};
//============================================
#endif // X_SERIESPOINTOPTIONS_H

//======================================================
#ifndef X_SERIESPOINTGRAPHICSITEM_H
#define X_SERIESPOINTGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>
#include "X_VisibilityPointF.h"

//======================================================
class X_ChartPointOptions;
class X_CorrelationPlotterDataManager;
//======================================================
class X_ChartPointGraphicsItem : public QGraphicsItem
{
    friend class X_ChartPointOptions;
public:

    explicit X_ChartPointGraphicsItem(const X_CorrelationPlotterDataManager* dataManager,
                                     const X_VisibilityPointF &visibilityPoint,
                                      X_ChartPointOptions* seriesPointOptions,
                                      qint64 relatedObjectId,
                                      QGraphicsItem * parent = 0);
    ~X_ChartPointGraphicsItem();
    static void xp_setCurrentSpectrumId(qint64);
    static void xp_setCurrentSpectrumColor(QColor);

    // overrided
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const override;
    int type() const override;

    // FUNCS
    X_ChartPointOptions* xp_seriesPointOptions() const;
    void xp_setNewBoundingRect(const QRectF& boundingRect);

    qint64 xp_relatedObjectId() const;
    void xp_applyVisibilityAndPos(const X_VisibilityPointF&, X_ChartPointOptions* chartPointOptions);
    //qint64 xp_pointId();
    void xp_updateCurrentItem();

protected:

    // FUNCS
    virtual void	mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    virtual void	mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

private:

    // VARS
    const X_CorrelationPlotterDataManager* xv_dataManager;
    X_ChartPointOptions* xv_chartPointOptions;
    qint64 xv_relatedObjectId;
    QColor xv_paintColor;
    static QColor xv_currentColor;
    static qint64 xv_currentSpectrumId;

    //QRectF xv_boundingRect;
};
//======================================================
#endif // X_SERIESPOINTGRAPHICSITEM_H

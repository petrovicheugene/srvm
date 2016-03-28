//======================================================
#ifndef ZSERIESPOINTGRAPHICSITEM_H
#define ZSERIESPOINTGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>
#include "ZVisibilityPointF.h"

//======================================================
class ZChartPointOptions;
//======================================================
class ZChartPointGraphicsItem : public QGraphicsItem
{
    friend class ZChartPointOptions;
public:

    explicit ZChartPointGraphicsItem(const ZVisibilityPointF &visibilityPoint,
                                      ZChartPointOptions* seriesPointOptions,
                                      qint64 relatedObjectId,
                                      QGraphicsItem * parent = 0);
    ~ZChartPointGraphicsItem();

    // overrided
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const override;
    int type() const override;

    // FUNCS
    ZChartPointOptions* zp_seriesPointOptions() const;
    void zp_setNewBoundingRect(const QRectF& boundingRect);

    qint64 zp_relatedObjectId() const;
    void zp_applyVisibilityAndPos(const ZVisibilityPointF&, ZChartPointOptions* chartPointOptions);
    //qint64 zp_pointId();

protected:

    // FUNCS

private:

    // VARS
    ZChartPointOptions* zv_chartPointOptions;
    qint64 zv_relatedObjectId;
    //QRectF zv_boundingRect;
};
//======================================================
#endif // ZSERIESPOINTGRAPHICSITEM_H

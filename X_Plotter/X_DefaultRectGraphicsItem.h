//========================================================
#ifndef X_DEFAULTRECTGRAPHICSITEM_H
#define X_DEFAULTRECTGRAPHICSITEM_H
//========================================================
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QPainter>
//========================================================
class X_AntialiasingLine : public QGraphicsLineItem
{
public:
    explicit X_AntialiasingLine(QGraphicsItem * parent = 0) : QGraphicsLineItem(parent){}
    explicit X_AntialiasingLine(const QLineF & line, QGraphicsItem * parent = 0) : QGraphicsLineItem(line, parent){}
    explicit X_AntialiasingLine(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem * parent = 0) : QGraphicsLineItem(x1, y1, x2, y2,parent){}
    virtual ~X_AntialiasingLine(){}
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        QGraphicsLineItem::paint(painter, option, widget);
        painter->restore();
    }
};

//========================================================
class X_DefaultRectGraphicsItem : public QGraphicsItem
{
public:
    X_DefaultRectGraphicsItem(const QRectF &, bool XAxesVisible, bool YAxesVisible, bool medianaLineVisible, QGraphicsItem* parent = 0);
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const override;
    int type() const override;

    void xp_fitItemInRect(const QRectF& rect);

    void xp_setAxisVisible(Qt::Orientation orientation, bool visibilityFlag);
    void xp_setAxisColor(QColor);
    void xp_setMedianaLineColor(QColor);

    void xp_setMedianaLineVisible(bool visibilityFlag);


private:

    // VARS
    QRectF xv_boundingRect;
    QPainterPath xv_shape;
    QPainterPath xv_axisPath;
    QPainterPath xv_medianaLinePath;

    QColor xv_axisColor;
    QColor xv_medianaLineColor;


    bool xv_XAxesVisible;
    bool xv_YAxesVisible;
    bool xv_medianaLineVisible;

    // FUNCS
    void zh_remakePainterPaths();


};
//========================================================
#endif // X_DEFAULTRECTGRAPHICSITEM_H

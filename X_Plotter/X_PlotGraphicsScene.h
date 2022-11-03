//====================================================
#ifndef X_PLOTGRAPHICSSCENE_H
#define X_PLOTGRAPHICSSCENE_H
//====================================================
#include <QGraphicsScene>
//====================================================
class X_WindowGraphicsItem;
//====================================================
class X_PlotGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit X_PlotGraphicsScene(QObject *parent = 0);
    explicit X_PlotGraphicsScene(const QRectF & sceneRect, QObject * parent = 0);
    explicit X_PlotGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject * parent = 0);
    ~X_PlotGraphicsScene();
    void xp_addItem(QGraphicsItem * item);
    void 	xp_removeItem(QGraphicsItem * item);

signals:


protected:

    // VARS
    QList<X_WindowGraphicsItem*> xv_windowItemList;
    QList<QGraphicsItem*> xv_itemToRemoveList;
    // FUNCS
    // resectioned
    void addItem(QGraphicsItem * item);
    void 	removeItem(QGraphicsItem * item);

    // my
    void xh_createConnections();

protected slots:

    void xh_onSceneRectChange(const QRectF&);

};
//====================================================
#endif // X_PLOTGRAPHICSSCENE_H

//====================================================
#include "X_PlotGraphicsScene.h"
#include "QPushButton"

#include <QApplication>
#include <QGraphicsItem>
//====================================================
X_PlotGraphicsScene::X_PlotGraphicsScene(QObject * parent)
    : QGraphicsScene(parent)
{
    xh_createConnections();
}
//====================================================
X_PlotGraphicsScene::X_PlotGraphicsScene(const QRectF & sceneRect, QObject * parent)
    : QGraphicsScene(sceneRect,  parent)
{
    xh_createConnections();
}
//====================================================
X_PlotGraphicsScene::X_PlotGraphicsScene(double x, double y, double width, double height, QObject * parent)
    : QGraphicsScene(x,  y,  width,  height, parent)
{
    xh_createConnections();
}
//====================================================
X_PlotGraphicsScene::~X_PlotGraphicsScene()
{
//    disconnect(this, &QGraphicsScene::sceneRectChanged,
//               this, &X_PlotGraphicsScene::xh_onSceneRectChange);

    QList<QGraphicsItem*> itemList = items();

    foreach(auto item, itemList)
    {
        QGraphicsScene::removeItem(item);
    }

    qDeleteAll(itemList);
}
//====================================================
void X_PlotGraphicsScene::xp_addItem(QGraphicsItem * item)
{
    //    if(item->type() == WindowItemType)
    //    {
    //        X_WindowGraphicsItem* windowItem = qgraphicsitem_cast<X_WindowGraphicsItem*>(item);
    //        windowItem->xp_fitInSceneRect(this->sceneRect());
    //        xv_windowItemList << windowItem;
    //    }

    QGraphicsScene::addItem(item);
}
//====================================================
void X_PlotGraphicsScene::xp_removeItem(QGraphicsItem * item)
{
    //    if(item->type() == WindowItemType)
    //    {
    //        X_WindowGraphicsItem* windowItem = qgraphicsitem_cast<X_WindowGraphicsItem*>(item);
    //        xv_windowItemList.removeAt(xv_windowItemList.indexOf(windowItem));
    //    }
    if(!items().contains(item))
    {
        return;
    }

    QGraphicsScene::removeItem(item);
    delete item;
}
//====================================================
void X_PlotGraphicsScene::xh_createConnections()
{
    connect(this, &QGraphicsScene::sceneRectChanged,
            this, &X_PlotGraphicsScene::xh_onSceneRectChange);
}
//====================================================
void X_PlotGraphicsScene::xh_onSceneRectChange(const QRectF& rect)
{
    // fitting window items in scene trect
    //    foreach(X_WindowGraphicsItem* windowItem, xv_windowItemList)
    //    {
    //        windowItem->xp_fitInSceneRect(rect);
    //    }
}
//====================================================

//====================================================
#include "X_PlotGraphicsScene.h"
#include "X_GraphicsItemUserTypes.h"
#include "X_WindowGraphicsItem.h"
#include "X_SpectrumGraphicsItem.h"
#include "X_General.h"
#include "QPushButton"

#include <QGraphicsItem>
//====================================================
X_PlotGraphicsScene::X_PlotGraphicsScene(QObject * parent)
    : QGraphicsScene(parent)
{
    zh_createConnections();
}
//====================================================
X_PlotGraphicsScene::X_PlotGraphicsScene(const QRectF & sceneRect, QObject * parent)
    : QGraphicsScene(sceneRect,  parent)
{
    zh_createConnections();
}
//====================================================
X_PlotGraphicsScene::X_PlotGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject * parent)
    : QGraphicsScene(x,  y,  width,  height, parent)
{
    zh_createConnections();
}
//====================================================
X_PlotGraphicsScene::~X_PlotGraphicsScene()
{
    //    QList<QGraphicsItem*> itemList = items();
    //    qDeleteAll(itemList);
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
    QGraphicsScene::removeItem(item);
    delete item;
}
//====================================================
void X_PlotGraphicsScene::zh_createConnections()
{
    connect(this, &QGraphicsScene::sceneRectChanged,
            this, &X_PlotGraphicsScene::zh_onSceneRectChange);
}
//====================================================
void X_PlotGraphicsScene::zh_onSceneRectChange(const QRectF& rect)
{
    // fitting window items in scene trect
    //    foreach(X_WindowGraphicsItem* windowItem, xv_windowItemList)
    //    {
    //        windowItem->xp_fitInSceneRect(rect);
    //    }
}
//====================================================

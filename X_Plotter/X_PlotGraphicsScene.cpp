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
    QGraphicsScene::addItem(item);
}
//====================================================
void X_PlotGraphicsScene::xp_removeItem(QGraphicsItem * item)
{
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

}
//====================================================

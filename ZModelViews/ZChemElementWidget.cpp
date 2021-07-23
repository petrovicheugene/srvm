//==============================================================
#include "ZChemElementWidget.h"
#include "ZStringDelegate.h"
#include "ZVisibilityStringDelegate.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAbstractItemModel>
#include <QHeaderView>
#include <QPushButton>
#include <QAction>
#include <QMenu>
//==============================================================
ZChemElementWidget::ZChemElementWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
}
//==============================================================
void ZChemElementWidget::zh_createComponents()
{
    zv_mainLayout = new QVBoxLayout;
    zv_mainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(zv_mainLayout);

    zv_table = new QTableView(this);
    zv_table->setContextMenuPolicy(Qt::CustomContextMenu);
    zv_mainLayout->addWidget(zv_table);

    zv_buttonLayout = new QHBoxLayout;
    zv_mainLayout->addLayout(zv_buttonLayout);
}
//==============================================================
void ZChemElementWidget::zh_createConnections()
{
    connect(zv_table, &QTableView::customContextMenuRequested,
            this, &ZChemElementWidget::zh_onContextMenuRequest);
}
//==============================================================
void ZChemElementWidget::zp_setModel(QAbstractItemModel* model)
{
    zv_table->setModel(model);
    zv_table->setItemDelegate(new ZStringDelegate(zv_table));
    ZVisibilityStringDelegate* visibilityStringDelegate = new ZVisibilityStringDelegate(zv_table);
    zv_table->viewport()->installEventFilter(visibilityStringDelegate);
    zv_table->setItemDelegateForColumn(0, visibilityStringDelegate);
    zv_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    zv_table->setAlternatingRowColors(true);

    connect(zv_table->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &ZChemElementWidget::zh_onCurrentElementChanged);
}
//==============================================================
void ZChemElementWidget::zp_appendButtonActions(QList<QAction*> actionList)
{
    zv_buttonLayout->addStretch();
    for(int a = 0; a < actionList.count(); a++)
    {
        QPushButton* button = new QPushButton(this);
        button->setFlat(true);
        button->setIcon(actionList.at(a)->icon());
        button->setToolTip(actionList.at(a)->toolTip());
        connect(button, &QPushButton::clicked,
                actionList[a], &QAction::trigger);
        zv_buttonLayout->addWidget(button);
    }
}
//==============================================================
void ZChemElementWidget::zp_appendContextMenuActions(QList<QAction*> actionList)
{
    foreach(QAction* action, actionList)
    {
        if(action != 0 && zv_contextMenuActionList.contains(action))
        {
            continue;
        }
        zv_contextMenuActionList.append(action);
    }
}
//==============================================================
void ZChemElementWidget::zp_setMainLayoutMargin(int margin)
{
    zv_mainLayout->setContentsMargins(margin, margin, margin, margin);
}
//==============================================================
void ZChemElementWidget::zp_setCurrentChemElementIndex(int chemElementIndex)
{
    if(!zv_table->model())
    {
        return;
    }

    QModelIndex index = zv_table->model()->index(chemElementIndex, 0, QModelIndex());
    if(index.isValid())
    {
        zv_table->setCurrentIndex(index);
    }
}
//==============================================================
void ZChemElementWidget::zp_currentChemElementIndex(int& chemElementIndex) const
{
    QModelIndex currentIndex = zv_table->currentIndex();
    if(!currentIndex.isValid())
    {
        chemElementIndex = -1;
        return;
    }

    chemElementIndex = currentIndex.row();
}
//==============================================================
void ZChemElementWidget::zp_startCurrentChemElementEdition()
{
    QModelIndex currentIndex = zv_table->currentIndex();
    if(!currentIndex.isValid())
    {
        return;
    }

    zv_table->edit(currentIndex);
}
//==============================================================
void ZChemElementWidget::zp_selectedChemElementIndexList(QList<int>& selectedIndexList) const
{
    QModelIndexList indexList = zv_table->selectionModel()->selectedIndexes();
    foreach(QModelIndex index, indexList)
    {
        if(!selectedIndexList.contains(index.row()))
        {
            selectedIndexList << index.row();
        }
    }
}
//==============================================================
void ZChemElementWidget::zh_onCurrentElementChanged(const QModelIndex & current,
                                                    const QModelIndex & previous)
{
    emit zg_currentCalibrationChanged(current.row(), previous.row());
}
//==============================================================
void ZChemElementWidget::zh_onContextMenuRequest(const QPoint &pos)
{
    QMenu *menu=new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    foreach(QAction* action, zv_contextMenuActionList)
    {
        if(action == 0)
        {
            menu->addSeparator();
            continue;
        }

        menu->addAction(action);
    }

    menu->popup(zv_table->viewport()->mapToGlobal(pos));
}
//==============================================================

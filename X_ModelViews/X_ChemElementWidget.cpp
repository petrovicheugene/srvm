//==============================================================
#include "X_ChemElementWidget.h"
#include "X_StringDelegate.h"
#include "X_VisibilityStringDelegate.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAbstractItemModel>
#include <QHeaderView>
#include <QPushButton>
#include <QAction>
#include <QMenu>
//==============================================================
X_ChemElementWidget::X_ChemElementWidget(QWidget *parent) : QWidget(parent)
{
    xh_createComponents();
    xh_createConnections();
}
//==============================================================
void X_ChemElementWidget::xh_createComponents()
{
    xv_mainLayout = new QVBoxLayout;
    xv_mainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(xv_mainLayout);

    xv_table = new QTableView(this);
    xv_table->setContextMenuPolicy(Qt::CustomContextMenu);
    xv_mainLayout->addWidget(xv_table);

    xv_buttonLayout = new QHBoxLayout;
    xv_mainLayout->addLayout(xv_buttonLayout);
}
//==============================================================
void X_ChemElementWidget::xh_createConnections()
{
    connect(xv_table, &QTableView::customContextMenuRequested,
            this, &X_ChemElementWidget::xh_onContextMenuRequest);
}
//==============================================================
void X_ChemElementWidget::xp_setModel(QAbstractItemModel* model)
{
    xv_table->setModel(model);
    xv_table->setItemDelegate(new X_StringDelegate(xv_table));
    X_VisibilityStringDelegate* visibilityStringDelegate = new X_VisibilityStringDelegate(xv_table);
    xv_table->viewport()->installEventFilter(visibilityStringDelegate);
    xv_table->setItemDelegateForColumn(0, visibilityStringDelegate);
    xv_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    xv_table->setAlternatingRowColors(true);

    connect(xv_table->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &X_ChemElementWidget::xh_onSelectedElementChanged);
}
//==============================================================
void X_ChemElementWidget::xp_appendButtonActions(QList<QAction*> actionList)
{
    xv_buttonLayout->addStretch();
    for(int a = 0; a < actionList.count(); a++)
    {
        QPushButton* button = new QPushButton(this);
        button->setFlat(true);
        button->setIcon(actionList.at(a)->icon());
        button->setToolTip(actionList.at(a)->toolTip());
        connect(button, &QPushButton::clicked,
                actionList[a], &QAction::trigger);
        xv_buttonLayout->addWidget(button);
    }
}
//==============================================================
void X_ChemElementWidget::xp_appendContextMenuActions(QList<QAction*> actionList)
{
    foreach(QAction* action, actionList)
    {
        if(action != 0 && xv_contextMenuActionList.contains(action))
        {
            continue;
        }
        xv_contextMenuActionList.append(action);
    }
}
//==============================================================
void X_ChemElementWidget::xp_setMainLayoutMargin(int margin)
{
    xv_mainLayout->setContentsMargins(margin, margin, margin, margin);
}
//==============================================================
void X_ChemElementWidget::xp_setCurrentChemElementIndex(int chemElementIndex)
{
    if(!xv_table->model())
    {
        return;
    }

    QModelIndex index = xv_table->model()->index(chemElementIndex, 0, QModelIndex());
    if(index.isValid())
    {
        xv_table->setCurrentIndex(index);
    }
}
//==============================================================
void X_ChemElementWidget::xp_currentChemElementIndex(int& chemElementIndex) const
{
    QModelIndex currentIndex = xv_table->currentIndex();
    if(!currentIndex.isValid())
    {
        chemElementIndex = -1;
        return;
    }

    chemElementIndex = currentIndex.row();
}
//==============================================================
void X_ChemElementWidget::xp_startCurrentChemElementEdition()
{
    QModelIndex currentIndex = xv_table->currentIndex();
    if(!currentIndex.isValid())
    {
        return;
    }

    xv_table->edit(currentIndex);
}
//==============================================================
void X_ChemElementWidget::xp_selectedChemElementIndexList(QList<int>& selectedIndexList) const
{
    QModelIndexList indexList = xv_table->selectionModel()->selectedIndexes();
    foreach(QModelIndex index, indexList)
    {
        if(!selectedIndexList.contains(index.row()))
        {
            selectedIndexList << index.row();
        }
    }
}
//==============================================================
void X_ChemElementWidget::xh_onSelectedElementChanged(const QItemSelection & current,
                                                    const QItemSelection & previous)
{
    emit xg_selectedChemElementChanged();
}
//==============================================================
void X_ChemElementWidget::xh_onContextMenuRequest(const QPoint &pos)
{
    QMenu *menu=new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    foreach(QAction* action, xv_contextMenuActionList)
    {
        if(action == 0)
        {
            menu->addSeparator();
            continue;
        }

        menu->addAction(action);
    }

    menu->popup(xv_table->viewport()->mapToGlobal(pos));
}
//==============================================================

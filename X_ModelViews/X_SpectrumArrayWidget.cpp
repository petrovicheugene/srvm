//==============================================================
#include "X_SpectrumArrayWidget.h"
#include "X_General.h"
#include "X_ChannelNumberDelegate.h"
#include "X_ArrayModel.h"
#include "X_StringDelegate.h"
#include "X_VisibilityStringDelegate.h"
#include "X_SpectrumArrayRepository.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAbstractItemModel>
#include <QHeaderView>
#include <QAction>
#include <QPushButton>
//==============================================================
X_SpectrumArrayWidget::X_SpectrumArrayWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
}
//==============================================================
void X_SpectrumArrayWidget::zh_createComponents()
{
    xv_mainLayout = new QVBoxLayout;
    xv_mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(xv_mainLayout);

    xv_table = new QTableView(this);
    xv_table->setContextMenuPolicy(Qt::CustomContextMenu);

    xv_mainLayout->addWidget(xv_table, INT_MAX);
    xv_buttonLayout = new QHBoxLayout;
    xv_mainLayout->addLayout(xv_buttonLayout);
}
//==============================================================
void X_SpectrumArrayWidget::xp_setModel(X_ArrayModel *model)
{
    xv_table->setModel(model);
    xv_table->setItemDelegateForColumn(0, new X_StringDelegate(xv_table));
    X_ChannelNumberDelegate* intDelegate = new X_ChannelNumberDelegate;
    intDelegate->xp_setEditorMinMax(0, glMaxGainFactor);
    xv_table->setItemDelegateForColumn(1, intDelegate);
    xv_table->setItemDelegateForColumn(2, new X_StringDelegate(xv_table));

    //xv_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    xv_table->setAlternatingRowColors(true);
    connect(xv_table->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &X_SpectrumArrayWidget::zh_onCurrentArrayChanged);
    connect(model, &X_ArrayModel::xg_checkCurrentArray,
            this, &X_SpectrumArrayWidget::zh_checkCurrentArray);
    xv_table->setSelectionMode(QAbstractItemView::SingleSelection);
}
//==============================================================
void X_SpectrumArrayWidget::xp_appendButtonActions(QList<QAction*> actionList)
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
void X_SpectrumArrayWidget::xp_appendContextMenuActions(QList<QAction*> actionList)
{
    foreach(QAction* action, actionList)
    {
        if(action != nullptr && xv_contextMenuActionList.contains(action))
        {
            continue;
        }
        xv_contextMenuActionList.append(action);
    }
}
//==============================================================
void X_SpectrumArrayWidget::xp_connectToSpectrumArrayRepository(X_SpectrumArrayRepository* repository)
{
    xp_appendButtonActions(repository->xp_arrayActions());
    xp_appendContextMenuActions(repository->xp_arrayContextMenuActions());

    connect(repository, &X_SpectrumArrayRepository::xg_setCurrentArrayIndex,
            this, &X_SpectrumArrayWidget::xp_setCurrentArrayIndex);

    connect(repository, &X_SpectrumArrayRepository::xg_startCurrentArrayEdition,
            this, &X_SpectrumArrayWidget::xp_startCurrentArrayEdition);
    connect(repository, &X_SpectrumArrayRepository::xg_requestCurrentArrayIndex,
            this, &X_SpectrumArrayWidget::xp_currentArrayIndex);
    connect(this, &X_SpectrumArrayWidget::xg_currentArrayChanged,
            repository, &X_SpectrumArrayRepository::xp_currentArrayChanged);
}
//==============================================================
void X_SpectrumArrayWidget::xp_setMainLayoutMargin(int margin)
{
    xv_mainLayout->setContentsMargins(margin, margin, margin, margin);
}
//==============================================================
void X_SpectrumArrayWidget::xp_setCurrentArrayIndex(int arrayIndex)
{
    if(!xv_table->model())
    {
        return;
    }

    QModelIndex previousIndex = xv_table->currentIndex();
    QModelIndex index = xv_table->model()->index(arrayIndex, 0, QModelIndex());
    if(index.isValid())
    {
        xv_table->setCurrentIndex(index);
        int currentRow;
        if(index.isValid())
        {
            currentRow = index.row();
        }
        else
        {
            currentRow = -1;
        }
        int previousRow;
        if(previousIndex.isValid())
        {
            previousRow = previousIndex.row();
        }
        else
        {
            previousRow = -1;
        }

        emit xg_currentArrayChanged(currentRow, previousRow);
    }
}
//==============================================================
void X_SpectrumArrayWidget::xp_currentArrayIndex(int& arrayIndex)
{
    QModelIndex currentIndex = xv_table->currentIndex();
    if(!currentIndex.isValid())
    {
        arrayIndex = -1;
        return;
    }

    arrayIndex = currentIndex.row();
}
//==============================================================
void X_SpectrumArrayWidget::xp_startCurrentArrayEdition()
{
    QModelIndex currentIndex = xv_table->currentIndex();
    if(!currentIndex.isValid())
    {
        return;
    }

    xv_table->edit(currentIndex);
}
//==============================================================
void X_SpectrumArrayWidget::zh_createConnections()
{
    connect(xv_table, &QTableView::customContextMenuRequested,
            this, &X_SpectrumArrayWidget::zh_onContextMenuRequest);
}
//==============================================================
void X_SpectrumArrayWidget::zh_checkCurrentArray()
{
    if(!xv_table->selectionModel())
    {
        return;
    }

    QModelIndex currentIndex = xv_table->selectionModel()->currentIndex();
    if(!currentIndex.isValid() || currentIndex.row() < 0
            || currentIndex.row() >= xv_table->model()->rowCount())
    {
        QModelIndex indexToGo = xv_table->model()->index(0,0);
        xv_table->setCurrentIndex(indexToGo);
    }
}
//==============================================================
void X_SpectrumArrayWidget::zh_onCurrentArrayChanged(const QModelIndex & current,
                                                    const QModelIndex & previous)
{
    int currentRow;
    if(current.isValid())
    {
        currentRow = current.row();
    }
    else
    {
        currentRow = -1;
    }
    int previousRow;
    if(previous.isValid())
    {
        previousRow = previous.row();
    }
    else
    {
        previousRow = -1;
    }

    emit xg_currentArrayChanged(currentRow, previousRow);
}
//==============================================================
void X_SpectrumArrayWidget::zh_onContextMenuRequest(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    foreach(QAction* action, xv_contextMenuActionList)
    {
        if(!action)
        {
            menu->addSeparator();
            continue;
        }

        menu->addAction(action);
    }

    menu->popup(xv_table->viewport()->mapToGlobal(pos));
}
//==============================================================
// Хороший  набор  пользовательских  требований  (stakeholder  requirements)  может
// обеспечить  краткое  и  не  техническое  описание  того,  что  будет  разработано,
// на  уровне, который доступен для понимания высокого руководства.

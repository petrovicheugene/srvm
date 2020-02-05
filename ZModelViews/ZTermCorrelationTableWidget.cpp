//=============================================================
#include "ZTermCorrelationTableWidget.h"
#include "ZTermCorrelationTableModel.h"
#include "ZCustomCheckableVerticalHeaderView.h"
#include "ZNumericDelegate.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAction>
#include <QItemSelectionModel>
//=============================================================
ZTermCorrelationTableWidget::ZTermCorrelationTableWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
}
//=============================================================
void ZTermCorrelationTableWidget::zp_setModel(ZTermCorrelationTableModel* model)
{
    zv_table->setModel(model);

    connect(this, &ZTermCorrelationTableWidget::zg_userChangesTermState,
            model, &ZTermCorrelationTableModel::zp_onUserChangesTermState);
    ZNumericDelegate* numericDelegate = new ZNumericDelegate(zv_table);
    //    connect(numericDelegate, &ZNumericDelegate::zg_editNext,
    //            this, &ZTermCorrelationTableWidget::zh_editNext);
    zv_table->setItemDelegate(numericDelegate);
    //    ZSpectrumTableDelegate* spectrumDelegate = new ZSpectrumTableDelegate(zv_table);
    //    zv_table->setItemDelegateForColumn(0, new ZVisibilityStringDelegate(zv_table));
    //    zv_table->setItemDelegateForColumn(1, spectrumDelegate);
    zv_table->setAlternatingRowColors(true);


    connect(zv_table->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &ZTermCorrelationTableWidget::zh_onCurrentTermChanged);
    connect(this, &ZTermCorrelationTableWidget::zg_currentTermChanged,
            model, &ZTermCorrelationTableModel::zg_currentTermChanged);
    connect(this, &ZTermCorrelationTableWidget::zg_termDoubleClicked,
            model, &ZTermCorrelationTableModel::zg_termDoubleClicked);
}
//=============================================================
void ZTermCorrelationTableWidget::zp_appendButtonActions(QList<QAction*> actionList)
{
    //    zv_buttonLayout->addStretch();
    //    for(int a = 0; a < actionList.count(); a++)
    //    {
    //        if(actionList.at(a) == 0)
    //        {
    //            // Separator for context menu
    //            continue;
    //        }

    //        QPushButton* button = new QPushButton(this);
    //        button->setFlat(true);
    //        button->setIcon(actionList.at(a)->icon());
    //        button->setToolTip(actionList.at(a)->toolTip());
    //        connect(button, &QPushButton::clicked,
    //                actionList[a], &QAction::trigger);
    //        zv_buttonLayout->addWidget(button);
    //    }
}
//=============================================================
void ZTermCorrelationTableWidget::zp_appendContextMenuActions(QList<QAction*> actionList)
{
    foreach(QAction* action, actionList)
    {
        if(action != nullptr && zv_contextMenuActionList.contains(action))
        {
            continue;
        }
        zv_contextMenuActionList.append(action);
    }
}
//=============================================================
void ZTermCorrelationTableWidget::zp_setMainLayoutMargin(int margin)
{
    zv_mainLayout->setMargin(margin);
}
//=============================================================
void ZTermCorrelationTableWidget::zh_createComponents()
{
    zv_mainLayout = new QVBoxLayout;
    zv_mainLayout->setMargin(0);
    setLayout(zv_mainLayout);

    zv_table = new QTableView(this);
    zv_table->setContextMenuPolicy(Qt::CustomContextMenu);

    // Vertical Header Replacing
    ZCustomCheckableVerticalHeaderView* checkableHeader = new ZCustomCheckableVerticalHeaderView();
    connect(checkableHeader, &ZCustomCheckableVerticalHeaderView::zg_userChangesTermState,
            this, &ZTermCorrelationTableWidget::zg_userChangesTermState);
    connect(checkableHeader, &ZCustomCheckableVerticalHeaderView::doubleClicked,
            this, &ZTermCorrelationTableWidget::zh_onTermDoubleClick);
    zv_table->setVerticalHeader(checkableHeader);
    zv_table->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    zv_mainLayout->addWidget(zv_table, INT_MAX);

    zv_buttonLayout = new QHBoxLayout;
    zv_mainLayout->addLayout(zv_buttonLayout);
}
//=============================================================
void ZTermCorrelationTableWidget::zh_createConnections()
{
    connect(zv_table, &QTableView::customContextMenuRequested,
            this, &ZTermCorrelationTableWidget::zh_onContextMenuRequest);
    connect(zv_table->verticalHeader(), &QHeaderView::customContextMenuRequested,
            this, &ZTermCorrelationTableWidget::zh_onContextMenuRequest);
}
//=============================================================
void ZTermCorrelationTableWidget::zh_editNext(QModelIndex editedIndex)
{
    if(!editedIndex.isValid() || editedIndex.row() < 0)
    {
        return;
    }

    if(editedIndex.row()+1 >= editedIndex.model()->rowCount())
    {
        // end of column - quit editing keeping current index as "current"
        zv_table->setCurrentIndex(editedIndex);
        return;
    }

    // defining next index
    QModelIndex nextIndex = editedIndex.model()->index(editedIndex.row()+1, editedIndex.column());
    if(!nextIndex.isValid())
    {
        return;
    }

    zv_table->setCurrentIndex(nextIndex);
    zv_table->edit(nextIndex);
}
//=============================================================
void ZTermCorrelationTableWidget::zp_currentTermIndex(int& index)
{
    if(!zv_table)
    {
        index = -1;
    }

    index = zv_table->currentIndex().row();
}
//=============================================================
void ZTermCorrelationTableWidget::zh_onCurrentTermChanged(QModelIndex current, QModelIndex previous)
{
    emit zg_currentTermChanged(current.row(), previous.row());
}
//=============================================================
void ZTermCorrelationTableWidget::zh_onTermDoubleClick(const QModelIndex& index)
{
    if (!index.isValid() || !index.model())
    {
        return;
    }

    emit zg_termDoubleClicked(index.row());
}
//=============================================================
void ZTermCorrelationTableWidget::zh_onContextMenuRequest(const QPoint &pos)
{
    QMenu *menu=new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    foreach(QAction* action, zv_contextMenuActionList)
    {
        if(action == nullptr)
        {
            menu->addSeparator();
            continue;
        }

        menu->addAction(action);
    }

    QPoint menuPos;
    if(sender() == zv_table->verticalHeader())
    {
        menuPos = zv_table->verticalHeader()->mapToGlobal(pos);
    }
    else
    {
        menuPos = zv_table->viewport()->mapToGlobal(pos);
    }

    menu->popup(menuPos);
}
//=============================================================

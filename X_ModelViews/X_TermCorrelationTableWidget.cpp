//=============================================================
#include "X_TermCorrelationTableWidget.h"
#include "X_TermCorrelationTableModel.h"
#include "X_CustomCheckableVerticalHeaderView.h"
#include "X_NumericDelegate.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAction>
#include <QItemSelectionModel>
//=============================================================
X_TermCorrelationTableWidget::X_TermCorrelationTableWidget(QWidget *parent) : QWidget(parent)
{
    xh_createComponents();
    xh_createConnections();
}
//=============================================================
void X_TermCorrelationTableWidget::xp_setModel(X_TermCorrelationTableModel* model)
{
    xv_table->setModel(model);

    connect(this, &X_TermCorrelationTableWidget::xg_userChangesTermState,
            model, &X_TermCorrelationTableModel::xp_onUserChangesTermState);
    X_NumericDelegate* numericDelegate = new X_NumericDelegate(xv_table);
    //    connect(numericDelegate, &X_NumericDelegate::xg_editNext,
    //            this, &X_TermCorrelationTableWidget::xh_editNext);
    xv_table->setItemDelegate(numericDelegate);
    //    X_SpectrumTableDelegate* spectrumDelegate = new X_SpectrumTableDelegate(xv_table);
    //    xv_table->setItemDelegateForColumn(0, new X_VisibilityStringDelegate(xv_table));
    //    xv_table->setItemDelegateForColumn(1, spectrumDelegate);
    xv_table->setAlternatingRowColors(true);


    connect(xv_table->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &X_TermCorrelationTableWidget::xh_onCurrentTermChanged);
    connect(this, &X_TermCorrelationTableWidget::xg_currentTermChanged,
            model, &X_TermCorrelationTableModel::xg_currentTermChanged);
    connect(this, &X_TermCorrelationTableWidget::xg_termDoubleClicked,
            model, &X_TermCorrelationTableModel::xg_termDoubleClicked);
}
//=============================================================
void X_TermCorrelationTableWidget::xp_appendButtonActions(QList<QAction*> actionList)
{
    //    xv_buttonLayout->addStretch();
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
    //        xv_buttonLayout->addWidget(button);
    //    }
}
//=============================================================
void X_TermCorrelationTableWidget::xp_appendContextMenuActions(QList<QAction*> actionList)
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
//=============================================================
void X_TermCorrelationTableWidget::xp_setMainLayoutMargin(int margin)
{
    xv_mainLayout->setContentsMargins(margin, margin, margin, margin);
}
//=============================================================
void X_TermCorrelationTableWidget::xh_createComponents()
{
    xv_mainLayout = new QVBoxLayout;
    xv_mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(xv_mainLayout);

    xv_table = new QTableView(this);
    xv_table->setContextMenuPolicy(Qt::CustomContextMenu);

    // Vertical Header Replacing
    X_CustomCheckableVerticalHeaderView* checkableHeader = new X_CustomCheckableVerticalHeaderView();
    connect(checkableHeader, &X_CustomCheckableVerticalHeaderView::xg_userChangesTermState,
            this, &X_TermCorrelationTableWidget::xg_userChangesTermState);
    connect(checkableHeader, &X_CustomCheckableVerticalHeaderView::doubleClicked,
            this, &X_TermCorrelationTableWidget::xh_onTermDoubleClick);
    xv_table->setVerticalHeader(checkableHeader);
    xv_table->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    xv_mainLayout->addWidget(xv_table, INT_MAX);

    xv_buttonLayout = new QHBoxLayout;
    xv_mainLayout->addLayout(xv_buttonLayout);
}
//=============================================================
void X_TermCorrelationTableWidget::xh_createConnections()
{
    connect(xv_table, &QTableView::customContextMenuRequested,
            this, &X_TermCorrelationTableWidget::xh_onContextMenuRequest);
    connect(xv_table->verticalHeader(), &QHeaderView::customContextMenuRequested,
            this, &X_TermCorrelationTableWidget::xh_onContextMenuRequest);
}
//=============================================================
void X_TermCorrelationTableWidget::xh_editNext(QModelIndex editedIndex)
{
    if(!editedIndex.isValid() || editedIndex.row() < 0)
    {
        return;
    }

    if(editedIndex.row()+1 >= editedIndex.model()->rowCount())
    {
        // end of column - quit editing keeping current index as "current"
        xv_table->setCurrentIndex(editedIndex);
        return;
    }

    // defining next index
    QModelIndex nextIndex = editedIndex.model()->index(editedIndex.row()+1, editedIndex.column());
    if(!nextIndex.isValid())
    {
        return;
    }

    xv_table->setCurrentIndex(nextIndex);
    xv_table->edit(nextIndex);
}
//=============================================================
void X_TermCorrelationTableWidget::xp_currentTermIndex(int& index)
{
    if(!xv_table)
    {
        index = -1;
    }

    index = xv_table->currentIndex().row();
}
//=============================================================
void X_TermCorrelationTableWidget::xh_onCurrentTermChanged(QModelIndex current, QModelIndex previous)
{
    emit xg_currentTermChanged(current.row(), previous.row());
}
//=============================================================
void X_TermCorrelationTableWidget::xh_onTermDoubleClick(const QModelIndex& index)
{
    if (!index.isValid() || !index.model())
    {
        return;
    }

    emit xg_termDoubleClicked(index.row());
}
//=============================================================
void X_TermCorrelationTableWidget::xh_onContextMenuRequest(const QPoint &pos)
{
    QMenu *menu=new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    foreach(QAction* action, xv_contextMenuActionList)
    {
        if(action == nullptr)
        {
            menu->addSeparator();
            continue;
        }

        menu->addAction(action);
    }

    QPoint menuPos;
    if(sender() == xv_table->verticalHeader())
    {
        menuPos = xv_table->verticalHeader()->mapToGlobal(pos);
    }
    else
    {
        menuPos = xv_table->viewport()->mapToGlobal(pos);
    }

    menu->popup(menuPos);
}
//=============================================================

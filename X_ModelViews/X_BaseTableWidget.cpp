//=========================================================================
#include "X_BaseTableWidget.h"
#include "X_General.h"
#include "X_ControlAction.h"

#include <QAbstractItemModel>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QList>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>
//=========================================================================
X_BaseTableWidget::X_BaseTableWidget(QWidget *parent) : QWidget(parent)
{
    xv_captionLabel = 0;
    zh_createComponents();
    zh_createConnections();
}
//=========================================================================
void X_BaseTableWidget::xp_setModel(QAbstractItemModel* model, bool alternatingRowColorFlag)
{
    xv_table->setModel(model);
    connect(xv_table->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &X_BaseTableWidget::xg_currentChanged);
    connect(xv_table->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &X_BaseTableWidget::zh_onSelectionChange);

    xv_table->setAlternatingRowColors(alternatingRowColorFlag);
}
//=========================================================================
void X_BaseTableWidget::zh_onSelectionChange(const QItemSelection& selected,
                                            const QItemSelection& deselected)
{
    emit xg_selectionChanged();
}
//=========================================================================
QTableView* X_BaseTableWidget::xp_tableView() const
{
    return xv_table;
}
//=========================================================================
void X_BaseTableWidget::xp_setCaption(const QString& caption)
{
    if(xv_captionLabel != 0)
    {
        if(caption.isEmpty())
        {
            delete xv_captionLabel;
            xv_captionLabel = 0;
            return;
        }
        else
        {
            xv_captionLabel->setText(caption);
            return;
        }
    }
    else
    {
        if(!caption.isEmpty())
        {
            xv_captionLabel = new QLabel(this);
            xv_captionLabel->setText(caption);
            xv_mainLayout->insertWidget(0, xv_captionLabel);
            return;
        }
    }
}
//=========================================================================
void X_BaseTableWidget::xp_setColumnHidden(int column, bool hidden)
{
    xv_table->setColumnHidden(column, hidden);
}
//=========================================================================
void X_BaseTableWidget::xp_setHorizontalHeaderHidden(bool hidden)
{
    xv_table->horizontalHeader()->setHidden(hidden);
}
//=========================================================================
void X_BaseTableWidget::xp_setVerticalHeaderHidden(bool hidden)
{
    xv_table->verticalHeader()->setHidden(hidden);
}
//=========================================================================
void X_BaseTableWidget::xp_setStretchLastSection(bool stretch)
{
    xv_table->horizontalHeader()->setStretchLastSection(stretch);
}
//=========================================================================
void X_BaseTableWidget::xp_appendButtonActions(const QList<X_ControlAction*>& actionList)
{
    xv_buttonLayout->addStretch();
    // xv_table->addActions(actionList);

    for(int a = 0; a < actionList.count(); a++)
    {
        if(actionList.at(a) == nullptr)
        {
            // Separator for context menu
            continue;
        }
        QPushButton* button = new QPushButton(this);
        button->setFlat(true);
        button->setIcon(actionList.at(a)->icon());
        button->setText(actionList.at(a)->text());
        button->setToolTip(actionList.at(a)->toolTip());
        button->setEnabled(actionList[a]->isEnabled());
        connect(button, &QPushButton::clicked,
                actionList[a], &QAction::trigger);
        connect(actionList.at(a), &X_ControlAction::xg_enableChanged,
                button, &QPushButton::setEnabled);

        xv_buttonLayout->addWidget(button, 0, Qt::AlignRight);
    }
}
//=========================================================================
void X_BaseTableWidget::xp_appendContextActions(const QList<X_ControlAction*>& actionList)
{
    QList<QAction*> qactionList;
    foreach(X_ControlAction* action, actionList)
    {
        qactionList.append(static_cast<QAction*>(action));
    }
    xv_table->addActions(qactionList);
    xv_table->setContextMenuPolicy(Qt::ActionsContextMenu);
}
//=========================================================================
void X_BaseTableWidget::zh_createComponents()
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
//=========================================================================
void X_BaseTableWidget::zh_createConnections()
{
    //    connect(xv_table, &QTableView::customContextMenuRequested,
    //            this, &X_JointSpectrumTableWidget::zh_onContextMenuRequest);
}
//=========================================================================
QList<int> X_BaseTableWidget::xp_selectedRowList()
{
    QList<int> selectedRowList;
    QModelIndexList indexList = xv_table->selectionModel()->selectedIndexes();
    foreach(QModelIndex index, indexList)
    {
        if(!selectedRowList.contains(index.row()) )
        {
            selectedRowList.append(index.row());
        }
    }

    return selectedRowList;
}
//=========================================================================
QModelIndexList X_BaseTableWidget::xp_selectedModelIndexList() const
{
    QModelIndexList indexList = xv_table->selectionModel()->selectedIndexes();
    return indexList;
}
//=========================================================================
void X_BaseTableWidget::xp_setCurrentRow(int row)
{
    QModelIndex currentIndex;
    QAbstractItemModel* model = xv_table->model();
    if(model)
    {
        currentIndex = model->index(row, 0);
    }

    xv_table->selectionModel()->setCurrentIndex(currentIndex, QItemSelectionModel::Clear);
    xv_table->selectionModel()->select(currentIndex, QItemSelectionModel::Clear);
}
//=========================================================================
void X_BaseTableWidget::xp_currentIndex(QModelIndex& index)
{
    index = QModelIndex();
    if(!xv_table)
    {
        return;
    }

    index = xv_table->currentIndex();
}
//=========================================================================

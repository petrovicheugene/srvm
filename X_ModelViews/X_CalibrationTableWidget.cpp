//==============================================================
#include "X_CalibrationTableWidget.h"
#include "X_VisibilityStringDelegate.h"
#include "X_ChemElementComboBoxDelegate.h"
#include "X_CalibrationRepository.h"
#include "X_EquationDelegate.h"
#include "X_NormaDelegate.h"
#include "X_NumericDelegate.h"
#include "X_General.h"

#include <QAbstractItemModel>
#include <QAction>
#include <QDebug>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>

//==============================================================
X_CalibrationTableWidget::X_CalibrationTableWidget(QWidget *parent) : QWidget(parent)
{
    xh_createComponents();
    xh_createConnections();
}
//==============================================================
void X_CalibrationTableWidget::xh_createComponents()
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
void X_CalibrationTableWidget::xh_createConnections()
{
    connect(xv_table, &QTableView::customContextMenuRequested,
            this, &X_CalibrationTableWidget::xh_onContextMenuRequest);
}
//==============================================================
void X_CalibrationTableWidget::xp_setModel(QAbstractItemModel* model)
{
    xv_table->setModel(model);

    X_VisibilityStringDelegate* visibilityStringDelegate = new X_VisibilityStringDelegate(xv_table);
    //xv_table->viewport()->installEventFilter(visibilityStringDelegate);
    xv_table->setItemDelegateForColumn(0, visibilityStringDelegate);
    X_ChemElementComboBoxDelegate* chemElementComboBoxDelegate = new X_ChemElementComboBoxDelegate(xv_table);
    xv_table->setItemDelegateForColumn(1, chemElementComboBoxDelegate);

    X_EquationDelegate* equationDelegate = new X_EquationDelegate(xv_table);
    connect(this,
            &X_CalibrationTableWidget::xg_currentCalibrationWindowName,
            equationDelegate,
            &X_EquationDelegate::xg_currentCalibrationWindowName);
    xv_table->setItemDelegateForColumn(2, equationDelegate);

    X_NormaDelegate* normaDelegate = new X_NormaDelegate(xv_table);
    connect(this,
            &X_CalibrationTableWidget::xg_currentCalibrationWindowName,
            normaDelegate,
            &X_NormaDelegate::xg_currentCalibrationWindowName);
    xv_table->setItemDelegateForColumn(3, normaDelegate);

    X_NumericDelegate* interceptDelegate = new X_NumericDelegate(xv_table);
    xv_table->setItemDelegateForColumn(4, interceptDelegate);

    //xv_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    // xv_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    //xv_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    xv_table->setAlternatingRowColors(true);

    connect(xv_table->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &X_CalibrationTableWidget::xh_onCurrentCalibrationChange);
    connect(xv_table->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &X_CalibrationTableWidget::xh_onSelectedCalibrationChange);
    connect(model,
            &QAbstractItemModel::rowsMoved,
            this,
            &X_CalibrationTableWidget::xh_onRowCountChange);
    connect(model,
            &QAbstractItemModel::rowsRemoved,
            this,
            &X_CalibrationTableWidget::xh_onRowCountChange);

    connect(chemElementComboBoxDelegate,
            &X_ChemElementComboBoxDelegate::xg_requestChemElementList,
            this,
            &X_CalibrationTableWidget::xg_requestChemElementList);
}
//==============================================================
void X_CalibrationTableWidget::xh_onRowCountChange(const QModelIndex& parent,
                                                  int first,
                                                  int last)
{
    QModelIndex currentIndex = xv_table->selectionModel()->currentIndex();
    emit xg_currentCalibrationChanged(currentIndex.row(), -1);
}
//==============================================================
void X_CalibrationTableWidget::xp_appendButtonActions(QList<QAction*> actionList)
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
void X_CalibrationTableWidget::xp_appendContextMenuActions(QList<QAction*> actionList)
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
void X_CalibrationTableWidget::xp_connectToCalibrationRepository(X_CalibrationRepository* repository)
{
    this->xp_appendButtonActions(repository->xp_calibrationActions());
    this->xp_appendContextMenuActions(repository->xp_calibrationContextMenuActions());

    connect(repository, &X_CalibrationRepository::xg_requestSelectedCalibrationIndexList,
            this, &X_CalibrationTableWidget::xp_selectedCalibrationIndexList);
    connect(repository, &X_CalibrationRepository::xg_requestCurrentCalibrationIndex,
            this, &X_CalibrationTableWidget::xp_currentCalibrationIndex);
    connect(this, &X_CalibrationTableWidget::xg_currentCalibrationChanged,
            repository, &X_CalibrationRepository::xp_onCurrentCalibrationChange);
    connect(this, &X_CalibrationTableWidget::xg_selectedCalibrationChanged,
            repository, &X_CalibrationRepository::xp_onSelectedCalibrationChange);

    connect(repository, &X_CalibrationRepository::xg_setCurrentCalibrationIndex,
            this, &X_CalibrationTableWidget::xp_setCurrentCalibrationIndex);
    connect(repository, &X_CalibrationRepository::xg_startCurrentCalibrationEdition,
            this, &X_CalibrationTableWidget::xp_startCurrentCalibrationEdition);

    connect(repository,
            &X_CalibrationRepository::xg_currentCalibrationWindowName,
            this,
            &X_CalibrationTableWidget::xg_currentCalibrationWindowName);

}
//==============================================================
void X_CalibrationTableWidget::xp_setMainLayoutMargin(int margin)
{
    xv_mainLayout->setContentsMargins(margin, margin, margin, margin);
}
//==============================================================
void X_CalibrationTableWidget::xp_selectedCalibrationIndexList(QList<int>& selectedList)
{
    QModelIndexList indexList = xv_table->selectionModel()->selectedIndexes();
    foreach(QModelIndex index, indexList)
    {
        if(!selectedList.contains(index.row()))
        {
            selectedList << index.row();
        }
    }
}
//==============================================================
void X_CalibrationTableWidget::xp_currentCalibrationIndex(int& row)
{
    QModelIndex index = xv_table->selectionModel()->currentIndex();
    if(index.isValid())
    {
        row = index.row();
    }
}
//==============================================================
void X_CalibrationTableWidget::xp_setCurrentCalibrationIndex(int calibrationIndex)
{
    if(!xv_table->model())
    {
        return;
    }

    QModelIndex previousIndex = xv_table->currentIndex();
    QModelIndex index = xv_table->model()->index(calibrationIndex, 0, QModelIndex());
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

        emit xg_currentCalibrationChanged(currentRow, previousRow);
    }
}
//==============================================================
void X_CalibrationTableWidget::xp_startCurrentCalibrationEdition()
{
    QModelIndex currentIndex = xv_table->currentIndex();
    if(!currentIndex.isValid())
    {
        return;
    }

    xv_table->edit(currentIndex);
}
//==============================================================
void X_CalibrationTableWidget::xh_onCurrentCalibrationChange(const QModelIndex & current, const QModelIndex & previous)
{
    emit xg_currentCalibrationChanged(current.row(), previous.row());
}
//==============================================================
void X_CalibrationTableWidget::xh_onSelectedCalibrationChange(const QItemSelection & selected, const QItemSelection & deselected)
{
    QList<int> selectedList;
    xp_selectedCalibrationIndexList(selectedList);
    emit xg_selectedCalibrationChanged(selectedList);
}
//==============================================================
void X_CalibrationTableWidget::xh_onContextMenuRequest(const QPoint &pos)
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

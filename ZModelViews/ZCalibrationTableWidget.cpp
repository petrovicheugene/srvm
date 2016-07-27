//==============================================================
#include "ZCalibrationTableWidget.h"
#include "ZVisibilityStringDelegate.h"
#include "ZChemElementComboBoxDelegate.h"
#include "ZCalibrationRepository.h"
#include "ZEquationDelegate.h"
#include "ZNormaDelegate.h"
#include "ZNumericDelegate.h"
#include "ZConstants.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAbstractItemModel>
#include <QHeaderView>
#include <QAction>
#include <QPushButton>

//==============================================================
ZCalibrationTableWidget::ZCalibrationTableWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
}
//==============================================================
void ZCalibrationTableWidget::zh_createComponents()
{
    zv_mainLayout = new QVBoxLayout(this);
    zv_mainLayout->setMargin(0);

    setLayout(zv_mainLayout);

    zv_table = new QTableView(this);
    zv_table->setContextMenuPolicy(Qt::CustomContextMenu);

    zv_mainLayout->addWidget(zv_table);

    zv_buttonLayout = new QHBoxLayout(this);
    zv_mainLayout->addLayout(zv_buttonLayout);
}
//==============================================================
void ZCalibrationTableWidget::zh_createConnections()
{
    connect(zv_table, &QTableView::customContextMenuRequested,
            this, &ZCalibrationTableWidget::zh_onContextMenuRequest);
}
//==============================================================
void ZCalibrationTableWidget::zp_setModel(QAbstractItemModel* model)
{
    zv_table->setModel(model);

    ZVisibilityStringDelegate* visibilityStringDelegate = new ZVisibilityStringDelegate(zv_table);
    //zv_table->viewport()->installEventFilter(visibilityStringDelegate);
    zv_table->setItemDelegateForColumn(0, visibilityStringDelegate);
    ZChemElementComboBoxDelegate* chemElementComboBoxDelegate = new ZChemElementComboBoxDelegate(zv_table);
    zv_table->setItemDelegateForColumn(1, chemElementComboBoxDelegate);

    ZEquationDelegate* equationDelegate = new ZEquationDelegate(zv_table);
    zv_table->setItemDelegateForColumn(2, equationDelegate);

    ZNormaDelegate* normaDelegate = new ZNormaDelegate(zv_table);
    zv_table->setItemDelegateForColumn(3, normaDelegate);

    ZNumericDelegate* interceptDelegate = new ZNumericDelegate(zv_table);
    zv_table->setItemDelegateForColumn(4, interceptDelegate);

    //zv_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    // zv_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    //zv_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    zv_table->setAlternatingRowColors(true);

    connect(zv_table->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &ZCalibrationTableWidget::zh_onCurrentCalibrationChange);
    connect(zv_table->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &ZCalibrationTableWidget::zh_onSelectedCalibrationChange);

    connect(chemElementComboBoxDelegate, &ZChemElementComboBoxDelegate::zg_requestChemElementList,
            this, &ZCalibrationTableWidget::zg_requestChemElementList);

}
//==============================================================
void ZCalibrationTableWidget::zp_appendButtonActions(QList<QAction*> actionList)
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
void ZCalibrationTableWidget::zp_appendContextMenuActions(QList<QAction*> actionList)
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
void ZCalibrationTableWidget::zp_connectToCalibrationRepository(ZCalibrationRepository* repository)
{
    this->zp_appendButtonActions(repository->zp_calibrationActions());
    this->zp_appendContextMenuActions(repository->zp_calibrationContextMenuActions());

    connect(repository, &ZCalibrationRepository::zg_requestSelectedCalibrationIndexList,
            this, &ZCalibrationTableWidget::zp_selectedCalibrationIndexList);
    connect(repository, &ZCalibrationRepository::zg_requestCurrentCalibrationIndex,
            this, &ZCalibrationTableWidget::zp_currentCalibrationIndex);
    connect(this, &ZCalibrationTableWidget::zg_currentCalibrationChanged,
            repository, &ZCalibrationRepository::zp_onCurrentCalibrationChange);
    connect(this, &ZCalibrationTableWidget::zg_selectedCalibrationChanged,
            repository, &ZCalibrationRepository::zp_onSelectedCalibrationChange);

    connect(repository, &ZCalibrationRepository::zg_setCurrentCalibrationIndex,
            this, &ZCalibrationTableWidget::zp_setCurrentCalibrationIndex);
    connect(repository, &ZCalibrationRepository::zg_startCurrentCalibrationEdition,
            this, &ZCalibrationTableWidget::zp_startCurrentCalibrationEdition);

}
//==============================================================
void ZCalibrationTableWidget::zp_setMainLayoutMargin(int margin)
{
    zv_mainLayout->setMargin(margin);
}
//==============================================================
void ZCalibrationTableWidget::zp_selectedCalibrationIndexList(QList<int>& selectedList)
{
    QModelIndexList indexList = zv_table->selectionModel()->selectedIndexes();
    foreach(QModelIndex index, indexList)
    {
        if(!selectedList.contains(index.row()))
        {
            selectedList << index.row();
        }
    }
}
//==============================================================
void ZCalibrationTableWidget::zp_currentCalibrationIndex(int& row)
{
    QModelIndex index = zv_table->selectionModel()->currentIndex();
    if(index.isValid())
    {
        row = index.row();
    }
}
//==============================================================
void ZCalibrationTableWidget::zp_setCurrentCalibrationIndex(int calibrationIndex)
{
    if(!zv_table->model())
    {
        return;
    }

    QModelIndex previousIndex = zv_table->currentIndex();
    QModelIndex index = zv_table->model()->index(calibrationIndex, 0, QModelIndex());
    if(index.isValid())
    {
        zv_table->setCurrentIndex(index);
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

        emit zg_currentCalibrationChanged(currentRow, previousRow);
    }
}
//==============================================================
void ZCalibrationTableWidget::zp_startCurrentCalibrationEdition()
{
    QModelIndex currentIndex = zv_table->currentIndex();
    if(!currentIndex.isValid())
    {
        return;
    }

    zv_table->edit(currentIndex);
}
//==============================================================
void ZCalibrationTableWidget::zh_onCurrentCalibrationChange(const QModelIndex & current, const QModelIndex & previous)
{
    emit zg_currentCalibrationChanged(current.row(), previous.row());
}
//==============================================================
void ZCalibrationTableWidget::zh_onSelectedCalibrationChange(const QItemSelection & selected, const QItemSelection & deselected)
{
    QList<int> selectedList;
    zp_selectedCalibrationIndexList(selectedList);
    emit zg_selectedCalibrationChanged(selectedList);
}
//==============================================================
void ZCalibrationTableWidget::zh_onContextMenuRequest(const QPoint &pos)
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

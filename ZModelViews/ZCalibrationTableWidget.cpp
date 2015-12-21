//==============================================================
#include "ZCalibrationTableWidget.h"
#include "ZVisibilityStringDelegate.h"
#include "ZChemElementComboBoxDelegate.h"
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
}
//==============================================================
void ZCalibrationTableWidget::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    zv_table = new QTableView(this);
    mainLayout->addWidget(zv_table);

    zv_buttonLayout = new QHBoxLayout(this);
    mainLayout->addLayout(zv_buttonLayout);
}
//==============================================================
void zh_onCurrentArrayChanged(const QModelIndex & current, const QModelIndex & previous)
{

}
//==============================================================
void ZCalibrationTableWidget::zp_setModel(QAbstractItemModel* model)
{
    zv_table->setModel(model);
    zv_table->setItemDelegateForColumn(0, new ZVisibilityStringDelegate(zv_table));
    ZChemElementComboBoxDelegate* chemElementComboBoxDelegate = new ZChemElementComboBoxDelegate(zv_table);
    zv_table->setItemDelegateForColumn(1, chemElementComboBoxDelegate);

    //zv_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    // zv_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    //zv_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    zv_table->setAlternatingRowColors(true);

    connect(zv_table->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &ZCalibrationTableWidget::zh_onCurrentCalibrationChanged);
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
void ZCalibrationTableWidget::zh_onCurrentCalibrationChanged(const QModelIndex & current, const QModelIndex & previous)
{
    emit zg_currentCalibrationChanged(current.row(), previous.row());
}
//==============================================================

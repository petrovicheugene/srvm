//==============================================================
#include "ZJointCalibrationWindowTableWidget.h"
#include "ZCalibrationWindowModel.h"
#include "ZIntDelegate.h"
#include "ZVisibilityStringDelegate.h"
#include "ZWindowTypeComboBoxDelegate.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAction>
//==============================================================
ZJointCalibrationWindowTableWidget::ZJointCalibrationWindowTableWidget(QWidget *parent) : QWidget(parent)
{
    zv_channelDelegate = 0;
    zh_createComponents();
}
//==============================================================
void ZJointCalibrationWindowTableWidget::zp_setModel(ZCalibrationWindowModel* model)
{
    zv_table->setModel(model);
    zv_table->setItemDelegateForColumn(0, new ZVisibilityStringDelegate(zv_table));

    ZWindowTypeComboBoxDelegate* windowTypeComboBoxDelegate = new ZWindowTypeComboBoxDelegate(zv_table);
    zv_table->setItemDelegateForColumn(1, windowTypeComboBoxDelegate);

    zv_channelDelegate = new ZIntDelegate(zv_table);
    zp_setChannelNumberMinMax(0,zv_defaultChannelCount);
    zv_table->setItemDelegateForColumn(2, zv_channelDelegate);
    zv_table->setItemDelegateForColumn(3, zv_channelDelegate);

    zv_table->setAlternatingRowColors(true);
}
//==============================================================
void ZJointCalibrationWindowTableWidget::zp_appendButtonActions(QList<QAction*> actionList)
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
//=============================================================
void ZJointCalibrationWindowTableWidget::zp_selectedSpectrumWindowIndexList(QList<int>& selectedSpectrumList)
{
    QModelIndexList indexList = zv_table->selectionModel()->selectedIndexes();
    foreach(QModelIndex index, indexList)
    {
        if(!selectedSpectrumList.contains(index.row()))
        {
            selectedSpectrumList << index.row();
        }
    }
}
//==============================================================
void ZJointCalibrationWindowTableWidget::zp_setChannelNumberMinMax(int first, int last)
{
    if(!zv_channelDelegate)
    {
        return;
    }

    if(first > last)
    {
        qSwap(first, last);
    }

    if(first == last)
    {
        first = 0;
        last = zv_defaultChannelCount;
    }

    zv_channelDelegate->zp_setEditorMinMax(first, last);
}
//==============================================================
void ZJointCalibrationWindowTableWidget::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    zv_table = new QTableView(this);
    mainLayout->addWidget(zv_table);

    zv_buttonLayout = new QHBoxLayout(this);
    mainLayout->addLayout(zv_buttonLayout);

}
//==============================================================


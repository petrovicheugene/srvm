//==============================================================
#include "ZCalibrationWindowTableWidget.h"
#include "ZCalibrationWindowModel.h"
#include "ZChannelNumberDelegate.h"
#include "ZVisibilityStringDelegate.h"
#include "ZWindowTypeComboBoxDelegate.h"
#include "ZCalibrationRepository.h"
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAction>
//==============================================================
ZCalibrationWindowTableWidget::ZCalibrationWindowTableWidget(QWidget *parent) : QWidget(parent)
{
    zv_channelDelegate = 0;
    zh_createComponents();
}
//==============================================================
void ZCalibrationWindowTableWidget::zp_setModel(ZCalibrationWindowModel* model)
{
    zv_table->setModel(model);
    ZVisibilityStringDelegate* visibilityStringDelegate = new ZVisibilityStringDelegate(zv_table);
    //zv_table->viewport()->installEventFilter(visibilityStringDelegate);
    zv_table->setItemDelegateForColumn(0, visibilityStringDelegate);

    ZWindowTypeComboBoxDelegate* windowTypeComboBoxDelegate = new ZWindowTypeComboBoxDelegate(zv_table);
    zv_table->setItemDelegateForColumn(1, windowTypeComboBoxDelegate);

    zv_channelDelegate = new ZChannelNumberDelegate(zv_table);
    zp_setChannelNumberMinMax(0,zv_defaultChannelCount);
    zv_table->setItemDelegateForColumn(2, zv_channelDelegate);
    zv_table->setItemDelegateForColumn(3, zv_channelDelegate);

    zv_table->setAlternatingRowColors(true);
    connect(zv_table->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &ZCalibrationWindowTableWidget::zh_onCurrentCalibrationWindowChanged);

}
//==============================================================
void ZCalibrationWindowTableWidget::zp_appendButtonActions(QList<QAction*> actionList)
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
void ZCalibrationWindowTableWidget::zp_connectToCalibrationRepository(ZCalibrationRepository* repository)
{
   connect(this, &ZCalibrationWindowTableWidget::zg_currentCalibrationWindowChanged,
           repository, &ZCalibrationRepository::zp_onCurrentCalibrationWindowChanged);
   connect(repository, &ZCalibrationRepository::zg_requestSelectedWindowIndexList,
           this, &ZCalibrationWindowTableWidget::zp_selectedSpectrumWindowIndexList);

}
//=============================================================
void ZCalibrationWindowTableWidget::zp_setCurrentWindowIndex(int windowIndex)
{
    if(!zv_table->model())
    {
        return;
    }

    QModelIndex index = zv_table->model()->index(windowIndex, 0, QModelIndex());
    if(index.isValid())
    {
        zv_table->setCurrentIndex(index);
    }
}
//=============================================================
void ZCalibrationWindowTableWidget::zp_currentWindowIndex(int& windowIndex) const
{
    QModelIndex currentIndex = zv_table->currentIndex();
    if(!currentIndex.isValid())
    {
        windowIndex = -1;
        return;
    }

    windowIndex = currentIndex.row();
}
//=============================================================
void ZCalibrationWindowTableWidget::zp_startCurrentWindowEdition()
{
    QModelIndex currentIndex = zv_table->currentIndex();
    if(!currentIndex.isValid())
    {
        return;
    }

    zv_table->edit(currentIndex);
}
//=============================================================
void ZCalibrationWindowTableWidget::zp_selectedSpectrumWindowIndexList(QList<int>& selectedSpectrumList)
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
void ZCalibrationWindowTableWidget::zp_setChannelNumberMinMax(int first, int last)
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
void ZCalibrationWindowTableWidget::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    zv_table = new QTableView(this);
    mainLayout->addWidget(zv_table);

    zv_buttonLayout = new QHBoxLayout(this);
    mainLayout->addLayout(zv_buttonLayout);

}
//==============================================================
void ZCalibrationWindowTableWidget::zh_onCurrentCalibrationWindowChanged(const QModelIndex & current,
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

   emit zg_currentCalibrationWindowChanged(currentRow, previousRow);
}
//==============================================================

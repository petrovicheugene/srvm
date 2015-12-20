//=============================================================
#include "ZJointSpectraTableWidget.h"
#include "globalVariables.h"
#include "ZNumericDelegate.h"
#include "ZSpectrumTableDelegate.h"
#include "ZVisibilityStringDelegate.h"
#include "ZJointSpectraModel.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAction>
//=============================================================
ZJointSpectrumTableWidget::ZJointSpectrumTableWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
}
//=============================================================
void ZJointSpectrumTableWidget::zp_selectedSpectrumIndexList(QList<int>& selectedSpectrumList)
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
//=============================================================
void ZJointSpectrumTableWidget::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    zv_table = new QTableView(this);
    mainLayout->addWidget(zv_table, INT_MAX);

    zv_buttonLayout = new QHBoxLayout(this);
    mainLayout->addLayout(zv_buttonLayout);
}
//=============================================================
void ZJointSpectrumTableWidget::zh_createConnections()
{

}
//=============================================================
void ZJointSpectrumTableWidget::zp_setModel(ZJointSpectraModel* model)
{
    zv_table->setModel(model);
    ZNumericDelegate* numericDelegate = new ZNumericDelegate(zv_table);
    connect(numericDelegate, &ZNumericDelegate::editNext,
            this, &ZJointSpectrumTableWidget::zp_editNext);
    zv_table->setItemDelegate(numericDelegate);
    ZSpectrumTableDelegate* spectrumDelegate = new ZSpectrumTableDelegate(zv_table);
    zv_table->setItemDelegateForColumn(0, new ZVisibilityStringDelegate(zv_table));
    zv_table->setItemDelegateForColumn(1, spectrumDelegate);
    zv_table->setAlternatingRowColors(true);
}
//==============================================================
void ZJointSpectrumTableWidget::zp_appendButtonActions(QList<QAction*> actionList)
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
void ZJointSpectrumTableWidget::zp_editNext(QModelIndex editedIndex)
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
//==============================================================

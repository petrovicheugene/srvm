//==============================================================
#include "ZSpectrumArrayWidget.h"
#include "globalVariables.h"
#include "ZArrayModel.h"
#include "ZStringDelegate.h"
#include "ZVisibilityStringDelegate.h"
#include "ZSpectrumArrayRepository.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAbstractItemModel>
#include <QHeaderView>
#include <QAction>
#include <QPushButton>
//==============================================================
ZSpectrumArrayWidget::ZSpectrumArrayWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
}
//==============================================================
void ZSpectrumArrayWidget::zh_createComponents()
{
    zv_mainLayout = new QVBoxLayout(this);
    zv_mainLayout->setMargin(0);
    setLayout(zv_mainLayout);

    zv_table = new QTableView(this);
    zv_mainLayout->addWidget(zv_table, INT_MAX);

    zv_buttonLayout = new QHBoxLayout(this);
    zv_mainLayout->addLayout(zv_buttonLayout);
}
//==============================================================
void ZSpectrumArrayWidget::zp_setModel(ZArrayModel *model)
{
    zv_table->setModel(model);
    zv_table->setItemDelegate(new ZStringDelegate(zv_table));

    zv_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    zv_table->setAlternatingRowColors(true);
    connect(zv_table->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &ZSpectrumArrayWidget::zh_onCurrentArrayChanged);
    connect(model, &ZArrayModel::zg_checkCurrentArray,
            this, &ZSpectrumArrayWidget::zh_checkCurrentArray);
    zv_table->setSelectionMode(QAbstractItemView::SingleSelection);

}
//==============================================================
void ZSpectrumArrayWidget::zp_appendButtonActions(QList<QAction*> actionList)
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
void ZSpectrumArrayWidget::zp_connectToSpectrumArrayRepository(ZSpectrumArrayRepository* repository)
{
    zp_appendButtonActions(repository->zp_arrayActions());

    connect(repository, &ZSpectrumArrayRepository::zg_setCurrentArrayIndex,
            this, &ZSpectrumArrayWidget::zp_setCurrentArrayIndex);
    connect(repository, &ZSpectrumArrayRepository::zg_startCurrentArrayEdition,
            this, &ZSpectrumArrayWidget::zp_startCurrentArrayEdition);
    connect(repository, &ZSpectrumArrayRepository::zg_requestCurrentArrayIndex,
            this, &ZSpectrumArrayWidget::zp_currentArrayIndex);
    connect(this, &ZSpectrumArrayWidget::zg_currentArrayChanged,
            repository, &ZSpectrumArrayRepository::zp_currentArrayChanged);


}
//==============================================================
void ZSpectrumArrayWidget::zp_setMargin(int margin)
{
    zv_mainLayout->setMargin(margin);
}
//==============================================================
void ZSpectrumArrayWidget::zp_setCurrentArrayIndex(int arrayIndex)
{
    if(!zv_table->model())
    {
        return;
    }

    QModelIndex previousIndex = zv_table->currentIndex();
    QModelIndex index = zv_table->model()->index(arrayIndex, 0, QModelIndex());
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

        emit zg_currentArrayChanged(currentRow, previousRow);
    }
}
//==============================================================
void ZSpectrumArrayWidget::zp_currentArrayIndex(int& arrayIndex)
{
    QModelIndex currentIndex = zv_table->currentIndex();
    if(!currentIndex.isValid())
    {
        arrayIndex = -1;
        return;
    }

    arrayIndex = currentIndex.row();
}
//==============================================================
void ZSpectrumArrayWidget::zp_startCurrentArrayEdition()
{
    QModelIndex currentIndex = zv_table->currentIndex();
    if(!currentIndex.isValid())
    {
        return;
    }

    zv_table->edit(currentIndex);
}
//==============================================================
void ZSpectrumArrayWidget::zh_createConnections()
{

}
//==============================================================
void ZSpectrumArrayWidget::zh_checkCurrentArray()
{
    if(!zv_table->selectionModel())
    {
        return;
    }

    QModelIndex currentIndex = zv_table->selectionModel()->currentIndex();
    if(!currentIndex.isValid() || currentIndex.row() < 0
            || currentIndex.row() >= zv_table->model()->rowCount())
    {
        QModelIndex indexToGo = zv_table->model()->index(0,0);
        zv_table->setCurrentIndex(indexToGo);
    }
}
//==============================================================
void ZSpectrumArrayWidget::zh_onCurrentArrayChanged(const QModelIndex & current,
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

    emit zg_currentArrayChanged(currentRow, previousRow);
}
//==============================================================

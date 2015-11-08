//==============================================================
#include "ZSpectrumArrayWidget.h"
#include "glVariables.h"
#include "ZArrayModel.h"
#include "ZStringDelegate.h"

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
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    zv_table = new QTableView(this);
    mainLayout->addWidget(zv_table, INT_MAX);

    zv_buttonLayout = new QHBoxLayout(this);
    mainLayout->addLayout(zv_buttonLayout);
}
//==============================================================
void ZSpectrumArrayWidget::zp_setModel(ZArrayModel *model)
{
    zv_table->setModel(model);
    zv_table->setItemDelegate(new ZStringDelegate(false, zv_table));
    zv_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    connect(zv_table->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &ZSpectrumArrayWidget::zh_onCurrentArrayChanged);
    connect(model, &ZArrayModel::zg_checkCurrentArray,
            this, &ZSpectrumArrayWidget::zh_checkCurrentArray);

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
void ZSpectrumArrayWidget::zp_setCurrentArrayIndex(int arrayIndex)
{
    if(!zv_table->model())
    {
        return;
    }

    QModelIndex index = zv_table->model()->index(arrayIndex, 0, QModelIndex());
    if(index.isValid())
    {
        zv_table->setCurrentIndex(index);
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
void ZSpectrumArrayWidget::zh_onCurrentArrayChanged(const QModelIndex & current, const QModelIndex & previous)
{
    emit zg_currentArrayChanged(current.row(), previous.row());
}
//==============================================================

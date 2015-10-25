//=============================================================
#include "ZJointSpectraTableWidget.h"
#include "glVariables.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAbstractItemModel>
#include <QPushButton>
#include <QAction>
//=============================================================
ZSpectrumTableWidget::ZSpectrumTableWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
}
//=============================================================
void ZSpectrumTableWidget::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    zv_table = new QTableView(this);
    mainLayout->addWidget(zv_table, INT_MAX);

    zv_buttonLayout = new QHBoxLayout(this);
    mainLayout->addLayout(zv_buttonLayout);
}
//=============================================================
void ZSpectrumTableWidget::zp_setModel(QAbstractItemModel* model)
{
    zv_table->setModel(model);
}
//==============================================================
void ZSpectrumTableWidget::zp_appendButtonActions(QList<QAction*> actionList)
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

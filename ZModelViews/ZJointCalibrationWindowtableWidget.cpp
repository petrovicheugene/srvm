//==============================================================
#include "ZJointCalibrationWindowTableWidget.h"
#include "ZJointCalibrationWindowModel.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAction>
//==============================================================
ZJointCalibrationWindowTableWidget::ZJointCalibrationWindowTableWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
}
//==============================================================
void ZJointCalibrationWindowTableWidget::zp_setModel(ZJointCalibrationWindowModel* model)
{
    zv_table->setModel(model);
//    ZNumericDelegate* numericDelegate = new ZNumericDelegate(zv_table);
//    connect(numericDelegate, &ZNumericDelegate::editNext,
//            this, &ZJointSpectrumTableWidget::zp_editNext);
//    zv_table->setItemDelegate(numericDelegate);
//    ZSpectrumTableDelegate* spectrumDelegate = new ZSpectrumTableDelegate(zv_table);
//    zv_table->setItemDelegateForColumn(0, new ZVisibilityStringDelegate(zv_table));
//    zv_table->setItemDelegateForColumn(1, spectrumDelegate);
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


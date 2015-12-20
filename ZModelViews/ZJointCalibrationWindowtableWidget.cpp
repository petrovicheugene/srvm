//==============================================================
#include "ZJointCalibrationWindowtableWidget.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
//==============================================================
ZJointCalibrationWindowtableWidget::ZJointCalibrationWindowtableWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
}
//==============================================================
void ZJointCalibrationWindowtableWidget::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    zv_table = new QTableView(this);
    mainLayout->addWidget(zv_table);

    zv_buttonLayout = new QHBoxLayout(this);
    mainLayout->addLayout(zv_buttonLayout);

}
//==============================================================


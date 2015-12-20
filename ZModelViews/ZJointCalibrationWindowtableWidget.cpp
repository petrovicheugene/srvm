//==============================================================
#include "ZJointCalibrationWindowTableWidget.h"
#include "ZJointCalibrationWindowModel.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
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


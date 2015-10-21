//==============================================================
#include "ZCalibrationTableWidget.h"
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAbstractItemModel>

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

}
//==============================================================
void ZCalibrationTableWidget::zp_setModel(QAbstractItemModel* model)
{

}
//==============================================================

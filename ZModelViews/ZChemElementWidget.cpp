//==============================================================
#include "ZCalibrationArrayWidget.h"
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
//==============================================================
ZCalibrationArrayWidget::ZCalibrationArrayWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
}
//==============================================================
void ZCalibrationArrayWidget::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    zv_table = new QTableView(this);
    mainLayout->addWidget(zv_table);

}
//==============================================================

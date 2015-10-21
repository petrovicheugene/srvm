//==============================================================
#include "ZSpectrumArrayWidget.h"
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAbstractItemModel>

//==============================================================
ZSpectrumArrayWidget::ZSpectrumArrayWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
}
//==============================================================
void ZSpectrumArrayWidget::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    zv_table = new QTableView(this);
    mainLayout->addWidget(zv_table);

}
//==============================================================
void ZSpectrumArrayWidget::zp_setModel(QAbstractItemModel* model)
{

}
//==============================================================

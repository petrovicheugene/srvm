//==============================================================
#include "ZChemElementWidget.h"
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAbstractItemModel>

//==============================================================
ZChemElementWidget::ZChemElementWidget(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
}
//==============================================================
void ZChemElementWidget::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    zv_table = new QTableView(this);
    mainLayout->addWidget(zv_table);

}
//==============================================================
void ZChemElementWidget::zp_setModel(QAbstractItemModel* model)
{

}
//==============================================================

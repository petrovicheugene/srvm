//=============================================================
#include "ZSpectrumTableWidget.h"
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>

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
    mainLayout->addWidget(zv_table);

}
//=============================================================

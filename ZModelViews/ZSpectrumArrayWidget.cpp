//==============================================================
#include "ZSpectrumArrayWidget.h"
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAbstractItemModel>
#include <QHeaderView>
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
    mainLayout->addWidget(zv_table);

}
//==============================================================
void ZSpectrumArrayWidget::zp_setModel(QAbstractItemModel* model)
{
    zv_table->setModel(model);
    zv_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    connect(zv_table->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &ZSpectrumArrayWidget::zh_onCurrentArrayChanged);
}
//==============================================================
void ZSpectrumArrayWidget::zh_createConnections()
{

}
//==============================================================
void ZSpectrumArrayWidget::zh_onCurrentArrayChanged(const QModelIndex & current, const QModelIndex & previous)
{
    emit zg_currentArrayChenged(current.row(), previous.row());
}
//==============================================================

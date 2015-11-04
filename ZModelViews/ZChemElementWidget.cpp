//==============================================================
#include "ZChemElementWidget.h"
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAbstractItemModel>
#include <QHeaderView>
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
    zv_table->setModel(model);
    zv_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    connect(zv_table->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &ZChemElementWidget::zh_onCurrentElementChanged);

}
//==============================================================
void ZChemElementWidget::zh_onCurrentElementChanged(const QModelIndex & current,
                                                    const QModelIndex & previous)
{
    emit zg_currentCalibrationChanged(current.row(), previous.row());
}
//==============================================================

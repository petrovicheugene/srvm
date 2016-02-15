//=============================================================
#include "ZTermCorrelationTableWidget.h"
#include "ZTermCorrelationTableModel.h"
#include "ZCustomCheckableVerticalHeaderView.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAction>
//=============================================================
ZTermCorrelationTableWidget::ZTermCorrelationTableWidget(QWidget *parent) : QWidget(parent)
{
   zh_createComponents();
   zh_createConnections();
}
//=============================================================
void ZTermCorrelationTableWidget::zp_setModel(ZTermCorrelationTableModel* model)
{
    zv_table->setModel(model);

    connect(this, &ZTermCorrelationTableWidget::zg_userChangesTermState,
            model, &ZTermCorrelationTableModel::zp_onUserChangesTermState);
//    ZNumericDelegate* numericDelegate = new ZNumericDelegate(zv_table);
//    connect(numericDelegate, &ZNumericDelegate::editNext,
//            this, &ZJointSpectrumTableWidget::zh_editNext);
//    zv_table->setItemDelegate(numericDelegate);
//    ZSpectrumTableDelegate* spectrumDelegate = new ZSpectrumTableDelegate(zv_table);
//    zv_table->setItemDelegateForColumn(0, new ZVisibilityStringDelegate(zv_table));
//    zv_table->setItemDelegateForColumn(1, spectrumDelegate);
//    zv_table->setAlternatingRowColors(true);

//    connect(zv_table->selectionModel(), &QItemSelectionModel::currentRowChanged,
//            this, &ZJointSpectrumTableWidget::zh_onCurrentSpectrumChanged);
}
//=============================================================
void ZTermCorrelationTableWidget::zp_appendButtonActions(QList<QAction*> actionList)
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
//=============================================================
void ZTermCorrelationTableWidget::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    zv_table = new QTableView(this);

    // Vertical Header Replacing
    ZCustomCheckableVerticalHeaderView* checkableHeader = new ZCustomCheckableVerticalHeaderView();
    connect(checkableHeader, &ZCustomCheckableVerticalHeaderView::zg_userChangesTermState,
            this, &ZTermCorrelationTableWidget::zg_userChangesTermState);
    zv_table->setVerticalHeader(checkableHeader);

    mainLayout->addWidget(zv_table, INT_MAX);

    zv_buttonLayout = new QHBoxLayout(this);
    mainLayout->addLayout(zv_buttonLayout);
}
//=============================================================
void ZTermCorrelationTableWidget::zh_createConnections()
{

}
//=============================================================


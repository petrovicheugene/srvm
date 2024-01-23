//==============================================================
#include "X_CalibrationWindowTableWidget.h"
#include "X_CalibrationRepository.h"
#include "X_CalibrationWindowModel.h"
#include "X_ChannelNumberDelegate.h"
#include "X_VisibilityStringDelegate.h"
#include "X_WindowTypeComboBoxDelegate.h"

#include <QAction>
#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>
//==============================================================
X_CalibrationWindowTableWidget::X_CalibrationWindowTableWidget(QWidget* parent) : QWidget(parent)
{
    xv_channelDelegate = 0;
    xh_createComponents();
    xh_createConnections();
}
//==============================================================
void X_CalibrationWindowTableWidget::xp_setModel(X_CalibrationWindowModel* model)
{
    xv_table->setModel(model);
    X_VisibilityStringDelegate* visibilityStringDelegate = new X_VisibilityStringDelegate(xv_table);
    xv_table->viewport()->installEventFilter(visibilityStringDelegate);
    xv_table->setItemDelegateForColumn(0, visibilityStringDelegate);

    X_WindowTypeComboBoxDelegate* windowTypeComboBoxDelegate = new X_WindowTypeComboBoxDelegate(
        xv_table);
    xv_table->setItemDelegateForColumn(1, windowTypeComboBoxDelegate);

    xv_channelDelegate = new X_ChannelNumberDelegate(xv_table);
    xp_setChannelNumberMinMax(0, xv_defaultChannelCount);
    xv_table->setItemDelegateForColumn(3, xv_channelDelegate);
    xv_table->setItemDelegateForColumn(4, xv_channelDelegate);

    xv_table->setAlternatingRowColors(true);
    connect(xv_table->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            this,
            &X_CalibrationWindowTableWidget::xh_onCurrentCalibrationWindowChanged);
    connect(xv_table->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &X_CalibrationWindowTableWidget::xh_onCalibrationWindowSelectionChanged);

    connect(xv_table,
            &QTableView::clicked,
            this,
            &X_CalibrationWindowTableWidget::xh_onCalibrationWindowClick);
}
//==============================================================
void X_CalibrationWindowTableWidget::xp_appendButtonActions(QList<QAction*> actionList)
{
    xv_buttonLayout->addStretch();
    for (int a = 0; a < actionList.count(); a++)
    {
        QPushButton* button = new QPushButton(this);
        button->setFlat(true);
        button->setIcon(actionList.at(a)->icon());
        button->setToolTip(actionList.at(a)->toolTip());
        connect(button, &QPushButton::clicked, actionList[a], &QAction::trigger);
        xv_buttonLayout->addWidget(button);
    }
}
//=============================================================
void X_CalibrationWindowTableWidget::xp_appendContextMenuActions(QList<QAction*> actionList)
{
    foreach (QAction* action, actionList)
    {
        if (action != 0 && xv_contextMenuActionList.contains(action))
        {
            continue;
        }
        xv_contextMenuActionList.append(action);
    }
}
//=============================================================
void X_CalibrationWindowTableWidget::xp_connectToCalibrationRepository(
    X_CalibrationRepository* repository)
{
    this->xp_appendButtonActions(repository->xp_windowActions());
    this->xp_appendContextMenuActions(repository->xp_windowContextMenuActions());

    connect(this,
            &X_CalibrationWindowTableWidget::xg_currentCalibrationWindowChanged,
            repository,
            &X_CalibrationRepository::xp_onCurrentCalibrationWindowChange);
    connect(this,
            &X_CalibrationWindowTableWidget::xg_calibrationWindowSelectionChanged,
            repository,
            &X_CalibrationRepository::xp_onCalibrationWindowSelectionChange);
    connect(repository,
            &X_CalibrationRepository::xg_requestSelectedWindowIndexList,
            this,
            &X_CalibrationWindowTableWidget::xp_selectedSpectrumWindowIndexList);
    connect(this,
            &X_CalibrationWindowTableWidget::xg_calibrationWindowClicked,
            repository,
            &X_CalibrationRepository::xp_onCalibrationWindowClick);
}
//=============================================================
void X_CalibrationWindowTableWidget::xp_setMainLayoutMargin(int margin)
{
    xv_mainLayout->setContentsMargins(margin, margin, margin, margin);
}
//=============================================================
void X_CalibrationWindowTableWidget::xp_setCurrentWindowIndex(int windowIndex)
{
    if (!xv_table->model())
    {
        return;
    }

    QModelIndex index = xv_table->model()->index(windowIndex, 0, QModelIndex());
    if (index.isValid())
    {
        xv_table->setCurrentIndex(index);
    }
}
//=============================================================
void X_CalibrationWindowTableWidget::xp_currentWindowIndex(int& windowIndex) const
{
    QModelIndex currentIndex = xv_table->currentIndex();
    if (!currentIndex.isValid())
    {
        windowIndex = -1;
        return;
    }

    windowIndex = currentIndex.row();
}
//=============================================================
void X_CalibrationWindowTableWidget::xp_startCurrentWindowEdition()
{
    QModelIndex currentIndex = xv_table->currentIndex();
    if (!currentIndex.isValid())
    {
        return;
    }

    xv_table->edit(currentIndex);
}
//=============================================================
void X_CalibrationWindowTableWidget::xp_selectedSpectrumWindowIndexList(
    QList<int>& selectedSpectrumList)
{
    QModelIndexList indexList = xv_table->selectionModel()->selectedIndexes();
    foreach (QModelIndex index, indexList)
    {
        if (!selectedSpectrumList.contains(index.row()))
        {
            selectedSpectrumList << index.row();
        }
    }
}
//==============================================================
void X_CalibrationWindowTableWidget::xp_setChannelNumberMinMax(int first, int last)
{
    if (!xv_channelDelegate)
    {
        return;
    }

    if (first > last)
    {
        qSwap(first, last);
    }

    if (first == last)
    {
        first = 0;
        last = xv_defaultChannelCount;
    }

    xv_channelDelegate->xp_setEditorMinMax(first, last);
}
//==============================================================
void X_CalibrationWindowTableWidget::xh_createComponents()
{
    xv_mainLayout = new QVBoxLayout;
    xv_mainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(xv_mainLayout);

    xv_table = new QTableView(this);
    xv_table->setContextMenuPolicy(Qt::CustomContextMenu);

    xv_mainLayout->addWidget(xv_table);

    xv_buttonLayout = new QHBoxLayout;
    xv_mainLayout->addLayout(xv_buttonLayout);
}
//==============================================================
void X_CalibrationWindowTableWidget::xh_createConnections()
{
    connect(xv_table,
            &QTableView::customContextMenuRequested,
            this,
            &X_CalibrationWindowTableWidget::xh_onContextMenuRequest);
}
//==============================================================
void X_CalibrationWindowTableWidget::xh_onCurrentCalibrationWindowChanged(const QModelIndex& current,
                                                                         const QModelIndex& previous)
{
    int currentRow;
    if (current.isValid())
    {
        currentRow = current.row();
    }
    else
    {
        currentRow = -1;
    }
    int previousRow;
    if (previous.isValid())
    {
        previousRow = previous.row();
    }
    else
    {
        previousRow = -1;
    }

    emit xg_currentCalibrationWindowChanged(currentRow, previousRow);
}
//==============================================================
void X_CalibrationWindowTableWidget::xh_onCalibrationWindowSelectionChanged(
    const QItemSelection& selected, const QItemSelection& deselected)
{
    emit xg_calibrationWindowSelectionChanged();
}
//==============================================================
void X_CalibrationWindowTableWidget::xh_onCalibrationWindowClick(const QModelIndex& index) const
{

    emit xg_calibrationWindowClicked(index);

}
//==============================================================
void X_CalibrationWindowTableWidget::xh_onContextMenuRequest(const QPoint& pos)
{
    QMenu* menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    foreach (QAction* action, xv_contextMenuActionList)
    {
        if (action == 0)
        {
            menu->addSeparator();
            continue;
        }

        menu->addAction(action);
    }

    menu->popup(xv_table->viewport()->mapToGlobal(pos));
}
//==============================================================

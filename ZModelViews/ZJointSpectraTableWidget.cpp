//=============================================================
#include "ZJointSpectraTableWidget.h"
#include "globalVariables.h"
#include "ZNumericDelegate.h"
#include "ZSpectrumTableDelegate.h"
#include "ZActiveStyledItemDelegate.h"
#include "ZJointSpectraModel.h"
#include "ZSpectrumArrayRepository.h"
#include "ZCalibrationRepository.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAction>
//=============================================================
ZJointSpectrumTableWidget::ZJointSpectrumTableWidget(QWidget *parent) : QWidget(parent)
{
    //zh_createActions();
    zh_createComponents();
    zh_createConnections();
}
//=============================================================
void ZJointSpectrumTableWidget::zp_setCurrentSpectrumIndex(int spectrumIndex)
{
    if(!zv_table->model())
    {
        return;
    }

    QModelIndex previousIndex = zv_table->currentIndex();

    int column = previousIndex.isValid() ? previousIndex.column() : 0;
    QModelIndex index = zv_table->model()->index(spectrumIndex, column, QModelIndex());
    if(index.isValid())
    {
        zv_table->setCurrentIndex(index);
        int currentRow;
        if(index.isValid())
        {
            currentRow = index.row();
        }
        else
        {
            currentRow = -1;
        }
        int previousRow;
        if(previousIndex.isValid())
        {
            previousRow = previousIndex.row();
        }
        else
        {
            previousRow = -1;
        }

        emit zg_currentSpectrumChanged(currentRow, previousRow);
    }
}
//=============================================================
void ZJointSpectrumTableWidget::zp_selectedSpectrumIndexList(QList<int>& selectedSpectrumList)
{
    QModelIndexList indexList = zv_table->selectionModel()->selectedIndexes();
    foreach(QModelIndex index, indexList)
    {
        if(!selectedSpectrumList.contains(index.row()))
        {
            selectedSpectrumList << index.row();
        }
    }
}
//=============================================================
void ZJointSpectrumTableWidget::zp_currentIndex(QModelIndex& index) const
{
    if(!zv_table)
    {
        index = QModelIndex();
        return;
    }

    index = zv_table->currentIndex();
}
//=============================================================
void ZJointSpectrumTableWidget::zh_createComponents()
{
    zv_mainLayout = new QVBoxLayout(this);
    zv_mainLayout->setMargin(0);

    setLayout(zv_mainLayout);

    zv_table = new QTableView(this);
    zv_table->setContextMenuPolicy(Qt::CustomContextMenu);

    zv_mainLayout->addWidget(zv_table, INT_MAX);

    zv_buttonLayout = new QHBoxLayout(this);
    zv_mainLayout->addLayout(zv_buttonLayout);

}
//=============================================================
void ZJointSpectrumTableWidget::zh_createConnections()
{
    connect(zv_table, &QTableView::customContextMenuRequested,
            this, &ZJointSpectrumTableWidget::zh_onContextMenuRequest);
}
//=============================================================
void ZJointSpectrumTableWidget::zp_setModel(ZJointSpectraModel* model)
{
    zv_table->setModel(model);
    ZNumericDelegate* numericDelegate = new ZNumericDelegate(zv_table);
    double min;
    double max;
    numericDelegate->zp_editorMinMax(min, max);
    min = 0.0;
    numericDelegate->zp_setEditorMinMax(min, max);
    connect(numericDelegate, &ZNumericDelegate::zg_editNext,
            this, &ZJointSpectrumTableWidget::zh_editNext);
    zv_table->setItemDelegate(numericDelegate);
    ZSpectrumTableDelegate* spectrumDelegate = new ZSpectrumTableDelegate(zv_table);
    // tracking mouse event to prevent current event setting when visible changed
    zv_table->viewport()->installEventFilter(spectrumDelegate);
    zv_table->setItemDelegateForColumn(0, new ZActiveStyledItemDelegate);
    zv_table->setItemDelegateForColumn(1, spectrumDelegate);
    zv_table->setAlternatingRowColors(true);

    connect(zv_table->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &ZJointSpectrumTableWidget::zh_onCurrentSpectrumChanged);
    connect(zv_table->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &ZJointSpectrumTableWidget::zh_onSelectionChange);
    connect(model, &ZJointSpectraModel::zg_requestCurrentIndex,
            this, &ZJointSpectrumTableWidget::zp_currentIndex);
    connect(this, &ZJointSpectrumTableWidget::zg_selectedIndexListChanged,
            model, &ZJointSpectraModel::zp_onSelectedIndexListChange);

}
//==============================================================
void ZJointSpectrumTableWidget::zp_appendButtonActions(QList<QAction*> actionList)
{
    zv_buttonLayout->addStretch();
    // zv_table->addActions(actionList);

    for(int a = 0; a < actionList.count(); a++)
    {
        if(actionList.at(a) == 0)
        {
            // Separator for context menu
            continue;
        }
        QPushButton* button = new QPushButton(this);
        button->setFlat(true);
        button->setIcon(actionList.at(a)->icon());
        button->setToolTip(actionList.at(a)->toolTip());
        connect(button, &QPushButton::clicked,
                actionList[a], &QAction::trigger);
        zv_buttonLayout->addWidget(button, 0, Qt::AlignRight);
    }
}
//==============================================================
void ZJointSpectrumTableWidget::zp_appendContextMenuActions(QList<QAction*> actionList)
{
    foreach(QAction* action, actionList)
    {
        if(action != 0 && zv_contextMenuActionList.contains(action))
        {
            continue;
        }
        zv_contextMenuActionList.append(action);
    }
}
//==============================================================
void ZJointSpectrumTableWidget::zp_connectToSpectrumArrayRepository(ZSpectrumArrayRepository* repository)
{
    zp_appendButtonActions(repository->zp_spectrumActions());
    zp_appendContextMenuActions(repository->zp_spectrumContextMenuActions());

    connect(this, &ZJointSpectrumTableWidget::zg_currentSpectrumChanged,
            repository, &ZSpectrumArrayRepository::zp_currentSpectrumChanged);
    connect(repository, &ZSpectrumArrayRepository::zg_setCurrentSpectrumIndex,
            this, &ZJointSpectrumTableWidget::zp_setCurrentSpectrumIndex);

}
//==============================================================
void ZJointSpectrumTableWidget::zp_connectToCalibrationRepository(ZCalibrationRepository* repository)
{
    QList<QAction*> separatorList;
    separatorList << 0;
    zp_appendContextMenuActions(separatorList);
    zp_appendContextMenuActions(repository->zp_calibrationVisibilityActions());
}
//==============================================================
void ZJointSpectrumTableWidget::zp_setMargin(int margin)
{
    zv_mainLayout->setMargin(margin);
}
//==============================================================
void ZJointSpectrumTableWidget::zh_editNext(QModelIndex editedIndex)
{
    if(!editedIndex.isValid() || editedIndex.row() < 0)
    {
        return;
    }

    if(editedIndex.row()+1 >= editedIndex.model()->rowCount())
    {
        // end of column - quit editing keeping current index as "current"
        zv_table->setCurrentIndex(editedIndex);
        return;
    }

    // defining next index
    QModelIndex nextIndex = editedIndex.model()->index(editedIndex.row()+1, editedIndex.column());
    if(!nextIndex.isValid())
    {
        return;
    }

    zv_table->setCurrentIndex(nextIndex);
    zv_table->edit(nextIndex);
}
//==============================================================
void ZJointSpectrumTableWidget::zh_onCurrentSpectrumChanged(const QModelIndex & current,
                                                            const QModelIndex & previous)
{
    int currentRow;
    if(current.isValid())
    {
        currentRow = current.row();
    }
    else
    {
        currentRow = -1;
    }
    int previousRow;
    if(previous.isValid())
    {
        previousRow = previous.row();
    }
    else
    {
        previousRow = -1;
    }

    emit zg_currentSpectrumChanged(currentRow, previousRow);
}
//==============================================================
void ZJointSpectrumTableWidget::zh_onContextMenuRequest(const QPoint &pos)
{
    QMenu *menu=new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    foreach(QAction* action, zv_contextMenuActionList)
    {
        if(action == 0)
        {
            menu->addSeparator();
            continue;
        }

        menu->addAction(action);
    }

    menu->popup(zv_table->viewport()->mapToGlobal(pos));
}
//==============================================================
void ZJointSpectrumTableWidget::zh_onSelectionChange(const QItemSelection & selected,
                                                     const QItemSelection & deselected) const
{
    QModelIndexList selectedIndexList = zv_table->selectionModel()->selectedIndexes();
    emit zg_selectedIndexListChanged(selectedIndexList);
}
//==============================================================

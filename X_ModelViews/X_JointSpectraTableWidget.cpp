//=============================================================
#include "X_JointSpectraTableWidget.h"
#include "X_General.h"
#include "X_NumericDelegate.h"
#include "X_SpectrumTableDelegate.h"
#include "X_ActiveStyledItemDelegate.h"
#include "X_JointSpectraModel.h"
#include "X_SpectrumArrayRepository.h"
#include "X_CalibrationRepository.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAction>
//=============================================================
X_JointSpectrumTableWidget::X_JointSpectrumTableWidget(QWidget *parent) : QWidget(parent)
{
    //zh_createActions();
    zh_createComponents();
    zh_createConnections();
}
//=============================================================
void X_JointSpectrumTableWidget::xp_setCurrentSpectrumIndex(int spectrumIndex)
{
    if(!xv_table->model())
    {
        return;
    }

    QModelIndex previousIndex = xv_table->currentIndex();

    int column = previousIndex.isValid() ? previousIndex.column() : 0;
    QModelIndex index = xv_table->model()->index(spectrumIndex, column, QModelIndex());
    if(index.isValid())
    {
        xv_table->setCurrentIndex(index);
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

        emit xg_currentSpectrumChanged(currentRow, previousRow);
    }
}
//=============================================================
void X_JointSpectrumTableWidget::xp_selectedSpectrumIndexList(QList<int>& selectedSpectrumList)
{
    QModelIndexList indexList = xv_table->selectionModel()->selectedIndexes();
    foreach(QModelIndex index, indexList)
    {
        if(!selectedSpectrumList.contains(index.row()))
        {
            selectedSpectrumList << index.row();
        }
    }
}
//=============================================================
void X_JointSpectrumTableWidget::xp_currentIndex(QModelIndex& index) const
{
    if(!xv_table)
    {
        index = QModelIndex();
        return;
    }

    index = xv_table->currentIndex();
}
//=============================================================
void X_JointSpectrumTableWidget::xp_currentSpectrumRow(int& row, bool* ok) const
{
    QModelIndex currentIndex;
    xp_currentIndex(currentIndex);

    if(ok)
    {
        *ok = currentIndex.isValid();
    }

    row = currentIndex.row();
}
//=============================================================
void X_JointSpectrumTableWidget::zh_createComponents()
{
    xv_mainLayout = new QVBoxLayout;
    xv_mainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(xv_mainLayout);

    xv_table = new QTableView(this);
    xv_table->setContextMenuPolicy(Qt::CustomContextMenu);

    xv_mainLayout->addWidget(xv_table, INT_MAX);

    xv_buttonLayout = new QHBoxLayout;
    xv_mainLayout->addLayout(xv_buttonLayout);

}
//=============================================================
void X_JointSpectrumTableWidget::zh_createConnections()
{
    connect(xv_table, &QTableView::customContextMenuRequested,
            this, &X_JointSpectrumTableWidget::zh_onContextMenuRequest);
}
//=============================================================
void X_JointSpectrumTableWidget::xp_setModel(X_JointSpectraModel* model)
{
    xv_table->setModel(model);
    X_NumericDelegate* numericDelegate = new X_NumericDelegate(xv_table);
    double min;
    double max;
    numericDelegate->xp_editorMinMax(min, max);
    min = 0.0;
    numericDelegate->xp_setEditorMinMax(min, max);
    connect(numericDelegate, &X_NumericDelegate::xg_editNext,
            this, &X_JointSpectrumTableWidget::zh_editNext);
    xv_table->setItemDelegate(numericDelegate);
    X_SpectrumTableDelegate* spectrumDelegate = new X_SpectrumTableDelegate(xv_table);
    // tracking mouse event to prevent current event setting when visible changed
    xv_table->viewport()->installEventFilter(spectrumDelegate);
    xv_table->setItemDelegateForColumn(0, new X_ActiveStyledItemDelegate);
    xv_table->setItemDelegateForColumn(1, spectrumDelegate);
    xv_table->setAlternatingRowColors(true);

    connect(xv_table->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &X_JointSpectrumTableWidget::zh_onCurrentSpectrumChanged);
    connect(xv_table->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &X_JointSpectrumTableWidget::zh_onSelectionChange);
    connect(model, &X_JointSpectraModel::xg_requestCurrentIndex,
            this, &X_JointSpectrumTableWidget::xp_currentIndex);
    connect(this, &X_JointSpectrumTableWidget::xg_selectedIndexListChanged,
            model, &X_JointSpectraModel::xp_onSelectedIndexListChange);

}
//==============================================================
void X_JointSpectrumTableWidget::xp_appendButtonActions(QList<QAction*> actionList)
{
    xv_buttonLayout->addStretch();
    // xv_table->addActions(actionList);

    for(int a = 0; a < actionList.count(); a++)
    {
        if(actionList.at(a) == nullptr)
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
        xv_buttonLayout->addWidget(button, 0, Qt::AlignRight);
    }
}
//==============================================================
void X_JointSpectrumTableWidget::xp_appendContextMenuActions(QList<QAction*> actionList)
{
    foreach(QAction* action, actionList)
    {
        if(action != nullptr && xv_contextMenuActionList.contains(action))
        {
            continue;
        }
        xv_contextMenuActionList.append(action);
    }
}
//==============================================================
void X_JointSpectrumTableWidget::xp_connectToSpectrumArrayRepository(X_SpectrumArrayRepository* repository)
{
    xp_appendButtonActions(repository->xp_spectrumActions());
    xp_appendContextMenuActions(repository->xp_spectrumContextMenuActions());

    connect(this, &X_JointSpectrumTableWidget::xg_currentSpectrumChanged,
            repository, &X_SpectrumArrayRepository::xp_currentSpectrumChanged);
    connect(repository, &X_SpectrumArrayRepository::xg_setCurrentSpectrumIndex,
            this, &X_JointSpectrumTableWidget::xp_setCurrentSpectrumIndex);
    connect(repository, &X_SpectrumArrayRepository::xg_requestCurrentSpectrumRow,
            this, &X_JointSpectrumTableWidget::xp_currentSpectrumRow);

}
//==============================================================
void X_JointSpectrumTableWidget::xp_connectToCalibrationRepository(X_CalibrationRepository* repository)
{
    QList<QAction*> separatorList;
    separatorList << nullptr;
    xp_appendContextMenuActions(separatorList);
    xp_appendContextMenuActions(repository->xp_calibrationVisibilityActions());
}
//==============================================================
void X_JointSpectrumTableWidget::xp_setMainLayoutMargin(int margin)
{
    xv_mainLayout->setContentsMargins(margin, margin, margin, margin);
}
//==============================================================
void X_JointSpectrumTableWidget::zh_editNext(QModelIndex editedIndex)
{
    if(!editedIndex.isValid() || editedIndex.row() < 0)
    {
        return;
    }

    if(editedIndex.row()+1 >= editedIndex.model()->rowCount())
    {
        // end of column - quit editing keeping current index as "current"
        xv_table->setCurrentIndex(editedIndex);
        return;
    }

    // defining next index
    QModelIndex nextIndex = editedIndex.model()->index(editedIndex.row()+1, editedIndex.column());
    if(!nextIndex.isValid())
    {
        return;
    }

    xv_table->setCurrentIndex(nextIndex);
    xv_table->edit(nextIndex);
}
//==============================================================
void X_JointSpectrumTableWidget::zh_onCurrentSpectrumChanged(const QModelIndex & current,
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

    emit xg_currentSpectrumChanged(currentRow, previousRow);
}
//==============================================================
void X_JointSpectrumTableWidget::zh_onContextMenuRequest(const QPoint &pos)
{
    QMenu *menu=new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    foreach(QAction* action, xv_contextMenuActionList)
    {
        if(action == nullptr)
        {
            menu->addSeparator();
            continue;
        }

        menu->addAction(action);
    }

    menu->popup(xv_table->viewport()->mapToGlobal(pos));
}
//==============================================================
void X_JointSpectrumTableWidget::zh_onSelectionChange(const QItemSelection & selected,
                                                     const QItemSelection & deselected) const
{
    QModelIndexList selectedIndexList = xv_table->selectionModel()->selectedIndexes();
    emit xg_selectedIndexListChanged(selectedIndexList);
}
//==============================================================

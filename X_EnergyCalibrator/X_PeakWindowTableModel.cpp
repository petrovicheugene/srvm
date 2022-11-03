//======================================================
#include "X_PeakWindowTableModel.h"
#include "X_General.h"
#include "X_ControlAction.h"
#include "X_PeakWindow.h"
//======================================================
X_PeakWindowTableModel::X_PeakWindowTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    xh_createActions();
    xh_createConnections();

}
//======================================================
X_PeakWindowTableModel::~X_PeakWindowTableModel()
{

}
//======================================================
Qt::ItemFlags	X_PeakWindowTableModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    return flags;
}
//======================================================
QModelIndex X_PeakWindowTableModel::index(int row, int column, const QModelIndex &parent) const
{
    return QAbstractTableModel::index(row, column, parent);
}
//======================================================
int	X_PeakWindowTableModel::columnCount(const QModelIndex & parent) const
{
    return 3;
}
//======================================================
int	X_PeakWindowTableModel::rowCount(const QModelIndex & parent) const
{
    return xv_peakWindowList.count();
}
//======================================================
QVariant	X_PeakWindowTableModel::data(const QModelIndex & index, int role) const
{
    if(!index.isValid() || index.column() < 0 || index.column() >= columnCount()
            || index.row() < 0 || index.row() >= rowCount())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            return QVariant(xv_peakWindowList.at(index.row())->xp_peakName());
        }
    }

    return QVariant();
}
//======================================================
bool X_PeakWindowTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    return false;
}
//======================================================
QVariant	X_PeakWindowTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}
//======================================================
QList<X_ControlAction*> X_PeakWindowTableModel::xp_buttonActions() const
{
    QList<X_ControlAction*> actionList;
    actionList.append(xv_addPeakWindowAction);
    actionList.append(xv_removePeakWindowAction);

    return actionList;
}
//======================================================
QList<X_ControlAction*> X_PeakWindowTableModel::xp_contextActions() const
{
    QList<X_ControlAction*> actionList;
    actionList.append(xv_addPeakWindowAction);
    actionList.append(xv_removePeakWindowAction);

    return actionList;
}
//======================================================
void X_PeakWindowTableModel::xh_createActions()
{
    xv_addPeakWindowAction = new X_ControlAction(this);
    xv_addPeakWindowAction->setIcon(QIcon());
    xv_addPeakWindowAction->setText("Add");
    xv_addPeakWindowAction->setToolTip(tr("Append peak window"));

    xv_removePeakWindowAction = new X_ControlAction(this);
    xv_removePeakWindowAction->setIcon(QIcon());
    xv_removePeakWindowAction->setText("Remove");
    xv_removePeakWindowAction->setToolTip(tr("Remove peak window"));

}
//===================================================
void X_PeakWindowTableModel::xh_createConnections()
{
    connect(xv_addPeakWindowAction, &X_ControlAction::triggered,
            this, &X_PeakWindowTableModel::xh_onAddPeakWindowAction);
    connect(xv_removePeakWindowAction, &X_ControlAction::triggered,
            this, &X_PeakWindowTableModel::xh_onRemovePeakWindowAction);

}
//===================================================
void X_PeakWindowTableModel::xh_onAddPeakWindowAction()
{
    // define peak window margins
    // ask plotter for visible rect
//    QRectF currentVisibleSceneRect;
//    emit xg_inquiryCurrentVisibleSceneRect(currentVisibleSceneRect);

//    int windowWidth = qRound(currentVisibleSceneRect.width() / 20);
//    int offset = qRound((currentVisibleSceneRect.width() - windowWidth) / 2);
//    int firstChannel;
//    int lastChannel;
//    if(currentVisibleSceneRect.width() > 0)
//    {
//        firstChannel = qRound(currentVisibleSceneRect.left()) + offset;
//        lastChannel = qRound(currentVisibleSceneRect.right()) - offset;
//    }
//    else
//    {
//        firstChannel = 0;
//        lastChannel = 1;
//    }

    // create a new Peak Window
    X_PeakWindow* peakWindow = new X_PeakWindow(this);

    int row = xv_peakWindowList.count();
    beginInsertRows(QModelIndex(), row, row);
    xv_peakWindowList.append(peakWindow);
    endInsertRows();

}
//===================================================
void X_PeakWindowTableModel::xh_onRemovePeakWindowAction()
{
    QModelIndex currentIndex;
    emit xg_inquiryCurrentIndex(currentIndex);
    if(!currentIndex.isValid() || currentIndex.row() < 0
            || currentIndex.row() >=  xv_peakWindowList.count())
    {
        return;
    }

     beginRemoveRows(QModelIndex(), currentIndex.row(), currentIndex.row());
     delete xv_peakWindowList.takeAt(currentIndex.row());
     endRemoveRows();
}
//===================================================

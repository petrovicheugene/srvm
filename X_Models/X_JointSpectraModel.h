//===========================================================
#ifndef X_JOINTSPECTRAMODEL_H
#define X_JOINTSPECTRAMODEL_H
//===========================================================
#include <QAbstractTableModel>
#include "X_JointSpectraDataManager.h"
#include <QItemSelection>
//===========================================================
//===========================================================
class X_JointSpectraModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit X_JointSpectraModel(QObject *parent = 0);

    virtual Qt::ItemFlags flags(const QModelIndex & index) const;
    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;

    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    // FUNCS
    void xp_connectToSpectraDataManager(X_JointSpectraDataManager* dataManager);

public slots:

    void xp_onSelectedIndexListChange(QModelIndexList selectionIndexList);
    void xp_clearSelectedConcentrations();
    void xp_convertSelectedToString(QString&) const;

signals:

    void xg_requestCurrentIndex(QModelIndex& currentIndex) const;
    void xg_selectionChanged(bool selectionEnabled, bool concentrationSelected, bool spectrumSelected ) const;

private:

    // VARS
    X_JointSpectraDataManager* xv_dataManager;
    QMap<int, QMap<int, QModelIndex> > xv_selectedIndexMap;


private slots:

    void xh_onDataManagerOperation(X_JointSpectraDataManager::OperationType, int, int);

};
//===========================================================
#endif // X_JOINTSPECTRAMODEL_H

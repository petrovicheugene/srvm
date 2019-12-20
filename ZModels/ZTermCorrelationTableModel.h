//===========================================================
#ifndef ZTERMCORRELATIONTABLEMODEL_H
#define ZTERMCORRELATIONTABLEMODEL_H
//===========================================================
#include "ZTermCorrelationTableManager.h"
#include <QAbstractTableModel>

//===========================================================
class ZTermCorrelationTableManager;
//===========================================================
class ZTermCorrelationTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZTermCorrelationTableModel(QObject* parent = 0);

    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    virtual QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;

    void zp_connectToTermCorrelationTableManager(ZTermCorrelationTableManager* dataManager);

signals:

    void zg_currentTermChanged(int currentTermIndex, int previousTermIndex) const;
    void zg_termDoubleClicked(int row);

public slots:

    void zp_onUserChangesTermState(int termLogIndex);


private slots:

    void zh_onDataManagerOperation(ZTermCorrelationTableManager::OperationType type,
                                   int first,
                                   int last);

private:
    // VARS
    ZTermCorrelationTableManager* zv_dataManager;
};
//===========================================================
#endif // ZTERMCORRELATIONTABLEMODEL_H

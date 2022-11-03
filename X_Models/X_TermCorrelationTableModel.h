//===========================================================
#ifndef X_TERMCORRELATIONTABLEMODEL_H
#define X_TERMCORRELATIONTABLEMODEL_H
//===========================================================
#include "X_TermCorrelationTableManager.h"
#include <QAbstractTableModel>

//===========================================================
class X_TermCorrelationTableManager;
//===========================================================
class X_TermCorrelationTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit X_TermCorrelationTableModel(QObject* parent = 0);

    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    virtual QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;

    void xp_connectToTermCorrelationTableManager(X_TermCorrelationTableManager* dataManager);

signals:

    void xg_currentTermChanged(int currentTermIndex, int previousTermIndex) const;
    void xg_termDoubleClicked(int row);

public slots:

    void xp_onUserChangesTermState(int termLogIndex);


private slots:

    void xh_onDataManagerOperation(X_TermCorrelationTableManager::OperationType type,
                                   int first,
                                   int last);

private:
    // VARS
    X_TermCorrelationTableManager* xv_dataManager;
};
//===========================================================
#endif // X_TERMCORRELATIONTABLEMODEL_H

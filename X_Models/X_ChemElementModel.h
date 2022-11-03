//===========================================================
#ifndef X_CHEMELEMENTMODEL_H
#define X_CHEMELEMENTMODEL_H
//===========================================================
#include <QAbstractTableModel>
#include "X_ChemElementDataManager.h"
//===========================================================

//===========================================================
class X_ChemElementModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit X_ChemElementModel(QObject *parent = 0);

    virtual Qt::ItemFlags	flags(const QModelIndex & index) const;
    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;

    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool	setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    // FUNCS
    void xp_connectToChemElementDataManager(X_ChemElementDataManager* dataManager);

signals:


public slots:


private:

    // VARS
    X_ChemElementDataManager* xv_dataManager;

private slots:

    void zh_onRepositoryOperation(X_ChemElementDataManager::OperationType, int first, int last);

};
//===========================================================
#endif // X_CHEMELEMENTMODEL_H

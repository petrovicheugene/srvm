//==================================================================
#ifndef X_ARRAYMODEL_H
#define X_ARRAYMODEL_H
//==================================================================
#include <QAbstractTableModel>
#include "X_SpectrumArrayRepository.h"

//==================================================================
//==================================================================
class X_ArrayModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit X_ArrayModel(QObject *parent = 0);

    // override FUNCS
    virtual Qt::ItemFlags	flags(const QModelIndex & index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;

    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;

    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool	setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    // FUNCS
    void xp_connectToSpectraArrayRepository(X_SpectrumArrayRepository*);

public slots:

signals:

    void xg_checkCurrentArray();

private:

    // VARS
    X_SpectrumArrayRepository* xv_repositiry;

private slots:

    void zh_onRepositoryOperation(X_SpectrumArrayRepository::ArrayOperationType type, int first, int last);

 };
//==================================================================
#endif // X_ARRAYMODEL_H

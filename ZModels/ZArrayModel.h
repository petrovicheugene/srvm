//==================================================================
#ifndef ZARRAYMODEL_H
#define ZARRAYMODEL_H
//==================================================================
#include <QAbstractTableModel>
#include "ZSpectraArrayRepository.h"

//==================================================================
//==================================================================
class ZArrayModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZArrayModel(QObject *parent = 0);

    // override FUNCS
    virtual Qt::ItemFlags	flags(const QModelIndex & index) const;
    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;

    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool	setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    // FUNCS
    void zp_setSpectraArrayRepository(ZSpectraArrayRepository*);

public slots:

signals:

private:

    // VARS
    ZSpectraArrayRepository* zv_repositiry;

private slots:

    void zh_onRepositoryOperation(ZSpectraArrayRepository::ArrayOperationType type, int first, int last);

 };
//==================================================================
#endif // ZARRAYMODEL_H

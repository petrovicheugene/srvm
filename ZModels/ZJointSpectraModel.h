//===========================================================
#ifndef ZSPECTRAMODEL_H
#define ZSPECTRAMODEL_H
//===========================================================
#include <QAbstractTableModel>
//===========================================================

//===========================================================
class ZSpectraModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZSpectraModel(QObject *parent = 0);

    virtual Qt::ItemFlags	flags(const QModelIndex & index) const;
    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;

    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool	setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

signals:

public slots:
};
//===========================================================
#endif // ZSPECTRAMODEL_H

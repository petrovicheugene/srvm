//===========================================================
#ifndef ZJOINTSPECTRAMODEL_H
#define ZJOINTSPECTRAMODEL_H
//===========================================================
#include <QAbstractTableModel>
//===========================================================

//===========================================================
class ZJointSpectraModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ZJointSpectraModel(QObject *parent = 0);

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
#endif // ZJOINTSPECTRAMODEL_H

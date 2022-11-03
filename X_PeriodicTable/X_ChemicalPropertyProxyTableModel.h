//====================================================
#ifndef X_CHEMPROPERTYPROXYMODEL_H
#define X_CHEMPROPERTYPROXYMODEL_H
//====================================================
#include <QAbstractTableModel>
//====================================================
class QAbstractItemModel;
//====================================================
class X_ChemicalPropertyProxyTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit X_ChemicalPropertyProxyTableModel(QObject *parent = nullptr);

    // reimplemented FUNCS
    Qt::ItemFlags	flags(const QModelIndex & index) const override;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const override;
    int	rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant & data, int role) override;

    void xp_setItemIsEditable(bool editable);

signals:


public slots:

    void xp_onCurrentTreeElementChanged(const QModelIndex& current,
                                        const QModelIndex& previous);
    void xp_onSourceModelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);
private:

    // VARS
    QModelIndex xv_currentSourceModelIndex;
    QStringList xv_headerList;
    bool xv_itemIsEditable;

    // FUNCS

};
//====================================================
#endif // X_CHEMPROPERTYPROXYMODEL_H

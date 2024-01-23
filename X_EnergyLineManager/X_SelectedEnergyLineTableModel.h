//=============================================================
#ifndef X_SELECTEDENERGYLINETABLEMODEL_H
#define X_SELECTEDENERGYLINETABLEMODEL_H
//=============================================================
#include <QAbstractTableModel>
#include <QMap>

#include "X_EnergyLineSetItem.h"
//=============================================================
typedef QList<QPair<QString, QString> > PropertyList;
typedef int EnergyLineOperation;
//=============================================================
class X_SelectedEnergyLineTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit X_SelectedEnergyLineTableModel(QObject *parent = nullptr);
    ~X_SelectedEnergyLineTableModel();
    // reimplemented FUNCS
    Qt::ItemFlags	flags(const QModelIndex & index) const override;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const override;
    int	rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant & data, int role) override;

    //
    bool xp_energyLineEnergyValue(const QString& elementSymbol,
                           const QString& lineName,
                           double& energyValue) const;
    bool xp_energyLineReletiveIntensity(const QString& elementSymbol,
                                                                       const QString& lineName,
                                                                       int& reletiveIntensity) const;
    bool xp_energyLineVisibility(const QString& elementSymbol,
                           const QString& lineName,
                           bool& visible) const;
    bool xp_energyLineColor(const QString& elementSymbol,
                           const QString& lineName,
                           QColor& color) const;
signals:

    void xg_requestSelectedChemicalElements(QList<int>& X_NumberList) const;
    void xg_requestEnergyLinesForX_Number(int X_Number, PropertyList&) const;
    void xg_requestEnergyLineRelativeIntensityForX_Number(int X_Number, PropertyList&) const;
    void xg_requestChemicalElementSymbol(int X_Number, QString& symbol) const;

    void xg_energyLineVisibilityChanged(QString elementSymbol, QString lineName,
                                        double energy, bool visible) const;

    void xg_energyLineOperation(QString elementSymbol, QString lineName,
                                EnergyLineOperation operationType) const;

public slots:

    void xp_onSelectedChemicalElementChange(int X_Number, bool selected);


private slots:



private:

    // VARS
    QMap<int, X_EnergyLineSetItem*> xv_itemMap;
    QStringList xv_columnHederList;

    // FUNCS
    int xh_findRowToInsert(int X_Number) const;
    void xh_updateColumns();
    void xh_loadEnergyLinesToItem(X_EnergyLineSetItem* item);


};
//=============================================================
#endif // X_SELECTEDENERGYLINETABLEMODEL_H

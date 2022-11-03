//======================================================
#ifndef X_CHEMICALELEMENTENERGYLINEWIDGET_H
#define X_CHEMICALELEMENTENERGYLINEWIDGET_H
//======================================================
#include <QWidget>

//======================================================
class X_PeriodicTableWidget;
class X_ChemicalElementPropertyTreeModel;
class X_SelectedEnergyLineTableModel;

class QSplitter;
class QTableView;

//======================================================
typedef QList<QPair<QString, QString> > PropertyList;
typedef int EnergyLineOperationType;
//======================================================
class X_EnergyLineTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit X_EnergyLineTableWidget(QWidget *parent = nullptr);

signals:

    void xg_energyLineVisibilityChanged(QString elementSymbol, QString lineName,
                                        double energy,
                                        bool visible) const;

    void xg_energyLineOperation(const QString& elementSymbol,
                                const QString& lineName,
                                EnergyLineOperationType operationType);

public slots:


    void xp_energyLineEnergyValue(const QString& elementSymbol,
                                  const QString& lineName,
                                  double& energyValue) const;

    void xp_energyLineVisibility(const QString& elementSymbol,
                                 const QString& lineName,
                                 bool& visible) const;

    void xp_energyLineColor(const QString& elementSymbol,
                            const QString& lineName,
                            QColor& visible) const;

    void xp_energyLineRelativeIntensity(const QString &elementSymbol,
                                        const QString& lineName,
                                        int& reletiveIntensity) const;
private:

    // VARS
    X_PeriodicTableWidget* xv_periodicTableWidget;
    QTableView* xv_selectedChemicalElementTableView;
    QSplitter* xv_mainSplitter;

    X_ChemicalElementPropertyTreeModel* xv_chemicalElementPropertyTreeModel;
    X_SelectedEnergyLineTableModel* xv_selectedEnergyLineTableModel;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_energyLinesForX_Number(int X_Number, PropertyList &propertyList);
    void zh_energyLinesRelativeIntensityForX_Number(int X_Number, PropertyList &propertyList);
    void zh_chemicalElementSymbol(int X_Number, QString& symbol);

};
//======================================================
#endif // X_CHEMICALELEMENTENERGYLINEWIDGET_H

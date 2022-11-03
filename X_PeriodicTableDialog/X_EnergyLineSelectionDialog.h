//=============================================================
#ifndef X_ENERGYLINEDIALOG_H
#define X_ENERGYLINEDIALOG_H
//=============================================================
#include <QDialog>
//=============================================================
class QLineEdit;
class QPushButton;
class QSplitter;
class QTableView;

class X_PeriodicTableWidget;
class X_ChemicalElementPropertyTreeModel;
class X_ChemicalPropertyProxyTableModel;

//=============================================================
class X_EnergyLineSelectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit X_EnergyLineSelectionDialog(QWidget *parent = nullptr);
    ~X_EnergyLineSelectionDialog();

    double xp_energyValue() const;
    QString xp_chemicalElementName() const;
    QString xp_energyLineName() const;

signals:

public slots:

private slots:

    void xh_onOkClick();
    void xh_onSelectedChemicalElementChange(int X_Number, bool selected);
    void xh_onCurrentEnergyLineChange(const QModelIndex& current, const QModelIndex& previous);

private:

    // VARS
    const QString xv_settingsGroupName = "energyLineSelectionDialog";
    const QString xv_splitterStateSectionName = "mainSplitterState";
    const QString xv_geometrySectionName = "geometry";

    QString xv_chemicalElementSymbol;
    QString xv_energyLineName;
    double xv_energyLineEnergyValue;

    QSplitter* xv_mainSplitter;

    X_PeriodicTableWidget* xv_periodicTableWidget;
    QTableView* xv_chemicalElementPropertyTableView;
    QLineEdit* xv_selectedEnergyLineLineEdit;

    X_ChemicalElementPropertyTreeModel* xv_chemicalElementPropertyTreeModel;
    X_ChemicalPropertyProxyTableModel* xv_chemicalPropertyProxyTableModel;

    QPushButton* xv_okButton;
    QPushButton* xv_cancelButton;

    // FUNCS
    void xh_createComponents();
    void xh_createConnections();
    void xh_saveSettings();
    void xh_restoreSettings();
    void xh_updateSelectedLineLineEdit();


};
//=============================================================
#endif // X_ENERGYLINEDIALOG_H

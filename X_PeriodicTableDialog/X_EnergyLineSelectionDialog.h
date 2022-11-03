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

    void zh_onOkClick();
    void zh_onSelectedChemicalElementChange(int X_Number, bool selected);
    void zh_onCurrentEnergyLineChange(const QModelIndex& current, const QModelIndex& previous);

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
    void zh_createComponents();
    void zh_createConnections();
    void zh_saveSettings();
    void zh_restoreSettings();
    void zh_updateSelectedLineLineEdit();


};
//=============================================================
#endif // X_ENERGYLINEDIALOG_H

//======================================================
#include "X_EnergyLineTableWidget.h"

#include "X_ChemicalElementPropertyTreeModel.h"
#include "X_EnergyLineDelegate.h"
#include "X_PeriodicTableWidget.h"
#include "X_SelectedEnergyLineTableModel.h"

#include <QApplication>
#include <QDebug>
#include <QSplitter>
#include <QTableView>
#include <QVBoxLayout>
//======================================================
X_EnergyLineTableWidget::X_EnergyLineTableWidget(QWidget *parent)
    : QWidget(parent)
{
    xh_createComponents();
    xh_createConnections();
}
//======================================================
void X_EnergyLineTableWidget::xh_createComponents()
{
    xv_chemicalElementPropertyTreeModel = new X_ChemicalElementPropertyTreeModel(this);
    xv_chemicalElementPropertyTreeModel->xp_setNamePropertyName("Name ru");
    xv_selectedEnergyLineTableModel = new X_SelectedEnergyLineTableModel(this);

    // main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // slitter
    xv_mainSplitter = new QSplitter(Qt::Vertical, this);
    mainLayout->addWidget(xv_mainSplitter);


    // periodic table
    xv_periodicTableWidget = new X_PeriodicTableWidget(this);
    xv_periodicTableWidget->xp_setSelectionMode(X_PeriodicTableWidget::SM_MULTIPLE_SELECTION);
    xv_mainSplitter->addWidget(xv_periodicTableWidget);

    // selected chemical element table view
    xv_selectedChemicalElementTableView = new QTableView(this);
    xv_mainSplitter->addWidget(xv_selectedChemicalElementTableView);

}
//======================================================
void X_EnergyLineTableWidget::xh_createConnections()
{
    xv_periodicTableWidget->xp_setModel(xv_chemicalElementPropertyTreeModel);

    xv_selectedChemicalElementTableView->setModel(xv_selectedEnergyLineTableModel);
    xv_selectedChemicalElementTableView->setItemDelegate(new X_EnergyLineDelegate);


    connect(xv_periodicTableWidget, &X_PeriodicTableWidget::xg_selectedChemicalElementChanged,
            xv_selectedEnergyLineTableModel, &X_SelectedEnergyLineTableModel::xp_onSelectedChemicalElementChange);

    connect(xv_selectedEnergyLineTableModel, &X_SelectedEnergyLineTableModel::xg_requestSelectedChemicalElements,
            xv_periodicTableWidget, &X_PeriodicTableWidget::xp_fillSelectedChemicalElementList);

    connect(xv_selectedEnergyLineTableModel, &X_SelectedEnergyLineTableModel::xg_requestEnergyLinesForX_Number,
            this, &X_EnergyLineTableWidget::xh_energyLinesForX_Number);
    connect(xv_selectedEnergyLineTableModel, &X_SelectedEnergyLineTableModel::xg_requestEnergyLineRelativeIntensityForX_Number,
            this, &X_EnergyLineTableWidget::xh_energyLinesRelativeIntensityForX_Number);


    connect(xv_selectedEnergyLineTableModel, &X_SelectedEnergyLineTableModel::xg_requestChemicalElementSymbol,
            this, &X_EnergyLineTableWidget::xh_chemicalElementSymbol);

    connect(xv_selectedEnergyLineTableModel, &X_SelectedEnergyLineTableModel::xg_energyLineVisibilityChanged,
            this, &X_EnergyLineTableWidget::xg_energyLineVisibilityChanged);

    connect(xv_selectedEnergyLineTableModel, &X_SelectedEnergyLineTableModel::xg_energyLineOperation,
            this, &X_EnergyLineTableWidget::xg_energyLineOperation);


}
//======================================================
void X_EnergyLineTableWidget::xh_energyLinesForX_Number(int X_Number, PropertyList& propertyList)
{
    QStringList propertySectionBranch;
    propertySectionBranch << "Energy Lines";
    xv_chemicalElementPropertyTreeModel->xp_chemicalElementProperties(X_Number, propertySectionBranch, propertyList);
}
//======================================================
void X_EnergyLineTableWidget::xh_energyLinesRelativeIntensityForX_Number(int X_Number, PropertyList& propertyList)
{
    QStringList propertySectionBranch;
    propertySectionBranch << "Relative Intensity";
    xv_chemicalElementPropertyTreeModel->xp_chemicalElementProperties(X_Number, propertySectionBranch, propertyList);
}
//======================================================
void X_EnergyLineTableWidget::xh_chemicalElementSymbol(int X_Number, QString& symbol)
{
    PropertyList propertyList;
    xv_chemicalElementPropertyTreeModel->xp_chemicalElementProperties(X_Number, QStringList(), propertyList);

    for(auto& property : propertyList)
    {
        if(property.first.toLower() == "symbol")
        {
            symbol = property.second;
        }
    }
}
//======================================================
void X_EnergyLineTableWidget::xp_energyLineEnergyValue(const QString& elementSymbol,
                       const QString& lineName,
                       double& energyValue) const
{
    if(!xv_selectedEnergyLineTableModel->xp_energyLineEnergyValue(elementSymbol, lineName, energyValue))
    {
        energyValue = 0.0;
    }
}
//======================================================
void X_EnergyLineTableWidget::xp_energyLineRelativeIntensity(const QString& elementSymbol,
                                    const QString& lineName,
                                    int& reletiveIntensity) const
{
    if(!xv_selectedEnergyLineTableModel->xp_energyLineReletiveIntensity(elementSymbol, lineName, reletiveIntensity))
    {
        reletiveIntensity = 100;
    }
}
//======================================================
void X_EnergyLineTableWidget::xp_energyLineVisibility(const QString& elementSymbol,
                       const QString& lineName,
                       bool& visible) const
{
    if(!xv_selectedEnergyLineTableModel->xp_energyLineVisibility(elementSymbol, lineName, visible))
    {
        visible = false;
    }
}
//======================================================
void X_EnergyLineTableWidget::xp_energyLineColor(const QString& elementSymbol,
                        const QString& lineName,
                        QColor& color) const
{
    if(!xv_selectedEnergyLineTableModel->xp_energyLineColor(elementSymbol, lineName, color))
    {
        color = QColor();
    }
}
//======================================================

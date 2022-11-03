//=============================================================
#include "X_EnergyLineSelectionDialog.h"
#include "X_General.h"
#include "X_PeriodicTableWidget.h"
#include "X_ChemicalElementPropertyTreeModel.h"
#include "X_ChemicalPropertyProxyTableModel.h"

#include <QApplication>
#include <QDialogButtonBox>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QSplitter>
#include <QTableView>
#include <QVBoxLayout>

//=============================================================
X_EnergyLineSelectionDialog::X_EnergyLineSelectionDialog(QWidget *parent) : QDialog(parent)
{
    xv_chemicalElementSymbol = QString();
    xv_energyLineName = QString();
    xv_energyLineEnergyValue = 0.0;

    xh_createComponents();
    xh_createConnections();
    xh_restoreSettings();
}
//=============================================================
X_EnergyLineSelectionDialog::~X_EnergyLineSelectionDialog()
{
    xh_saveSettings();
}
//=============================================================
void X_EnergyLineSelectionDialog::xh_createComponents()
{
    // models
    xv_chemicalElementPropertyTreeModel = new X_ChemicalElementPropertyTreeModel(this);
    xv_chemicalElementPropertyTreeModel->xp_setNamePropertyName("Name ru");

    xv_chemicalPropertyProxyTableModel = new X_ChemicalPropertyProxyTableModel(this);
    xv_chemicalPropertyProxyTableModel->xp_setItemIsEditable(false);
    xv_chemicalPropertyProxyTableModel->setHeaderData(0, Qt::Horizontal, tr("Energy line"), Qt::EditRole);
    xv_chemicalPropertyProxyTableModel->setHeaderData(1, Qt::Horizontal, tr("Energy value"), Qt::EditRole);

    // main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // slitter
    xv_mainSplitter = new QSplitter(this);
    mainLayout->addWidget(xv_mainSplitter);

    // periodic table
    xv_periodicTableWidget = new X_PeriodicTableWidget(this);
    xv_periodicTableWidget->xp_setSelectionMode(X_PeriodicTableWidget::SM_SINGLE_SELECTION);
    xv_mainSplitter->addWidget(xv_periodicTableWidget);

    // property table view
    xv_chemicalElementPropertyTableView = new QTableView(this);
    xv_chemicalElementPropertyTableView->setModel(xv_chemicalPropertyProxyTableModel);
    xv_chemicalElementPropertyTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    xv_chemicalElementPropertyTableView->setSelectionMode( QAbstractItemView::SingleSelection);
    xv_mainSplitter->addWidget(xv_chemicalElementPropertyTableView);


    QHBoxLayout* selectedLineLayout = new QHBoxLayout;
    mainLayout->addLayout(selectedLineLayout);

    QLabel* label = new QLabel(this);
    QString text = tr("Selected line:");
    label->setText(text);
    selectedLineLayout->addWidget(label);

    xv_selectedEnergyLineLineEdit = new QLineEdit(this);
    xv_selectedEnergyLineLineEdit->setReadOnly(true);
    selectedLineLayout->addWidget(xv_selectedEnergyLineLineEdit);

    // Basement
    // sparator line
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    mainLayout->addWidget(line);

    // dialog buttons
    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    xv_okButton = new QPushButton(tr("Ok"), this);
    buttonBox->addButton(xv_okButton, QDialogButtonBox::ActionRole);

    xv_cancelButton = new QPushButton(tr("Cancel"), this);
    buttonBox->addButton(xv_cancelButton, QDialogButtonBox::ActionRole);
}
//=============================================================
void X_EnergyLineSelectionDialog::xh_createConnections()
{
    xv_periodicTableWidget->xp_setModel(xv_chemicalElementPropertyTreeModel);

    connect(xv_periodicTableWidget, &X_PeriodicTableWidget::xg_selectedChemicalElementChanged,
            this, &X_EnergyLineSelectionDialog::xh_onSelectedChemicalElementChange);

    connect(xv_okButton, &QPushButton::clicked,
            this, &X_EnergyLineSelectionDialog::xh_onOkClick);
    connect(xv_cancelButton, &QPushButton::clicked,
            this, &X_EnergyLineSelectionDialog::reject);

    connect(xv_chemicalElementPropertyTableView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &X_EnergyLineSelectionDialog::xh_onCurrentEnergyLineChange);

}
//=============================================================
double X_EnergyLineSelectionDialog::xp_energyValue() const
{
    return xv_energyLineEnergyValue;
}
//=============================================================
QString X_EnergyLineSelectionDialog::xp_chemicalElementName() const
{
    return xv_chemicalElementSymbol;
}
//=============================================================
QString X_EnergyLineSelectionDialog::xp_energyLineName() const
{
    return xv_energyLineName;
}
//=============================================================
void X_EnergyLineSelectionDialog::xh_saveSettings()
{
    QSettings settings;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup(xv_settingsGroupName);
    settings.setValue(xv_geometrySectionName, QVariant(saveGeometry()));
    if(xv_mainSplitter)
    {
        settings.setValue(xv_splitterStateSectionName, QVariant(xv_mainSplitter->saveState()));
    }
    settings.endGroup();
    settings.endGroup();
}
//=============================================================
void X_EnergyLineSelectionDialog::xh_restoreSettings()
{
    QSettings settings;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup(xv_settingsGroupName);

    QVariant vData = settings.value(xv_geometrySectionName);
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        restoreGeometry(vData.toByteArray());
    }

    if(xv_mainSplitter)
    {
        vData = settings.value(xv_splitterStateSectionName);
        if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
        {
            xv_mainSplitter->restoreState(vData.toByteArray());
        }
    }

    settings.endGroup();
    settings.endGroup();
}
//=============================================================
void X_EnergyLineSelectionDialog::xh_onOkClick()
{
    accept();
}
//=============================================================
void X_EnergyLineSelectionDialog::xh_onSelectedChemicalElementChange(int X_Number, bool selected)
{
    QList<int> selectedChemicalElementList = xv_periodicTableWidget->xp_selectedChemicalElementList();

    if(selectedChemicalElementList.isEmpty())
    {
        xv_chemicalPropertyProxyTableModel->xp_onCurrentTreeElementChanged(QModelIndex(), QModelIndex());
        return;
    }

    QStringList sectionBranch;
    sectionBranch.append("Energy Lines");

    QModelIndex energyLinesSectionIndex = xv_chemicalElementPropertyTreeModel->xp_indexForSection(selectedChemicalElementList.first(), sectionBranch);
    xv_chemicalPropertyProxyTableModel->xp_onCurrentTreeElementChanged(energyLinesSectionIndex, QModelIndex());
}
//=============================================================
void X_EnergyLineSelectionDialog::xh_onCurrentEnergyLineChange(const QModelIndex& current, const QModelIndex& previous)
{
    if(!current.isValid())
    {
        return;
    }

    // change selected chem element and line
    // define current chemical elment symbol
    QList<int> selectedChemicalElementList = xv_periodicTableWidget->xp_selectedChemicalElementList();
    if(selectedChemicalElementList.isEmpty())
    {
        return;
    }

    PropertyList propertyList;
    xv_chemicalElementPropertyTreeModel->xp_chemicalElementProperties(selectedChemicalElementList.first(), QStringList(), propertyList);
    QString symbolPropertyName = xv_chemicalElementPropertyTreeModel->xp_symbolPropertyName();
    for(int i = 0; i < propertyList.count(); i++)
    {
        if(propertyList.at(i).first == symbolPropertyName)
        {
            xv_chemicalElementSymbol = propertyList.at(i).second;
            break;
        }
    }

    int propertyRow = current.row();
    // line name
    QModelIndex propertyIndex = xv_chemicalPropertyProxyTableModel->index(propertyRow, 0);
    if(!propertyIndex.isValid())
    {
        xv_energyLineName = tr("not defined");
    }
    else
    {
        QVariant vData = propertyIndex.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<QString>())
        {
            xv_energyLineName = tr("not defined");
        }
        else
        {
            xv_energyLineName = vData.toString();
        }
    }
    // line value
    propertyIndex = xv_chemicalPropertyProxyTableModel->index(propertyRow, 1);
    if(!propertyIndex.isValid())
    {
        xv_energyLineEnergyValue = 0.0;
    }
    else
    {
        QVariant vData = propertyIndex.data(Qt::DisplayRole);
        if(!vData.isValid() || !vData.canConvert<double>())
        {
            xv_energyLineEnergyValue = 0.0;
        }
        else
        {
            xv_energyLineEnergyValue = vData.toDouble();
        }
    }

    xh_updateSelectedLineLineEdit();

}
//=============================================================
void X_EnergyLineSelectionDialog::xh_updateSelectedLineLineEdit()
{
    QString selectedLineText = tr("%1 %2 - %3 (kEv)").
            arg(xv_chemicalElementSymbol, xv_energyLineName, QString::number(xv_energyLineEnergyValue));

    xv_selectedEnergyLineLineEdit->setText(selectedLineText);
}
//=============================================================


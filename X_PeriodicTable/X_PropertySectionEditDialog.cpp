//======================================================
#include "X_PropertySectionEditDialog.h"

#include <QApplication>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QRadioButton>
#include <QRegExp>
#include <QVBoxLayout>
//======================================================
X_PropertySectionEditDialog::X_PropertySectionEditDialog(const QStringList& branch,
                                                       bool currentVisible,
                                                       bool selectedVisible,
                                                       QWidget *parent) : X_BaseDialog("X_PropertySectionEditDialog", parent)
{
    setWindowTitle(qApp->applicationDisplayName());

    xh_createComponents();
    xh_createConnections();
    xp_setCurrentSectionBranch(branch);
    xh_setUpAdditionTargetButtons(currentVisible, selectedVisible );

}
//======================================================
void X_PropertySectionEditDialog::xh_createComponents()
{
    xh_addWidgetToMainLayout(xh_createControlWidget());
    xh_addStretchToMainLayout();

    xv_okButton = xh_createBasementButton(tr("Ok"),
                                          tr("Save section and close dialog"),
                                          QIcon());
    xv_cancelButton = xh_createBasementButton(tr("Cancel"),
                                              tr("Close dialog without savimg section"),
                                              QIcon());
}
//======================================================
void X_PropertySectionEditDialog::xh_createConnections()
{
    connect(xv_okButton, &QPushButton::clicked,
            this, &X_PropertySectionEditDialog::xh_onOkButtonClick);
    connect(xv_cancelButton, &QPushButton::clicked,
            this, &X_PropertySectionEditDialog::xh_onCloseButtonClick);

}
//======================================================
QWidget* X_PropertySectionEditDialog::xh_createControlWidget()
{
    QWidget* widget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    widget->setLayout(mainLayout);

    QLabel* label = new QLabel(this);
    label->setText(tr("Current section branch:"));
    mainLayout->addWidget(label);

    xv_parentSectionBranchLabel = new QLabel(this);
    mainLayout->addWidget(xv_parentSectionBranchLabel);

    //    xv_parentSectionBranchLineEdit = new QLineEdit(this);
    //    mainLayout->addWidget(xv_parentSectionBranchLineEdit);

    label = new QLabel(this);
    label->setText(tr("Section name:"));
    mainLayout->addWidget(label);

    xv_sectionLineEdit  = new QLineEdit(this);
    xv_sectionLineEdit->installEventFilter(this);
    mainLayout->addWidget(xv_sectionLineEdit);

    QGroupBox* groupBox = new QGroupBox(widget);
    groupBox->setTitle(tr("Add to"));
    mainLayout->addWidget(groupBox);
    QVBoxLayout* groupBoxLayout = new QVBoxLayout;
    groupBox->setLayout(groupBoxLayout);

    xv_toCurrentChemicalElementRadioButton = new QRadioButton(this);
    xv_toCurrentChemicalElementRadioButton->installEventFilter(this);
    xv_toCurrentChemicalElementRadioButton->setText(tr("Current chemical element"));
    groupBoxLayout->addWidget(xv_toCurrentChemicalElementRadioButton);

    xv_toSelectedChemicalElementsRadioButton = new QRadioButton(this);
    xv_toSelectedChemicalElementsRadioButton->installEventFilter(this);
    xv_toSelectedChemicalElementsRadioButton->setText(tr("Selected chemical elements"));
    groupBoxLayout->addWidget(xv_toSelectedChemicalElementsRadioButton);

    xv_toAllChemicalElementsRadioButton = new QRadioButton(this);
    xv_toAllChemicalElementsRadioButton->installEventFilter(this);
    xv_toAllChemicalElementsRadioButton->setText(tr("All chemical elements"));
    groupBoxLayout->addWidget(xv_toAllChemicalElementsRadioButton);

    return widget;
}
//======================================================
void X_PropertySectionEditDialog::xh_onOkButtonClick()
{
    if(xv_sectionLineEdit->text().isEmpty())
    {
        QString msg = tr("");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return;
    }

    accept();
}
//======================================================
void X_PropertySectionEditDialog::xh_onCloseButtonClick()
{
    reject();
}
//======================================================
void X_PropertySectionEditDialog::xh_setUpAdditionTargetButtons(bool currentVisible,
                                                               bool selectedVisible)
{
    xv_toCurrentChemicalElementRadioButton->setVisible(currentVisible);
    xv_toSelectedChemicalElementsRadioButton->setVisible(selectedVisible);

    if(currentVisible)
    {
        xv_toCurrentChemicalElementRadioButton->setChecked(true);
    }
    else
    {
        if(selectedVisible)
        {
            xv_toSelectedChemicalElementsRadioButton->setChecked(true);
        }
        else
        {
            xv_toAllChemicalElementsRadioButton->setChecked(true);
        }
    }
}
//======================================================
X_PropertySectionEditDialog::AdditionTarget X_PropertySectionEditDialog::xp_additionTarget() const
{
    if(xv_toAllChemicalElementsRadioButton->isChecked())
    {
        return AT_ALL;
    }
    else if(xv_toSelectedChemicalElementsRadioButton->isChecked())
    {
        return AT_SELECTED;
    }
    else // to current
    {
        return AT_CURRENT;
    }
}
//======================================================
QStringList X_PropertySectionEditDialog::xp_newSectionBranch() const
{
    QStringList sectionBranch = xv_parentSectionBranch;
    QStringList newSectionBranch = xv_sectionLineEdit->text().split(QRegularExpression("[\\\\/-]"), Qt::SkipEmptyParts);

    foreach(QString section, newSectionBranch)
    {
        sectionBranch.append(section);
    }

    return sectionBranch;
}
//======================================================
void X_PropertySectionEditDialog::xp_setCurrentSectionBranch(const QStringList& branch)
{
    xv_parentSectionBranch = branch;
    QString currentSectionBranchString;
    for(int s = 0; s < xv_parentSectionBranch.count(); s++)
    {
        currentSectionBranchString += xv_parentSectionBranch.at(s);
        if(s < xv_parentSectionBranch.count() - 1)
        {
            currentSectionBranchString += "/";
        }

    }
    xv_parentSectionBranchLabel->setText(QString("<b>%1</b>").arg(currentSectionBranchString));

    xv_toCurrentChemicalElementRadioButton->setEnabled(!xv_parentSectionBranch.isEmpty());
    xv_toSelectedChemicalElementsRadioButton->setEnabled(!xv_parentSectionBranch.isEmpty());
    xv_toAllChemicalElementsRadioButton->setEnabled(!xv_parentSectionBranch.isEmpty());
}
//======================================================

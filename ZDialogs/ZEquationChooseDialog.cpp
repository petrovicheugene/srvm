//============================================================
#include "ZEquationChooseDialog.h"
#include "globalVariables.h"
#include "ZCalibration.h"

#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>

//============================================================
ZEquationChooseDialog::ZEquationChooseDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Equation settings"));
    setWindowFlags(Qt::Tool | Qt::MSWindowsFixedSizeDialogHint);
    zv_accepted = false;
    zh_createComponents();
    zh_createConnections();
}
//============================================================
void ZEquationChooseDialog::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // simple polynom
    QHBoxLayout* simplePolynomLayout = new QHBoxLayout(this);
    mainLayout->addLayout(simplePolynomLayout);

    zv_simplePolynomEquationRadioButton = new QRadioButton(this);
    simplePolynomLayout->addWidget(zv_simplePolynomEquationRadioButton);
    zv_simplePolynomEquationRadioButton->setText(ZCalibration::zp_equationTypeString(ZCalibration::ET_POLYNOMIAL));

    QLabel* label = new QLabel(this);
    label->setText(ZCalibration::simplePolynomEquationString);
    simplePolynomLayout->addWidget(label);
    simplePolynomLayout->addStretch();

    // fractional
    QHBoxLayout* fractionalLayout = new QHBoxLayout(this);
    mainLayout->addLayout(fractionalLayout);

    zv_fractionalEquationRadioButton = new QRadioButton(this);
    fractionalLayout->addWidget(zv_fractionalEquationRadioButton);
    zv_fractionalEquationRadioButton->setText(ZCalibration::zp_equationTypeString(ZCalibration::ET_FRACTIONAL));

    label = new QLabel(this);
    label->setText(ZCalibration::fractionalEquationString);
    fractionalLayout->addWidget(label);
    fractionalLayout->addStretch();

    // base term
    QHBoxLayout* baseTermLayout = new QHBoxLayout(this);
    mainLayout->addLayout(baseTermLayout);

    label = new QLabel(this);
    label->setText(tr("Base term:"));
    baseTermLayout->addWidget(label);

    zv_baseTermComboBox = new QComboBox(this);
    zv_baseTermComboBox->setEnabled(false);
    baseTermLayout->addWidget(zv_baseTermComboBox);
    baseTermLayout->addStretch();

    // buttons
    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    zv_okButton = new QPushButton(this);
    buttonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(this);
    buttonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);
}
//============================================================
void ZEquationChooseDialog::zh_createConnections()
{
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZEquationChooseDialog::zh_onDialogAccepted);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZEquationChooseDialog::reject);

    connect(zv_simplePolynomEquationRadioButton, &QRadioButton::toggled,
            zv_baseTermComboBox, &QComboBox::setDisabled);
    connect(zv_fractionalEquationRadioButton, &QRadioButton::toggled,
            zv_baseTermComboBox, &QComboBox::setEnabled);


}
//============================================================
void ZEquationChooseDialog::zh_onDialogAccepted()
{
    zv_accepted = true;
    accept();
}
//============================================================
void ZEquationChooseDialog::zp_setEquationSettings(const ZEquationSettingsData& settings)
{
    if(settings.equationType == ZCalibration::ET_POLYNOMIAL)
    {
        zv_simplePolynomEquationRadioButton->setChecked(true);
    }
    else if(settings.equationType == ZCalibration::ET_FRACTIONAL)
    {
        zv_fractionalEquationRadioButton->setChecked(true);
    }

    int currentTermIndex = 0;
    for(int i = 0; i < settings.termList.count(); i++)
    {
        zv_baseTermComboBox->insertItem(i, settings.termList.at(i).second, QVariant(settings.termList.at(i).first));
        if(settings.termList.at(i).first == settings.baseTermId)
        {
            currentTermIndex = i;
        }
    }

    zv_baseTermComboBox->setCurrentIndex(currentTermIndex);
}
//============================================================
ZEquationSettingsData ZEquationChooseDialog::zp_equationSettings() const
{
    ZEquationSettingsData settings;
    QVariant vData = zv_baseTermComboBox->currentData();

    if(vData.isValid() && !vData.isNull() && vData.canConvert<qint64>())
    {
        bool ok;
        settings.baseTermId = (qint64)vData.toInt(&ok);

        if(!ok)
        {
            settings.baseTermId = -1;
        }
    }

    if(zv_simplePolynomEquationRadioButton->isChecked())
    {
        settings.equationType = ZCalibration::ET_POLYNOMIAL;
    }
    else if(zv_fractionalEquationRadioButton->isChecked())
    {
        settings.equationType = ZCalibration::ET_FRACTIONAL;
    }
    else
    {
        settings.equationType = ZCalibration::ET_NOT_DEFINED;
    }

    return settings;
}
//============================================================
bool ZEquationChooseDialog::zp_accepted() const
{
    return zv_accepted;
}
//============================================================


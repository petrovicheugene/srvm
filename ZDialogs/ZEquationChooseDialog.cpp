//============================================================
#include "ZEquationChooseDialog.h"
#include "ZGeneral.h"
#include "ZCalibration.h"
#include "ZNormaSettingsDialog.h"
#include "ZTermNormalizer.h"

#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
//============================================================
ZEquationChooseDialog::ZEquationChooseDialog(QWidget *parent) : QDialog(parent)
{
    zv_normaDialog = 0;
    setWindowTitle(tr("Equation settings"));
    setWindowFlags(Qt::Tool | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint);

    zh_createComponents();
    zh_createConnections();
}
//============================================================
ZEquationChooseDialog::~ZEquationChooseDialog()
{
    if(zv_normaDialog)
    {
        delete zv_normaDialog;
    }
}
//============================================================
bool ZEquationChooseDialog::eventFilter(QObject * receiver, QEvent *event)
{
    if(receiver == zv_normTermLineEdit
            && event->type() == QEvent::MouseButtonDblClick
            && zv_fractionalEquationRadioButton->isChecked())
    {
        zh_runNormalizationDialog();
        return true;
    }

    return QObject::eventFilter(receiver, event);
}
//============================================================
void ZEquationChooseDialog::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // simple polynom
    QHBoxLayout* simplePolynomLayout = new QHBoxLayout;
    mainLayout->addLayout(simplePolynomLayout);

    zv_simplePolynomEquationRadioButton = new QRadioButton(this);
    simplePolynomLayout->addWidget(zv_simplePolynomEquationRadioButton);
    zv_simplePolynomEquationRadioButton->setText(ZCalibration::zp_equationTypeString(ZCalibration::ET_POLYNOMIAL));

    QLabel* label = new QLabel(this);
    label->setText(ZCalibration::simplePolynomEquationString);
    simplePolynomLayout->addWidget(label);
    simplePolynomLayout->addStretch();

    // fractional
    QHBoxLayout* fractionalLayout = new QHBoxLayout;
    mainLayout->addLayout(fractionalLayout);

    zv_fractionalEquationRadioButton = new QRadioButton(this);
    fractionalLayout->addWidget(zv_fractionalEquationRadioButton);
    zv_fractionalEquationRadioButton->setText(ZCalibration::zp_equationTypeString(ZCalibration::ET_FRACTIONAL));

    label = new QLabel(this);
    label->setText(ZCalibration::fractionalEquationString);
    fractionalLayout->addWidget(label);
    fractionalLayout->addStretch();

    // base term
    QHBoxLayout* baseTermLayout = new QHBoxLayout;
    mainLayout->addLayout(baseTermLayout);

    label = new QLabel(this);
    label->setText(tr("Base term:"));
    baseTermLayout->addWidget(label);

    zv_baseTermComboBox = new QComboBox(this);
    zv_baseTermComboBox->setEnabled(false);
    baseTermLayout->addWidget(zv_baseTermComboBox);

    baseTermLayout->addSpacing(10);

    label = new QLabel(this);
    label->setText(tr("Normalization term:"));
    baseTermLayout->addWidget(label);

    zv_normTermLineEdit = new QLineEdit(this);
    zv_normTermLineEdit->setEnabled(false);
    zv_normTermLineEdit->setReadOnly(true);
    zv_normTermLineEdit->installEventFilter(this);
    baseTermLayout->addWidget(zv_normTermLineEdit);

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
    connect(zv_simplePolynomEquationRadioButton, &QRadioButton::toggled,
            zv_normTermLineEdit, &QLineEdit::setDisabled);
    connect(zv_fractionalEquationRadioButton, &QRadioButton::toggled,
            zv_normTermLineEdit, &QLineEdit::setEnabled);

}
//============================================================
void ZEquationChooseDialog::zh_onDialogAccepted()
{
    setResult(QDialog::Accepted);
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
    zh_setNormalizationToLineEdit(settings.fractionalBaseNormaSettingsData);
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

    settings.fractionalBaseNormaSettingsData = zv_fractionalBaseNormaSettingsData;

    return settings;
}
//============================================================
void ZEquationChooseDialog::zh_runNormalizationDialog()
{
    zv_normaDialog = new ZNormaSettingsDialog();

    //  dialog position
    QPoint dialogPos = mapToGlobal(zv_normTermLineEdit->geometry().bottomLeft());
    dialogPos.setY(dialogPos.y() + 5);
    zv_normaDialog->move(dialogPos);
    zv_normaDialog->zp_setNormaSettings(zv_fractionalBaseNormaSettingsData);

    // exec and set
    if(zv_normaDialog->exec())
    {
        zh_setNormalizationToLineEdit(zv_normaDialog->zp_normaSettings());
    }

    delete zv_normaDialog;
    zv_normaDialog = 0;
}
//============================================================
void ZEquationChooseDialog::zh_setNormalizationToLineEdit(ZNormaSettingsData fractionalBaseNormaSettingsData)
{
    zv_fractionalBaseNormaSettingsData = fractionalBaseNormaSettingsData;
    QString normaString;
    if(zv_fractionalBaseNormaSettingsData.normaType == ZTermNormalizer::NT_CUSTOM)
    {
        normaString = zv_fractionalBaseNormaSettingsData.customNormaString;
    }
    else
    {
        normaString = ZTermNormalizer::zp_normaTypeString(zv_fractionalBaseNormaSettingsData.normaType);
    }

    zv_normTermLineEdit->setText(normaString);
}
//============================================================

//============================================================
#include "X_EquationChooseDialog.h"
#include "X_Calibration.h"
#include "X_NormaSettingsDialog.h"
#include "X_TermNormalizer.h"

#include <QApplication>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
//============================================================
X_EquationChooseDialog::X_EquationChooseDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle(tr("Equation settings"));
    setWindowFlags(Qt::Tool | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint);

    xh_createComponents();
    xh_createConnections();
}
//============================================================
X_EquationChooseDialog::~X_EquationChooseDialog() {}
//============================================================
bool X_EquationChooseDialog::eventFilter(QObject* receiver, QEvent* event)
{
    if (receiver == xv_normTermLineEdit && event->type() == QEvent::MouseButtonDblClick
        && xv_fractionalEquationRadioButton->isChecked())
    {
        xh_runNormalizationDialog();
        return true;
    }

    return QObject::eventFilter(receiver, event);
}
//============================================================
void X_EquationChooseDialog::xh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // simple polynom
    QHBoxLayout* simplePolynomLayout = new QHBoxLayout;
    mainLayout->addLayout(simplePolynomLayout);

    xv_simplePolynomEquationRadioButton = new QRadioButton(this);
    simplePolynomLayout->addWidget(xv_simplePolynomEquationRadioButton);
    xv_simplePolynomEquationRadioButton->setText(
        X_Calibration::xp_equationTypeString(X_Calibration::ET_POLYNOMIAL));

    QLabel* label = new QLabel(this);
    label->setText(X_Calibration::simplePolynomEquationString);
    simplePolynomLayout->addWidget(label);
    simplePolynomLayout->addStretch();

    // fractional
    QHBoxLayout* fractionalLayout = new QHBoxLayout;
    mainLayout->addLayout(fractionalLayout);

    xv_fractionalEquationRadioButton = new QRadioButton(this);
    fractionalLayout->addWidget(xv_fractionalEquationRadioButton);
    xv_fractionalEquationRadioButton->setText(
        X_Calibration::xp_equationTypeString(X_Calibration::ET_FRACTIONAL));

    label = new QLabel(this);
    label->setText(X_Calibration::fractionalEquationString);
    fractionalLayout->addWidget(label);
    fractionalLayout->addStretch();

    // base term
    QHBoxLayout* baseTermLayout = new QHBoxLayout;
    mainLayout->addLayout(baseTermLayout);

    label = new QLabel(this);
    label->setText(tr("Base term:"));
    baseTermLayout->addWidget(label);

    xv_baseTermComboBox = new QComboBox(this);
    xv_baseTermComboBox->setEnabled(false);
    baseTermLayout->addWidget(xv_baseTermComboBox);

    baseTermLayout->addSpacing(10);

    label = new QLabel(this);
    label->setText(tr("Normalization term:"));
    baseTermLayout->addWidget(label);

    xv_normTermLineEdit = new QLineEdit(this);
    xv_normTermLineEdit->setEnabled(false);
    xv_normTermLineEdit->setReadOnly(true);
    xv_normTermLineEdit->installEventFilter(this);
    baseTermLayout->addWidget(xv_normTermLineEdit);

    baseTermLayout->addStretch();

    // buttons
    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    xv_okButton = new QPushButton(tr("Ok"), this);
    buttonBox->addButton(xv_okButton, QDialogButtonBox::ActionRole);

    xv_cancelButton = new QPushButton(tr("Cancel"), this);
    buttonBox->addButton(xv_cancelButton, QDialogButtonBox::ActionRole);
}
//============================================================
void X_EquationChooseDialog::xh_createConnections()
{
    connect(xv_okButton, &QPushButton::clicked, this, &X_EquationChooseDialog::xh_onDialogAccepted);
    connect(xv_cancelButton, &QPushButton::clicked, this, &X_EquationChooseDialog::reject);
    connect(xv_simplePolynomEquationRadioButton,
            &QRadioButton::toggled,
            xv_baseTermComboBox,
            &QComboBox::setDisabled);
    connect(xv_fractionalEquationRadioButton,
            &QRadioButton::toggled,
            xv_baseTermComboBox,
            &QComboBox::setEnabled);
    connect(xv_simplePolynomEquationRadioButton,
            &QRadioButton::toggled,
            xv_normTermLineEdit,
            &QLineEdit::setDisabled);
    connect(xv_fractionalEquationRadioButton,
            &QRadioButton::toggled,
            xv_normTermLineEdit,
            &QLineEdit::setEnabled);
}
//============================================================
void X_EquationChooseDialog::xh_onDialogAccepted()
{
    setResult(QDialog::Accepted);
    accept();
}
//============================================================
void X_EquationChooseDialog::xp_setEquationSettings(const X_EquationSettingsData& settings)
{
    if (settings.equationType == X_Calibration::ET_POLYNOMIAL)
    {
        xv_simplePolynomEquationRadioButton->setChecked(true);
    }
    else if (settings.equationType == X_Calibration::ET_FRACTIONAL)
    {
        xv_fractionalEquationRadioButton->setChecked(true);
    }

    int currentTermIndex = 0;
    for (int i = 0; i < settings.termList.count(); i++)
    {
        xv_baseTermComboBox->insertItem(i,
                                        settings.termList.at(i).second,
                                        QVariant(settings.termList.at(i).first));
        if (settings.termList.at(i).first == settings.baseTermId)
        {
            currentTermIndex = i;
        }
    }

    xv_baseTermComboBox->setCurrentIndex(currentTermIndex);
    xh_setNormalizationToLineEdit(settings.fractionalBaseNormaSettingsData);
}
//============================================================
X_EquationSettingsData X_EquationChooseDialog::xp_equationSettings() const
{
    X_EquationSettingsData settings;
    QVariant vData = xv_baseTermComboBox->currentData();

    if (vData.isValid() && !vData.isNull() && vData.canConvert<qint64>())
    {
        bool ok;
        settings.baseTermId = (qint64) vData.toInt(&ok);

        if (!ok)
        {
            settings.baseTermId = -1;
        }
    }

    if (xv_simplePolynomEquationRadioButton->isChecked())
    {
        settings.equationType = X_Calibration::ET_POLYNOMIAL;
    }
    else if (xv_fractionalEquationRadioButton->isChecked())
    {
        settings.equationType = X_Calibration::ET_FRACTIONAL;
    }
    else
    {
        settings.equationType = X_Calibration::ET_NOT_DEFINED;
    }

    settings.fractionalBaseNormaSettingsData = xv_fractionalBaseNormaSettingsData;

    return settings;
}
//============================================================
void X_EquationChooseDialog::xh_runNormalizationDialog()
{
    X_NormaSettingsDialog* normaDialog = new X_NormaSettingsDialog();

    //  dialog position
    QPoint dialogPos = mapToGlobal(xv_normTermLineEdit->geometry().bottomLeft());
    dialogPos.setY(dialogPos.y() + 5);
    normaDialog->move(dialogPos);
    normaDialog->xp_setNormaSettings(xv_fractionalBaseNormaSettingsData);
    connect(this,
            &X_EquationChooseDialog::xg_currentCalibrationWindowName,
            normaDialog,
            &X_NormaSettingsDialog::xp_insertVariable);

    connect(normaDialog,
            &X_NormaSettingsDialog::accepted,
            this,
            &X_EquationChooseDialog::xh_onNormaSettingsDialogAccept);

    normaDialog->show();
}
//============================================================
void X_EquationChooseDialog::xh_onNormaSettingsDialogAccept()
{
    if (!sender())
    {
        return;
    }

    X_NormaSettingsDialog* normaDialog = qobject_cast<X_NormaSettingsDialog*>(sender());
    if (!normaDialog)
    {
        return;
    }

    xh_setNormalizationToLineEdit(normaDialog->xp_normaSettings());
}
//============================================================
void X_EquationChooseDialog::xh_setNormalizationToLineEdit(
    X_NormaSettingsData fractionalBaseNormaSettingsData)
{
    xv_fractionalBaseNormaSettingsData = fractionalBaseNormaSettingsData;
    QString normaString;
    if (xv_fractionalBaseNormaSettingsData.normaType == X_TermNormalizer::NT_CUSTOM)
    {
        normaString = xv_fractionalBaseNormaSettingsData.customNormaString;
    }
    else
    {
        normaString = X_TermNormalizer::xp_normaTypeString(
            xv_fractionalBaseNormaSettingsData.normaType);
    }

    xv_normTermLineEdit->setText(normaString);
}
//============================================================

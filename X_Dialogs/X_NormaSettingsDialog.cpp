//============================================================
#include "X_NormaSettingsDialog.h"

#include <QApplication>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSettings>

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
//============================================================
X_NormaSettingsDialog::X_NormaSettingsDialog(QWidget* parent) : X_BaseDialog(parent)
{
    setWindowTitle(tr("Normalization settings"));
    setWindowFlags(Qt::Tool | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_DeleteOnClose);

    xv_normaSettingsApplied = false;
    xh_createComponents();
    xh_createConnections();
    xh_restoreSettings();
}
//============================================================
X_NormaSettingsDialog::~X_NormaSettingsDialog()
{
    xh_saveSettings();
}
//============================================================
void X_NormaSettingsDialog::xh_createComponents()
{
    xh_addWidgetToMainLayout(xh_createControlWidget());
    xh_addStretchToMainLayout();

    xv_okButton = xh_createBasementButton(tr("OK"), tr("Save parameter"), QIcon());

    xv_closeButton = xh_createBasementButton(tr("Cancel"),
                                             tr("Close dialog without saving changes"),
                                             QIcon());
}
//============================================================
QWidget* X_NormaSettingsDialog::xh_createControlWidget()
{
    QWidget* widget = new QWidget(this);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    widget->setLayout(mainLayout);

    xv_noNormaRadioButton = new QRadioButton(this);
    xv_noNormaRadioButton->setText(X_TermNormalizer::xp_displayNormalizerTypeName(X_TermNormalizer::NT_NONE));
    xv_noNormaRadioButton->setChecked(true);
    mainLayout->addWidget(xv_noNormaRadioButton);

    xv_coherentNormaRadioButton = new QRadioButton(this);
    xv_coherentNormaRadioButton->setText(
        X_TermNormalizer::xp_displayNormalizerTypeName(X_TermNormalizer::NT_COHERENT));
    mainLayout->addWidget(xv_coherentNormaRadioButton);

    xv_incoherentNormaRadioButton = new QRadioButton(this);
    xv_incoherentNormaRadioButton->setText(
        X_TermNormalizer::xp_displayNormalizerTypeName(X_TermNormalizer::NT_INCOHERENT));
    mainLayout->addWidget(xv_incoherentNormaRadioButton);

    xv_coherentToIncoherentNormaRadioButton = new QRadioButton(this);
    xv_coherentToIncoherentNormaRadioButton->setText(
        X_TermNormalizer::xp_displayNormalizerTypeName(X_TermNormalizer::NT_COHERENT_INCOHERENT));
    mainLayout->addWidget(xv_coherentToIncoherentNormaRadioButton);

    xv_incoherentToCoherentNormaRadioButton = new QRadioButton(this);
    xv_incoherentToCoherentNormaRadioButton->setText(
        X_TermNormalizer::xp_displayNormalizerTypeName(X_TermNormalizer::NT_INCOHERENT_COHERENT));
    mainLayout->addWidget(xv_incoherentToCoherentNormaRadioButton);

    xv_customNormaRadioButton = new QRadioButton(this);
    xv_customNormaRadioButton->setText(
        X_TermNormalizer::xp_displayNormalizerTypeName(X_TermNormalizer::NT_CUSTOM));
    mainLayout->addWidget(xv_customNormaRadioButton);

    xv_customNormaLineEdit = new QLineEdit(this);
    xv_customNormaLineEdit->setDisabled(true);
    xv_customNormaLineEdit->setEnabled(false);
    mainLayout->addWidget(xv_customNormaLineEdit);

    // use modifier for inserting
    xv_useModifierForInsertingCheckBox = new QCheckBox(this);
    xv_useModifierForInsertingCheckBox->setText(tr("Use modifier for inserting window name"));
    xv_useModifierForInsertingCheckBox->setEnabled(false);
    mainLayout->addWidget(xv_useModifierForInsertingCheckBox);

    return widget;
}
//============================================================
void X_NormaSettingsDialog::xh_createConnections()
{
    connect(xv_okButton, &QPushButton::clicked,
            this, &X_NormaSettingsDialog::xh_onDialogAccepted);
    connect(xv_closeButton, &QPushButton::clicked,
            this, &X_NormaSettingsDialog::reject);

    connect(xv_noNormaRadioButton,
            &QRadioButton::toggled,
            xv_customNormaLineEdit,
            &QLineEdit::setDisabled);

    connect(xv_coherentNormaRadioButton,
            &QRadioButton::toggled,
            xv_customNormaLineEdit,
            &QLineEdit::setDisabled);
    connect(xv_incoherentNormaRadioButton,
            &QRadioButton::toggled,
            xv_customNormaLineEdit,
            &QLineEdit::setDisabled);
    connect(xv_coherentToIncoherentNormaRadioButton,
            &QRadioButton::toggled,
            xv_customNormaLineEdit,
            &QLineEdit::setDisabled);
    connect(xv_incoherentToCoherentNormaRadioButton,
            &QRadioButton::toggled,
            xv_customNormaLineEdit,
            &QLineEdit::setDisabled);
    connect(xv_customNormaRadioButton,
            &QRadioButton::toggled,
            xv_customNormaLineEdit,
            &QLineEdit::setEnabled);

    connect(xv_noNormaRadioButton,
            &QRadioButton::toggled,
            xv_useModifierForInsertingCheckBox,
            &QLineEdit::setDisabled);
    connect(xv_coherentNormaRadioButton,
            &QRadioButton::toggled,
            xv_useModifierForInsertingCheckBox,
            &QLineEdit::setDisabled);
    connect(xv_incoherentNormaRadioButton,
            &QRadioButton::toggled,
            xv_useModifierForInsertingCheckBox,
            &QLineEdit::setDisabled);
    connect(xv_coherentToIncoherentNormaRadioButton,
            &QRadioButton::toggled,
            xv_useModifierForInsertingCheckBox,
            &QLineEdit::setDisabled);
    connect(xv_incoherentToCoherentNormaRadioButton,
            &QRadioButton::toggled,
            xv_useModifierForInsertingCheckBox,
            &QLineEdit::setDisabled);
    connect(xv_customNormaRadioButton,
            &QRadioButton::toggled,
            xv_useModifierForInsertingCheckBox,
            &QLineEdit::setEnabled);


}
//=========================================================
void X_NormaSettingsDialog::xh_restoreSettings()
{
    X_BaseDialog::xh_restoreSettings();
    QSettings settings;
    xh_openDialogSettingsGroup(&settings);

    xv_useModifierForInsertingCheckBox->setChecked(settings.value(xv_useModifierString).toBool());

    xh_closeDialogSettingsGroup(&settings);
}
//=========================================================
void X_NormaSettingsDialog::xh_saveSettings()
{
    X_BaseDialog::xh_saveSettings();
    QSettings settings;
    xh_openDialogSettingsGroup(&settings);

    settings.setValue(xv_useModifierString, xv_useModifierForInsertingCheckBox->isChecked());

    xh_closeDialogSettingsGroup(&settings);
}
//============================================================
void X_NormaSettingsDialog::xh_onDialogAccepted()
{
    setResult(QDialog::Accepted);
    accept();
}
//============================================================
void X_NormaSettingsDialog::xp_setNormaSettings(const X_NormaSettingsData& settings)
{
    if(xv_normaSettingsApplied)
    {
        return;
    }

    switch (settings.normaType)
    {
    case X_TermNormalizer::NT_NONE:
        xv_noNormaRadioButton->setChecked(true);
        break;
    case X_TermNormalizer::NT_COHERENT:
        xv_coherentNormaRadioButton->setChecked(true);
        break;
    case X_TermNormalizer::NT_INCOHERENT:
        xv_incoherentNormaRadioButton->setChecked(true);
        break;
    case X_TermNormalizer::NT_COHERENT_INCOHERENT:
        xv_coherentToIncoherentNormaRadioButton->setChecked(true);
        break;
    case X_TermNormalizer::NT_INCOHERENT_COHERENT:
        xv_incoherentToCoherentNormaRadioButton->setChecked(true);
        break;
    case X_TermNormalizer::NT_CUSTOM:
        xv_customNormaRadioButton->setChecked(true);
        break;
    }

    xv_customNormaLineEdit->setText(settings.customNormaString);
    xv_normaSettingsApplied = true;
    // TODO xp_setNormaSettings termList
    // settings.termList;
}
//============================================================
X_NormaSettingsData X_NormaSettingsDialog::xp_normaSettings() const
{
    X_NormaSettingsData settings;

    settings.customNormaString = xv_customNormaLineEdit->text();

    if (xv_noNormaRadioButton->isChecked())
    {
        settings.normaType = X_TermNormalizer::NT_NONE;
    }
    else if (xv_coherentNormaRadioButton->isChecked())
    {
        settings.normaType = X_TermNormalizer::NT_COHERENT;
    }
    else if (xv_incoherentNormaRadioButton->isChecked())
    {
        settings.normaType = X_TermNormalizer::NT_INCOHERENT;
    }
    else if (xv_coherentToIncoherentNormaRadioButton->isChecked())
    {
        settings.normaType = X_TermNormalizer::NT_COHERENT_INCOHERENT;
    }
    else if (xv_incoherentToCoherentNormaRadioButton->isChecked())
    {
        settings.normaType = X_TermNormalizer::NT_INCOHERENT_COHERENT;
    }
    else if (xv_customNormaRadioButton->isChecked())
    {
        settings.normaType = X_TermNormalizer::NT_CUSTOM;
    }
    else
    {
        settings.normaType = X_TermNormalizer::NT_NONE;
    }

    return settings;
}
//============================================================
void X_NormaSettingsDialog::xp_insertVariable(const QString& variableName)
{
    if (!xv_useModifierForInsertingCheckBox->isChecked() || qApp->keyboardModifiers() & Qt::ALT)
    {
        activateWindow();
        xv_customNormaLineEdit->setFocus();
        xv_customNormaLineEdit->insert(variableName);
    }
}
//============================================================

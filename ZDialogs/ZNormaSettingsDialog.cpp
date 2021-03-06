//============================================================
#include "ZNormaSettingsDialog.h"
#include "ZGeneral.h"

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
ZNormaSettingsDialog::ZNormaSettingsDialog(QWidget* parent) : ZBaseDialog(parent)
{
    setWindowTitle(tr("Normalization settings"));
    setWindowFlags(Qt::Tool | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_DeleteOnClose);

    zv_normaSettingsApplied = false;
    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();
}
//============================================================
ZNormaSettingsDialog::~ZNormaSettingsDialog()
{
    zh_saveSettings();
}
//============================================================
void ZNormaSettingsDialog::zh_createComponents()
{
    zh_addWidgetToMainLayout(zh_createControlWidget());
    zh_addStretchToMainLayout();

    zv_okButton = zh_createBasementButton(tr("OK"), tr("Save parameter"), QIcon());

    zv_closeButton = zh_createBasementButton(tr("Cancel"),
                                             tr("Close dialog withiut saving changes"),
                                             QIcon());
}
//============================================================
QWidget* ZNormaSettingsDialog::zh_createControlWidget()
{
    QWidget* widget = new QWidget(this);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    widget->setLayout(mainLayout);

    zv_noNormaRadioButton = new QRadioButton(this);
    zv_noNormaRadioButton->setText(ZTermNormalizer::zp_normaTypeString(ZTermNormalizer::NT_NONE));
    zv_noNormaRadioButton->setChecked(true);
    mainLayout->addWidget(zv_noNormaRadioButton);

    zv_coherentNormaRadioButton = new QRadioButton(this);
    zv_coherentNormaRadioButton->setText(
        ZTermNormalizer::zp_normaTypeString(ZTermNormalizer::NT_COHERENT));
    mainLayout->addWidget(zv_coherentNormaRadioButton);

    zv_incoherentNormaRadioButton = new QRadioButton(this);
    zv_incoherentNormaRadioButton->setText(
        ZTermNormalizer::zp_normaTypeString(ZTermNormalizer::NT_INCOHERENT));
    mainLayout->addWidget(zv_incoherentNormaRadioButton);

    zv_coherentToIncoherentNormaRadioButton = new QRadioButton(this);
    zv_coherentToIncoherentNormaRadioButton->setText(
        ZTermNormalizer::zp_normaTypeString(ZTermNormalizer::NT_COHERENT_INCOHERENT));
    mainLayout->addWidget(zv_coherentToIncoherentNormaRadioButton);

    zv_incoherentToCoherentNormaRadioButton = new QRadioButton(this);
    zv_incoherentToCoherentNormaRadioButton->setText(
        ZTermNormalizer::zp_normaTypeString(ZTermNormalizer::NT_INCOHERENT_COHERENT));
    mainLayout->addWidget(zv_incoherentToCoherentNormaRadioButton);

    zv_customNormaRadioButton = new QRadioButton(this);
    zv_customNormaRadioButton->setText(
        ZTermNormalizer::zp_normaTypeString(ZTermNormalizer::NT_CUSTOM));
    mainLayout->addWidget(zv_customNormaRadioButton);

    zv_customNormaLineEdit = new QLineEdit(this);
    zv_customNormaLineEdit->setDisabled(true);
    zv_customNormaLineEdit->setEnabled(false);
    mainLayout->addWidget(zv_customNormaLineEdit);

    // use modifier for inserting
    zv_useModifierForInsertingCheckBox = new QCheckBox(this);
    zv_useModifierForInsertingCheckBox->setText(tr("Use modifier for inserting window name"));
    zv_useModifierForInsertingCheckBox->setEnabled(false);
    mainLayout->addWidget(zv_useModifierForInsertingCheckBox);

    return widget;
}
//============================================================
void ZNormaSettingsDialog::zh_createConnections()
{
    connect(zv_okButton, &QPushButton::clicked, this, &ZNormaSettingsDialog::zh_onDialogAccepted);
    connect(zv_closeButton, &QPushButton::clicked, this, &ZNormaSettingsDialog::reject);

    connect(zv_noNormaRadioButton,
            &QRadioButton::toggled,
            zv_customNormaLineEdit,
            &QLineEdit::setDisabled);

    connect(zv_coherentNormaRadioButton,
            &QRadioButton::toggled,
            zv_customNormaLineEdit,
            &QLineEdit::setDisabled);
    connect(zv_incoherentNormaRadioButton,
            &QRadioButton::toggled,
            zv_customNormaLineEdit,
            &QLineEdit::setDisabled);
    connect(zv_coherentToIncoherentNormaRadioButton,
            &QRadioButton::toggled,
            zv_customNormaLineEdit,
            &QLineEdit::setDisabled);
    connect(zv_incoherentToCoherentNormaRadioButton,
            &QRadioButton::toggled,
            zv_customNormaLineEdit,
            &QLineEdit::setDisabled);
    connect(zv_customNormaRadioButton,
            &QRadioButton::toggled,
            zv_customNormaLineEdit,
            &QLineEdit::setEnabled);

    connect(zv_noNormaRadioButton,
            &QRadioButton::toggled,
            zv_useModifierForInsertingCheckBox,
            &QLineEdit::setDisabled);
    connect(zv_coherentNormaRadioButton,
            &QRadioButton::toggled,
            zv_useModifierForInsertingCheckBox,
            &QLineEdit::setDisabled);
    connect(zv_incoherentNormaRadioButton,
            &QRadioButton::toggled,
            zv_useModifierForInsertingCheckBox,
            &QLineEdit::setDisabled);
    connect(zv_coherentToIncoherentNormaRadioButton,
            &QRadioButton::toggled,
            zv_useModifierForInsertingCheckBox,
            &QLineEdit::setDisabled);
    connect(zv_incoherentToCoherentNormaRadioButton,
            &QRadioButton::toggled,
            zv_useModifierForInsertingCheckBox,
            &QLineEdit::setDisabled);
    connect(zv_customNormaRadioButton,
            &QRadioButton::toggled,
            zv_useModifierForInsertingCheckBox,
            &QLineEdit::setEnabled);


}
//=========================================================
void ZNormaSettingsDialog::zh_restoreSettings()
{
    ZBaseDialog::zh_restoreSettings();
    QSettings settings;
    zh_openDialogSettingsGroup(&settings);

    zv_useModifierForInsertingCheckBox->setChecked(settings.value(zv_useModifierString).toBool());

    zh_closeDialogSettingsGroup(&settings);
}
//=========================================================
void ZNormaSettingsDialog::zh_saveSettings()
{
    ZBaseDialog::zh_saveSettings();
    QSettings settings;
    zh_openDialogSettingsGroup(&settings);

    settings.setValue(zv_useModifierString, zv_useModifierForInsertingCheckBox->isChecked());

    zh_closeDialogSettingsGroup(&settings);
}
//============================================================
void ZNormaSettingsDialog::zh_onDialogAccepted()
{
    setResult(QDialog::Accepted);
    accept();
}
//============================================================
void ZNormaSettingsDialog::zp_setNormaSettings(const ZNormaSettingsData& settings)
{
    if(zv_normaSettingsApplied)
    {
        return;
    }

    switch (settings.normaType)
    {
    case ZTermNormalizer::NT_NONE:
        zv_noNormaRadioButton->setChecked(true);
        break;
    case ZTermNormalizer::NT_COHERENT:
        zv_coherentNormaRadioButton->setChecked(true);
        break;
    case ZTermNormalizer::NT_INCOHERENT:
        zv_incoherentNormaRadioButton->setChecked(true);
        break;
    case ZTermNormalizer::NT_COHERENT_INCOHERENT:
        zv_coherentToIncoherentNormaRadioButton->setChecked(true);
        break;
    case ZTermNormalizer::NT_INCOHERENT_COHERENT:
        zv_incoherentToCoherentNormaRadioButton->setChecked(true);
        break;
    case ZTermNormalizer::NT_CUSTOM:
        zv_customNormaRadioButton->setChecked(true);
        break;
    }

    zv_customNormaLineEdit->setText(settings.customNormaString);
    zv_normaSettingsApplied = true;
    // TODO zp_setNormaSettings termList
    // settings.termList;
}
//============================================================
ZNormaSettingsData ZNormaSettingsDialog::zp_normaSettings() const
{
    ZNormaSettingsData settings;

    settings.customNormaString = zv_customNormaLineEdit->text();

    if (zv_noNormaRadioButton->isChecked())
    {
        settings.normaType = ZTermNormalizer::NT_NONE;
    }
    else if (zv_coherentNormaRadioButton->isChecked())
    {
        settings.normaType = ZTermNormalizer::NT_COHERENT;
    }
    else if (zv_incoherentNormaRadioButton->isChecked())
    {
        settings.normaType = ZTermNormalizer::NT_INCOHERENT;
    }
    else if (zv_coherentToIncoherentNormaRadioButton->isChecked())
    {
        settings.normaType = ZTermNormalizer::NT_COHERENT_INCOHERENT;
    }
    else if (zv_incoherentToCoherentNormaRadioButton->isChecked())
    {
        settings.normaType = ZTermNormalizer::NT_INCOHERENT_COHERENT;
    }
    else if (zv_customNormaRadioButton->isChecked())
    {
        settings.normaType = ZTermNormalizer::NT_CUSTOM;
    }
    else
    {
        settings.normaType = ZTermNormalizer::NT_NONE;
    }

    return settings;
}
//============================================================
void ZNormaSettingsDialog::zp_insertVariable(const QString& variableName)
{
    if (!zv_useModifierForInsertingCheckBox->isChecked() || qApp->keyboardModifiers() & Qt::ALT)
    {
        activateWindow();
        zv_customNormaLineEdit->setFocus();
        zv_customNormaLineEdit->insert(variableName);
    }
}
//============================================================

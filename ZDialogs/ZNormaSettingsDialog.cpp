//============================================================
#include "ZNormaSettingsDialog.h"
#include "ZGeneral.h"

#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>

#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
//============================================================
ZNormaSettingsDialog::ZNormaSettingsDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Normalization settings"));
    setWindowFlags(Qt::Tool | Qt::MSWindowsFixedSizeDialogHint
                   | Qt::WindowStaysOnTopHint);

    zh_createComponents();
    zh_createConnections();
}
//============================================================
void ZNormaSettingsDialog::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    zv_noNormaRadioButton = new QRadioButton(this);
    zv_noNormaRadioButton->setText(ZTermNormalizer::zp_normaTypeString(ZTermNormalizer::NT_NONE));
    zv_noNormaRadioButton->setChecked(true);
    mainLayout->addWidget(zv_noNormaRadioButton);

    zv_coherentNormaRadioButton = new QRadioButton(this);
    zv_coherentNormaRadioButton->setText(ZTermNormalizer::zp_normaTypeString(ZTermNormalizer::NT_COHERENT));
    mainLayout->addWidget(zv_coherentNormaRadioButton);

    zv_incoherentNormaRadioButton = new QRadioButton(this);
    zv_incoherentNormaRadioButton->setText(ZTermNormalizer::zp_normaTypeString(ZTermNormalizer::NT_INCOHERENT));
    mainLayout->addWidget(zv_incoherentNormaRadioButton);

    zv_coherentToIncoherentNormaRadioButton = new QRadioButton(this);
    zv_coherentToIncoherentNormaRadioButton->setText(ZTermNormalizer::zp_normaTypeString(ZTermNormalizer::NT_COHERENT_INCOHERENT));
    mainLayout->addWidget(zv_coherentToIncoherentNormaRadioButton);

    zv_incoherentToCoherentNormaRadioButton = new QRadioButton(this);
    zv_incoherentToCoherentNormaRadioButton->setText(ZTermNormalizer::zp_normaTypeString(ZTermNormalizer::NT_INCOHERENT_COHERENT));
    mainLayout->addWidget(zv_incoherentToCoherentNormaRadioButton);

    zv_customNormaRadioButton = new QRadioButton(this);
    zv_customNormaRadioButton->setText(ZTermNormalizer::zp_normaTypeString(ZTermNormalizer::NT_CUSTOM));
    mainLayout->addWidget(zv_customNormaRadioButton);

    zv_customNormaLineEdit = new QLineEdit(this);
    zv_customNormaLineEdit->setDisabled(true);
    mainLayout->addWidget(zv_customNormaLineEdit);

    // buttons
    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    zv_okButton = new QPushButton(this);
    buttonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(this);
    buttonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);
}
//============================================================
void ZNormaSettingsDialog::zh_createConnections()
{
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZNormaSettingsDialog::zh_onDialogAccepted);
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZNormaSettingsDialog::reject);

    connect(zv_noNormaRadioButton, &QRadioButton::toggled,
            zv_customNormaLineEdit, &QLineEdit::setDisabled);
    connect(zv_coherentNormaRadioButton, &QRadioButton::toggled,
            zv_customNormaLineEdit, &QLineEdit::setDisabled);
    connect(zv_incoherentNormaRadioButton, &QRadioButton::toggled,
            zv_customNormaLineEdit, &QLineEdit::setDisabled);
    connect(zv_coherentToIncoherentNormaRadioButton, &QRadioButton::toggled,
            zv_customNormaLineEdit, &QLineEdit::setDisabled);
    connect(zv_incoherentToCoherentNormaRadioButton, &QRadioButton::toggled,
            zv_customNormaLineEdit, &QLineEdit::setDisabled);
    connect(zv_customNormaRadioButton, &QRadioButton::toggled,
            zv_customNormaLineEdit, &QLineEdit::setEnabled);

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
    switch(settings.normaType)
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
    // TODO zp_setNormaSettings termList
    // settings.termList;
}
//============================================================
ZNormaSettingsData ZNormaSettingsDialog::zp_normaSettings() const
{
    ZNormaSettingsData settings;

    settings.customNormaString = zv_customNormaLineEdit->text();

    if(zv_noNormaRadioButton->isChecked())
    {
        settings.normaType = ZTermNormalizer::NT_NONE;
    }
    else if(zv_coherentNormaRadioButton->isChecked())
    {
        settings.normaType = ZTermNormalizer::NT_COHERENT;
    }
    else if(zv_incoherentNormaRadioButton->isChecked())
    {
        settings.normaType = ZTermNormalizer::NT_INCOHERENT;
    }
    else if(zv_coherentToIncoherentNormaRadioButton->isChecked())
    {
        settings.normaType = ZTermNormalizer::NT_COHERENT_INCOHERENT;
    }
    else if(zv_incoherentToCoherentNormaRadioButton->isChecked())
    {
        settings.normaType = ZTermNormalizer::NT_INCOHERENT_COHERENT;
    }
    else if(zv_customNormaRadioButton->isChecked())
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

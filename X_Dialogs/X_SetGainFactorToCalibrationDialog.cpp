//=========================================================
#include "X_SetGainFactorToCalibrationDialog.h"
#include "X_General.h"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

//=========================================================
X_SetGainFactorToCalibrationDialog::X_SetGainFactorToCalibrationDialog(const QString& calibrationName, QWidget *parent)
    : QDialog(parent)
{
    xh_createComponents(calibrationName);
    xh_createConnections();
}
//=========================================================
void X_SetGainFactorToCalibrationDialog::xh_createComponents(const QString& calibrationName)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QLabel* label = new QLabel(this);
    QString msg = tr("Input gain factor value for the calibration \"%1\".").arg(calibrationName);

    label->setText(msg);
    mainLayout->addWidget(label);

    QHBoxLayout* levelLayout = new QHBoxLayout;
    mainLayout->addLayout(levelLayout);
    xv_gainFactorSpinBox = new QSpinBox(this);
    xv_gainFactorSpinBox->setRange(1, glMaxGainFactor);
    levelLayout->addWidget(xv_gainFactorSpinBox);
    levelLayout->addStretch();

    // buttons
    QDialogButtonBox* buttonBox = new QDialogButtonBox;
    mainLayout->addWidget(buttonBox);

    xv_okButton = new QPushButton(this);
    xv_okButton->setText(tr("Ok"));
    buttonBox->addButton(xv_okButton, QDialogButtonBox::ActionRole);
    xv_cancelButton = new QPushButton(this);
    xv_cancelButton->setText(tr("Cancel"));

    buttonBox->addButton(xv_cancelButton, QDialogButtonBox::ActionRole);
}
//=========================================================
void X_SetGainFactorToCalibrationDialog::xh_createConnections()
{
    connect(xv_okButton, &QPushButton::clicked,
            this, &X_SetGainFactorToCalibrationDialog::accept);
    connect(xv_cancelButton, &QPushButton::clicked,
            this, &X_SetGainFactorToCalibrationDialog::reject);

}
//=========================================================
int X_SetGainFactorToCalibrationDialog::xp_gainFactor() const
{
    return xv_gainFactorSpinBox->value();
}
//=========================================================

//======================================================
#include "X_EnergyCalibrationDialog.h"
#include "X_General.h"
#include "X_CaptionFormatter.h"
#include "X_BaseTableWidget.h"
#include "X_SpeSpectrum.h"
#include "X_PeakWindowTableModel.h"

#include <QApplication>
#include <limits>
#include <QDialogButtonBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QVBoxLayout>

//======================================================
X_EnergyCalibrationDialog::X_EnergyCalibrationDialog(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("Energy calbration"));
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Tool);
    zh_createComponents();
    zh_createConnections();
    zh_restoreSettings();
}
//======================================================
X_EnergyCalibrationDialog::~X_EnergyCalibrationDialog()
{
    zh_saveSettings();
}
//======================================================
void X_EnergyCalibrationDialog::zh_createComponents()
{
    // invisible components
    xv_peakWindowTableModel = new X_PeakWindowTableModel(this);

    // layouts
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // control widget
    mainLayout->addWidget(zh_createControlWidget());

    // sparator line
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    mainLayout->addWidget(line);

    // bottom
    QDialogButtonBox* bottomButtonBox = new QDialogButtonBox(Qt::Horizontal, this);
    mainLayout->addWidget(bottomButtonBox);

    xv_okButton = new QPushButton(this);
    xv_okButton->setText(tr("OK"));
    bottomButtonBox->addButton(xv_okButton, QDialogButtonBox::ActionRole);

    xv_cancelButton = new QPushButton(this);
    xv_cancelButton->setText(tr("Cancel"));
    bottomButtonBox->addButton(xv_cancelButton, QDialogButtonBox::ActionRole);

    xv_cancelButton->setFocus();
}
//======================================================
QWidget* X_EnergyCalibrationDialog::zh_createControlWidget()
{
    QWidget* controlWidget = new QWidget();

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    controlWidget->setLayout(mainLayout);

    // gain factor spinbox
    QHBoxLayout* spinBoxLayout = new QHBoxLayout;
    QLabel* label = new QLabel(this);
    QString caption = tr("Gain factor:");
    label->setText(X_CaptionFormatter::xp_format(caption));
    spinBoxLayout->addWidget(label);

    xv_gainFactorSpinBox = new QSpinBox(this);
    xv_gainFactorSpinBox->setMinimum(0);
    xv_gainFactorSpinBox->setMaximum(std::numeric_limits<quint8>::max());
    spinBoxLayout->addWidget(xv_gainFactorSpinBox);
    spinBoxLayout->addStretch();
    mainLayout->addLayout(spinBoxLayout);

    xv_peakTableWidget = new X_BaseTableWidget(this);
    mainLayout->addWidget(xv_peakTableWidget);

    return controlWidget;
}
//======================================================
void X_EnergyCalibrationDialog::zh_createConnections()
{
    connect(xv_cancelButton, &QPushButton::clicked,
            this, &X_EnergyCalibrationDialog::close);
    connect(xv_okButton, &QPushButton::clicked,
            this, &X_EnergyCalibrationDialog::zh_calculateSaveAndClose);

    connect(xv_okButton, &QPushButton::clicked,
            this, &X_EnergyCalibrationDialog::zh_calculateSaveAndClose);
    connect(xv_okButton, &QPushButton::clicked,
            this, &X_EnergyCalibrationDialog::zh_calculateSaveAndClose);

    xv_peakTableWidget->xp_setModel(xv_peakWindowTableModel);
    connect(xv_peakWindowTableModel, &X_PeakWindowTableModel::xg_inquiryCurrentIndex,
            xv_peakTableWidget, &X_BaseTableWidget::xp_currentIndex);
    connect(xv_peakWindowTableModel, &X_PeakWindowTableModel::xg_inquiryCurrentVisibleSceneRect,
            this, &X_EnergyCalibrationDialog::xg_inquiryCurrentVisibleSceneRect);

    xv_peakTableWidget->xp_appendButtonActions(xv_peakWindowTableModel->xp_buttonActions());
    xv_peakTableWidget->xp_appendContextActions(xv_peakWindowTableModel->xp_contextActions());
}
//======================================================
void X_EnergyCalibrationDialog::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("EnergyCalibratorWidget");

    vData = settings.value("widgetGeometry");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        this->restoreGeometry(vData.value<QByteArray>());
    }

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void X_EnergyCalibrationDialog::zh_saveSettings() const
{
    QSettings settings;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("EnergyCalibratorWidget");

    settings.setValue("widgetGeometry", QVariant::fromValue<QByteArray>(this->saveGeometry()));
    //settings.setValue("splitterGeometry", QVariant::fromValue<QByteArray>(xv_tableSplitter->saveState()));

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void X_EnergyCalibrationDialog::xp_setMeasuringConditionsAndSpectrum(quint8 gainFactor,
                                                                    int exposition,
                                                                    const X_SpeSpectrum* spectrum)
{

    QString spectrumName = spectrum ? spectrum->xp_name() : "NON SPECTRUM";

    // xv_gainFactorSpinBox->setValue(gainFactor);

}
//======================================================
void X_EnergyCalibrationDialog::zh_calculateSaveAndClose()
{

    close();

}
//======================================================

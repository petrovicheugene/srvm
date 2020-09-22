//=========================================================
#include "ZCustomTermDialog.h"
#include "ZCalibration.h"
#include "ZCaptionFormatter.h"

#include <QApplication>
#include <QCheckBox>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QTextEdit>
#include <QVBoxLayout>
//=========================================================
ZCustomTermDialog::ZCustomTermDialog(ZRawCustomTerm rawTerm) : ZBaseDialog()
{
    setWindowTitle(qApp->applicationDisplayName());

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlag(Qt::WindowStaysOnTopHint);

    zv_rawTerm = rawTerm;
    zh_createComponents();
    zh_createConnections();

    zh_restoreSettings();

    if (zv_rawTerm.termType != ZAbstractTerm::TT_CUSTOM)
    {
        zv_messageLabel->setText(
            QString("<font color=red>%1</font>").arg(tr("The parameter type is not \"Custom\"!")));
        zv_nameLineEdit->setEnabled(false);
        zv_expressionLineEdit->setEnabled(false);
        zv_descriptionTextEdit->setEnabled(false);
    }
}
//=========================================================
ZCustomTermDialog::ZCustomTermDialog(ZCalibration* calibration, int termIndex)
{
    setWindowTitle(qApp->applicationDisplayName());

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlag(Qt::WindowStaysOnTopHint);

    zv_rawTerm = ZRawCustomTerm(ZAbstractTerm::TT_CUSTOM);
    zv_rawTerm.termState = ZAbstractTerm::TS_CONST_EXCLUDED;


    zv_calibration = calibration;
    zh_createComponents();
    zh_createConnections();

    zh_restoreSettings();

    if (termIndex > -1)
    {
        // raw for edited term
        zv_rawTerm.termId = calibration->zp_termId(termIndex);
        zv_nameLineEdit->setText(calibration->zp_termName(termIndex));
        zv_expressionLineEdit->setText(calibration->zp_termExpression(termIndex));
        zv_descriptionTextEdit->setText(calibration->zp_termDescription(termIndex));
    }

//    if (zv_rawTerm.termType != ZAbstractTerm::TT_CUSTOM)
//    {
//        zv_messageLabel->setText(
//            QString("<font color=red>%1</font>").arg(tr("The parameter type is not \"Custom\"!")));
//        zv_nameLineEdit->setEnabled(false);
//        zv_expressionLineEdit->setEnabled(false);
//        zv_descriptionTextEdit->setEnabled(false);
//    }
}
//=========================================================
ZCustomTermDialog::~ZCustomTermDialog()
{
    zh_saveSettings();
}
//=========================================================
void ZCustomTermDialog::zh_restoreSettings()
{
    ZBaseDialog::zh_restoreSettings();
    QSettings settings;
    zh_openDialogSettingsGroup(&settings);

    zv_useModifierForInsertingCheckBox->setChecked(settings.value(zv_useModifierString).toBool());

    zh_closeDialogSettingsGroup(&settings);
}
//=========================================================
void ZCustomTermDialog::zh_saveSettings()
{
    ZBaseDialog::zh_saveSettings();
    QSettings settings;
    zh_openDialogSettingsGroup(&settings);

    settings.setValue(zv_useModifierString, zv_useModifierForInsertingCheckBox->isChecked());

    zh_closeDialogSettingsGroup(&settings);
}
//=========================================================
void ZCustomTermDialog::zh_createComponents()
{
    zh_addWidgetToMainLayout(zh_createControlWidget());
    zh_addStretchToMainLayout();

    zv_okButton = zh_createBasementButton(tr("OK"), tr("Save parameter"), QIcon());

    zv_closeButton = zh_createBasementButton(tr("Close"),
                                              tr("Close dialog"),
                                              QIcon());
}
//=========================================================
void ZCustomTermDialog::zp_showMsg(QString msg)
{
    zv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
}
//=========================================================
void ZCustomTermDialog::zp_insertVariable(const QString& variableName)
{
    if (!zv_useModifierForInsertingCheckBox->isChecked() || qApp->keyboardModifiers() & Qt::ALT)
    {
        activateWindow();
        zv_expressionLineEdit->setFocus();
        zv_expressionLineEdit->insert(variableName);
    }
}
//=========================================================
void ZCustomTermDialog::zh_onOkButtonClick()
{
    zv_messageLabel->clear();
    bool res = false;
    zv_rawTerm.name = zv_nameLineEdit->text();
    zv_rawTerm.customString = zv_expressionLineEdit->text();
    zv_rawTerm.descriptionString = zv_descriptionTextEdit->toPlainText();

    zv_calibration->zp_updateCustomTerm(res, zv_rawTerm);

    if (res)
    {
        reject();
    }

    //emit zg_requestTermUpdate(res, zv_rawTerm);
}
//=========================================================
void ZCustomTermDialog::zh_onCloseButtonClick()
{
    reject();
}
//=========================================================
void ZCustomTermDialog::zh_createConnections()
{
    connect(zv_okButton, &QPushButton::clicked, this, &ZCustomTermDialog::zh_onOkButtonClick);
    connect(zv_closeButton, &QPushButton::clicked, this, &ZCustomTermDialog::zh_onCloseButtonClick);
    connect(zv_calibration, &ZCalibration::zg_message, this, &ZCustomTermDialog::zp_showMsg);
}
//=========================================================
QWidget* ZCustomTermDialog::zh_createControlWidget()
{
    QWidget* widget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    widget->setLayout(mainLayout);

    // name
    QLabel* label = new QLabel(this);
    QString caption = tr("Term name:");
    label->setText(ZCaptionFormatter::zp_format(caption));
    mainLayout->addWidget(label);
    zv_nameLineEdit = new QLineEdit(this);
    label->setBuddy(zv_nameLineEdit);
    mainLayout->addWidget(zv_nameLineEdit);

    // equation
    label = new QLabel(this);
    caption = tr("Equation:");
    label->setText(ZCaptionFormatter::zp_format(caption));
    mainLayout->addWidget(label);
    zv_expressionLineEdit = new QLineEdit(this);
    label->setBuddy(zv_expressionLineEdit);
    mainLayout->addWidget(zv_expressionLineEdit);

    // description
    label = new QLabel(this);
    caption = tr("Description:");
    label->setText(ZCaptionFormatter::zp_format(caption));
    mainLayout->addWidget(label);
    zv_descriptionTextEdit = new QTextEdit(this);
    label->setBuddy(zv_descriptionTextEdit);
    mainLayout->addWidget(zv_descriptionTextEdit);

    // use modifier for inserting
    zv_useModifierForInsertingCheckBox = new QCheckBox(this);
    zv_useModifierForInsertingCheckBox->setText(tr("Use modifier for inserting window name"));
    mainLayout->addWidget(zv_useModifierForInsertingCheckBox);
    // message label
    zv_messageLabel = new QLabel(this);
    mainLayout->addWidget(zv_messageLabel);

    return widget;
}
//=========================================================

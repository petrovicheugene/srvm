//=========================================================
#include "X_CustomTermDialog.h"
#include "X_Calibration.h"
#include "X_CaptionFormatter.h"

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
X_CustomTermDialog::X_CustomTermDialog(X_RawCustomTerm rawTerm) : X_BaseDialog()
{
    setWindowTitle(qApp->applicationDisplayName());

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlag(Qt::WindowStaysOnTopHint);

    xv_rawTerm = rawTerm;
    zh_createComponents();
    zh_createConnections();

    zh_restoreSettings();

    if (xv_rawTerm.termType != X_AbstractTerm::TT_CUSTOM)
    {
        xv_messageLabel->setText(
            QString("<font color=red>%1</font>").arg(tr("The parameter type is not \"Custom\"!")));
        xv_nameLineEdit->setEnabled(false);
        xv_expressionLineEdit->setEnabled(false);
        xv_descriptionTextEdit->setEnabled(false);
    }
}
//=========================================================
X_CustomTermDialog::X_CustomTermDialog(X_Calibration* calibration, int termIndex)
{
    setWindowTitle(qApp->applicationDisplayName());

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlag(Qt::WindowStaysOnTopHint);

    xv_rawTerm = X_RawCustomTerm(X_AbstractTerm::TT_CUSTOM);
    xv_rawTerm.termState = X_AbstractTerm::TS_CONST_EXCLUDED;

    xv_calibration = calibration;
    zh_createComponents();
    zh_createConnections();

    zh_restoreSettings();

    if (termIndex > -1)
    {
        // raw for edited term
        xv_rawTerm.termId = calibration->xp_termId(termIndex);
        xv_nameLineEdit->setText(calibration->xp_termName(termIndex));
        xv_expressionLineEdit->setText(calibration->xp_termExpression(termIndex));
        xv_descriptionTextEdit->setText(calibration->xp_termDescription(termIndex));
    }

//    if (xv_rawTerm.termType != X_AbstractTerm::TT_CUSTOM)
//    {
//        xv_messageLabel->setText(
//            QString("<font color=red>%1</font>").arg(tr("The parameter type is not \"Custom\"!")));
//        xv_nameLineEdit->setEnabled(false);
//        xv_expressionLineEdit->setEnabled(false);
//        xv_descriptionTextEdit->setEnabled(false);
//    }
}
//=========================================================
X_CustomTermDialog::~X_CustomTermDialog()
{
    zh_saveSettings();
}
//=========================================================
void X_CustomTermDialog::zh_restoreSettings()
{
    X_BaseDialog::zh_restoreSettings();
    QSettings settings;
    zh_openDialogSettingsGroup(&settings);

    xv_useModifierForInsertingCheckBox->setChecked(settings.value(xv_useModifierString).toBool());

    zh_closeDialogSettingsGroup(&settings);
}
//=========================================================
void X_CustomTermDialog::zh_saveSettings()
{
    X_BaseDialog::zh_saveSettings();
    QSettings settings;
    zh_openDialogSettingsGroup(&settings);

    settings.setValue(xv_useModifierString, xv_useModifierForInsertingCheckBox->isChecked());

    zh_closeDialogSettingsGroup(&settings);
}
//=========================================================
void X_CustomTermDialog::zh_createComponents()
{
    zh_addWidgetToMainLayout(zh_createControlWidget());
    zh_addStretchToMainLayout();

    xv_okButton = zh_createBasementButton(tr("OK"), tr("Save parameter"), QIcon());

    xv_closeButton = zh_createBasementButton(tr("Close"),
                                              tr("Close dialog"),
                                              QIcon());
}
//=========================================================
void X_CustomTermDialog::xp_showMsg(QString msg)
{
    xv_messageLabel->setText(QString("<font color=red>%1</font>").arg(msg));
}
//=========================================================
void X_CustomTermDialog::xp_insertVariable(const QString& variableName)
{
    if (!xv_useModifierForInsertingCheckBox->isChecked() || qApp->keyboardModifiers() & Qt::ALT)
    {
        activateWindow();
        xv_expressionLineEdit->setFocus();
        xv_expressionLineEdit->insert(variableName);
    }
}
//=========================================================
void X_CustomTermDialog::zh_onOkButtonClick()
{
    xv_messageLabel->clear();
    bool res = false;
    xv_rawTerm.name = xv_nameLineEdit->text();
    xv_rawTerm.customString = xv_expressionLineEdit->text();
    xv_rawTerm.descriptionString = xv_descriptionTextEdit->toPlainText();

    xv_calibration->xp_updateCustomTerm(res, xv_rawTerm);

    if (res)
    {
        reject();
    }

    //emit xg_requestTermUpdate(res, xv_rawTerm);
}
//=========================================================
void X_CustomTermDialog::zh_onCloseButtonClick()
{
    reject();
}
//=========================================================
void X_CustomTermDialog::zh_createConnections()
{
    connect(xv_okButton, &QPushButton::clicked, this, &X_CustomTermDialog::zh_onOkButtonClick);
    connect(xv_closeButton, &QPushButton::clicked, this, &X_CustomTermDialog::zh_onCloseButtonClick);
    connect(xv_calibration, &X_Calibration::xg_message, this, &X_CustomTermDialog::xp_showMsg);
}
//=========================================================
QWidget* X_CustomTermDialog::zh_createControlWidget()
{
    QWidget* widget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    widget->setLayout(mainLayout);

    // name
    QLabel* label = new QLabel(this);
    QString caption = tr("Term name:");
    label->setText(X_CaptionFormatter::xp_format(caption));
    mainLayout->addWidget(label);
    xv_nameLineEdit = new QLineEdit(this);
    label->setBuddy(xv_nameLineEdit);
    mainLayout->addWidget(xv_nameLineEdit);

    // equation
    label = new QLabel(this);
    caption = tr("Equation:");
    label->setText(X_CaptionFormatter::xp_format(caption));
    mainLayout->addWidget(label);
    xv_expressionLineEdit = new QLineEdit(this);
    label->setBuddy(xv_expressionLineEdit);
    mainLayout->addWidget(xv_expressionLineEdit);

    // description
    label = new QLabel(this);
    caption = tr("Description:");
    label->setText(X_CaptionFormatter::xp_format(caption));
    mainLayout->addWidget(label);
    xv_descriptionTextEdit = new QTextEdit(this);
    label->setBuddy(xv_descriptionTextEdit);
    mainLayout->addWidget(xv_descriptionTextEdit);

    // use modifier for inserting
    xv_useModifierForInsertingCheckBox = new QCheckBox(this);
    xv_useModifierForInsertingCheckBox->setText(tr("Use modifier for inserting window name"));
    mainLayout->addWidget(xv_useModifierForInsertingCheckBox);
    // message label
    xv_messageLabel = new QLabel(this);
    mainLayout->addWidget(xv_messageLabel);

    return widget;
}
//=========================================================

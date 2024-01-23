//=========================================================
#ifndef X_CUSTOMTERMDIALOG_H
#define X_CUSTOMTERMDIALOG_H
//=========================================================
#include "X_BaseDialog.h"
#include "X_CustomTerm.h"
#include "X_RawTerm.h"
#include <QMap>
//=========================================================
class QCheckBox;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QLabel;

class X_Calibration;
//=========================================================
class X_CustomTermDialog : public X_BaseDialog
{
    Q_OBJECT
public:
    explicit X_CustomTermDialog(X_RawCustomTerm rawTerm = X_RawCustomTerm(X_AbstractTerm::TT_CUSTOM));
    explicit X_CustomTermDialog(X_Calibration* xv_calibration, int termIndex = -1);
    ~X_CustomTermDialog();

signals:

    void xg_requestTermUpdate(bool&res, X_RawCustomTerm& rawTerm) const;

public slots:

    void xp_showMsg(QString msg);
    void xp_insertVariable(const QString& variableName);

private slots:

    void xh_onOkButtonClick();
    void xh_onCloseButtonClick();

private:
    // VARS
    X_RawCustomTerm xv_rawTerm;
    X_Calibration* xv_calibration;

    QLineEdit* xv_nameLineEdit;
    QLineEdit* xv_expressionLineEdit;
    QTextEdit* xv_descriptionTextEdit;
    QLabel* xv_messageLabel;
    QCheckBox* xv_useModifierForInsertingCheckBox;
    QPushButton* xv_okButton;
    QPushButton* xv_closeButton;
    const QString xv_useModifierString = "useModifier";

    // FUNCS
    void xh_createComponents();
    void xh_createConnections();
    QWidget* xh_createControlWidget();

    void xh_restoreSettings() override;
    void xh_saveSettings() override;

};
//=========================================================
#endif // X_CUSTOMTERMDIALOG_H

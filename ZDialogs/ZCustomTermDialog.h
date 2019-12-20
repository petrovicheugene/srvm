//=========================================================
#ifndef ZCUSTOMTERMDIALOG_H
#define ZCUSTOMTERMDIALOG_H
//=========================================================
#include "ZBaseDialog.h"
#include "ZCustomTerm.h"
#include "ZRawTerm.h"
#include <QMap>
//=========================================================
class QCheckBox;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QLabel;

class ZCalibration;
//=========================================================
class ZCustomTermDialog : public ZBaseDialog
{
    Q_OBJECT
public:
    explicit ZCustomTermDialog(ZRawCustomTerm rawTerm = ZRawCustomTerm(ZAbstractTerm::TT_CUSTOM));
    explicit ZCustomTermDialog(ZCalibration* zv_calibration, int termIndex = -1);

    ~ZCustomTermDialog();


signals:

    void zg_requestTermUpdate(bool&res, ZRawCustomTerm& rawTerm) const;

public slots:

    void zp_showMsg(QString msg);
    void zp_insertVariable(const QString& variableName);

private slots:

    void zh_onOkButtonClick();
    void zh_onCloseButtonClick();

private:
    // VARS
    ZRawCustomTerm zv_rawTerm;
    ZCalibration* zv_calibration;

    QLineEdit* zv_nameLineEdit;
    QLineEdit* zv_expressionLineEdit;
    QTextEdit* zv_descriptionTextEdit;
    QLabel* zv_messageLabel;
    QCheckBox* zv_useModifierForInsertingCheckBox;
    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;
    const QString zv_useModifierString = "useModifier";

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    QWidget* zh_createControlWidget();

    void zh_restoreSettings() override;
    void zh_saveSettings() override;

};
//=========================================================
#endif // ZCUSTOMTERMDIALOG_H

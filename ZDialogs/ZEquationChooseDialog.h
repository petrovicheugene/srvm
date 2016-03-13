//============================================================
#ifndef ZEQUATIONCHOOSEDIALOG_H
#define ZEQUATIONCHOOSEDIALOG_H
//============================================================
#include <QDialog>
#include "ZEquationSettingsData.h"
//============================================================
class QPushButton;
class QLabel;
class QRadioButton;
class QComboBox;
//============================================================
class ZEquationChooseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZEquationChooseDialog(QWidget *parent = 0);

    void zp_setEquationSettings(const ZEquationSettingsData& settings);
    ZEquationSettingsData zp_equationSettings() const;
    bool zp_accepted() const;

signals:

public slots:


private slots:

    void zh_onDialogAccepted();

private:

    // VARS
    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;

    QRadioButton* zv_simplePolynomEquationRadioButton;
    QRadioButton* zv_fractionalEquationRadioButton;
    QComboBox* zv_baseTermComboBox;

    bool zv_accepted;
    // FUNCS
    void zh_createComponents();
    void zh_createConnections();


};
//============================================================
#endif // ZEQUATIONCHOOSEDIALOG_H

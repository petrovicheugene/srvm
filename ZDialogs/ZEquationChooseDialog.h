//============================================================
#ifndef ZEQUATIONCHOOSEDIALOG_H
#define ZEQUATIONCHOOSEDIALOG_H
//============================================================
#include <QDialog>
#include "ZEquationSettingsData.h"
#include "ZNormaSettingsData.h"

//============================================================
class QPushButton;
class QLabel;
class QRadioButton;
class QComboBox;
class QLineEdit;
class ZNormaSettingsDialog;
//============================================================
class ZEquationChooseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZEquationChooseDialog(QWidget *parent = 0);
    ~ZEquationChooseDialog();
    void zp_setEquationSettings(const ZEquationSettingsData& settings);
    ZEquationSettingsData zp_equationSettings() const;

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
    QLineEdit* zv_normTermLineEdit;
    ZNormaSettingsDialog* zv_normaDialog;
    ZNormaSettingsData zv_fractionalBaseNormaSettingsData;
    // FUNCS
    bool eventFilter(QObject* receiver, QEvent* event);
    void zh_createComponents();
    void zh_createConnections();
    void zh_runNormalizationDialog();
    void zh_setNormalizationToLineEdit(ZNormaSettingsData fractionalBaseNormaSettingsData);
};
//============================================================
#endif // ZEQUATIONCHOOSEDIALOG_H

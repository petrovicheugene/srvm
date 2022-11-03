//============================================================
#ifndef X_EQUATIONCHOOSEDIALOG_H
#define X_EQUATIONCHOOSEDIALOG_H
//============================================================
#include <QDialog>
#include "X_EquationSettingsData.h"
#include "X_NormaSettingsData.h"

//============================================================
class QPushButton;
class QLabel;
class QRadioButton;
class QComboBox;
class QLineEdit;
class X_NormaSettingsDialog;
//============================================================
class X_EquationChooseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit X_EquationChooseDialog(QWidget *parent = 0);
    ~X_EquationChooseDialog();
    void xp_setEquationSettings(const X_EquationSettingsData& settings);
    X_EquationSettingsData xp_equationSettings() const;

signals:

    void xg_currentCalibrationWindowName(const QString& windowName) const;

public slots:

    void zh_onNormaSettingsDialogAccept();

private slots:

    void zh_onDialogAccepted();

private:

    // VARS
    QPushButton* xv_okButton;
    QPushButton* xv_cancelButton;

    QRadioButton* xv_simplePolynomEquationRadioButton;
    QRadioButton* xv_fractionalEquationRadioButton;
    QComboBox* xv_baseTermComboBox;
    QLineEdit* xv_normTermLineEdit;
   // X_NormaSettingsDialog* xv_normaDialog;
    X_NormaSettingsData xv_fractionalBaseNormaSettingsData;
    // FUNCS
    bool eventFilter(QObject* receiver, QEvent* event);
    void zh_createComponents();
    void zh_createConnections();
    void zh_runNormalizationDialog();
    void zh_setNormalizationToLineEdit(X_NormaSettingsData fractionalBaseNormaSettingsData);
};
//============================================================
#endif // X_EQUATIONCHOOSEDIALOG_H

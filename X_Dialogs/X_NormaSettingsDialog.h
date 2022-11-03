//============================================================
#ifndef X_NORMASETTINGSDIALOG_H
#define X_NORMASETTINGSDIALOG_H
//============================================================
#include "X_BaseDialog.h"
#include "X_NormaSettingsData.h"
//============================================================
class QCheckBox;
class QPushButton;
class QLabel;
class QRadioButton;
class QLineEdit;
//============================================================
class X_NormaSettingsDialog : public X_BaseDialog
{
    Q_OBJECT
public:
    explicit X_NormaSettingsDialog(QWidget* parent = 0);
    ~X_NormaSettingsDialog();
    void xp_setNormaSettings(const X_NormaSettingsData& settings);
    X_NormaSettingsData xp_normaSettings() const;

public slots:

    void xp_insertVariable(const QString& variableName);

private slots:

    void zh_onDialogAccepted();

private:

    // VARS
    QPushButton* xv_okButton;
    QPushButton* xv_closeButton;

    QRadioButton* xv_noNormaRadioButton;
    QRadioButton* xv_coherentNormaRadioButton;
    QRadioButton* xv_incoherentNormaRadioButton;
    QRadioButton* xv_coherentToIncoherentNormaRadioButton;
    QRadioButton* xv_incoherentToCoherentNormaRadioButton;
    QRadioButton* xv_customNormaRadioButton;

    QLineEdit* xv_customNormaLineEdit;
    QCheckBox* xv_useModifierForInsertingCheckBox;
    const QString xv_useModifierString = "useModifier";

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    QWidget* zh_createControlWidget();
    void zh_restoreSettings() override;
    void zh_saveSettings() override;

    // VARS
    bool xv_normaSettingsApplied;

};
//============================================================
#endif // X_NORMASETTINGSDIALOG_H

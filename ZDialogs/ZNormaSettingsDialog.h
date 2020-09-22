//============================================================
#ifndef ZNORMASETTINGSDIALOG_H
#define ZNORMASETTINGSDIALOG_H
//============================================================
#include "ZBaseDialog.h"
#include "ZNormaSettingsData.h"
//============================================================
class QCheckBox;
class QPushButton;
class QLabel;
class QRadioButton;
class QLineEdit;
//============================================================
class ZNormaSettingsDialog : public ZBaseDialog
{
    Q_OBJECT
public:
    explicit ZNormaSettingsDialog(QWidget* parent = 0);
    ~ZNormaSettingsDialog();
    void zp_setNormaSettings(const ZNormaSettingsData& settings);
    ZNormaSettingsData zp_normaSettings() const;

public slots:

    void zp_insertVariable(const QString& variableName);

private slots:

    void zh_onDialogAccepted();

private:

    // VARS
    QPushButton* zv_okButton;
    QPushButton* zv_closeButton;

    QRadioButton* zv_noNormaRadioButton;
    QRadioButton* zv_coherentNormaRadioButton;
    QRadioButton* zv_incoherentNormaRadioButton;
    QRadioButton* zv_coherentToIncoherentNormaRadioButton;
    QRadioButton* zv_incoherentToCoherentNormaRadioButton;
    QRadioButton* zv_customNormaRadioButton;

    QLineEdit* zv_customNormaLineEdit;
    QCheckBox* zv_useModifierForInsertingCheckBox;
    const QString zv_useModifierString = "useModifier";

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    QWidget* zh_createControlWidget();
    void zh_restoreSettings() override;
    void zh_saveSettings() override;

    // VARS
    bool zv_normaSettingsApplied;

};
//============================================================
#endif // ZNORMASETTINGSDIALOG_H

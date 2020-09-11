//============================================================
#ifndef ZNORMASETTINGSDIALOG_H
#define ZNORMASETTINGSDIALOG_H
//============================================================
#include <QDialog>
#include "ZNormaSettingsData.h"
//============================================================
class QPushButton;
class QLabel;
class QRadioButton;
class QLineEdit;
//============================================================
class ZNormaSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZNormaSettingsDialog(QWidget* parent = 0);
    void zp_setNormaSettings(const ZNormaSettingsData& settings);
    ZNormaSettingsData zp_normaSettings() const;


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

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

};
//============================================================
#endif // ZNORMASETTINGSDIALOG_H

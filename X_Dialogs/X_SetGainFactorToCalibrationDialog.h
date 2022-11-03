//=========================================================
#ifndef X_SETGAINFACTORTOCALIBRATIONDIALOG_H
#define X_SETGAINFACTORTOCALIBRATIONDIALOG_H
//=========================================================
#include <QDialog>
//=========================================================
class QPushButton;
class QSpinBox;
//=========================================================
class X_SetGainFactorToCalibrationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit X_SetGainFactorToCalibrationDialog(const QString& calibrationName, QWidget *parent = 0);

    int xp_gainFactor() const;

signals:

public slots:


private:
    // VARS

    QSpinBox* xv_gainFactorSpinBox;
    QPushButton* xv_okButton;
    QPushButton* xv_cancelButton;


    // FUNCS
    void xh_createComponents(const QString &calibrationName);
    void xh_createConnections();


};
//=========================================================
#endif // X_SETGAINFACTORTOCALIBRATIONDIALOG_H

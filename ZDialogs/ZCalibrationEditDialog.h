//===========================================================
#ifndef ZCALIBRATIONEDITDIALOG_H
#define ZCALIBRATIONEDITDIALOG_H
//===========================================================
#include <QDialog>
//===========================================================
class QPushButton;
class QLineEdit;
class QComboBox;
class QCloseEvent;
//===========================================================
class ZCalibrationEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ZCalibrationEditDialog(QWidget* parent = 0);

    QString zp_calibrationFullName();

private:
    // VARS
    QLineEdit* zv_nameLineEdit;
    QComboBox* zv_typeComboBox;
    QPushButton* zv_okButton;
    QPushButton* zv_cancelButton;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_fillComboBox(QString currentSuffix = QString());
    bool zh_checkData();

private slots:

    void onOkButtonClick();

};
//===========================================================
#endif // ZCALIBRATIONEDITDIALOG_H

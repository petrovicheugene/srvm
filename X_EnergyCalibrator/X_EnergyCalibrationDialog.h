//======================================================
#ifndef X_ENERGYCALIBRATIONDIALOG_H
#define X_ENERGYCALIBRATIONDIALOG_H
//======================================================
#include <QWidget>
//======================================================
class QSpinBox;
class X_BaseTableWidget;
class QPushButton;
class X_SpeSpectrum;
class X_PeakWindowTableModel;
//======================================================
class X_EnergyCalibrationDialog : public QWidget
{
    Q_OBJECT
public:
    explicit X_EnergyCalibrationDialog(QWidget *parent = 0);
    ~X_EnergyCalibrationDialog();

signals:

    void xg_inquiryCurrentVisibleSceneRect(QRectF& sceneRect) const;

public slots:

    void xp_setMeasuringConditionsAndSpectrum(quint8 gainFactor,
                                              int exposition,
                                              const X_SpeSpectrum *spectrum);

private slots:

    void xh_calculateSaveAndClose();

private:

    // VARS
    QSpinBox* xv_gainFactorSpinBox;
    X_BaseTableWidget* xv_peakTableWidget;
//    QPushButton* xv_addPeakWindowButton;
//    QPushButton* xv_removePeakWindowButton;

    QPushButton* xv_okButton;
    QPushButton* xv_cancelButton;

    X_PeakWindowTableModel* xv_peakWindowTableModel;

    // FUNCS
    void xh_createActions();
    void xh_createComponents();
    QWidget* xh_createControlWidget();
    void xh_createConnections();
    void xh_restoreSettings();
    void xh_saveSettings() const;

};
//======================================================
#endif // X_ENERGYCALIBRATIONDIALOG_H

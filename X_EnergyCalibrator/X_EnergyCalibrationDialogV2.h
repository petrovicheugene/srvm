//======================================================
#ifndef X_ENERGYCALIBRATIONDIALOGV2_H
#define X_ENERGYCALIBRATIONDIALOGV2_H
//======================================================
#include <QDialog>
#include <QGridLayout>
#include <QList>
#include <QMap>
#include <QModelIndex>
#include <X_SpeSpectrum.h>
//======================================================
class X_EnergyCalibrationSpectrumTableModel;
class X_EnergyCalibrationLine;
class X_Plotter;
class X_AbstractPlotterDataManager;

class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QTableView;
class QToolButton;
//======================================================
class X_EnergyCalibrationDialogV2 : public QDialog
{
    Q_OBJECT
public:
    explicit X_EnergyCalibrationDialogV2(QMap<quint8, QList<X_SpeSpectrum *> > &spectrumMap,
                                        QWidget *parent = nullptr);
    ~X_EnergyCalibrationDialogV2();
    void xp_setAskSaveForGainfactorFlag(bool flag);

signals:

    void xg_energyCalibrationChanged(int gainFactor, const QList<double>& energyCalibrationFactors) const;

public slots:


private slots:

    void xh_calculateAndWriteEnergyCalibration();
    void xh_onCurrentGainFactorIndexChange(int currentIndex);
    void xh_onCurrentPeackCountIndexChange(int currentIndex);

    void xh_onModelReset();
    void xh_onModelDataChange(const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void xh_onMousePressedOnPlotter(QPointF scenePos);
    void xh_onChannelNumberAction();
    void xh_onSelectEnergyLineAction();
    void xh_onClearEnergyLineAction();
    void xh_onLineColorChange(QColor color);
    void xh_onChannelNumberChange(int channelValue);

private:

    // VARS
    const QString xv_settingsGroupName = "energyCalibrationDialogV2";
    const QString xv_geometrySectionName = "geometry";
    const QString xv_lineColorListSectionName = "lineColorList";
    const QString xv_lineColorValueName = "lineColor";
    const int xv_calibrationLineNumber = 3;
    X_EnergyCalibrationSpectrumTableModel* xv_spectrumModel ;

    qreal xv_boundingRectTopFactor = 1.05;
    QRectF xv_defaultSceneRect = QRectF(QPointF(0.0,-100.0), QPointF(2048.0, 0.0));

    QComboBox* xv_gainFactorComboBox;
    QComboBox* xv_peakInEnergyCalibrationCountComboBox;
    QList<X_EnergyCalibrationLine*> xv_energyCalibrationLineList;

    QList<QAction*> xv_channelNumberActionList;

    QTableView* xv_spectrumTableView;
    X_Plotter* xv_plotter;

    QPushButton* xv_calculateButton;
    QPushButton* xv_closeButton;

    QList<QList<QWidget*> > xv_elementLinesWidgetList;
    bool xv_askSaveForGainFactor;

    // FUNCS
    void xh_createComponents();
    void xh_createContextMenu();

    QWidget* xh_createMainWidget();
    QWidget* xh_createElementLinesWidgetAndComponents();
    void xh_createElementLineControlsAndComponents(QGridLayout *layout, int elementNumber);

    void xh_createConnections();
    void xh_loadSpectrumData(QMap<quint8, QList<X_SpeSpectrum *> > &spectrumMap);

    //void xh_restoreLineColorList();
    void xh_restoreSettings();
    void xh_saveSettings();

    bool xh_calculateEnergyFactors(QList<double> &energyCalibrationFactorList);
    void xh_setElementLineControlsAndComponentVisibility();

    void xh_updateVerticalLines();

};
//======================================================
#endif // X_ENERGYCALIBRATIONDIALOGV2_H

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

    void zh_calculateAndWriteEnergyCalibration();
    void zh_onCurrentGainFactorIndexChange(int currentIndex);
    void zh_onCurrentPeackCountIndexChange(int currentIndex);

    void zh_onModelReset();
    void zh_onModelDataChange(const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void zh_onMousePressedOnPlotter(QPointF scenePos);
    void zh_onChannelNumberAction();
    void zh_onSelectEnergyLineAction();
    void zh_onClearEnergyLineAction();
    void zh_onLineColorChange(QColor color);
    void zh_onChannelNumberChange(int channelValue);

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
    void zh_createComponents();
    void zh_createContextMenu();

    QWidget* zh_createMainWidget();
    QWidget* zh_createElementLinesWidgetAndComponents();
    void zh_createElementLineControlsAndComponents(QGridLayout *layout, int elementNumber);

    void zh_createConnections();
    void zh_loadSpectrumData(QMap<quint8, QList<X_SpeSpectrum *> > &spectrumMap);

    //void zh_restoreLineColorList();
    void zh_restoreSettings();
    void zh_saveSettings();

    bool zh_calculateEnergyFactors(QList<double> &energyCalibrationFactorList);
    void zh_setElementLineControlsAndComponentVisibility();

    void zh_updateVerticalLines();

};
//======================================================
#endif // X_ENERGYCALIBRATIONDIALOGV2_H

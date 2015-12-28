//======================================================
#ifndef ZCALIBRATIONARRAYREPOSITORY_H
#define ZCALIBRATIONARRAYREPOSITORY_H
//======================================================
#include <QObject>
#include <QList>
#include <QMenu>
#include "ZCalibration.h"
//======================================================
class QAction;
class ZFileActionManager;
//======================================================
class ZCalibrationRepository : public QObject
{
    Q_OBJECT
public:
    explicit ZCalibrationRepository(QObject *parent = 0);

    // VARS
    enum CalibrationOperationType {COT_CALIBRATION_VISIBILITY_CHANGE,
                        COT_END_CALIBRATION_VISIBILITY_CHANGE,
                        COT_CALIBRATION_CHANGED,
                        COT_INSERT_CALIBRATIONS,
                        COT_END_INSERT_CALIBRATIONS,
                        COT_REMOVE_CALIBRATIONS,
                        COT_END_REMOVE_CALIBRATIONS,
                        COT_BEGIN_RESET,
                        COT_END_RESET
                       };

    enum WindowOperationType {WOT_WINDOW_VISIBILITY_CHANGE,
                        WOT_END_WINDOW_VISIBILITY_CHANGE,
                        WOT_WINDOW_CHANGED,
                        WOT_INSERT_WINDOWS,
                        WOT_END_INSERT_WINDOWS,
                        WOT_REMOVE_WINDOWS,
                        WOT_END_REMOVE_WINDOWS,
                        WOT_BEGIN_RESET,
                        WOT_END_RESET
                       };




    // FUNCS
    void zp_connectToFileManager(ZFileActionManager*);

    void zp_appendActionsToMenu(QMenu *menu) const;
    QList<QAction*> zp_arrayActions() const;
    QList<QAction*> zp_windowActions() const;

    bool zp_isEmpty() const;
    void zp_clear();

    int zp_calibrationCount() const;
    int zp_visibleCalibrationCount() const;
    QString zp_calibrationName(int calibrationIndex) const;
    qint64 zp_calibrationIdForCalibrationIndex(int calibrationIndex) const;
    bool zp_setCalibrationName(int row, const QString&);
    QString zp_visibleCalibrationName(int visibleCalibrationIndex) const;

    QString zp_calibrationChemicalElement(int row) const;
    bool zp_setCalibrationChemicalElement(int row, const QString&);

    bool zp_isCalibrationVisible(int row);
    QColor zp_calibrationColor(int row);
    bool zp_setVisible(int row, bool checked);

    bool zp_isDirty(int row);

    // Windows
    bool zp_isCalibrationWindowVisible(qreal, int) const;
    bool zp_setCalibrationWindowVisible(qreal, int windowIndex, bool visibility);
    int zp_calibrationWindowCount(qreal) const;
    QString zp_calibrationWindowName(qint64 calibrationId, int windowIndex) const;
    bool zp_setCalibrationWindowName(qint64 calibrationId, int windowIndex, const QString&);
    ZCalibrationWindow::WindowType zp_calibrationWindowType(qint64 calibrationId, int windowIndex) const;
    bool zp_setCalibrationWindowType(qint64 calibrationId, int windowIndex, ZCalibrationWindow::WindowType);
    int zp_calibrationWindowFirstChannel(qint64 calibrationId, int windowIndex) const;
    int zp_calibrationWindowLastChannel(qint64 calibrationId, int windowIndex) const;

    bool zp_setCalibrationWindowFirstChannel(qint64 calibrationId, int windowIndex, int channel);
    bool zp_setCalibrationWindowLastChannel(qint64 calibrationId, int windowIndex, int channel);


    QColor zp_calibrationWindowColor(qint64 calibrationId, int windowIndex) const;

    double zp_calculateConcentration(int row, const ZAbstractSpectrum* const, bool *ok = 0) const;

signals:

    void zg_message(QString) const;
    void zg_currentCalibrationOperation(CalibrationOperationType, int, int) const;
    void zg_currentWindowOperation(WindowOperationType, int, int, int) const;

    void zg_requestSelectedCalibrationIndexList(QList<int>&);
    void zg_requestCurrentCalibrationIndex(int&);
    void zg_openCalibrationsActionTriggered() const;

    void zg_setCurrentCalibrationIndex(int calibrationIndex);
    void zg_startCurrentCalibrationEdition();
    void zg_currentCalibration(qreal calibrationId, int current);

    void zg_requestCurrentWindowIndex(int&) const;
    void zg_requestSelectedWindowIndexList(QList<int>&) const;
    void zg_requestCurrentVisibleSceneRect(QRectF& sceneRect);

public slots:

    void zp_appendCalibrationsToArray(const QStringList&fileNameList);
    void zp_onCurrentCalibrationChanged(int current, int previous);

private slots:

    void zh_onNewCalibrationAction();
    void zh_onRemoveCalibrationsAction();
    void zh_onNewWindowAction();
    void zh_onRemoveWindowAction();
    void zh_onWindowOperation(ZCalibration::WindowOperationType type, int first, int last);

private:

    // VARS
    QList<ZCalibration*> zv_caibrationList;
    QAction* zv_newCalibrationAction;
    QAction* zv_openCalibrationsAction;
    QAction* zv_removeCalibrationAction;

    QAction* zv_newWindowAction;
    QAction* zv_removeWindowAction;

    QString zv_defaultCalibrationName;

    // FUNCS
    void zh_createActions();
    void zh_createConnections();

    void zh_removeCalibration(int);
    bool zh_createCalibrationFromFile(const QString&);
    bool zh_createNewCalibration(const QString&);
    bool zh_appendCalibrationToList(ZCalibration*);
    void zh_actionAvailabilityControl(int current);

    void zh_removeCalibrationWindow(int currentCalibrationIndex, int spectrumWindowIndex);

};
//======================================================
#endif // ZCALIBRATIONARRAYREPOSITORY_H

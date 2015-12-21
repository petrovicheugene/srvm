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
    enum OperationType {OT_CALIBRATION_VISIBILITY_CHANGE,
                        OT_END_CALIBRATION_VISIBILITY_CHANGE,
                        OT_CALIBRATION_CHANGED,
                        OT_INSERT_CALIBRATIONS,
                        OT_END_INSERT_CALIBRATIONS,
                        OT_REMOVE_CALIBRATIONS,
                        OT_END_REMOVE_CALIBRATIONS,
                        OT_BEGIN_RESET,
                        OT_END_RESET
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
    bool zp_setCalibrationName(int row, const QString&);
    QString zp_visibleCalibrationName(int visibleCalibrationIndex) const;

    QString zp_calibrationChemicalElement(int row) const;
    bool zp_setCalibrationChemicalElement(int row, const QString&);

    bool zp_calibrationIsVisible(int row);
    QColor zp_calibrationColor(int row);
    bool zp_setVisible(int row, bool checked);

    bool zp_isDirty(int row);

    //QString zp_fileSuffix(int row);

    double zp_calculateConcentration(int row, const ZAbstractSpectrum* const, bool *ok = 0) const;

signals:

    void zg_message(QString) const;
    void zg_currentOperation(OperationType, int, int);
    void zg_requestSelectedCalibrationIndexList(QList<int>&);
    void zg_requestCurrentCalibrationIndex(int&);
    void zg_openCalibrationsActionTriggered() const;

    void zg_setCurrentCalibrationIndex(int calibrationIndex);
    void zg_startCurrentCalibrationEdition();

public slots:

    void zp_appendCalibrationsToArray(const QStringList&fileNameList);
    void zp_onCurrentCalibrationChanged(int current, int previous);

private slots:

    void zh_onNewCalibrationAction();
    void zh_onEditCalibrationsAction();
    void zh_onRemoveCalibrationsAction();

private:

    // VARS
    QList<ZCalibration*> zv_caibrationList;
    QAction* zv_newCalibrationAction;
    QAction* zv_openCalibrationsAction;
    QAction* zv_editCalibrationsAction;
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

};
//======================================================
#endif // ZCALIBRATIONARRAYREPOSITORY_H

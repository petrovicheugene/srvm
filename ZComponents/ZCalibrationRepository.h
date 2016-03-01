//======================================================
#ifndef ZCALIBRATIONARRAYREPOSITORY_H
#define ZCALIBRATIONARRAYREPOSITORY_H
//======================================================
#include <QObject>
#include <QList>
#include <QMenu>
#include "ZCalibration.h"
#include "ZSpectrumArrayRepository.h"

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
                                   COT_CALIBRATION_NAME_CHANGED,
                                   COT_CALIBRATION_CHEM_ELEMENT_CHANGED,
                                   COT_INSERT_CALIBRATIONS,
                                   COT_END_INSERT_CALIBRATIONS,
                                   COT_REMOVE_CALIBRATIONS,
                                   COT_END_REMOVE_CALIBRATIONS,
                                   COT_BEGIN_RESET,
                                   COT_END_RESET
                                  };

    enum WindowOperationType {WOT_WINDOW_CHANGED,
                              WOT_INSERT_WINDOWS,
                              WOT_END_INSERT_WINDOWS,
                              WOT_REMOVE_WINDOWS,
                              WOT_END_REMOVE_WINDOWS,
                              WOT_BEGIN_RESET,
                              WOT_END_RESET
                             };

    enum TermOperationType {TOT_TERM_NAME_CHANGED,
                            TOT_BEGIN_INSERT_TERM,
                            TOT_END_INSERT_TERM,
                            TOT_BEGIN_REMOVE_TERM,
                            TOT_END_REMOVE_TERM,
                            TOT_BEGIN_RESET,
                            TOT_END_RESET,
                            TOT_TERM_STATE_CHANGED,
                            TOT_TERM_VALUE_CHANGED
                           };



    // FUNCS
    void zp_connectToFileManager(ZFileActionManager*);
    //void zp_connectToSpectrumArrayRepository(ZSpectrumArrayRepository*spectrumRepository);


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

    QString zp_chemElement(int row) const;
    QString zp_chemElementForCalibrationId(qint64 id) const;
    bool zp_setChemElement(int row, const QString&);

    bool zp_isCalibrationVisible(int row);
    QColor zp_calibrationColor(int row);
    bool zp_setVisible(int row, bool checked);

    bool zp_isDirty(int row);

    // Windows
    bool zp_isCalibrationWindowVisible(qint64, int) const;
    bool zp_isCalibrationWindowVisible(qint64) const;
    bool zp_setCalibrationWindowVisible(qreal, int windowIndex, bool visibility);

    int zp_calibrationWindowCount(qint64) const;
    QString zp_calibrationWindowName(qint64 calibrationId, int windowIndex) const;
    bool zp_setCalibrationWindowName(qint64 calibrationId, int windowIndex, const QString&);
    ZCalibrationWindow::WindowType zp_calibrationWindowType(qint64 calibrationId, int windowIndex) const;
    bool zp_setCalibrationWindowType(qint64 calibrationId, int windowIndex, ZCalibrationWindow::WindowType);
    int zp_calibrationWindowFirstChannel(qint64 calibrationId, int windowIndex) const;
    int zp_calibrationWindowLastChannel(qint64 calibrationId, int windowIndex) const;

    bool zp_setCalibrationWindowFirstChannel(qint64 calibrationId, int windowIndex, int channel);
    bool zp_setCalibrationWindowLastChannel(qint64 calibrationId, int windowIndex, int channel);


    QColor zp_calibrationWindowColor(qint64 calibrationId, int windowIndex) const;
    qint64 zp_calibrationWindowId(qint64 calibrationId, int windowIndex) const;
    const ZCalibrationWindow* zp_calibrationWindow(qint64 calibrationId, int windowIndex) const;
    bool zp_calculateConcentration(int row, const ZAbstractSpectrum*, qreal& concentration) const;


    // terms
    int zp_termCount(qint64 calibrationId) const;
    QString zp_termName(qint64 calibrationId, int termIndex) const;
    bool zp_termFactor(qint64 calibrationId, int termIndex, qreal& factor) const;
    bool zp_setTermFactor(qint64 calibrationId, int termIndex, qreal factor);
    //bool zp_termValue(qint64 calibrationId, int termIndex, const ZAbstractSpectrum *spectrum, qreal& value);
    bool zp_termVariablePart(qint64 calibrationId, int termIndex, const ZAbstractSpectrum *spectrum, qreal &value);
    //bool zp_termAverageValue(qint64 calibrationId, int termIndex, qreal& averageValue);

    ZAbstractTerm::TermState zp_termState(qint64 calibrationId, int termIndex);
    void zp_setNextUsersTermState(qint64 calibrationId, int termLogIndex);

    // normalizer
    ZTermNormalizer::NormaType zp_normaType(qint64 calibrationId) const;
    bool zp_setNormaType(qint64 calibrationId, ZTermNormalizer::NormaType type);


signals:

    void zg_message(QString) const;
    void zg_calibrationOperation(CalibrationOperationType, int, int) const;
    void zg_calibrationWindowOperation(WindowOperationType, int, int, int) const;
    void zg_termOperation(TermOperationType, int, int, int) const;

    void zg_requestSelectedCalibrationIndexList(QList<int>&);
    void zg_requestCurrentCalibrationIndex(int&);
    void zg_openCalibrationsActionTriggered() const;

    void zg_setCurrentCalibrationIndex(int calibrationIndex);
    void zg_startCurrentCalibrationEdition();
    void zg_currentCalibrationChanged(qint64 calibrationId, int current);

    void zg_currentCalibrationWindowChanged(qint64 currentWindowId, int currentWindowIndex,
                                            qint64 previousWindowId, int previousWindowIndex);

    void zg_requestCurrentWindowIndex(int&) const;
    void zg_requestSelectedWindowIndexList(QList<int>&) const;
    void zg_requestCurrentVisibleSceneRect(QRectF& sceneRect);

    void zg_setCurrentWindowIndex(int windowIndex);
    void zg_startCurrentWindowEdition();

    void zg_normalizerChanged(qint64 calibrationId) const;

public slots:


    void zp_appendCalibrationsToArray(const QStringList&fileNameList);
    void zp_onCurrentCalibrationChanged(int current, int previous);
    void zp_onCurrentCalibrationWindowChanged(int current, int previous);

private slots:

    void zh_onNewCalibrationAction();
    void zh_onRemoveCalibrationsAction();
    void zh_onNewWindowAction();
    void zh_onRemoveWindowAction();
    void zh_onWindowOperation(ZCalibration::WindowOperationType type, int first, int last);
    void zh_onTermOperation(ZCalibration::TremOperationType type, int first, int last);
    void zh_onNormalizerChange() const;
    //void zh_onCurrentSpectrumArrayIsAboutChange(qint64 arrayId, int arrayIndex);
    //void zh_onSpectrumOperation(ZSpectrumArrayRepository::SpectrumOperationType, int, int, int);

private:

    // VARS
    ZSpectrumArrayRepository* zv_spectrumArrayRepository;
    QList<ZCalibration*> zv_caibrationList;
    QAction* zv_newCalibrationAction;
    QAction* zv_openCalibrationsAction;
    QAction* zv_removeCalibrationAction;

    QAction* zv_newWindowAction;
    QAction* zv_removeWindowAction;

    QString zv_defaultCalibrationName;
    qint64 zv_currentArrayId;
    int zv_currentArrayIndex;

    // FUNCS
    void zh_createActions();
    void zh_createConnections();

    void zh_removeCalibration(int);
    bool zh_createCalibrationFromFile(const QString&);
    bool zh_createNewCalibration(const QString&);
    bool zh_appendCalibrationToList(ZCalibration*);
    void zh_actionAvailabilityControl(int current);

    void zh_removeCalibrationWindow(int currentCalibrationIndex, int spectrumWindowIndex);
    ZCalibration* zh_calibrationForId(qint64) const;
    //void zh_calcTermAverageValues();


};
//======================================================
#endif // ZCALIBRATIONARRAYREPOSITORY_H

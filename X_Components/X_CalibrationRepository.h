//======================================================
#ifndef X_CALIBRATIONARRAYREPOSITORY_H
#define X_CALIBRATIONARRAYREPOSITORY_H
//======================================================
#include <QObject>
#include <QList>
#include <QMenu>
#include "X_Calibration.h"
#include "X_SpectrumArrayRepository.h"
#include "X_EquationSettingsData.h"
#include "X_NormaSettingsData.h"
#include "X_CalibrationQualityData.h"
//======================================================
class QAction;
class X_FileActionManager;
//======================================================
class X_CalibrationRepository : public QObject
{
    Q_OBJECT
    friend class X_TermCorrelationTableManager;
public:
    explicit X_CalibrationRepository(QObject *parent = nullptr);

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
                                   COT_END_RESET,
                                   COT_CALIBRATION_FREE_MEMBER_CHANGED,
                                   COT_CALIBRATION_EQUATION_TYPE_CHANGED,
                                   COT_CALIBRATION_EQUATION_BASE_TERM_CHANGED,
                                   COT_CALIBRATION_NORMA_CHANGED,
                                   COT_CALIBRATION_DIRTY_CHANGED
                                  };

    enum WindowOperationType {WOT_WINDOW_CHANGED,
                              WOT_WINDOW_VISIBLE_CHANGED,
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
                            TOT_TERM_WINDOW_MARGIN_CHANGED,
                            TOT_TERM_FACTOR_CHANGED
                           };

    // FUNCS
    void xp_connectToFileActionManager(X_FileActionManager*);

    void xp_appendActionsToMenu(QMenu *menu) const;
    QList<QAction*> xp_calibrationActions() const;
    QList<QAction*> xp_calibrationVisibilityActions() const;
    QList<QAction*> xp_calibrationContextMenuActions() const;

    QList<QAction*> xp_windowActions() const;
    QList<QAction*> xp_windowContextMenuActions() const;

    QList<QAction*> xp_termActions() const;
    QList<QAction*> xp_termContextMenuActions() const;


    bool xp_isEmpty() const;
    void xp_clear();

    int xp_calibrationCount() const;
    int xp_visibleCalibrationCount() const;
    QString xp_calibrationName(int calibrationIndex) const;
    qint64 xp_calibrationIdForCalibrationIndex(int calibrationIndex) const;
    bool xp_setCalibrationName(int row, const QString&);
    QString xp_visibleCalibrationName(int visibleCalibrationIndex) const;
    qint64 xp_visibleCalibrationId(int visibleCalibrationIndex) const;
    int xp_calibrationIndexForVisibleIndex(int visibleCalibrationIndex) const;

    QString xp_chemElement(int row) const;
    QString xp_chemElementForCalibrationId(qint64 id) const;
    bool xp_setChemElement(int row, const QString&);
    QString xp_baseTermString(int row) const;

    bool xp_isCalibrationVisible(int row);
    QColor xp_calibrationColor(int row);
    bool xp_setCalibrationVisible(int row, bool visibility);

    bool xp_isDirty(int row);
    void xp_hasDirtyCalibrations(QList<int>& dirtyRowList) const;

    // Windows
    bool xp_isCalibrationWindowVisible(qint64, int) const;
    bool xp_isCalibrationWindowVisible(qint64) const;
    bool xp_setCalibrationWindowVisible(qint64, int windowIndex, bool visibility);

    int xp_calibrationWindowCount(qint64) const;
    QString xp_calibrationWindowName(qint64 calibrationId, int windowIndex) const;
    bool xp_setCalibrationWindowName(qint64 calibrationId, int windowIndex, const QString&);
    X_CalibrationWindow::WindowType xp_calibrationWindowType(qint64 calibrationId, int windowIndex) const;
    bool xp_setCalibrationWindowType(qint64 calibrationId, int windowIndex, X_CalibrationWindow::WindowType);
    int xp_calibrationWindowFirstChannel(qint64 calibrationId, int windowIndex) const;
    int xp_calibrationWindowLastChannel(qint64 calibrationId, int windowIndex) const;

    bool xp_setCalibrationWindowFirstChannel(qint64 calibrationId, int windowIndex, int channel);
    bool xp_setCalibrationWindowLastChannel(qint64 calibrationId, int windowIndex, int channel);

    QColor xp_calibrationWindowColor(qint64 calibrationId, int windowIndex) const;
    qint64 xp_calibrationWindowId(qint64 calibrationId, int windowIndex) const;
    const X_CalibrationWindow* xp_calibrationWindow(qint64 calibrationId, int windowIndex) const;
    bool xp_calculateConcentration(int row, const X_AbstractSpectrum *, qreal& concentration) const;
    bool xp_calculateConcentrationForId(qint64 calibrationId, const X_AbstractSpectrum *spectrum, qreal& concentration) const;

    bool xp_calculateActiveTermValueSumForId(qint64 calibrationId, const X_AbstractSpectrum* spectrum, qreal& value) const;



    // terms
    int xp_termCount(qint64 calibrationId) const;
    QString xp_termName(qint64 calibrationId, int termIndex) const;
    bool xp_termFactor(qint64 calibrationId, int termIndex, qreal& factor) const;
    // bool xp_setTermFactor(qint64 calibrationId, int termIndex, qreal factor);

    bool xp_termFactorString(qint64 calibrationId, int termIndex, QString &factorString) const;
    bool xp_setTermFactorString(qint64 calibrationId, int termIndex, const QString& factorString);

    bool xp_termVariablePart(qint64 calibrationId, int termIndex, const X_AbstractSpectrum *spectrum, qreal &value);
    bool xp_calcBaseTermValue(qint64 calibrationId, const X_AbstractSpectrum *spectrum, qreal& value);

    X_AbstractTerm::TermState xp_termState(qint64 calibrationId, int termIndex);
    void xp_setNextUsersTermState(qint64 calibrationId, int termLogIndex);

    // normalizer
    X_TermNormalizer::NormaType xp_normaTypeForCalibrationId(qint64 calibrationId) const;
    bool xp_setNormaTypeForCalibrationId(qint64 calibrationId, X_TermNormalizer::NormaType type);

    X_TermNormalizer::NormaType xp_normaType(int calibrationIndex) const;
    bool xp_setNormaType(int calibrationIndex, X_TermNormalizer::NormaType type);

    bool xp_equationTypeForCalibrationId(qint64 calibrationId, X_Calibration::EquationType& type) const;
    bool xp_setEquationTypeForCalibrationId(qint64 calibrationId, X_Calibration::EquationType type);

    X_NormaSettingsData xp_normaSettingsData(int calibrationIndex) const;
    bool xp_setNormaSettings(int calibrationIndex, X_NormaSettingsData);

    // equation
    X_Calibration::EquationType xp_equationType(int calibrationIndex) const;
    bool xp_setEquationType(int calibrationIndex, X_Calibration::EquationType type);

    X_EquationSettingsData xp_equationDataForSettings(int calibrationIndex) const;
    bool xp_setEquationDataFromSettings(int calibrationIndex, X_EquationSettingsData);

    qreal xp_equationIntercept(int calibrationIndex) const;
    bool xp_setEquationIntercept(int calibrationIndex, qreal);

    QString xp_equationInterceptString(int calibrationIndex) const;
    bool xp_setEquationInterceptString(int calibrationIndex, const QString& interceptString);

    int xp_baseTermIndex(int calibrationIndex);
    qint64 xp_baseTermId(int calibrationIndex);
    int xp_baseTermIndexForCalibrationId(qint64 calibrationId);
    qint64 xp_baseTermIdForCalibrationId(qint64 calibrationId);

    void xh_notifyCalibrationRecalc(qint64 calibrationId) const;

    X_CalibrationQualityData xp_calibrationQualityData(qint64 calibrationId) const;

signals:

    void xg_message(QString) const;
    void xg_calibrationOperation(CalibrationOperationType, int first, int last) const;
    void xg_calibrationWindowOperation(WindowOperationType, int calibrationIndex, int first, int last) const;
    void xg_termOperation(TermOperationType, int calibrationIndex, int first, int last) const;

    void xg_requestSelectedCalibrationIndexList(QList<int>&);
    void xg_requestCurrentCalibrationIndex(int&);
    //    void xg_openCalibrationsActionTriggered() const;
    //    void xg_saveCalibrationsActionTriggered() const;
    //    void xg_saveAsCalibrationsActionTriggered() const;

    void xg_setCurrentCalibrationIndex(int calibrationIndex);
    void xg_startCurrentCalibrationEdition();
    void xg_currentCalibrationChanged(qint64 calibrationId, int current);

    void xg_currentCalibrationWindowChanged(qint64 currentWindowId, int currentWindowIndex,
                                            qint64 previousWindowId, int previousWindowIndex);
    void xg_currentCalibrationWindowName(const QString& name) const;

    void xg_requestCurrentWindowIndex(int&) const;
    void xg_requestSelectedWindowIndexList(QList<int>&) const;
    void xg_requestCurrentVisibleSceneRect(QRectF& sceneRect);

    void xg_setCurrentWindowIndex(int windowIndex);
    void xg_startCurrentWindowEdition();

    void xg_normalizerChanged(qint64 calibrationId) const;
    void xg_invokeCalibrationRecalc() const;

    void xg_currentCalibrationDirtyChanged(bool dirty, bool currentCalibrationExists) const;
    void xg_saveCalibration(X_Calibration*calibration, QString path, QString name) const;

public slots:

    void xp_onSelectedCalibrationChange(QList<int>);
    void xp_onCurrentCalibrationChange(int current, int previous);
    void xp_onCurrentCalibrationWindowChange(int current, int previous);
    void xp_onCalibrationWindowClick(const QModelIndex& index);

    void xp_onCalibrationWindowSelectionChange();
    void xp_onCurrentTermChange(int currentTermIndex, int previousTermIndex);
    void xp_calibrationQualityDataChanged(bool saveTocalibration,
                                          qint64 calibrationId,
                                          X_CalibrationQualityData);
    void xp_onTermDoubleClocked(int row);

protected:

    bool eventFilter(QObject* object, QEvent* event);

private slots:

    void xh_appendCalibrationsToArray(const QStringList&fileNameList);
    void xh_onNewCalibrationAction();
    void xh_onRemoveCalibrationsAction();
    void xh_onNewWindowAction();
    void xh_onRemoveWindowAction();
    void xh_onRecalcEquationFactorsAction();

    void xh_onCreateMixedTermsAction();
    void xh_onRemoveMixedTermsAction();
    void xh_onCreateCustomTermAction();
    void xh_onRemoveCustomTermAction();

    void xh_onWindowOperation(X_Calibration::WindowOperationType type, int first, int last);
    void xh_onTermOperation(X_Calibration::TremOperationType type, int first, int last);
    void xh_onNormalizerChange() const;
    void xh_onCalibrationFreeTermChange() const;
    void xh_onCalibrationDirtyChange(bool dirty) const;
    void xh_createCalibrationAndStartSaving(QString path, QString name) const;
    void xh_onCalibrationSaving(const X_Calibration* calibration, QString absFilePath);

    void xh_onSetCalibrationVisibleAction();
    void xh_onSetCalibrationInvisibleAction();
    void xh_onInvertCalibrationVisibilityAction();
    void xh_onSetWindowsVisibleAction();
    void xh_onSetWindowsInvisibleAction();
    void xh_onInvertWindowsVisibilityAction();
    void xh_onCopySelectedCalibrationAction();
    void xh_onCopySelectedWindowsAction();
    void xh_onPasteWindowsAction();
    void xh_onResetTermStateAction();

private:

    // VARS
    X_SpectrumArrayRepository* xv_spectrumArrayRepository;
    QList<X_Calibration*> xv_caibrationList;
    QAction* xv_newCalibrationAction;
    QAction* xv_openCalibrationsAction;
    QAction* xv_saveCalibrationsAction;
    QAction* xv_saveAsCalibrationsAction;
    QAction* xv_removeCalibrationAction;

    QAction* xv_newWindowAction;
    QAction* xv_removeWindowAction;

    QAction* xv_createMixedTermsAction;
    QAction* xv_removeMixedTermsAction;
    QAction* xv_createCustomTermAction;
    QAction* xv_removeCustomTermAction;

    QAction* xv_recalcEquationFactorsAction;

    QAction* xv_setCalibrationVisibleAction;
    QAction* xv_setCalibrationInvisibleAction;
    QAction* xv_invertCalibrationVisibilityAction;

    QAction* xv_setWindowsVisibleAction;
    QAction* xv_setWindowsInvisibleAction;
    QAction* xv_invertWindowsVisibilityAction;

    QAction* xv_copySelectedCalibrationAction;

    QAction* xv_copySelectedWindowsAction;
    QAction* xv_pasteWindowsAction;

    QAction* xv_resetTermStateAction;


    QString xv_defaultCalibrationName;
    X_CalibrationWindow::WindowType xv_defaultCalibrationWindowType;
    qint64 xv_currentArrayId;
    int xv_currentArrayIndex;
    int xv_currentCalibrationIndex;
    int xv_currentWindowIndex;
    int xv_currentTermIndex;

    bool xv_blockCalibrationRecalc;

    // FUNCS
    void xh_createActions();
    void xh_createConnections();

    void xh_removeCalibration(int);
    bool xh_createCalibrationFromFile(const QString&);
    bool xh_createNewCalibration(const QString&);
    bool xh_appendCalibrationToList(X_Calibration*);
    void xh_actionEnablingControl();

    void xh_removeCalibrationWindow(int currentCalibrationIndex, int spectrumWindowIndex);
    X_Calibration* xh_calibrationForId(qint64) const;
    int xh_calibrationIndexForId(qint64) const;
    bool xh_copyCalibration(int calibrationIndex);
    bool xh_copyWindowsTermsAndEquation(int calibrationIndex, X_Calibration *trgCalibration);
    int xh_findCorrespondingTermIndex(int srcTermIndex,
                                      const X_Calibration *srcCalibration, const X_Calibration *trgCalibration);
};
//======================================================
#endif // X_CALIBRATIONARRAYREPOSITORY_H

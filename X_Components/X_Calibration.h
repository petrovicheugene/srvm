//=========================================================
#ifndef QABSTRACTCALIBRATION_H
#define QABSTRACTCALIBRATION_H
//=========================================================
#include "X_AbstractTerm.h"
#include "X_CalibrationQualityData.h"
#include "X_CalibrationWindow.h"
#include "X_CustomTerm.h"
#include "X_RawTerm.h"
#include "X_RawWindow.h"
#include <QColor>
#include <QDateTime>
#include <QMap>
#include <QObject>
//=========================================================
class X_AbstractSpectrum;
class X_SimpleTerm;
class X_QuadraticTerm;
class X_MixedTerm;
class X_CustomTerm;
class X_TermNormalizer;
//=========================================================

//=========================================================
class X_Calibration : public QObject
{
    Q_OBJECT
    friend class X_AbstractTerm;
    friend class X_TermNormalizer;
    friend class X_TermCorrelationTableManager;

public:
    explicit X_Calibration(const QString& name, QObject* parent = nullptr);
    explicit X_Calibration(const X_Calibration* calibration,
                          const QString& name,
                          QObject* parent = nullptr);
    virtual ~X_Calibration();

    // VARS
    enum WindowOperationType
    {
        WOT_BRGIN_INSERT_WINDOWS,
        WOT_END_INSERT_WINDOWS,
        WOT_BEGIN_REMOVE_WINDOWS,
        WOT_END_REMOVE_WINDOWS,
        WOT_WINDOW_CHANGED,
        WOT_WINDOW_VISIBLE_CHANGED
    };

    enum TremOperationType
    {
        TOT_BEGIN_INSERT_TERM,
        TOT_END_INSERT_TERM,
        TOT_BEGIN_REMOVE_TERM,
        TOT_END_REMOVE_TERM,
        TOT_TERM_NAME_CHANGED,
        TOT_TERM_STATE_CHANGED,
        TOT_TERM_WINDOW_MARGIN_CHANGED,
        TOT_TERM_FACTOR_CHANGED
    };

    enum EquationType
    {
        ET_NOT_DEFINED,
        ET_POLYNOMIAL,
        ET_FRACTIONAL
    };
    // FUNCS
    QDateTime xp_dateTime() const;
    QString xp_baseName() const;
    void xp_setBaseName(const QString&);
    QString xp_fileName() const;

    QString xp_suffix() const;
    void xp_setName(const QString&);

    QString xp_path() const;
    void xp_setPath(const QString&);

    QString xp_chemElement() const;
    bool xp_setChemElement(const QString&);

    //    void xp_setEnergyCalibration(qreal K0, qreal K1, qreal K2, const QString& energyUnit);
    //    void xp_setEnergyCalibrationK0(qreal K0);
    //    void xp_setEnergyCalibrationK1(qreal K1);
    //    void xp_setEnergyCalibrationK2(qreal K2);
    //    void xp_setEnergyUnit(const QString& energyUnit);
    void xp_setGainFactor(int gainFactor);
    void xp_setExposition(int exposition);

    //    qreal xp_energyCalibrationK0() const;
    //    qreal xp_energyCalibrationK1() const;
    //    qreal xp_energyCalibrationK2() const;
    //    QString xp_energyUnit() const;
    int xp_gainFactor() const;
    int xp_exposition() const;

    bool xp_isVisible() const;
    void xp_setVisible(bool);

    bool xp_isDirty() const;
    void xp_setDirty(bool);
    QColor xp_color() const;
    qint64 xp_calibrationId() const;
    bool xp_calcConcentration(const X_AbstractSpectrum* const, qreal& concentration);

    // windows
    int xp_createNewCalibrationWindow(
        int firstChannel = 0,
        int lastChannel = 0,
        X_CalibrationWindow::WindowType windowType = X_CalibrationWindow::WT_NOT_DEFINED,
        QString windowName = QString());
    int xp_createNewCalibrationWindow(const X_RawWindow& rawWindow);
    bool xp_isCalibrationWindowVisible(int windowIndex) const;
    bool xp_setCalibrationWindowVisible(int windowIndex, bool visibility);
    int xp_calibrationWindowCount() const;
    QString xp_calibrationWindowName(int windowIndex) const;
    const X_CalibrationWindow* xp_calibrationWindow(int windowIndex) const;
    QString xp_calibrationWindowNameForId(qint64 windowId) const;

    bool xp_setCalibrationWindowName(int windowIndex, const QString&);
    X_CalibrationWindow::WindowType xp_calibrationWindowType(int windowIndex) const;
    bool xp_setCalibrationWindowType(int windowIndex, X_CalibrationWindow::WindowType type);

    int xp_calibrationWindowFirstChannel(int windowIndex) const;
    bool xp_setCalibrationWindowFirstChannel(int windowIndex, int channel);
    int xp_calibrationWindowLastChannel(int windowIndex) const;
    bool xp_setCalibrationWindowLastChannel(int windowIndex, int channel);

    qint64 xp_calibrationWindowId(int windowIndex) const;
    int xp_windowIndexForName(const QString& windowName) const;
    bool xp_removeCalibrationWindow(int);
    void xp_removeCalibrationWindows();

    // terms
    int xp_termCount() const;
    QString xp_termDisplayName(int termIndex) const;
    QString xp_termName(int termIndex) const;
    QString xp_termExpression(int termIndex) const;
    QString xp_termDescription(int termIndex) const;

    X_AbstractTerm::TermType xp_termType(int termIndex) const;
    bool xp_mixedTermExists() const;
    qint64 xp_termId(int termIndex) const;
    bool xp_setTermState(int termIndex, X_AbstractTerm::TermState xp_termStateForDisplay);
    X_AbstractTerm::TermState xp_termStateForDisplay(int termIndex) const;
    X_AbstractTerm::TermState xp_termState(int termIndex) const;
    QString xp_termCustomString(int termIndex) const;

    void xp_setNextUsersTermState(int);
    bool xp_termFactor(int termIndex, qreal& factor) const;

    QList<int> xp_termWindowIndexList(int termIndex) const;
    QMap<QString, int> xp_termWindowIndexMap(int termIndex) const;

    QStringList xp_termWindowsNameList(int termIndex) const;

    QString xp_termFactorString(int termIndex) const;
    bool xp_termFactorString(int termIndex, QString& factorString) const;
    bool xp_setTermFactorString(int termIndex, const QString& factorString);
    bool xp_termVariablePart(int termIndex, const X_AbstractSpectrum* spectrum, qreal& value) const;
    bool xp_calcBaseTermValue(const X_AbstractSpectrum* spectrum, qreal& value) const;
    bool xp_createMixedTerms(int termIndex);
    bool xp_removeMixedTerms();
    bool xp_removeCustomTerm(int termIndex);
    int xp_createTerm(QList<int>& windowIndexList,
                      X_AbstractTerm::TermType termType,
                      X_AbstractTerm::TermState termState,
                      const QString& termFactor);
    int xp_createCustomTerm(X_AbstractTerm::TermState termState,
                            const QString& name,
                            const QString& equation,
                            const QString& description,
                            const QString& termFactor);

    //    int xp_createCustomTerm();
    int xp_applyRawTerm(X_RawTerm& rawTerm);

    // normalizer
    X_TermNormalizer::NormaType xp_normaType() const;
    bool xp_setNormaType(X_TermNormalizer::NormaType type);
    bool xp_setNormaCustomString(const QString& customString);
    QString xp_normaCustomString() const;

    X_TermNormalizer::NormaType xp_baseTermNormaType() const;
    bool xp_setBaseTermNormaType(X_TermNormalizer::NormaType type);
    bool xp_setBaseTermNormaCustomString(const QString& customString);
    QString xp_baseTermNormaCustomString() const;
    bool xp_setBaseTermNormalizerParameters(X_TermNormalizer::NormaType type,
                                            const QString& customString);

    EquationType xp_equationType() const;
    bool xp_setEquationType(EquationType type);

    qreal xp_equationIntercept() const;
    bool xp_setEquationIntercept(qreal);

    QString xp_equationInterceptString() const;
    bool xp_setEquationInterceptString(const QString&);

    QString xp_baseTermDisplayString() const;
    QString xp_baseTermString() const;

    qint64 xp_baseTermId() const;
    bool xp_setBaseTermId(qint64 id);
    int xp_baseTermIndex() const;
    bool xp_setBaseTermFromName(const QString& baseTermName);

    void xp_createEquationDataForEquationRecalc(QMap<int, qreal*>& factorMap, qreal*& freeTermPtr);

    void xh_notifyCalibrationRecalc();
    void xp_resetEquationTerms();
    X_CalibrationQualityData xp_calibrationQualityData() const;
    void xp_setCalibrationQualityData(X_CalibrationQualityData qualityData);

    void xp_setDetermination(const QString&);
    void xp_setAdjustedDetermination(const QString&);
    void xp_setStandardDeviation(const QString&);

    void xp_setCurrentDateTime();
    void xp_setDateTime(QDateTime);

    // STATIC
    // VARS
    static const QString simplePolynomEquationString;
    static const QString fractionalEquationString;
    // FUNCS
    static QString xp_equationTypeString(X_Calibration::EquationType type);
    static X_Calibration::EquationType xp_equationTypeFromString(const QString&);

signals:

    void xg_message(QString) const;
    void xg_windowOperation(X_Calibration::WindowOperationType, int first, int last) const;
    void xg_visibilityChanged(bool) const;
    void xg_dirtyChanged(bool) const;
    void xg_termOperation(X_Calibration::TremOperationType, int first, int last) const;
    void xg_normalizerChanged() const;
    void xg_interceptChanged() const;

public slots:

    void xp_updateCustomTerm(bool& res, X_RawCustomTerm& rawTerm);
    void xp_calcWindowIntensity(
        const QString& windowName,
        const QObject*, qreal&, bool keepBufferClean, bool* ok);

private slots:

    void xh_onTermNameChange();
    void xh_onTermWindowMarginChange();
    void xh_removeTerm(X_AbstractTerm*);
    void xh_onNormalizerChange();
    void xh_windowIsExist(const QString& windowName, bool& res) const;

private:
    // VARS
    QString xv_baseName;
    QString xv_suffix;
    QString xv_path;
    QString xv_chemElement;
    int xv_exposition;
    //    qreal xv_energyCalibrationFactorK0;
    //    qreal xv_energyCalibrationFactorK1;
    //    qreal xv_energyCalibrationFactorK2;
    int xv_gainFactor;
    //    QString xv_energyUnit;

    QColor xv_color;
    qint64 xv_calibrationId;
    X_CalibrationQualityData xv_calibrationQualityData;
    bool xv_visibility;
    bool xv_dirty;

    QList<X_CalibrationWindow*> xv_windowList;
    QList<X_AbstractTerm*> xv_termList;
    EquationType xv_equationType;
    X_TermNormalizer* xv_termNormalizer;
    QString xv_interceptString;
    qreal xv_intercept;
    qint64 xv_baseTermId;
    // qint64 xv_baseTermNormaId;
    X_TermNormalizer* xv_baseTermNormalizer;
    bool xv_useBaseTermInFractionalEquation;
    QDateTime xv_dateTime;

    // FUNCS
    bool xh_isWindowExist(const QString&);
    void xh_createTermsForWindow(const X_CalibrationWindow*);
    bool xh_windowHasTerms(const X_CalibrationWindow*, X_AbstractTerm::TermType) const;
    int xh_termIndex(const X_AbstractTerm*) const;
    void xh_chopTailX_eroesFromInterceptString();
    qreal* xh_termFactorPointer(int termIndex) const;
    int xh_termIndexForRawTerm(const X_RawTerm& rawTerm) const;
    int xh_termIndexForTermId(qint64 termId) const;
    int xh_findNextTermIndex(X_AbstractTerm::TermType) const;
    bool xh_updateExistingTerm(int termIndex, X_RawTerm& rawTerm);
    int xh_createNewTerm(X_RawTerm& rawTerm);
    bool xh_createWindowIndexList(QList<int>& windowIndexList, X_RawTerm& rawTerm);
    bool xh_createWindowIndexList(QMap<QString, int>& windowIndexMap, X_RawTerm& rawTerm);
    bool xh_checkCustomString(X_RawTerm& rawTerm);

    // STATIC
    // VARS
    static QList<QColor> xv_colorList;
    static qint64 xv_lastCalibrationId;
    static int xv_lastColorIndex;
    static int xv_precision;
    static const QString xv_defaultWindowName;

    static QMap<X_Calibration::EquationType, QString> xv_eqationTypeStringMap;
    static bool xv_useBaseTermInFractionalEquationByDefault;
    //  FUNCS
    static QList<QColor> xp_createColorList();
    static bool checkColor(QColor color);
    static QMap<X_Calibration::EquationType, QString> xh_initEquationTypeStringMap();
    static QString xh_initPlynomialEquationString();
    static QString xh_initFractionalEquationString();
    static QString xh_initFractionalEquationString1();
};
//=========================================================
#endif // QABSTRACTCALIBRATION_H

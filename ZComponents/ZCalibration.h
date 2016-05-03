//=========================================================
#ifndef QABSTRACTCALIBRATION_H
#define QABSTRACTCALIBRATION_H
//=========================================================
#include <QObject>
#include <QMap>
#include <QColor>
#include "ZCalibrationWindow.h"
#include "ZAbstractTerm.h"

//=========================================================
class ZAbstractSpectrum;
class ZSimpleTerm;
class ZQuadraticTerm;
class ZCrossProductTerm;
class ZCustomTerm;
class ZTermNormalizer;
//=========================================================

//=========================================================
class ZCalibration : public QObject
{
    Q_OBJECT
    friend class ZAbstractTerm;
    friend class ZTermNormalizer;
    friend class ZTermCorrelationTableManager;

public:

    explicit ZCalibration(const QString& name, QObject *parent = 0);
    virtual ~ZCalibration();

    // VARS
    enum WindowOperationType {WOT_BRGIN_INSERT_WINDOWS,
                              WOT_END_INSERT_WINDOWS,
                              WOT_BEGIN_REMOVE_WINDOWS,
                              WOT_END_REMOVE_WINDOWS,
                              WOT_WINDOW_CHANGED
                             };


    enum TremOperationType {TOT_BEGIN_INSERT_TERM,
                            TOT_END_INSERT_TERM,
                            TOT_BEGIN_REMOVE_TERM,
                            TOT_END_REMOVE_TERM,
                            TOT_TERM_NAME_CHANGED,
                            TOT_TERM_STATE_CHANGED,
                            TOT_TERM_WINDOW_MARGIN_CHANGED,
                            TOT_TERM_FACTOR_CHANGED
                           };

    enum EquationType {ET_NOT_DEFINED,
                       ET_POLYNOMIAL,
                       ET_FRACTIONAL
                      };
    // FUNCS
    QString zp_name() const;
    void zp_setBaseName(const QString&);

    QString zp_path() const;
    void zp_setPath(const QString&);

    QString zp_chemElement() const;
    bool zp_setChemElement(const QString&);

    bool zp_isVisible() const;
    void zp_setVisible(bool);

    bool zp_isDirty() const;
    QColor zp_color() const;
    qint64 zp_calibrationId() const;
    bool zp_calcConcentration(const ZAbstractSpectrum* const, qreal& concentration);

    // windows
    void zp_createNewCalibrationWindow(int& ,  int firstChannel = 0, int lastChannel = 0,
                                       ZCalibrationWindow::WindowType windowType = ZCalibrationWindow::WT_NOT_DEFINED);
    bool zp_isCalibrationWindowVisible(int windowIndex) const;
    bool zp_setCalibrationWindowVisible(int windowIndex, bool visibility);
    int zp_calibrationWindowCount() const;
    QString zp_calibrationWindowName(int windowIndex) const;
    const ZCalibrationWindow* zp_calibrationWindow(int windowIndex) const;

    bool zp_setCalibrationWindowName(int windowIndex, const QString&);
    ZCalibrationWindow::WindowType zp_calibrationWindowType(int windowIndex) const;
    bool zp_setCalibrationWindowType(int windowIndex, ZCalibrationWindow::WindowType type);

    int zp_calibrationWindowFirstChannel(int windowIndex) const;
    bool zp_setCalibrationWindowFirstChannel(int windowIndex, int channel);
    int zp_calibrationWindowLastChannel(int windowIndex) const;
    bool zp_setCalibrationWindowLastChannel(int windowIndex, int channel);

    qint64 zp_calibrationWindowId(int windowIndex) const;
    bool zp_removeCalibrationWindow(int);

    // terms
    int zp_termCount() const;
    QString zp_termName(int termIndex) const;
    qint64 zp_termId(int termIndex) const;
    ZAbstractTerm::TermState zp_termState(int termIndex) const;
    void zp_setNextUsersTermState(int) const;
    bool zp_termFactor(int termIndex, qreal &factor) const;

    bool zp_termFactorString(int termIndex, QString &factorString) const;
    bool zp_setTermFactorString(int termIndex, const QString& factorString) const;
    bool zp_termVariablePart(int termIndex, const ZAbstractSpectrum* spectrum,  qreal &value) const;
    bool zp_calcBaseTermValue(const ZAbstractSpectrum* spectrum, qreal& value) const;

    // normalizer
    ZTermNormalizer::NormaType zp_normaType() const;
    bool zp_setNormaType(ZTermNormalizer::NormaType type);
    bool zp_setNormaCustomString(const QString& customString);
    QString zp_customNormaString() const;

    ZTermNormalizer::NormaType zp_baseTermNormaType() const;
    bool zp_setBaseTermNormaType(ZTermNormalizer::NormaType type);
    bool zp_setBaseTermNormaCustomString(const QString& customString);
    QString zp_baseTermNormaCustomString() const;
    bool zp_setBaseTermNormalizerParameters(ZTermNormalizer::NormaType type,
                                  const QString& customString);


    EquationType zp_equationType() const;
    bool zp_setEquationType(EquationType type);

    qreal zp_equationIntercept() const;
    bool zp_setEquationIntercept(qreal);

    QString zp_equationInterceptString() const;
    bool zp_setEquationInterceptString(const QString&);

    QString zp_baseTermString() const;
    qint64 zp_baseTermId() const;
    bool zp_setBaseTermId(qint64 id);
    int zp_baseTermIndex() const;

    void zp_createEquationDataForEquationRecalc(QMap<int, qreal *> &factorMap,
                                                qreal *&freeTermPtr);

    void zh_notifyCalibrationRecalc();
    void zp_resetEquationTerms();

    // STATIC
    // VARS
    static const QString simplePolynomEquationString;
    static const QString fractionalEquationString;
    // FUNCS
    static QString zp_equationTypeString(ZCalibration::EquationType type);

signals:

    void zg_message(QString) const;
    void zg_windowOperation(ZCalibration::WindowOperationType, int first, int last) const;
    void zg_visibilityChanged(bool) const;
    void zg_dirtyChanged(bool) const;
    void zg_termOperation(ZCalibration::TremOperationType, int first, int last)const;
    void zg_normalizerChanged() const;
    void zg_interceptChanged() const;

public slots:


private slots:

    void zh_onTermNameChange() const;
    void zh_onTermWindowMarginChange();
    void zh_removeTerm(ZAbstractTerm*);
    void zh_onNormalizerChange();

private:

    // VARS
    QString zv_baseName;
    QString zv_path;
    QString zv_chemElement;
    QColor zv_color;
    qint64 zv_calibrationId;

    bool zv_visibility;
    bool zv_dirty;
    QString zv_defaultWindowName;

    QList<ZCalibrationWindow*> zv_windowList;
    QList<ZAbstractTerm*> zv_termList;
    EquationType zv_equationType;
    ZTermNormalizer* zv_termNormalizer;
    QString zv_interceptString;
    qreal zv_intercept;
    qint64 zv_baseTermId;
    // qint64 zv_baseTermNormaId;
    ZTermNormalizer* zv_baseTermNormalizer;
    bool zv_useBaseTermInFractionalEquation;

    //qreal zv_

    // FUNCS
    bool zh_isWindowExist(const QString&);
    void zh_createTermsForWindow(const ZCalibrationWindow*);
    bool zh_windowHasTerms(const ZCalibrationWindow*, ZAbstractTerm::TermType) const;
    int zh_termIndex(const ZAbstractTerm*) const;
    void zh_chopTailZeroesFromInterceptString();
    qreal* zh_termFactorPointer(int termIndex) const;

    // STATIC
    // VARS
    static QList<QColor> zv_colorList;
    static qint64 zv_lastCalibrationId;
    static int zv_lastColorIndex;
    static int zv_precision;

    static QMap<ZCalibration::EquationType, QString> zv_eqationTypeStringMap;
    static bool zv_useBaseTermInFractionalEquationByDefault;
    //  FUNCS
    static QList<QColor> zp_createColorList();
    static bool checkColor(QColor color);
    static QMap<ZCalibration::EquationType, QString> zh_initEquationTypeStringMap();
    static QString zh_initPlynomialEquationString();
    static QString zh_initFractionalEquationString();
    static QString zh_initFractionalEquationString1();

};
//=========================================================
#endif // QABSTRACTCALIBRATION_H

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
class ZCalibration : public QObject
{
    Q_OBJECT
    friend class ZAbstractTerm;
    friend class ZTermNormalizer;

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
                            TOT_TERM_VALUE_CHANGED
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
    void zp_createNewCalibrationWindow(int& , int firstChannel = 0, int lastChannel = 0);
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
    ZAbstractTerm::TermState zp_termState(int termIndex) const;
    void zp_setNextUsersTermState(int) const;
    bool zp_termFactor(int termIndex, qreal &factor) const;
    bool zp_setTermFactor(int termIndex, qreal factor) const;
    bool zp_termVariablePart(int termIndex, const ZAbstractSpectrum* spectrum,  qreal &value) const;
    //bool zp_termValue(int termIndex, const ZAbstractSpectrum*,  qreal &value) const;
    //bool zp_termAverageValue(int termIndex,  qreal &averageValue) const;
    //bool zp_calcTermAverageValues(const ZSpectrumArray* spectrumArray);

signals:

    void zg_message(QString) const;
    void zg_windowOperation(ZCalibration::WindowOperationType, int first, int last) const;
    void zg_visibilityChanged(bool) const;
    void zg_dirtyChanged(bool) const;
    void zg_termOperation(ZCalibration::TremOperationType, int first, int last)const;
    void zg_normalizerChanged() const;

public slots:


private slots:

    void zh_onTermNameChange() const;
    void zh_onTermAverageValueChange() const;
    void zh_normalizerValue(qreal&) const;
    void zh_onTermWindowMarginChange();
    void zh_removeTerm(ZAbstractTerm*);

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
    ZTermNormalizer* zv_termNormalizer;
    qreal zv_freeMemeber;
    //qreal zv_

    // FUNCS
    bool zh_isWindowExist(const QString&);
    void zh_createTermsForWindow(const ZCalibrationWindow*);
    bool zh_windowHasTerms(const ZCalibrationWindow*, ZAbstractTerm::TermType) const;
    int zp_termIndex(const ZAbstractTerm*) const;

    // STATIC
    // VARS
    static QList<QColor> zv_colorList;
    static qint64 zv_lastCalibrationId;
    static int zv_lastColorIndex;

    //  FUNCS
    static QList<QColor> zp_createColorList();
    static bool checkColor(QColor color);

};
//=========================================================
#endif // QABSTRACTCALIBRATION_H

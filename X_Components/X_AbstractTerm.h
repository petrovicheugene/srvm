//============================================================
#ifndef X_ABSTRACTTERM_H
#define X_ABSTRACTTERM_H
//============================================================
#include <QObject>
#include <QMap>
#include "X_TermNormalizer.h"
//============================================================
class X_AbstractSpectrum;
class X_CalibrationWindow;
class X_TermNormalizer;
class X_Calibration;
class X_SpectrumArray;
//============================================================
class X_AbstractTerm : public QObject
{
    Q_OBJECT
    friend  class X_Calibration;
public:
    explicit X_AbstractTerm(X_Calibration* calibration);

    // VARS
    enum TermType {TT_NOT_DEFINED,
                   TT_SIMPLE,
                   TT_QUADRATIC,
                   TT_MIXED,
                   TT_CUSTOM};

    enum TermState {TS_NOT_DEFINED,
                    TS_CONST_INCLUDED,
                    TS_EXAM_WAITING,
                    TS_INCLUDED,
                    TS_EXCEPTED,
                    TS_CONST_EXCLUDED,
                    TS_BASE};


    // FUNCS
    virtual bool xp_calcValue(const X_AbstractSpectrum*, qreal&) = 0;
    virtual bool xp_calcTermIntensity(const X_AbstractSpectrum*, qreal &value) = 0; // w/o factor
    QString xp_termName() const;
    X_AbstractTerm::TermType xp_termType() const;
    virtual bool xp_termBelongsToWindow(const X_CalibrationWindow*) const = 0;
    virtual QList<qint64> xp_termWindowIdList() const = 0;

    qint64 xp_termId();
    qreal xp_termFactor() const;
    QString xp_termFactorString() const;

    X_AbstractTerm::TermState xp_termState() const;
    bool xp_setTermState(X_AbstractTerm::TermState);
    void xp_setWindowMap(const QMap<QString, int>& windowMap);

    // STATIC
    static bool xp_setPrecision(int);
    static QString xp_termTypeName(X_AbstractTerm::TermType);
    static QString xp_termStateName(X_AbstractTerm::TermState);
    static X_AbstractTerm::TermType xp_termTypeFromString(const QString&);
    static X_AbstractTerm::TermState xp_termStateFromString(const QString&);

signals:

 //   void xg_requestNormalizerValue(qreal&);
    void xg_termWindowMarginChanged();
    void xg_termNameChanged() const;

    void xg_requestForDelete(X_AbstractTerm*);
    void xg_termStateChanged(X_AbstractTerm::TermState) const;
   // void xg_termValuesChanged() const;

public slots:


protected slots:

    void xh_normalizerChanged();
    void xh_onWindowDestroying();

protected slots:

    virtual void xh_updateTermNameForWindowName() = 0;
    virtual void xh_onWindowTypeChange(X_CalibrationWindow::WindowType previousType, X_CalibrationWindow::WindowType currentType) = 0;

protected:

    // VARS
    qint64 xv_termId = -1;
    TermType xv_type = TT_NOT_DEFINED;
    QString xv_termFactorString;
    qreal xv_termFactor = 0.0;
    TermState xv_termState = TS_NOT_DEFINED;
    QString xv_name;

    //    QList<qint64> xv_unnormalizedValueList;
    //    qreal xv_averageValue;
    // FUNCS
    //bool xh_setTermFactor(qreal factor);
    void xh_connectToNormalizer(X_TermNormalizer *);
    void xh_connectToCalibration(X_Calibration* calibration);
    void xh_chopTailX_eroesFromTermFactorString();
    qreal* xh_termFactorPointer();

    bool xh_setTermFactor(const QString&);
    bool xh_setTermFactor(qreal);
    void xh_conformStringWithValue();

    // STATIC
    // VARS
    static qint64 xv_lastTermId;
    static int xv_precision;

    static QMap<X_AbstractTerm::TermType, QString> xv_typeNameMap;
    static QMap<X_AbstractTerm::TermState, QString> xv_stateNameMap;

    // FUNCS
    static QMap<X_AbstractTerm::TermType, QString> xh_initTypeNameMap();
    static QMap<X_AbstractTerm::TermState, QString>xh_initStateNameMap();


};
//============================================================
#endif // X_ABSTRACTTERM_H

//============================================================
#ifndef ZABSTRACTTERM_H
#define ZABSTRACTTERM_H
//============================================================
#include <QObject>
#include <QMap>
#include "ZTermNormalizer.h"
//============================================================
class ZAbstractSpectrum;
class ZCalibrationWindow;
class ZTermNormalizer;
class ZCalibration;
class ZSpectrumArray;
//============================================================
class ZAbstractTerm : public QObject
{
    Q_OBJECT
    friend  class ZCalibration;
public:
    explicit ZAbstractTerm(ZCalibration*);

    // VARS
    enum TermType {TT_NOT_DEFINED,
                   TT_SIMPLE,
                   TT_QUADRATIC,
                   TT_MIXED,
                   TT_CUSTOM};

    enum TermState {TS_CONST_INCLUDED,
                    TS_EXAM_WAITING,
                    TS_INCLUDED,
                    TS_EXCEPTED,
                    TS_CONST_EXCLUDED};


    // FUNCS
    virtual bool zp_calcValue(const ZAbstractSpectrum*, qreal&) = 0;
    virtual bool zp_calcTermVariablePart(const ZAbstractSpectrum*, qreal &value) = 0; // w/o factor
    virtual QString zp_termName() const;
    virtual ZAbstractTerm::TermType zp_termType() const;
    virtual bool zp_termBelongsToWindow(const ZCalibrationWindow*) const = 0;

    qint64 zp_termId();
    qreal zp_termFactor() const;
    QString zp_termFactorString() const;
    bool zp_setTermFactorString(const QString&);

    ZAbstractTerm::TermState zp_termState() const;
    bool zp_setTermState(ZAbstractTerm::TermState);

    void zp_conformStringWithValue();

    // STATIC
    static bool zp_setPrecision(int);

signals:

 //   void zg_requestNormalizerValue(qreal&);
    void zg_termWindowMarginChanged();
    void zg_termNameChanged() const;
    void zg_requestForDelete(ZAbstractTerm*);
    void zg_termStateChanged(ZAbstractTerm::TermState) const;
   // void zg_termValuesChanged() const;

public slots:


protected slots:

    void zh_normalizerChanged();
    void zh_onWindowDestroying();

protected slots:

    virtual bool zh_updateTermNameForWindowName(const QString&) = 0;

protected:

    // VARS
    qint64 zv_termId;
    TermType zv_type;
    QString zv_termFactorString;
    qreal zv_termFactor;
    TermState zv_termState;
    QString zv_name;
    //    QList<qint64> zv_unnormalizedValueList;
    //    qreal zv_averageValue;
    // FUNCS
    //bool zh_setTermFactor(qreal factor);
    void zh_connectToNormalizer(ZTermNormalizer *);
    void zh_connectToCalibration(ZCalibration* calibration);
    void zh_chopTailZeroesFromTermFactorString();
    qreal* zh_termFactorPointer();

    // STATIC
    // VARS
    static qint64 zv_lastTermId;
    static int zv_precision;
    // FUNCS

};
//============================================================
#endif // ZABSTRACTTERM_H

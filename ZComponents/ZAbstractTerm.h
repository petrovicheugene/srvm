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
    virtual bool zp_calcTermVariablePart(const ZAbstractSpectrum*, qint64 &value) = 0; // w/o factor
    virtual QString zp_termName() const;
    virtual ZAbstractTerm::TermType zp_termType() const;
    virtual bool zp_termBelongsToWindow(const ZCalibrationWindow*) const = 0;

    qint64 zp_termId();
    qreal zp_termFactor() const;
    void zp_setTermFactor(qreal factor);

    ZAbstractTerm::TermState zp_termState() const;
    bool zp_setTermState(ZAbstractTerm::TermState);


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
    qreal zv_K;
    TermState zv_termState;
    QString zv_name;

//    QList<qint64> zv_unnormalizedValueList;
//    qreal zv_averageValue;
    // FUNCS
    void zh_connectToNormalizer(ZTermNormalizer *);
    void zh_connectToCalibration(ZCalibration* calibration);

    // STATIC
    // VARS
    static qint64 zv_lastTermId;

};
//============================================================
#endif // ZABSTRACTTERM_H

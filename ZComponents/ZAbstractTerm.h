//============================================================
#ifndef ZABSTRACTTERM_H
#define ZABSTRACTTERM_H
//============================================================
#include <QObject>
#include "ZTermNormalizer.h"
//============================================================
class ZAbstractSpectrum;
class ZCalibrationWindow;
class ZTermNormalizer;
class ZCalibration;

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
    virtual qreal zp_calcValue() = 0;
    virtual QString zp_termName() const;
    virtual ZAbstractTerm::TermType zp_termType() const;

    qreal zp_termFactor() const;

    ZAbstractTerm::TermState zp_termState() const;
    bool zp_setTermState(ZAbstractTerm::TermState);


signals:

    void zg_isNormalizerValid(bool&);
    void zg_normalizerValue(qreal&);
    void zg_termNameChanged() const;
    void zg_requestForDelete(ZAbstractTerm*);
    void zg_termStateChanged(ZAbstractTerm::TermState) const;

public slots:


protected slots:

    void zh_normalizerChanged();
    void zh_onWindowDestroying();

protected slots:

    virtual bool zh_updateTermNameForWindowName(const QString&) = 0;

protected:

    // VARS
    TermType zv_type;
    qreal zv_K;
    TermState zv_termState;
    QString zv_name;
    // FUNCS
    void zh_connectToNormalizer(ZTermNormalizer *);
    void zh_connectToCalibration(ZCalibration* calibration);
};
//============================================================
#endif // ZABSTRACTTERM_H

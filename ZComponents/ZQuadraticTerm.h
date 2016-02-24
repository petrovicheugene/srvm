//===================================================================
#ifndef ZQUADRATICTERM_H
#define ZQUADRATICTERM_H
//===================================================================
#include "ZAbstractTerm.h"
//===================================================================
class ZCalibrationWindow;
//===================================================================
class ZQuadraticTerm : public ZAbstractTerm
{
    Q_OBJECT
public:
    explicit ZQuadraticTerm(const ZCalibrationWindow* window,
                            ZCalibration *parent);

    // FUNCS
    bool zp_calcValue(const ZAbstractSpectrum*, qreal&) override;
    bool zp_calcTermVariablePart(const ZAbstractSpectrum*, qint64 &value) override; // w/o factor

signals:

    void zg_requestWindowIntensity(const QObject* spectrum, qint64& intensityValue, bool keepBufferClean, bool* ok = 0);

protected slots:

    virtual bool zh_updateTermNameForWindowName(const QString&windowName) override;

private:

    // VARS


    // FUNCS
    void zh_setName(const QString& windowName);
    void zh_connectToWindow(const ZCalibrationWindow* window);

};
//===================================================================
#endif // ZQUADRATICTERM_H

//===================================================================
#ifndef ZSIMPLETERM_H
#define ZSIMPLETERM_H
//===================================================================
#include "ZAbstractTerm.h"
//===================================================================
class ZCalibrationWindow;
//===================================================================
class ZSimpleTerm : public ZAbstractTerm
{
   Q_OBJECT
public:
   explicit ZSimpleTerm(const ZCalibrationWindow* window,
                             ZCalibration *parent);

   // FUNCS
   bool zp_calcValue(const ZAbstractSpectrum*, qreal&) override;
   bool zp_calcTermVariablePart(const ZAbstractSpectrum*, qint64 &value) override; // w/o factor


signals:

    void zg_requestWindowIntensity(const QObject* spectrum, qint64& intensityValue, bool* ok = 0);

protected slots:

   virtual bool zh_updateTermNameForWindowName(const QString&windowName) override;

private:

   // VARS


   // FUNCS

   void zh_connectToWindow(const ZCalibrationWindow* window);

};
//===================================================================
#endif // ZSIMPLETERM_H

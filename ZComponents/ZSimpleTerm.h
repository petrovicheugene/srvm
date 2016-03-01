//===================================================================
#ifndef ZSIMPLETERM_H
#define ZSIMPLETERM_H
//===================================================================
#include "ZAbstractTerm.h"
#include "ZCalibrationWindow.h"
//===================================================================

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
   bool zp_termBelongsToWindow(const ZCalibrationWindow*) const override;

   const ZCalibrationWindow* zp_window() const;

signals:

   void zg_requestWindowIntensity(const QObject* spectrum, qint64& intensityValue, bool keepBufferClean,  bool* ok = 0);

protected slots:

   virtual bool zh_updateTermNameForWindowName(const QString&windowName) override;
   void zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType,
                              ZCalibrationWindow::WindowType currentType);

private:

   // VARS
   const ZCalibrationWindow* zv_window;


   // FUNCS

   void zh_connectToWindow(const ZCalibrationWindow* window);

};
//===================================================================
#endif // ZSIMPLETERM_H

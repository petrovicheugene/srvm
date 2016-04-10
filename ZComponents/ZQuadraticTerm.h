//===================================================================
#ifndef ZQUADRATICTERM_H
#define ZQUADRATICTERM_H
//===================================================================
#include "ZAbstractTerm.h"
#include "ZCalibrationWindow.h"

//===================================================================

//===================================================================
class ZQuadraticTerm : public ZAbstractTerm
{
    Q_OBJECT
public:
    explicit ZQuadraticTerm(const ZCalibrationWindow* window,
                            ZCalibration *parent);

    // FUNCS
    bool zp_calcValue(const ZAbstractSpectrum*, qreal&) override;
    bool zp_calcTermVariablePart(const ZAbstractSpectrum*, qreal &value) override; // w/o factor
    bool zp_termBelongsToWindow(const ZCalibrationWindow*) const override;

    const ZCalibrationWindow* zp_window() const;


signals:

    void zg_requestWindowIntensity(const QObject* spectrum, qreal& intensityValue, bool keepBufferClean, bool* ok = 0);

protected slots:

    virtual bool zh_updateTermNameForWindowName(const QString&windowName) override;
    void zh_onWindowTypeChange(ZCalibrationWindow::WindowType previousType, ZCalibrationWindow::WindowType currentType);

private:

    // VARS
    const ZCalibrationWindow* zv_window;

    // FUNCS
    void zh_setName(const QString& windowName);
    void zh_connectToWindow(const ZCalibrationWindow* window);

};
//===================================================================
#endif // ZQUADRATICTERM_H

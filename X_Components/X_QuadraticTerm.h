//===================================================================
#ifndef X_QUADRATICTERM_H
#define X_QUADRATICTERM_H
//===================================================================
#include "X_AbstractTerm.h"
#include "X_CalibrationWindow.h"

//===================================================================

//===================================================================
class X_QuadraticTerm : public X_AbstractTerm
{
    Q_OBJECT
public:
    explicit X_QuadraticTerm(const X_CalibrationWindow* window,
                            X_Calibration *parent);

    // FUNCS
    bool xp_calcValue(const X_AbstractSpectrum*, qreal&) override;
    bool xp_calcTermVariablePart(const X_AbstractSpectrum*, qreal &value) override; // w/o factor
    bool xp_termBelongsToWindow(const X_CalibrationWindow*) const override;
    QList<qint64> xp_termWindowIdList() const override;
    const X_CalibrationWindow* xp_window() const;

signals:

    void xg_requestWindowIntensity(const QObject* spectrum, qreal& intensityValue, bool keepBufferClean, bool* ok = 0);

protected slots:

    virtual void xh_updateTermNameForWindowName() override;
    virtual void xh_onWindowTypeChange(X_CalibrationWindow::WindowType previousType, X_CalibrationWindow::WindowType currentType) override;

private:

    // VARS
    const X_CalibrationWindow* xv_window;

    // FUNCS
    void xh_setName();
    void xh_connectToWindow();

};
//===================================================================
#endif // X_QUADRATICTERM_H

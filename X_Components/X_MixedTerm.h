//===================================================================
#ifndef X_CROSSPRODUCTTERM_H
#define X_CROSSPRODUCTTERM_H
//===================================================================
#include "X_AbstractTerm.h"
//===================================================================
class X_CalibrationWindow;
//===================================================================
class X_MixedTerm : public X_AbstractTerm
{
    Q_OBJECT
public:
    explicit X_MixedTerm(const X_CalibrationWindow *window1,
                        const X_CalibrationWindow *window2,
                        X_Calibration *parent);

    // FUNCS
    bool xp_calcValue(const X_AbstractSpectrum*, qreal&) override;
    bool xp_calcTermIntensity(const X_AbstractSpectrum*, qreal &value) override; // w/o factor
    bool xp_termBelongsToWindow(const X_CalibrationWindow*) const override;
    QList<qint64> xp_termWindowIdList() const override;

    //QString xp_termName() const override;

signals:

    void xg_requestWindow1Intensity(const QObject* spectrum, qreal& intensityValue,  bool* ok = 0);
    void xg_requestWindow2Intensity(const QObject* spectrum, qreal& intensityValue,  bool* ok = 0);

protected slots:

    virtual void xh_updateTermNameForWindowName() override;
    virtual void xh_onWindowTypeChange(X_CalibrationWindow::WindowType previousType,
                                       X_CalibrationWindow::WindowType currentType) override;

private:

    // VARS
    const X_CalibrationWindow* xv_window1;
    const X_CalibrationWindow* xv_window2;

    // FUNCS
    void xh_setName();
    void xh_connectToWindows();

};
//===================================================================
#endif // X_CROSSPRODUCTTERM_H

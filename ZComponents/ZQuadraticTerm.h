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
    qreal zp_calcValue() override;

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

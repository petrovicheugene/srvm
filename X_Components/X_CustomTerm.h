//===================================================================
#ifndef X_CUSTOMTERM_H
#define X_CUSTOMTERM_H
//===================================================================
#include "X_AbstractTerm.h"
#include "X_MathExpressionHandler.h"
#include <QString>
//===================================================================
class X_CustomTerm : public X_AbstractTerm
{
   Q_OBJECT
public:
    explicit X_CustomTerm(X_Calibration* parent);


   // FUNCS
    bool xp_calcValue(const X_AbstractSpectrum*, qreal&) override;
    bool xp_calcTermVariablePart(const X_AbstractSpectrum*, qreal& value) override; // w/o factor
    bool xp_termBelongsToWindow(const X_CalibrationWindow*) const override;
    QList<qint64> xp_termWindowIdList() const override;

    QString xp_expression() const;
    QString xp_description() const;

    void xp_setName(const QString& name);
    bool xp_setExpression(const QString& equation);
    void xp_setDescription(const QString& description);

signals:

    void zs_calcWindowIntensity(
        const QString& windowName, const QObject*, qreal&, bool keepBufferClean, bool* ok);
    void zs_errorReport(const QString& errorMsg) const;
    void xg_windowIsExist(const QString& name, bool& res) const;

protected slots:

    virtual void xh_updateTermNameForWindowName() override;
    virtual void xh_onWindowTypeChange(X_CalibrationWindow::WindowType previousType,
                                       X_CalibrationWindow::WindowType currentType) override;
    void xh_handleWindowIntensityRequest(const QString& varName, double& value, bool& bRes);
    void xh_handleErrorReport(const QString& errorString,
                        int errorTokenStartPosition,
                        int errorTokenEndPosition) const;

private:
    // VARS
    X_MathExpressionHandler xv_mathExpressionHandler;
    QString xv_expression;
    QString xv_description;
    QStringList xv_windowList;
    const X_AbstractSpectrum* xv_spectrum;

    // FUNCS
    void xh_createConnections();
    bool xh_checkExpression(const QString &expression);

};
//===================================================================
#endif // X_CUSTOMTERM_H

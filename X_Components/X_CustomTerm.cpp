//===================================================================
#include "X_CustomTerm.h"
#include "X_CalibrationWindow.h"
#include "X_MathExpressionVariableListMaker.h"

//===================================================================
X_CustomTerm::X_CustomTerm(X_Calibration* parent) : X_AbstractTerm(parent)
{
    xv_type = TT_CUSTOM;
    xh_createConnections();
}
//===================================================================
void X_CustomTerm::xh_createConnections()
{
    connect(&xv_mathExpressionHandler,
            &X_MathExpressionHandler::zs_requestVariableValue,
            this,
            &X_CustomTerm::xh_handleWindowIntensityRequest);
    connect(&xv_mathExpressionHandler,
            &X_MathExpressionHandler::zs_errorReport,
            this,
            &X_CustomTerm::xh_handleErrorReport);
}
//===================================================================
void X_CustomTerm::xh_handleWindowIntensityRequest(const QString& windowName,
                                                  double& value,
                                                  bool& bRes)
{
    emit zs_calcWindowIntensity(windowName, (const QObject*) xv_spectrum, value, true, &bRes);
}
//===================================================================
void X_CustomTerm::xh_handleErrorReport(const QString& errorString,
                                       int errorTokenStartPosition,
                                       int errorTokenEndPosition) const
{
    emit zs_errorReport(QString("%1 (%2)!").arg(errorString, QString::number(errorTokenStartPosition)));

}
//===================================================================
bool X_CustomTerm::xp_calcValue(const X_AbstractSpectrum* spectrum, qreal& value)
{
    value = 0.0;
    if(xv_termState != TS_CONST_INCLUDED && xv_termState != TS_INCLUDED)
    {
        return true;
    }

    xv_spectrum = spectrum;
    bool res = xv_mathExpressionHandler.xp_calculateExpression(xv_expression, value);
    xv_spectrum = nullptr;
    value *= xv_termFactor;

    return res;
}
//===================================================================
bool X_CustomTerm::xp_calcTermVariablePart(const X_AbstractSpectrum* spectrum,
                                          qreal& value) // w/o factor
{
    xv_spectrum = spectrum;
    bool res = xv_mathExpressionHandler.xp_calculateExpression(xv_expression, value);
    xv_spectrum = nullptr;

    return res;
}
//===================================================================
bool X_CustomTerm::xp_termBelongsToWindow(const X_CalibrationWindow* window) const
{
    return xv_windowList.contains(window->xp_windowName());
}
//===================================================================
QList<qint64> X_CustomTerm::xp_termWindowIdList() const
{
    QList<qint64> windowIdList;
    return windowIdList;
}
//===================================================================
void X_CustomTerm::xh_updateTermNameForWindowName()
{
    if (!sender())
    {
        return;
    }
    //    else if(sender() != xv_window1 && sender() != xv_window2)
    //    {
    //        return;
    //    }

    //    xh_setName();
    //emit xg_termNameChanged();
}
//===================================================================
void X_CustomTerm::xh_onWindowTypeChange(X_CalibrationWindow::WindowType previousType,
                                        X_CalibrationWindow::WindowType currentType)
{
    //    if(!sender() || (sender() != xv_window1 && sender() != xv_window2))
    //    {
    //        return;
    //    }

    //    if(currentType != X_CalibrationWindow::WT_PEAK)
    //    {
    //        emit xg_requestForDelete(this);
    //    }
}
//===================================================================
QString X_CustomTerm::xp_expression() const
{
    return xv_expression;
}
//===================================================================
QString X_CustomTerm::xp_description() const
{
    return xv_description;
}
//===================================================================
void X_CustomTerm::xp_setName(const QString& name)
{
    xv_name = name;
    emit xg_termNameChanged();
}
//===================================================================
bool X_CustomTerm::xp_setExpression(const QString& expression)
{
    xv_expression = expression;
    return xh_checkExpression(xv_expression);
}
//===================================================================
void X_CustomTerm::xp_setDescription(const QString& description)
{
    xv_description = description;
}
//===================================================================
bool X_CustomTerm::xh_checkExpression(const QString& expression)
{
    X_MathExpressionHandler mathExpressionHandler;
    X_MathExpressionVariableListMaker mathExpressionVariableListMaker;
    connect(&mathExpressionHandler,
            &X_MathExpressionHandler::zs_requestVariableValue,
            &mathExpressionVariableListMaker,
            &X_MathExpressionVariableListMaker::xp_insertVariableValue);
    connect(&mathExpressionHandler,
            &X_MathExpressionHandler::zs_errorReport,
            &mathExpressionVariableListMaker,
            &X_MathExpressionVariableListMaker::xp_setError);
    connect(&mathExpressionVariableListMaker,
            &X_MathExpressionVariableListMaker::zs_variableCheckRequest,
            this,
            &X_CustomTerm::xg_windowIsExist);

    double res;
    if (!mathExpressionHandler.xp_calculateExpression(expression, res))
    {
        QString error;
        int errorStartPos;
        int errorEndPos;
        mathExpressionVariableListMaker.xp_error(error, errorStartPos, errorEndPos);
        QString msg = tr("Error in the term \"%1\" expression: %2").arg(xv_name, error);
        emit zs_errorReport(msg);
        return false;
    }
    xv_windowList = mathExpressionVariableListMaker.xp_variableList();
    return true;
}
//===================================================================


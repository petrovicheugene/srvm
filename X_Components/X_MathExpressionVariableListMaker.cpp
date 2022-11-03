//===================================================
#include "X_MathExpressionVariableListMaker.h"
#include <QRandomGenerator>
//===================================================
X_MathExpressionVariableListMaker::X_MathExpressionVariableListMaker(QObject* parent)
    : QObject(parent)
{}
//===================================================
QStringList X_MathExpressionVariableListMaker::xp_variableList() const
{
    return xv_variableNameMap.keys();
}
//===================================================
void X_MathExpressionVariableListMaker::xp_error(QString& errorString,
                                                int& errorTokenStartPosition,
                                                int& errorTokenEndPosition) const
{
    errorString = xv_errorString;
    errorTokenStartPosition = xv_errorTokenStartPosition;
    errorTokenEndPosition = xv_errorTokenEndPosition;
}
//===================================================
void X_MathExpressionVariableListMaker::xp_clear()
{
    xv_variableNameMap.clear();
    xv_errorString.clear();
    xv_errorTokenStartPosition = -1;
    xv_errorTokenEndPosition = -1;
}
//===================================================
void X_MathExpressionVariableListMaker::xp_insertVariableValue(const QString& varName,
                                                              double& value,
                                                              bool& bRes)
{
    emit zs_variableCheckRequest(varName, bRes);
    if (!bRes )
    {
        return;
    }

    if (!xv_variableNameMap.keys().contains(varName))
    {
        xv_variableNameMap.insert(varName, static_cast<qreal>(QRandomGenerator::global()->generate()));
    }

    value = xv_variableNameMap.value(varName);
}
//===================================================
void X_MathExpressionVariableListMaker::xp_setError(const QString& errorString,
                                                   int errorTokenStartPosition,
                                                   int errorTokenEndPosition)
{
    xv_errorString = errorString;
    xv_errorTokenStartPosition = errorTokenStartPosition;
    xv_errorTokenEndPosition = errorTokenEndPosition;
}
//===================================================

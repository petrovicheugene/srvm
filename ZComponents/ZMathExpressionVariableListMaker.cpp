//===================================================
#include "ZMathExpressionVariableListMaker.h"
//===================================================
ZMathExpressionVariableListMaker::ZMathExpressionVariableListMaker(QObject* parent)
    : QObject(parent)
{}
//===================================================
QStringList ZMathExpressionVariableListMaker::zp_variableList() const
{
    return zv_variableNameList;
}
//===================================================
void ZMathExpressionVariableListMaker::zp_error(QString& errorString,
                                                int& errorTokenStartPosition,
                                                int& errorTokenEndPosition) const
{
    errorString = zv_errorString;
    errorTokenStartPosition = zv_errorTokenStartPosition;
    errorTokenEndPosition = zv_errorTokenEndPosition;
}
//===================================================
void ZMathExpressionVariableListMaker::zp_clear()
{
    zv_variableNameList.clear();
    zv_errorString.clear();
    zv_errorTokenStartPosition = -1;
    zv_errorTokenEndPosition = -1;
}
//===================================================
void ZMathExpressionVariableListMaker::zp_insertVariableValue(const QString& varName,
                                                              double& value,
                                                              bool& bRes)
{
    emit zs_variableCheckRequest(varName, bRes);
    if(bRes)
    {
        zv_variableNameList.append(varName);
    }

    value = 0.0;
}
//===================================================
void ZMathExpressionVariableListMaker::zp_setError(const QString& errorString,
                                                   int errorTokenStartPosition,
                                                   int errorTokenEndPosition)
{
    zv_errorString = errorString;
    zv_errorTokenStartPosition = errorTokenStartPosition;
    zv_errorTokenEndPosition = errorTokenEndPosition;
}
//===================================================

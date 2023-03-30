//===================================================
#include "X_MathExpressionHandler.h"
#include <math.h>
#include <QDebug>
#include <QLocale>
#include <QRegExp>
#include <QStack>
//===================================================
X_MathExpressionHandler::X_MathExpressionHandler(QObject* parent) : QObject(parent)
{
    xv_tokenType = TT_NULL;
    xv_pairSymbolVector << QPair<QChar, QChar>('[', ']') << QPair<QChar, QChar>('(', ')')
                        << QPair<QChar, QChar>('{', '}');
    xv_delimiterSet << '(' << ')' << '+' << '-' << '*' << '/' << '^' << '[' << ']' << '\0';
    xv_operatorSet << "ln"
                   << "lg"
                   << "exp";
    xv_extraSymbolSet << "#" << "№" << "@" << "`" << "$";
}
//===================================================
void X_MathExpressionHandler::xh_findBracketForward(QChar bracketSymbol,
                                                   const QString& expression,
                                                   int pos,
                                                   QPair<int, bool>& highlight)
{
    int openBracketCount = 0;
    for (int i = pos + 1; i < expression.length(); i++)
    {
        QChar currentSymbol = expression.at(i);

        if (currentSymbol == bracketSymbol)
        {
            if (openBracketCount == 0)
            {
                highlight.first = i;
                highlight.second = true;
                return;
            }
            else
            {
                openBracketCount--;
                continue;
            }
        }
        else
        {
            bool quitCircle = false;
            for (int s = 0; s < xv_pairSymbolVector.count(); s++)
            {
                if (quitCircle)
                {
                    continue;
                }

                if (currentSymbol == xv_pairSymbolVector.value(s).first) // open bracket, not to find
                {
                    openBracketCount++;
                    quitCircle = true;
                    continue;
                }

                if (currentSymbol == xv_pairSymbolVector.value(s).second)
                {
                    if (openBracketCount < 1)
                    {
                        highlight.first = i;
                        highlight.second = false;
                        return;
                    }
                    else
                    {
                        openBracketCount--;
                        quitCircle = true;
                        continue;
                    }
                }
            }
        }
    } // end of for(int i = pos+1; i < equation.count(); i++)

    highlight.first = expression.length() - 1;
    highlight.second = false;
}
//===================================================
void X_MathExpressionHandler::xh_findBracketBackward(QChar bracketSymbol,
                                                    const QString& expression,
                                                    int pos,
                                                    QPair<int, bool>& highlight)
{
    int closeBracketCount = 0;
    for (int i = pos - 2; i >= 0; i--)
    {
        QChar currentSymbol = expression.at(i);

        if (currentSymbol == bracketSymbol)
        {
            if (closeBracketCount == 0)
            {
                highlight.first = i;
                highlight.second = true;
                return;
            }
            else
            {
                closeBracketCount--;
                continue;
            }
        }
        else
        {
            bool quitCircle = false;
            for (int s = 0; s < xv_pairSymbolVector.count(); s++)
            {
                if (quitCircle)
                {
                    continue;
                }

                if (currentSymbol
                    == xv_pairSymbolVector.value(s).second) // open bracket, not to find
                {
                    closeBracketCount++;
                    quitCircle = true;
                    continue;
                }

                if (currentSymbol == xv_pairSymbolVector.value(s).first)
                {
                    if (closeBracketCount < 1)
                    {
                        highlight.first = i;
                        highlight.second = false;
                        return;
                    }
                    else
                    {
                        closeBracketCount--;
                        quitCircle = true;
                        continue;
                    }
                }
            }
        }
    } // end of for(int i = pos+1; i < equation.count(); i++)

    highlight.first = 0;
    highlight.second = false;
}
//===================================================
bool X_MathExpressionHandler::xh_getToken()
{
    xv_token = QString();
    xv_tokenType = TT_NULL;
    int nError = -1;

    QChar currentChar;
    //QString token;

    xv_tokenStartPosition = xv_charPosition;
    QRegExp cellRegExp("^\\s{0,}\\[\\s{0,}$");

    // QRegExp("^~\\w{0,}$");

    for (; xv_charPosition < xv_expression.length(); xv_charPosition++)
    {
        currentChar = xv_expression.at(xv_charPosition);

        if (currentChar.isSpace())
        {
            if (xv_tokenType == TT_VARIABLE)
            {
                xv_token.append(currentChar);
            }
            continue;
        }

        if (xv_delimiterSet.contains(currentChar))
        {
            switch (xv_tokenType)
            {
            case TT_CELL:
                if (currentChar == ']')
                {
                    xv_token.append(currentChar);
                    xv_tokenEndPosition = xv_charPosition;
                    xv_charPosition++;

                    return true;
                }

                if ((currentChar == '-' || currentChar == '+')
                    && (!xv_token.contains('-') && !xv_token.contains('+')))
                {
                    xv_token.append(currentChar);
                    continue;
                }

                // mf_handleError(2);
                xv_tokenType = TT_ERROR;
                //return false;

                nError = 2;
                continue;

                break;
            case TT_NULL:
                xv_token.append(currentChar);
                if (currentChar == '[')
                {
                    xv_tokenType = TT_CELL;
                    continue;
                }

                if (currentChar == ']')
                {
                    // mf_handleError(3);
                    xv_tokenType = TT_ERROR;

                    //return false;
                    nError = 3;
                    continue;
                }

                xv_tokenType = TT_DELIMITER;
                xv_tokenEndPosition = xv_charPosition;
                xv_charPosition++;
                return true;

                break;
//            case TT_OPERATOR:
//                qDebug() << "TOK" << xv_token;
//                break;
            case TT_VARIABLE:
                if (xv_operatorSet.contains(xv_token.simplified()))
                {
                    xv_tokenType = TT_OPERATOR;
                }
                xv_tokenEndPosition = xv_charPosition - 1;
                return true;
                break;

            case TT_ERROR:
                xv_tokenEndPosition = xv_charPosition;
                xh_handleError(nError);
                return false;

                break;
            default:
                xv_tokenEndPosition = xv_charPosition - 1;
                return true;

                //return true;
            }
        }
        else if (currentChar.isLetter() || xv_extraSymbolSet.contains(currentChar))
        {
            switch (xv_tokenType)
            {
            case TT_NULL:
            case TT_VARIABLE:
                xv_tokenType = TT_VARIABLE;
                xv_token.append(currentChar);

                continue;
                // break;
            case TT_CELL:
                if (!xv_token.contains('-') && !xv_token.contains('+'))
                {
                    xv_token.append(currentChar);
                    continue;
                }

            default:
                //mf_handleError(2);
                nError = 2;
                xv_tokenType = TT_ERROR;
                continue;
                break;
            }
        }
        else if (currentChar == '~')
        {
            switch (xv_tokenType)
            {
            case TT_NULL:
                xv_tokenType = TT_VARIABLE;
                xv_token.append(currentChar);
                continue;

            case TT_CELL:

                if (cellRegExp.exactMatch(xv_token))
                {
                    xv_token.append(currentChar);
                }
                continue;

                //break;
                //            case TT_VARIABLE:

                //                mv_token.append(currentChar);
                //                continue;
                //                break;
                //            case TT_CELL:
                //                if(!mv_token.contains('-') && !mv_token.contains('+'))
                //                {
                //                    mv_token.append(currentChar);
                //                    continue;
                //                }

            default:
                //mf_handleError(2);
                nError = 2;
                xv_tokenType = TT_ERROR;
                continue;
                // return false;
            }
        }
        else if (currentChar.isDigit())
        {
            switch (xv_tokenType)
            {
            case TT_NULL:
                xv_tokenType = TT_INTEGER;
            case TT_VARIABLE:
            case TT_INTEGER:
            case TT_DOUBLE:
            case TT_CELL:
                xv_token.append(currentChar);

                continue;
                break;

            default:
                // mf_handleError(2);
                nError = 2;
                xv_tokenType = TT_ERROR;
                //return false;
                continue;
            }
        }
        else if (currentChar == '.' || currentChar == ',')
        {
            switch (xv_tokenType)
            {
            case TT_INTEGER:

                xv_tokenType = TT_DOUBLE;
                xv_token.append(currentChar);
                continue;
                break;

            default:
                // mf_handleError(2);
                nError = 2;
                xv_tokenType = TT_ERROR;
                //return false;
                continue;
            }
        }
        else
        {
            // mf_handleError(2);
            nError = 2;
            xv_tokenType = TT_ERROR;
            //return false;
            continue;
        }
    }

    xv_tokenEndPosition = xv_expression.length() - 1;

    if (xv_tokenType == TT_CELL)
    {
        xh_handleError(2);
        xv_tokenType = TT_ERROR;
        return false;
    }

    if (xv_tokenType == TT_ERROR)
    {
        xh_handleError(nError);
        return false;
    }

    return true;

    //return true;
}
//===================================================
bool X_MathExpressionHandler::xh_addOrSubtract(double& result)
{
    QString op;
    double temp;
    if (!xh_multiplyOrDevide(result))
    {
        return false;
    }
    while ((op = xv_token) == "+" || op == "-")
    {
        if (!xh_getToken())
        {
            return false;
        }
        if (!xh_multiplyOrDevide(temp))
        {
            return false;
        }

        if (op == "-")
        {
            result = result - temp;
        }
        else if (op == "+")
        {
            result = result + temp;
        }
    }

    return true;
}
//===================================================
bool X_MathExpressionHandler::xh_multiplyOrDevide(double& result)
{
    QString op;
    double temp;
    if (!xh_power(result))
    {
        return false;
    }

    while ((op = xv_token) == "*" || op == "/")
    {
        if (!xh_getToken())
        {
            return false;
        }
        if (!xh_power(temp))
        {
            return false;
        }

        if (op == "*")
        {
            result = result * temp;
        }
        else if (op == "/")
        {
            if (temp == 0)
            {
                xh_handleError(1);
                return false;
            }
            result = result / temp;
        }
    }

    return true;
}
//===================================================
bool X_MathExpressionHandler::xh_power(double& result)
{
    if (!xh_unaryOperation(result))
    {
        return false;
    }

    if (xv_token == "^")
    {
        if (!xh_getToken())
        {
            return false;
        }

        double temp = 0.0;
        if (!xh_unaryOperation(temp))
        {
            return false;
        }

        double ex = result;
        if (temp == 0.0)
        {
            result = 1.0;
            return true;
        }

        int t;
        for (t = (int) temp - 1; t > 0; --t)
        {
            result = result * (double) ex;
        }
        return true;
    }

    return true;
}
//===================================================
bool X_MathExpressionHandler::xh_unaryOperation(double& result)
{
    QString op;
    if ((xv_tokenType == TT_DELIMITER || xv_tokenType == TT_OPERATOR)
        && (xv_token == "+" || xv_token == "-" || xv_token == "ln" || xv_token == "lg"
            || xv_token == "exp"))
    {
        op = xv_token;
        if (!xh_getToken())
        {
            return false;
        }
    }

    if (xh_recognizeBrackets(result))
    {
        if (op == "-")
        {
            result = -result;
        }
        else if (op == "ln")
        {
            result = log(result);
        }
        else if (op == "lg")
        {
            result = log10(result);
        }
        else if (op == "exp")
        {
            result = exp(result);
        }

        return true;
    }
    else
    {
        return false;
    }
}
//===================================================
bool X_MathExpressionHandler::xh_recognizeBrackets(double& result)
{
    if (xv_token == "(")
    {
        if (!xh_getToken())
        {
            return false;
        }
        if (!xh_addOrSubtract(result))
        {
            return false;
        }

        if (xv_token != ")")
        {
            xh_handleError(3);
            return false;
        }
        return xh_getToken();
    }
    else
    {
        return xh_variableValue(result);
    }
}
//===================================================
bool X_MathExpressionHandler::xh_variableValue(double& result)
{
    bool res;
    QString simplifiedToken = xv_token.simplified();
    QLocale locale;
    if (xv_tokenType == TT_DOUBLE || xv_tokenType == TT_INTEGER)
    {
        result = locale.toDouble(simplifiedToken, &res);
        if (!res)
        {
            xh_handleError(4);
            return false;
        }
    }
    else if (xv_tokenType == TT_VARIABLE || xv_tokenType == TT_CELL)
    {
        emit xg_requestVariableValue(simplifiedToken, result, res);
        if (!res)
        {
            xh_handleError(5);
            return false;
        }
    }

    return xh_getToken();
}
//===================================================
void X_MathExpressionHandler::xh_handleError(int error)
{
    QString errorString;
    switch (error)
    {
    case 0:
        errorString = tr("Empty expression");
        break;
    case 1:
        errorString = tr("Division by zero");
        break;
    case 2:
        errorString = tr("Wrong token");
        break;
    case 3:
        errorString = tr("Bracket error");
        break;
    case 4:
        errorString = tr("Cannot convert to double");
        break;
    case 5:
        errorString = tr("Wrong variable");
        break;
    }

    emit xg_errorReport(errorString, xv_tokenStartPosition, xv_tokenEndPosition);
}
//===================================================
//bool X_EquationHandler::mf_checkSpacesInToken()
//{
//    mv_token = mv_token.trimmed();
//    QRegExp regExp("[' ',\t,\n,\v,\f,\r]");
//    if(regExp.exactMatch(mv_token))
//    {
//        return false;
//    }

//    return true;
//}
//===================================================
void X_MathExpressionHandler::xp_defineHighLights(const QString& expression,
                                                 int cursorPos,
                                                 QVector<QPair<int, bool>>& highlightedSymbolVector)
{
    QChar symbol;
    highlightedSymbolVector.clear();
    QPair<int, bool> forwardHighlight;
    QPair<int, bool> backwardHighlight;
    forwardHighlight.first = -1;
    backwardHighlight.first = -1;

    if (cursorPos < expression.length() && cursorPos >= 0)
    {
        symbol = expression.at(cursorPos);
        for (int i = 0; i < xv_pairSymbolVector.count(); i++)
        {
            if (xv_pairSymbolVector.value(i).first == symbol)
            {
                xh_findBracketForward(xv_pairSymbolVector.value(i).second,
                                      expression,
                                      cursorPos,
                                      forwardHighlight);
                highlightedSymbolVector << QPair<int, bool>(cursorPos, forwardHighlight.second);
                highlightedSymbolVector << forwardHighlight;
            }
        }
    }

    if (cursorPos > 0 && cursorPos <= expression.length())
    {
        symbol = expression.at(cursorPos - 1);
        for (int i = 0; i < xv_pairSymbolVector.count(); i++)
        {
            if (xv_pairSymbolVector.value(i).second == symbol)
            {
                xh_findBracketBackward(xv_pairSymbolVector.value(i).first,
                                       expression,
                                       cursorPos,
                                       backwardHighlight);
                highlightedSymbolVector
                    << QPair<int, bool>(cursorPos - 1, backwardHighlight.second);
                highlightedSymbolVector << backwardHighlight;
            }
        }
    }
}
//===================================================
bool X_MathExpressionHandler::xp_checkExpression(const QString& expression,
                                              int cursorPos,
                                              QVector<QPair<int, bool>>& highlightedSymbolVector)
{
    xp_defineHighLights(expression, cursorPos, highlightedSymbolVector);
    return true;
}
//===================================================
bool X_MathExpressionHandler::xp_calculateExpression(const QString& expression, double& dResult)
{
    xv_expression = expression;
    xv_charPosition = 0;

    xh_getToken();

    if (xv_tokenType == TT_NULL)
    {
        xh_handleError(0); //  Выражение пусто.
        dResult = 0.0;
        return false;
    }

    bool res = xh_addOrSubtract(dResult);

    //    if(mv_tokenType == TT_NULL)
    //    {
    //        mf_handleError(0);  //  Последней  лексемой  должен  быть
    //        //  нулевой  символ.
    //    }

    return res;
}
//===================================================

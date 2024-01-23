//================================================
#include "X_LocaleDoubleConverter.h"
#include <cmath>
#include <QLocale>
#include <QRegularExpression>

//================================================
double X_LocaleDoubleConverter::toDouble(const QString& doubleString, bool* ok)
{
    QString newDoubleString = toLocalDoubleString(doubleString, ok);

    if(!ok)
    {
        return nan("");
    }

    QLocale locale;
    return locale.toDouble(newDoubleString, ok);
}
//================================================
QString X_LocaleDoubleConverter::toLocalDoubleString(const QString& doubleString, bool *ok)
{
    QString newDoubleString = doubleString.simplified();

    static QRegularExpression re("^[+-]?\\d{1,}[,.]?(\\d{1,}([eE][+-]?\\d{1,})?)?$");
    QRegularExpressionMatch match = re.match(newDoubleString);

    if(!match.hasMatch())
    {
        if(ok)
        {
            *ok = false;
        }
        return QString("NaN");
    }

    static QRegularExpression rSep("[,.]");
    QLocale locale;
    QString decimalSeparator = locale.decimalPoint();
    newDoubleString.replace(rSep, decimalSeparator);

    // append 0 if the number string ends with decimal point
    if(newDoubleString.endsWith(decimalSeparator))
    {
        newDoubleString.append("0");
    }

    if(ok)
    {
        *ok = true;
    }

    return newDoubleString;
}
//================================================

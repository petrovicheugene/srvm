//===============================================
#include "X_QrealToStringConverter.h"
#include "qlocale.h"
//===============================================
char X_QrealToStringConverter::xv_format = 'g';
int X_QrealToStringConverter::xv_precision = 15;
//===============================================
X_QrealToStringConverter::X_QrealToStringConverter()
{

}
//===============================================
bool X_QrealToStringConverter::xp_setFormat(char format)
{
    if(format != 'g' && format != 'G' && format != 'f' && format != 'F')
    {
        return false;
    }

    xv_format = format;
    return true;
}
//===============================================
bool X_QrealToStringConverter::xp_setPrecision(int precision)
{
    if(precision < 0)
    {
        return false;
    }

    xv_precision = precision;
    return true;
}
//===============================================
QString X_QrealToStringConverter::xp_convert(qreal value)
{
    if(value != value)
    {
        return QString::number(value);
    }

    QString valueString;
    if(qAbs(value) < 1)
    {
        bool started = false;
        valueString = QString::number(value, 'f', xv_precision);
        // first zero count after point
        int firstX_eroCount = 0;
        for(int i = 0; i < valueString.length(); i++)
        {
            if(!started && (valueString.at(i) == '.' || valueString.at(i) == ','))
            {
                started = true;
                continue;
            }

            if(valueString.at(i) == '0')
            {
                firstX_eroCount++;
                continue;
            }

            break;
        }

        if(firstX_eroCount > 9)
        {
            valueString = QString::number(value, xv_format, xv_precision);
            if(xv_format == 'f')
            {
                // chop tail zero if they exist
                for(int i = valueString.length() - 1; i > 0; i--)
                {
                    if(valueString.at(i) != '0' || (valueString.at(i - 1) == '.' || valueString.at(i - 1) == ',' ))
                    {
                        break;
                    }
                    valueString.chop(1);
                }
            }
        }
        else
        {
            // chop tail zero if they exist
            for(int i = valueString.length() - 1; i > 0; i--)
            {
                if(valueString.at(i) != '0' || (valueString.at(i - 1) == '.' || valueString.at(i - 1) == ',' ))
                {
                    break;
                }
                valueString.chop(1);
            }
        }
    }
    else
    {
        valueString = QString::number(value, xv_format, xv_precision);
        if(xv_format == 'f')
        {
            // chop tail zero if they exist
            for(int i = valueString.length() - 1; i > 0; i--)
            {
                if(valueString.at(i) != '0' || (valueString.at(i - 1) == '.' || valueString.at(i - 1) == ',' ))
                {
                    break;
                }
                valueString.chop(1);
            }
        }
    }

    return valueString;
}
//===============================================
void X_QrealToStringConverter::xh_chopTailX_eroesFromDoubleString(QString& string)
{
    // check string
    bool ok;
    QLocale locale;
    locale.toDouble(string, &ok);
    if(!ok)
    {
        return;
    }

    // check format
    if(string.contains('e') || string.contains('E'))
    {
        //  format g
        return;
    }

    for(int i = string.length() - 1; i > 0; i--)
    {
        if(string.at(i) != '0' || (string.at(i - 1) == '.' || string.at(i - 1) == ',' ))
        {
            break;
        }
        string.chop(1);
    }
}
//===============================================

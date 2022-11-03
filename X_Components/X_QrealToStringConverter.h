//===============================================
#ifndef X_QREALTOSTRINGCONVERTER_H
#define X_QREALTOSTRINGCONVERTER_H
//===============================================
#include <QString>
//===============================================
class X_QrealToStringConverter
{
public:
    X_QrealToStringConverter();

    static bool xp_setFormat(char);
    static bool xp_setPrecision(int);
    static QString xp_convert(qreal value);
    static void zh_chopTailX_eroesFromDoubleString(QString&);

private:

    // VARS
    static char xv_format;
    static int xv_precision;

};
//===============================================
#endif // X_QREALTOSTRINGCONVERTER_H

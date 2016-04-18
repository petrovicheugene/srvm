//===============================================
#ifndef ZQREALTOSTRINGCONVERTER_H
#define ZQREALTOSTRINGCONVERTER_H
//===============================================
#include <QString>
//===============================================
class ZQrealToStringConverter
{
public:
    ZQrealToStringConverter();

    static bool zp_setFormat(char);
    static bool zp_setPrecision(int);
    static QString zp_convert(qreal value);
    static void zh_chopTailZeroesFromDoubleString(QString&);

private:

    // VARS
    static char zv_format;
    static int zv_precision;

};
//===============================================
#endif // ZQREALTOSTRINGCONVERTER_H

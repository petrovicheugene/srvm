//================================================
#ifndef X_LOCALEDOUBLECONVERTER_H
#define X_LOCALEDOUBLECONVERTER_H
//================================================
#include <QString>

//================================================
class X_LocaleDoubleConverter
{
public:
    X_LocaleDoubleConverter() = delete;

    static double toDouble(const QString& doubleString, bool *ok = nullptr);
    static QString toLocalDoubleString(const QString& doubleString, bool *ok = nullptr);
};
//================================================
#endif // X_LOCALEDOUBLECONVERTER_H

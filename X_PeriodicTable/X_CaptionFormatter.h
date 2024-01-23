//===================================================
#ifndef X_CAPTIONFORMATTER_H
#define X_CAPTIONFORMATTER_H
//===================================================
#include <QString>
//===================================================
class X_CaptionFormatter
{
public:

    static QString xp_format(QString& title);
    static void xp_setHtmlFomat(const QString& htmlFormat);

private:
    X_CaptionFormatter();
    ~X_CaptionFormatter();
    X_CaptionFormatter(X_CaptionFormatter const&) = delete;
    void operator=(X_CaptionFormatter const&) = delete;

    static QString xv_htmlFormat;

};
//===================================================
#endif // X_CAPTIONFORMATTER_H

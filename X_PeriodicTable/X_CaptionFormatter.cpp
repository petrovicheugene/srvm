//===================================================
#include "X_CaptionFormatter.h"
//===================================================
QString X_CaptionFormatter::xv_htmlFormat = QString("<b>%1</b>");
//===================================================
X_CaptionFormatter::X_CaptionFormatter()
{

}
//===================================================
QString X_CaptionFormatter::xp_format(QString& title)
{
    return title = QString(xv_htmlFormat).arg(title);
}
//===================================================
void X_CaptionFormatter::xp_setHtmlFomat(const QString& htmlFormat)
{
    xv_htmlFormat = htmlFormat;
}
//===================================================

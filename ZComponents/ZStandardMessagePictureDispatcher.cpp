//===================================================
#include "ZStandardMessagePictureDispatcher.h"
#include <QApplication>
#include <QPixmapCache>
#include <QSize>
#include <QStyle>

//===================================================
// STATIC
const QMap<QtMsgType, QString> ZStandardMessagePictureDispatcher::zv_msgTypeNameMap =
        ZStandardMessagePictureDispatcher::zh_initMsgTypeNameMap();
//===================================================
QMap<QtMsgType, QString> ZStandardMessagePictureDispatcher::zh_initMsgTypeNameMap()
{
    QMap<QtMsgType, QString> msgTypeNameMap;
    msgTypeNameMap.insert(QtDebugMsg, "debug");
    msgTypeNameMap.insert(QtInfoMsg, "info");
    msgTypeNameMap.insert(QtWarningMsg, "warning");
    msgTypeNameMap.insert(QtCriticalMsg, "critical");
    msgTypeNameMap.insert(QtFatalMsg, "fatal");

    return msgTypeNameMap;
}
// END STATIC
//===================================================
ZStandardMessagePictureDispatcher::ZStandardMessagePictureDispatcher(QObject *parent) : QObject(parent)
{

}
//===================================================
bool ZStandardMessagePictureDispatcher::zp_pictureForStandardMessage(QtMsgType type,
                                                                     QSize size,
                                                                     QStyle *style,
                                                                     QPixmap& msgIconPixmap)
{
    // define style
    QStyle* picstyle;
    if(style == 0)
    {
        picstyle = qApp->style();
    }
    else
    {
        picstyle = style;
    }

    // try to get pixmap from cache
    QString keyString =  zv_msgTypeNameMap.value(type)+QString::number(size.width())+"-"+QString::number(size.height());
    if (!QPixmapCache::find(keyString, msgIconPixmap))
    {
        // get standard pixmap and insert it into cache
        msgIconPixmap = ZStandardMessagePictureDispatcher::zh_getStandardMessageBoxPixmap(type, size, picstyle);
        QPixmapCache::insert(keyString, msgIconPixmap);
    }

    return !msgIconPixmap.isNull();
}
//===================================================
void ZStandardMessagePictureDispatcher::zp_pictureForStandardMessage(QtMsgType type,
                                                                     QSize size,
                                                                     QStyle *style,
                                                                     QPixmap& msgIconPixmap,
                                                                     bool &ok) const
{
    ok = ZStandardMessagePictureDispatcher::zp_pictureForStandardMessage(type,
                                                                               size,
                                                                               style,
                                                                               msgIconPixmap);
}
//===================================================
QPixmap ZStandardMessagePictureDispatcher::zh_getStandardMessageBoxPixmap(QtMsgType type,
                                                                          QSize size,
                                                                          QStyle *style)
{
    QStyle::StandardPixmap standardIcon = QStyle::SP_CustomBase;
    switch(type)
    {
    case QtDebugMsg:
        standardIcon = QStyle::SP_MessageBoxInformation;
        break;
    case QtInfoMsg:
        standardIcon = QStyle::SP_MessageBoxInformation;
        break;
    case QtWarningMsg:
        standardIcon = QStyle::SP_MessageBoxWarning;
        break;
    case QtFatalMsg:
        standardIcon = QStyle::SP_MessageBoxCritical;
        break;
    case QtCriticalMsg:
        standardIcon = QStyle::SP_MessageBoxCritical;
        break;
    }

    QIcon icon = style->standardIcon (standardIcon);
    return icon.pixmap(size);
}
//===================================================

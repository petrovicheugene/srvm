//===================================================
#include "X_StandardMessagePictureDispatcher.h"
#include <QApplication>
#include <QPixmapCache>
#include <QSize>
#include <QStyle>

//===================================================
// STATIC
const QMap<QtMsgType, QString> X_StandardMessagePictureDispatcher::xv_msgTypeNameMap =
        X_StandardMessagePictureDispatcher::xh_initMsgTypeNameMap();
//===================================================
QMap<QtMsgType, QString> X_StandardMessagePictureDispatcher::xh_initMsgTypeNameMap()
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
X_StandardMessagePictureDispatcher::X_StandardMessagePictureDispatcher(QObject *parent) : QObject(parent)
{

}
//===================================================
bool X_StandardMessagePictureDispatcher::xp_pictureForStandardMessage(QtMsgType type,
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
    QString keyString =  xv_msgTypeNameMap.value(type)+QString::number(size.width())+"-"+QString::number(size.height());
    if (!QPixmapCache::find(keyString, &msgIconPixmap))
    {
        // get standard pixmap and insert it into cache
        msgIconPixmap = X_StandardMessagePictureDispatcher::xh_getStandardMessageBoxPixmap(type, size, picstyle);
        QPixmapCache::insert(keyString, msgIconPixmap);
    }

    return !msgIconPixmap.isNull();
}
//===================================================
void X_StandardMessagePictureDispatcher::xp_pictureForStandardMessage(QtMsgType type,
                                                                     QSize size,
                                                                     QStyle *style,
                                                                     QPixmap& msgIconPixmap,
                                                                     bool &ok) const
{
    ok = X_StandardMessagePictureDispatcher::xp_pictureForStandardMessage(type,
                                                                               size,
                                                                               style,
                                                                               msgIconPixmap);
}
//===================================================
QPixmap X_StandardMessagePictureDispatcher::xh_getStandardMessageBoxPixmap(QtMsgType type,
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

    QIcon icon = style->standardIcon(standardIcon);
    return icon.pixmap(size);
}
//===================================================

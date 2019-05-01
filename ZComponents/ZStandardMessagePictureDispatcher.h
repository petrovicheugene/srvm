//===================================================
#ifndef ZSTANDARDMESSAGEPICTUREDISPATCHER_H
#define ZSTANDARDMESSAGEPICTUREDISPATCHER_H
//===================================================
#include <QObject>
#include <QPixmap>
//===================================================
class QStyle;
//===================================================
class ZStandardMessagePictureDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit ZStandardMessagePictureDispatcher(QObject *parent = 0);

    static bool zp_pictureForStandardMessage(QtMsgType type,
                                             QSize size,
                                             QStyle* style,
                                             QPixmap& msgIconPixmap);


signals:


public slots:

    void zp_pictureForStandardMessage(QtMsgType type, // message type
                                      QSize size, // picture max size
                                      QStyle* style, // picture style
                                      QPixmap& msgIconPixmap, // out picture
                                      bool& ok) const;

private:

    const static QMap<QtMsgType, QString> zv_msgTypeNameMap;
    static QMap<QtMsgType, QString> zh_initMsgTypeNameMap();
    static QPixmap zh_getStandardMessageBoxPixmap(QtMsgType type,
                                                  QSize size,
                                                  QStyle *style);



};
//===================================================
#endif // ZSTANDARDMESSAGEPICTUREDISPATCHER_H

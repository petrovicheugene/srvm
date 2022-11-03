//===================================================
#ifndef X_STANDARDMESSAGEPICTUREDISPATCHER_H
#define X_STANDARDMESSAGEPICTUREDISPATCHER_H
//===================================================
#include <QObject>
#include <QPixmap>
//===================================================
class QStyle;
//===================================================
class X_StandardMessagePictureDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit X_StandardMessagePictureDispatcher(QObject *parent = 0);

    static bool xp_pictureForStandardMessage(QtMsgType type,
                                             QSize size,
                                             QStyle* style,
                                             QPixmap& msgIconPixmap);


signals:


public slots:

    void xp_pictureForStandardMessage(QtMsgType type, // message type
                                      QSize size, // picture max size
                                      QStyle* style, // picture style
                                      QPixmap& msgIconPixmap, // out picture
                                      bool& ok) const;

private:

    const static QMap<QtMsgType, QString> xv_msgTypeNameMap;
    static QMap<QtMsgType, QString> zh_initMsgTypeNameMap();
    static QPixmap zh_getStandardMessageBoxPixmap(QtMsgType type,
                                                  QSize size,
                                                  QStyle *style);



};
//===================================================
#endif // X_STANDARDMESSAGEPICTUREDISPATCHER_H

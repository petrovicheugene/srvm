//===================================================
#ifndef ZMESSAGEWIDGET_H
#define ZMESSAGEWIDGET_H
//===================================================
#include <QWidget>
#include <QColor>
#include <QTextCursor>
//===================================================
class QFrame;
class QLabel;
class QLayout;
class QPushButton;
class QStyle;
class QTextDocument;
class QTextEdit;
//===================================================
class ZMessageWidget : public QWidget
{
    Q_OBJECT
public:
    enum MessageTextFormat {MTF_PLAIN, MTF_HTML};
    explicit ZMessageWidget(QWidget *parent = 0);

    void zp_setWidgetMargin(int margin);
    void zp_setBasementSeparatorVisible(bool visibility) ;
    void zp_setTitle(const QString& caption);
    void zp_setFrameStyle(int frameStyle);
    void zp_setFrameLineWidth(int width);

    void zp_setInsertIconFlag(bool insertIconFlag);
    void zp_setInsertTimeStamp(bool insertTimeStampFlag);

    void zp_setTimestampColor(QColor color);
    void zp_setMessageColor(QColor color,  int msgType);

    void zp_alignStringList(QStringList& stringList,
                            Qt::Alignment alignment,
                            const QString& spaceSymbol = " ");
    void zp_clearMessages();
signals:

    void zg_requestMessageIconPixmap(int type,
                                      QSize size,
                                      QStyle* style,
                                      QPixmap& pixmap,
                                      bool& ok) const;

public slots:

    void zp_appendMessage(int msgType,
                          const QString &msg);
    void zp_appendPlainMessage(int msgType,
                               const QString &msg);
    void zp_addToMessage(int msgType,
                          const QString &msg);

    QFont zp_cursorFont() const;

private:

    // VARS
    QLayout* zv_mainLayout;
    QFrame* zv_separatorLine;
    QLabel* zv_titleLabel;

    QTextDocument *zv_textDocument;
    QTextEdit* zv_msgTextEdit;
    QPushButton* zv_clearButton;
    bool zv_insertIconFlag;
    bool zv_insertTimeStampFlag;
    bool zv_scrollRequiredFlag;
    QMap<int, QColor> zv_messageColorMap;
    QColor zv_timestampColor;
    const static QMap<QtMsgType, QString> zv_msgTypeNameMap;


    //FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_initMessageColors();


    bool zh_insertIconPixmapForMsgType(QTextCursor &cursor, int type);
    void zh_insertMessage(int type,
                          const QString &msg,
                          bool htmlFlag,
                          bool suppressTime = false);
    void zh_scrollTextToEnd();
    void zh_defineScrollRequiredFlag();
    QString zh_createTimestampString(bool htmlFlag) const;
    bool zh_applyColorToString(QString& string, int msgType) const;
    static QMap<QtMsgType, QString> zh_initMsgTypeNameMap();

};
//===================================================
#endif // ZMESSAGEWIDGET_H

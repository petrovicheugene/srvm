//===================================================
#ifndef X_MESSAGEWIDGET_H
#define X_MESSAGEWIDGET_H
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
class X_MessageWidget : public QWidget
{
    Q_OBJECT
public:
    enum MessageTextFormat {MTF_PLAIN, MTF_HTML};
    explicit X_MessageWidget(QWidget *parent = 0);

    void xp_setWidgetMargin(int margin);
    void xp_setBasementSeparatorVisible(bool visibility) ;
    void xp_setTitle(const QString& caption);
    void xp_setFrameStyle(int frameStyle);
    void xp_setFrameLineWidth(int width);

    void xp_setInsertIconFlag(bool insertIconFlag);
    void xp_setInsertTimeStamp(bool insertTimeStampFlag);

    void xp_setTimestampColor(QColor color);
    void xp_setMessageColor(QColor color,  int msgType);

    void xp_alignStringList(QStringList& stringList,
                            Qt::Alignment alignment,
                            const QString& spaceSymbol = " ");

signals:

    void xg_requestMessageIconPixmap(int type,
                                      QSize size,
                                      QStyle* style,
                                      QPixmap& pixmap,
                                      bool& ok) const;

public slots:

    void xp_appendMessage(int msgType,
                          const QString &msg);
    void xp_appendPlainMessage(int msgType,
                               const QString &msg);
    void xp_addToMessage(int msgType,
                          const QString &msg);

    QFont xp_cursorFont() const;

private:

    // VARS
    QLayout* xv_mainLayout;
    QFrame* xv_separatorLine;
    QLabel* xv_titleLabel;

    QTextDocument *xv_textDocument;
    QTextEdit* xv_msgTextEdit;
    QPushButton* xv_clearButton;
    bool xv_insertIconFlag;
    bool xv_insertTimeStampFlag;
    bool xv_scrollRequiredFlag;
    QMap<int, QColor> xv_messageColorMap;
    QColor xv_timestampColor;
    const static QMap<QtMsgType, QString> xv_msgTypeNameMap;


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
#endif // X_MESSAGEWIDGET_H

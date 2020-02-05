//===================================================
#include "ZMessageWidget.h"
#include "ZCaptionFormatter.h"
#include <QDebug>

#include <QDialogButtonBox>
#include <QFont>
#include <QFontMetrics>
#include <QFrame>
#include <QIcon>
#include <QImage>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QPixmapCache>
#include <QPushButton>
#include <QScrollBar>
#include <QStyle>
#include <QTextCursor>
#include <QTextEdit>
#include <QTime>
#include <QVBoxLayout>
//===================================================
// STATIC
const QMap<QtMsgType, QString> ZMessageWidget::zv_msgTypeNameMap =
        ZMessageWidget::zh_initMsgTypeNameMap();
//===================================================
QMap<QtMsgType, QString> ZMessageWidget::zh_initMsgTypeNameMap()
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
ZMessageWidget::ZMessageWidget(QWidget *parent) : QWidget(parent)
{
    //
    zv_msgTextEdit = nullptr;;
    zv_clearButton = nullptr;
    zv_insertIconFlag = true;
    zv_insertTimeStampFlag = true;
    zv_scrollRequiredFlag = true;

    //
    zh_initMessageColors();
    zh_createComponents();
    zh_createConnections();
}
//===================================================
void ZMessageWidget::zp_setWidgetMargin(int margin)
{
    if(margin < 0)
    {
        return;
    }

    zv_mainLayout->setMargin(margin);
}
//===================================================
void ZMessageWidget::zp_setBasementSeparatorVisible(bool visibility)
{
    zv_separatorLine->setVisible(visibility);
}
//===================================================
void ZMessageWidget::zp_setTitle(const QString& titleString)
{
    if(titleString.isEmpty())
    {
        zv_titleLabel->setVisible(false);
        zv_titleLabel->setText(QString());
        return;
    }

    zv_titleLabel->setVisible(true);
    QString title = titleString;
    zv_titleLabel->setText(ZCaptionFormatter::zp_format(title));
}
//====================================================
void ZMessageWidget::zp_setFrameStyle(int frameStyle)
{
    zv_msgTextEdit->setFrameStyle(frameStyle);
}
//====================================================
void ZMessageWidget::zp_setFrameLineWidth(int width)
{
    zv_msgTextEdit->setLineWidth(width);
}
//===================================================
void ZMessageWidget::zh_initMessageColors()
{
    zv_timestampColor = QColor(Qt::darkGray);
    zv_messageColorMap.insert(QtDebugMsg, QColor(Qt::darkGreen));
    zv_messageColorMap.insert(QtInfoMsg, QColor(Qt::darkBlue));
    zv_messageColorMap.insert(QtWarningMsg, QColor(Qt::darkRed));
    zv_messageColorMap.insert(QtCriticalMsg, QColor(Qt::red));
    zv_messageColorMap.insert(QtFatalMsg, QColor(Qt::red));
}
//===================================================
void ZMessageWidget::zp_setInsertIconFlag(bool insertIconFlag)
{
    zv_insertIconFlag = insertIconFlag;
}
//===================================================
void ZMessageWidget::zp_setInsertTimeStamp(bool insertTimeStampFlag)
{
    zv_insertTimeStampFlag = insertTimeStampFlag;
}
//===================================================
void ZMessageWidget::zp_setTimestampColor(QColor color)
{
    zv_timestampColor = color;
}
//===================================================
void ZMessageWidget::zp_setMessageColor(QColor color, int msgType)
{
    zv_messageColorMap.insert(msgType, color);
}
//===================================================
void ZMessageWidget::zp_alignStringList(QStringList& stringList, Qt::Alignment alignment, const QString &spaceSymbol)
{
    QFont font = zp_cursorFont();
    QFontMetrics fontMetrix(font);
    int maxWidth = 0;
    foreach(QString string, stringList)
    {
        if (maxWidth < fontMetrix.horizontalAdvance(string))
        {
            maxWidth = fontMetrix.horizontalAdvance(string);
        }
    }

    double spaceSymbolWidth = static_cast<double>(fontMetrix.horizontalAdvance(spaceSymbol));

    for(int i = 0; i < stringList.count(); i++)
    {
        int spaceCount = qRound(
            static_cast<double>(maxWidth - (fontMetrix.horizontalAdvance(stringList.at(i))))
            / spaceSymbolWidth);

        if(alignment & Qt::AlignRight)
        {
            for(int sc = 0; sc < spaceCount; sc++)
            {
                stringList[i] = spaceSymbol + stringList[i];
            }
        }
        else if(alignment & Qt::AlignLeft)
        {
            for(int sc = 0; sc < spaceCount; sc++)
            {
                stringList[i] = stringList[i] + spaceSymbol;
            }
        }
        else if(alignment & Qt::AlignHCenter)
        {
            for(int sc = 0; sc < spaceCount / 2; sc++)
            {
                stringList[i] = QString(spaceSymbol) + stringList[i];
            }
            for(int sc = 0; sc < spaceCount - spaceCount / 2; sc++)
            {
                stringList[i] = stringList[i] + spaceSymbol;
            }
        }
    }
}
//===================================================
void ZMessageWidget::zh_createComponents()
{
    zv_mainLayout = new QVBoxLayout;
    setLayout(zv_mainLayout);

    zv_titleLabel = new QLabel(this);
    zv_mainLayout->addWidget(zv_titleLabel);
    zv_titleLabel->setVisible(false);

    zv_msgTextEdit = new QTextEdit(this);
    zv_msgTextEdit->setReadOnly(true);
    zv_mainLayout->addWidget(zv_msgTextEdit);

    // basement sparator line
    zv_separatorLine = new QFrame(this);
    zv_separatorLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    zv_separatorLine->setLineWidth(1);
    zv_mainLayout->addWidget(zv_separatorLine);

    // basement

    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    zv_mainLayout->addWidget(buttonBox);

    zv_clearButton = new QPushButton(this);
    zv_clearButton->setFocusPolicy(Qt::NoFocus);
    zv_clearButton->setIcon(QIcon(":/images/ZImages/clear-8.png"));
    zv_clearButton->setFlat(true);
    zv_clearButton->setToolTip(tr("Clear messages"));
    //zv_clearButton->setFlat(true);
    buttonBox->addButton(zv_clearButton, QDialogButtonBox::ActionRole);
}
//===================================================
void ZMessageWidget::zh_createConnections()
{
    connect(zv_clearButton, &QPushButton::clicked,
            zv_msgTextEdit, &QTextEdit::clear);
}
//===================================================
void ZMessageWidget::zp_addToMessage(int msgType,
                                     const QString &msg)
{
    zh_defineScrollRequiredFlag();
    zh_insertMessage(msgType, msg, true, true);
    zh_scrollTextToEnd();
}
//===================================================
void ZMessageWidget::zp_appendMessage(int msgType,
                                      const QString &msg)
{
    zh_defineScrollRequiredFlag();
    zh_insertMessage(msgType, msg, true);
    zh_scrollTextToEnd();
}
//===================================================
void  ZMessageWidget::zp_appendPlainMessage(int msgType,
                                            const QString &msg)
{
    zh_defineScrollRequiredFlag();
    zh_insertMessage(msgType, msg, false);
    zh_scrollTextToEnd();
}
//===================================================
void ZMessageWidget::zh_insertMessage(int type,
                                      const QString &msg,
                                      bool htmlFlag,
                                      bool suppressTime)
{
    QTextCursor cursor(zv_msgTextEdit->document());

    // move cursor to the end of document
    cursor.movePosition(QTextCursor::End);

    // timestamp
    if(zv_insertTimeStampFlag && !suppressTime)
    {
        QString timestampString = zh_createTimestampString(htmlFlag);
        cursor.insertHtml(timestampString);
        // insert spacing
        cursor.insertText(" ");

    }

    // message icon
    if(zh_insertIconPixmapForMsgType(cursor, type))
    {
        // insert spacing
        cursor.insertText(" ");
    }

    // message text
    if(htmlFlag)
    {
        // set color to the msg string
        QString htmlMsg = msg;
        zh_applyColorToString(htmlMsg, type);
        // set text to textEdit
        cursor.insertHtml(htmlMsg);
    }
    else
    {
        // set text to textEdit as it is
        cursor.insertText(msg);
    }

    if(!msg.endsWith("\n"))
    {
        cursor.insertHtml("<br>");
    }
}
//===================================================
void ZMessageWidget::zh_defineScrollRequiredFlag()
{
    zv_scrollRequiredFlag = zv_msgTextEdit->verticalScrollBar()->maximum()
            == zv_msgTextEdit->verticalScrollBar()->value();
}
//===================================================
QString ZMessageWidget::zh_createTimestampString(bool htmlFlag) const
{
    if(!htmlFlag)
    {
        return QTime::currentTime().toString("hh:mm:ss");
    }


    QString timestamp = QString("<font color=%2>%1</font>").arg(QTime::currentTime().toString("hh:mm:ss"),
                                                                zv_timestampColor.name());
    return timestamp;
}
//===================================================
bool ZMessageWidget::zh_applyColorToString(QString& string, int msgType) const
{
    if(!zv_messageColorMap.keys().contains(msgType))
    {
        return false;
    }

    string = QString("<font color=%2>%1</font>").arg(string,
                                                     zv_messageColorMap.value(msgType).name());
    return true;
}
//===================================================
void ZMessageWidget::zh_scrollTextToEnd()
{
    if(zv_scrollRequiredFlag)
    {
        zv_msgTextEdit->verticalScrollBar()->setValue(zv_msgTextEdit->verticalScrollBar()->maximum());
    }
}
//===================================================
bool ZMessageWidget::zh_insertIconPixmapForMsgType(QTextCursor& cursor,
                                                   int type)
{
    if(!zv_insertIconFlag)
    {
        return false;
    }

    // get font height for defining icon size
    QTextCharFormat format = cursor.blockCharFormat();
    QFont textEditFont = format.font();
    QFontMetrics fontMetrics(textEditFont);
    int fontHeight = fontMetrics.height();

    QPixmap msgIconPixmap;
    bool ok = false;
    // try to get incon pixmap from outside
    emit zg_requestMessageIconPixmap(type,
                                     QSize(fontHeight, fontHeight),
                                     style(),
                                     msgIconPixmap,
                                     ok);

    if(!ok || msgIconPixmap.isNull()) // getting pixmap from outside failed
    {
        //        // try to get pixmap from cache
        //        QString keyString =  zv_msgTypeNameMap.value(type)+QString::number(fontHeight);
        //        if (!QPixmapCache::find(keyString, msgIconPixmap))
        //        {
        //            // get standard pixmap and insert it into cache
        //            msgIconPixmap = zh_getStandardMessageBoxPixmap(type, fontHeight);
        //            QPixmapCache::insert(keyString, msgIconPixmap);
        //        }
        return false;
    }

    cursor.insertImage(msgIconPixmap.toImage());
    return true;
}
//===================================================
QFont ZMessageWidget::zp_cursorFont() const
{
    QTextCursor cursor(zv_msgTextEdit->document());
    QTextCharFormat format = cursor.blockCharFormat();
    QFont textEditFont = format.font();
    return textEditFont;
}
//===================================================

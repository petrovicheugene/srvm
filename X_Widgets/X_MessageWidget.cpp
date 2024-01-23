//===================================================
#include "X_MessageWidget.h"
#include "X_CaptionFormatter.h"
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
const QMap<QtMsgType, QString> X_MessageWidget::xv_msgTypeNameMap =
        X_MessageWidget::xh_initMsgTypeNameMap();
//===================================================
QMap<QtMsgType, QString> X_MessageWidget::xh_initMsgTypeNameMap()
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
X_MessageWidget::X_MessageWidget(QWidget *parent) : QWidget(parent)
{
    //
    xv_msgTextEdit = nullptr;;
    xv_clearButton = nullptr;
    xv_insertIconFlag = true;
    xv_insertTimeStampFlag = true;
    xv_scrollRequiredFlag = true;

    //
    xh_initMessageColors();
    xh_createComponents();
    xh_createConnections();
}
//===================================================
void X_MessageWidget::xp_setWidgetMargin(int margin)
{
    if(margin < 0)
    {
        return;
    }

    xv_mainLayout->setContentsMargins(margin, margin, margin, margin);
}
//===================================================
void X_MessageWidget::xp_setBasementSeparatorVisible(bool visibility)
{
    xv_separatorLine->setVisible(visibility);
}
//===================================================
void X_MessageWidget::xp_setTitle(const QString& titleString)
{
    if(titleString.isEmpty())
    {
        xv_titleLabel->setVisible(false);
        xv_titleLabel->setText(QString());
        return;
    }

    xv_titleLabel->setVisible(true);
    QString title = titleString;
    xv_titleLabel->setText(X_CaptionFormatter::xp_format(title));
}
//====================================================
void X_MessageWidget::xp_setFrameStyle(int frameStyle)
{
    xv_msgTextEdit->setFrameStyle(frameStyle);
}
//====================================================
void X_MessageWidget::xp_setFrameLineWidth(int width)
{
    xv_msgTextEdit->setLineWidth(width);
}
//===================================================
void X_MessageWidget::xh_initMessageColors()
{
    xv_timestampColor = QColor(Qt::darkGray);
    xv_messageColorMap.insert(QtDebugMsg, QColor(Qt::darkGreen));
    xv_messageColorMap.insert(QtInfoMsg, QColor(Qt::darkBlue));
    xv_messageColorMap.insert(QtWarningMsg, QColor(Qt::darkRed));
    xv_messageColorMap.insert(QtCriticalMsg, QColor(Qt::red));
    xv_messageColorMap.insert(QtFatalMsg, QColor(Qt::red));
}
//===================================================
void X_MessageWidget::xp_setInsertIconFlag(bool insertIconFlag)
{
    xv_insertIconFlag = insertIconFlag;
}
//===================================================
void X_MessageWidget::xp_setInsertTimeStamp(bool insertTimeStampFlag)
{
    xv_insertTimeStampFlag = insertTimeStampFlag;
}
//===================================================
void X_MessageWidget::xp_setTimestampColor(QColor color)
{
    xv_timestampColor = color;
}
//===================================================
void X_MessageWidget::xp_setMessageColor(QColor color, int msgType)
{
    xv_messageColorMap.insert(msgType, color);
}
//===================================================
void X_MessageWidget::xp_alignStringList(QStringList& stringList, Qt::Alignment alignment, const QString &spaceSymbol)
{
    QFont font = xp_cursorFont();
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
void X_MessageWidget::xh_createComponents()
{
    xv_mainLayout = new QVBoxLayout;
    setLayout(xv_mainLayout);

    xv_titleLabel = new QLabel(this);
    xv_mainLayout->addWidget(xv_titleLabel);
    xv_titleLabel->setVisible(false);

    xv_msgTextEdit = new QTextEdit(this);
    xv_msgTextEdit->setReadOnly(true);
    xv_mainLayout->addWidget(xv_msgTextEdit);

    // basement sparator line
    xv_separatorLine = new QFrame(this);
    xv_separatorLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    xv_separatorLine->setLineWidth(1);
    xv_mainLayout->addWidget(xv_separatorLine);

    // basement

    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    xv_mainLayout->addWidget(buttonBox);

    xv_clearButton = new QPushButton(this);
    xv_clearButton->setFocusPolicy(Qt::NoFocus);
    xv_clearButton->setIcon(QIcon(":/images/X_Images/clear-8.png"));
    xv_clearButton->setFlat(true);
    xv_clearButton->setToolTip(tr("Clear messages"));
    //xv_clearButton->setFlat(true);
    buttonBox->addButton(xv_clearButton, QDialogButtonBox::ActionRole);
}
//===================================================
void X_MessageWidget::xh_createConnections()
{
    connect(xv_clearButton, &QPushButton::clicked,
            xv_msgTextEdit, &QTextEdit::clear);
}
//===================================================
void X_MessageWidget::xp_addToMessage(int msgType,
                                     const QString &msg)
{
    xh_defineScrollRequiredFlag();
    xh_insertMessage(msgType, msg, true, true);
    xh_scrollTextToEnd();
}
//===================================================
void X_MessageWidget::xp_appendMessage(int msgType,
                                      const QString &msg)
{
    xh_defineScrollRequiredFlag();
    xh_insertMessage(msgType, msg, true);
    xh_scrollTextToEnd();
}
//===================================================
void  X_MessageWidget::xp_appendPlainMessage(int msgType,
                                            const QString &msg)
{
    xh_defineScrollRequiredFlag();
    xh_insertMessage(msgType, msg, false);
    xh_scrollTextToEnd();
}
//===================================================
void X_MessageWidget::xh_insertMessage(int type,
                                      const QString &msg,
                                      bool htmlFlag,
                                      bool suppressTime)
{
    QTextCursor cursor(xv_msgTextEdit->document());

    // move cursor to the end of document
    cursor.movePosition(QTextCursor::End);

    // timestamp
    if(xv_insertTimeStampFlag && !suppressTime)
    {
        QString timestampString = xh_createTimestampString(htmlFlag);
        cursor.insertHtml(timestampString);
        // insert spacing
        cursor.insertText(" ");

    }

    // message icon
    if(xh_insertIconPixmapForMsgType(cursor, type))
    {
        // insert spacing
        cursor.insertText(" ");
    }

    // message text
    if(htmlFlag)
    {
        // set color to the msg string
        QString htmlMsg = msg;
        xh_applyColorToString(htmlMsg, type);
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
void X_MessageWidget::xh_defineScrollRequiredFlag()
{
    xv_scrollRequiredFlag = xv_msgTextEdit->verticalScrollBar()->maximum()
            == xv_msgTextEdit->verticalScrollBar()->value();
}
//===================================================
QString X_MessageWidget::xh_createTimestampString(bool htmlFlag) const
{
    if(!htmlFlag)
    {
        return QTime::currentTime().toString("hh:mm:ss");
    }


    QString timestamp = QString("<font color=%2>%1</font>").arg(QTime::currentTime().toString("hh:mm:ss"),
                                                                xv_timestampColor.name());
    return timestamp;
}
//===================================================
bool X_MessageWidget::xh_applyColorToString(QString& string, int msgType) const
{
    if(!xv_messageColorMap.keys().contains(msgType))
    {
        return false;
    }

    string = QString("<font color=%2>%1</font>").arg(string,
                                                     xv_messageColorMap.value(msgType).name());
    return true;
}
//===================================================
void X_MessageWidget::xh_scrollTextToEnd()
{
    if(xv_scrollRequiredFlag)
    {
        xv_msgTextEdit->verticalScrollBar()->setValue(xv_msgTextEdit->verticalScrollBar()->maximum());
    }
}
//===================================================
bool X_MessageWidget::xh_insertIconPixmapForMsgType(QTextCursor& cursor,
                                                   int type)
{
    if(!xv_insertIconFlag)
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
    emit xg_requestMessageIconPixmap(type,
                                     QSize(fontHeight, fontHeight),
                                     style(),
                                     msgIconPixmap,
                                     ok);

    if(!ok || msgIconPixmap.isNull()) // getting pixmap from outside failed
    {
        //        // try to get pixmap from cache
        //        QString keyString =  xv_msgTypeNameMap.value(type)+QString::number(fontHeight);
        //        if (!QPixmapCache::find(keyString, msgIconPixmap))
        //        {
        //            // get standard pixmap and insert it into cache
        //            msgIconPixmap = xh_getStandardMessageBoxPixmap(type, fontHeight);
        //            QPixmapCache::insert(keyString, msgIconPixmap);
        //        }
        return false;
    }

    cursor.insertImage(msgIconPixmap.toImage());
    return true;
}
//===================================================
QFont X_MessageWidget::xp_cursorFont() const
{
    QTextCursor cursor(xv_msgTextEdit->document());
    QTextCharFormat format = cursor.blockCharFormat();
    QFont textEditFont = format.font();
    return textEditFont;
}
//===================================================

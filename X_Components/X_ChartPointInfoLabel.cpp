//======================================================
#include "X_ChartPointInfoLabel.h"
#include "qscreen.h"
#include <QApplication>
#include <QMainWindow>
//======================================================
X_ChartPointInfoLabel::X_ChartPointInfoLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel{parent, f}
{
    xh_initLabel();
}
//======================================================
X_ChartPointInfoLabel::X_ChartPointInfoLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
    : QLabel{text, parent, f}
{
    xh_initLabel();
}
//======================================================
X_ChartPointInfoLabel::~X_ChartPointInfoLabel()
{
    qDebug() << "LABEL DESTRUCTOR - " << text();
}
//======================================================
bool X_ChartPointInfoLabel::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress
        || event->type() == QEvent::Wheel
        || event->type() == QEvent::Close)
    {
        deleteLater();
    }

    return QObject::eventFilter(object, event); // Pass the event on to the parent class
}
//======================================================
void X_ChartPointInfoLabel::xh_initLabel()
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint); // | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setFocusPolicy(Qt::NoFocus);
    // setStyleSheet("background-color: #ffffe0; border: 1px solid #000000; padding: 5px;");
    setStyleSheet("background-color: #ffffff; border: 1px solid #000000; padding: 5px;");
    qApp->installEventFilter(this);
}
//======================================================
void X_ChartPointInfoLabel::xp_correctBottomRightShift() // to prevent the window from rolling out the right edge of the main window.
{
    QScreen* screen = QApplication::primaryScreen();

    if (screen)
    {
        // Get the screen geometry
        QRect screenGeometry = screen->geometry();

        // Calculate the global position of the bottom-right corner
        QPoint globalBottomRight = screenGeometry.bottomRight();
        QPoint labelGlobalTopLeft = mapToGlobal(rect().topLeft());

        QSize labelSize = sizeHint();
        QPoint labelGlobalBottomRight = QPoint(labelGlobalTopLeft.x() + labelSize.width(),
                                               labelGlobalTopLeft.y() + labelSize.height());

        QPoint newLabelGlobalPos = labelGlobalTopLeft;


        if(labelGlobalBottomRight.x() > globalBottomRight.x())
        {
            newLabelGlobalPos.setX(newLabelGlobalPos.x() - (labelGlobalBottomRight.x() - globalBottomRight.x()));
        }

        if(labelGlobalBottomRight.y() > globalBottomRight.y())
        {
            newLabelGlobalPos.setY(newLabelGlobalPos.y() - (labelGlobalBottomRight.y() - globalBottomRight.y()));
        }

        move(newLabelGlobalPos);
    }
}
//======================================================


//======================================================
#include "X_ChemicalElementButton.h"

#include <QApplication>
#include <QDebug>
#include <QFontMetrics>
#include <QFrame>
#include <QPainter>
#include <QPalette>
#include <QPaintEvent>
#include <QPixmap>
#include <QPen>
#include <QRect>
#include <QResizeEvent>
#include <QStyle>
#include <QStyleOptionToolButton>
#include <QStylePainter>

//======================================================
// STATIC
//
QSize X_ChemicalElementButton::xv_symbolBoundingRectSize = QSize();
QSize X_ChemicalElementButton::xv_zNumberBoundingRectSize = QSize();
QSize X_ChemicalElementButton::xv_nameBoundingRectSize = QSize();
double X_ChemicalElementButton::xv_smallFontSizeRatio = 1.8;
//======================================================
//======================================================
QSize X_ChemicalElementButton::xp_symbolBoundingRectSize()
{
    return xv_symbolBoundingRectSize;
}
//======================================================
QSize X_ChemicalElementButton::xp_zNumberBoundingRectSize()
{
    return xv_zNumberBoundingRectSize;
}
//======================================================
QSize X_ChemicalElementButton::xp_nameBoundingRectSize()
{
    return xv_nameBoundingRectSize;
}
//======================================================
void X_ChemicalElementButton::xp_setSymbolBoundingRectSize(QSize size)
{
    xv_symbolBoundingRectSize = size;
}
//======================================================
void X_ChemicalElementButton::xp_setX_NumberBoundingRectSize(QSize size)
{
    xv_zNumberBoundingRectSize = size;
}
//======================================================
void X_ChemicalElementButton::xp_setNameBoundingRectSize(QSize size)
{
    xv_nameBoundingRectSize = size;
}
//======================================================
int X_ChemicalElementButton::xp_smallFontSizeRatio()
{
    return xv_smallFontSizeRatio;
}
//======================================================
bool X_ChemicalElementButton::xp_setSmallFontSizeRatio(int ratio)
{
    if(ratio <= 0)
    {
        return false;
    }

    xv_smallFontSizeRatio = ratio;
    return true;
}
//======================================================
// END STATIC
//======================================================
X_ChemicalElementButton::X_ChemicalElementButton(X_Number zNumber, QWidget *parent)
    : QToolButton(parent)
{
    xv_frame = new QFrame;
    xv_frameLineWidth = 1;
    xv_margin = 1;
    xv_zNumber = zNumber;
    xv_name = QString();
    zh_setDefaultFonts();
    zh_setDefaultColors();

    zh_createConnections();
}
//======================================================
X_ChemicalElementButton::~X_ChemicalElementButton()
{
    delete xv_frame;
}
//======================================================
void X_ChemicalElementButton::zh_setDefaultFonts()
{
    QFont font = this->font();
    //    font.setBold(true);
    xp_setPropertyFont(PN_SYMBOL, font);

    qreal fontPointSizeF = font.pointSizeF() / xv_smallFontSizeRatio;
    font.setPointSizeF(fontPointSizeF);
    font.setBold(false);

    xp_setPropertyFont(PN_X_NUMBER, font);
    xp_setPropertyFont(PN_NAME, font);
}
//======================================================
void X_ChemicalElementButton::zh_setDefaultColors()
{
    //    QColor color = palette().color(QPalette::Active, QPalette::ButtonText);
    //    xp_setPropertyColor(PN_SYMBOL, color);
    //    xp_setPropertyColor(PN_X_NUMBER, color);
    //    xp_setPropertyColor(PN_NAME, color);
}
//======================================================
void X_ChemicalElementButton::zh_createConnections()
{
    connect(qApp, &QApplication::fontDatabaseChanged,
            this, &X_ChemicalElementButton::zh_onAppFontDatabaseChange);
}
//======================================================
void X_ChemicalElementButton::resizeEvent(QResizeEvent* event)
{
    QToolButton::resizeEvent(event);
}
//======================================================
void X_ChemicalElementButton::paintEvent(QPaintEvent *event)
{
    if(xv_frame == nullptr)
    {
        return;
    }

    QStyleOptionToolButton option;
    initStyleOption(&option);

    int frameStyle = QFrame::Panel;
    if(this->isDown() || this->isChecked())
    {
        frameStyle |= QFrame::Sunken;
    }
    else
    {
        frameStyle |= QFrame::Raised;
    }

    xv_frame->setPalette(zh_currentPalette());

    xv_frame->setFrameStyle(frameStyle);
    xv_frame->setLineWidth(xv_frameLineWidth);
    xv_frame->resize(event->rect().size());
    // create pixmap and grab the frame on it
    QPixmap buttonPixmap(event->rect().size());
    buttonPixmap =  xv_frame->grab();
    // painter
    QStylePainter pixPainter(&buttonPixmap, this);
    // view port rect without frame
    QRect commonViewportRect = xp_buttonViewPortRect();

    // z number
    if(!xv_zNumberViewPortRect.isEmpty() && !xv_zNumberWindowRect.isEmpty())
    {
        pixPainter.save();
        pixPainter.setFont(zh_zNumberFont());
        pixPainter.setPen(zh_textColor());
        pixPainter.setBrush(zh_textColor());
        pixPainter.setRenderHint(QPainter::Antialiasing);

        xv_zNumberViewPortRect.moveTopRight(commonViewportRect.topRight());
        pixPainter.setViewport(xv_zNumberViewPortRect);
        pixPainter.setWindow(xv_zNumberWindowRect);
        pixPainter.drawText(xv_zNumberWindowRect,
                            Qt::AlignCenter,
                            QString::number(xv_zNumber));
        pixPainter.restore();
    }

    // symbol
    if(!xv_symbolViewPortRect.isEmpty() && !xv_symbolWindowRect.isEmpty())
    {
        pixPainter.save();
        pixPainter.setFont(zh_symbolFont());
        pixPainter.setPen(zh_textColor());
        pixPainter.setBrush(zh_textColor());
        pixPainter.setRenderHint(QPainter::Antialiasing);

        // free part of commonViewportRect
        QRect freePartCommonViewportRect = commonViewportRect.adjusted(0,xv_zNumberViewPortRect.height(),
                                                                       0, -xv_nameViewPortRect.height());

        xv_symbolViewPortRect.moveCenter(freePartCommonViewportRect.center());
        pixPainter.setViewport(xv_symbolViewPortRect);
        pixPainter.setWindow(xv_symbolWindowRect);
        pixPainter.drawText(xv_symbolWindowRect,
                            Qt::AlignCenter,
                            xv_symbol);

        pixPainter.restore();
    }

    // name
    if(!xv_nameViewPortRect.isEmpty() && !xv_nameWindowRect.isEmpty())
    {
        pixPainter.save();
        pixPainter.setFont(zh_nameFont());
        pixPainter.setPen(zh_textColor());
        pixPainter.setBrush(zh_textColor());
        pixPainter.setRenderHint(QPainter::Antialiasing);

        QPoint nameCenterPoint;
        nameCenterPoint.setX(commonViewportRect.center().x());
        nameCenterPoint.setY(commonViewportRect.bottom() - (xv_nameViewPortRect.height() / 2));

        xv_nameViewPortRect.moveCenter(nameCenterPoint);
        pixPainter.setViewport(xv_nameViewPortRect);
        pixPainter.setWindow(xv_nameWindowRect);
        pixPainter.drawText(xv_nameWindowRect,
                            Qt::AlignCenter,
                            xv_name);

        pixPainter.restore();
    }

    // Draw Pixmap
    QPainter painter(this);
    painter.drawPixmap(event->rect(), buttonPixmap);
}
//======================================================
void X_ChemicalElementButton::zh_onAppFontDatabaseChange()
{
    zh_recalcPropertyWindowRects();
    update();
}
//======================================================
void X_ChemicalElementButton::xp_setChemicalElementSymbol(const QString& symbol)
{
    xv_symbol = symbol;
    zh_recalcPropertyWindowRects();
    emit xg_symbolChanged(xv_symbol);
    update();
}
//======================================================
QString X_ChemicalElementButton::xp_chemicalElementSymbol() const
{
    return xv_symbol;
}
//======================================================
void X_ChemicalElementButton::xp_setChemicalElementName(const QString& name)
{
    xv_name = name;
    zh_recalcPropertyWindowRects();
    update();
}
//======================================================
QString X_ChemicalElementButton::xp_chemicalElementName() const
{
    return xv_name;
}
//======================================================
void X_ChemicalElementButton::xp_updatePropertyCaptions()
{
    xp_updatePropertyViewPortRects();
    update();
}
//======================================================
QRect X_ChemicalElementButton::xp_buttonViewPortRect() const
{
    QRect rect;
    if(this->isDown())
    {
        rect = this->rect().adjusted(1,1,-1,-1);
        // rect = this->rect().adjusted(0,0,-2,-2);
    }
    else
    {
        rect = this->rect().adjusted(0,0,-2,-2);
        // rect = this->rect().adjusted(1,1,-1,-1);
    }

    int spacing = zh_marginSpacing();
    rect.adjust(spacing, spacing, -spacing, -spacing);
    return rect;
}
//======================================================
int X_ChemicalElementButton::zh_marginSpacing() const
{
    int spacing = xv_frameLineWidth + xv_margin;
    return spacing;
}
//======================================================
QSize X_ChemicalElementButton::xp_symbolWindowRectSize() const
{
    return xv_symbolWindowRect.size();
}
//======================================================
QSize X_ChemicalElementButton::xp_zNumberWindowRectSize() const
{
    return xv_zNumberWindowRect.size();
}
//======================================================
QSize X_ChemicalElementButton::xp_nameWindowRectSize() const
{
    return xv_nameWindowRect.size();
}
//======================================================
void X_ChemicalElementButton::xp_updateMinimumSizeForSymbolRectSize(QSize symbolSize)
{
    int spacing = zh_marginSpacing();
    symbolSize.setHeight(symbolSize.height() + (spacing * 2));
    symbolSize.setWidth(symbolSize.width() + (spacing * 2));

    setMinimumSize(symbolSize);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    updateGeometry();
}
//======================================================
void X_ChemicalElementButton::xp_setPropertyFont(PropertyName proprtyName,
                                                QFont font)
{
    xv_fontMap[proprtyName] = font;
    update();
}
//======================================================
void X_ChemicalElementButton::xp_updatePropertyViewPortRects()
{
    QSize symbolViewPortRectSize;
    zh_recalcViewPortRectSize(xv_symbolBoundingRectSize,
                              xv_symbolWindowRect.size(),
                              symbolViewPortRectSize);
    xv_symbolViewPortRect = QRect(QPoint(0, 0), symbolViewPortRectSize);

    QSize zNumberViewPortRectSize;
    zh_recalcViewPortRectSize(xv_zNumberBoundingRectSize,
                              xv_zNumberWindowRect.size(),
                              zNumberViewPortRectSize);
    xv_zNumberViewPortRect = QRect(QPoint(0, 0), zNumberViewPortRectSize);

    QSize nameViewPortRectSize;
    zh_recalcViewPortRectSize(xv_nameBoundingRectSize,
                              xv_nameWindowRect.size(),
                              nameViewPortRectSize);
    xv_nameViewPortRect = QRect(QPoint(0, 0), nameViewPortRectSize);

    update();
}
//======================================================
void X_ChemicalElementButton::zh_recalcViewPortRectSize(QSize boundingViewPortSize,
                                                       QSize windowSize,
                                                       QSize&  viewPortSize) const
{
    if(boundingViewPortSize.isEmpty() || windowSize.isEmpty())
    {
        viewPortSize = QSize();
    }
    else
    {
        double windowRectSideRatio = static_cast<double>(windowSize.width())
                / static_cast<double>(windowSize.height());
        double viewPortRectSideRatio = static_cast<double>(boundingViewPortSize.width())
                / static_cast<double>(boundingViewPortSize.height());

        double scaleFactor;
        if(windowRectSideRatio > viewPortRectSideRatio)
        {
            // fit by width
            scaleFactor = boundingViewPortSize.width()
                    / static_cast<double>(windowSize.width());
        }
        else
        {
            // fit by height
            scaleFactor = boundingViewPortSize.height()
                    / static_cast<double>(windowSize.height());
        }

        viewPortSize.setHeight(qRound(windowSize.height() * scaleFactor));
        viewPortSize.setWidth(qRound(windowSize.width() * scaleFactor));
    }
}
//======================================================
void X_ChemicalElementButton::zh_recalcPropertyWindowRects()
{
    QFontMetrics fontMetrics(zh_symbolFont());
    xv_symbolWindowRect.setWidth(fontMetrics.horizontalAdvance(xv_symbol));
    xv_symbolWindowRect.setHeight(fontMetrics.height());

    fontMetrics = QFontMetrics(zh_zNumberFont());
    xv_zNumberWindowRect.setWidth(fontMetrics.horizontalAdvance(QString::number(xv_zNumber)));
    xv_zNumberWindowRect.setHeight(fontMetrics.height());

    fontMetrics = QFontMetrics(zh_nameFont());
    xv_nameWindowRect.setWidth(fontMetrics.horizontalAdvance(xv_name));
    xv_nameWindowRect.setHeight(fontMetrics.height());
}
//======================================================
X_Number X_ChemicalElementButton::xp_zNumber() const
{
    return xv_zNumber;
}
//======================================================
QFont X_ChemicalElementButton::zh_symbolFont() const
{
    QFont font = xv_fontMap.value(PN_SYMBOL);
    return font;
}
//======================================================
QFont X_ChemicalElementButton::zh_zNumberFont() const
{
    QFont font = xv_fontMap.value(PN_X_NUMBER);
    return font;
}
//======================================================
QFont X_ChemicalElementButton::zh_nameFont() const
{
    QFont font = xv_fontMap.value(PN_NAME);
    return font;
}
//======================================================
QColor X_ChemicalElementButton::zh_textColor() const
{
    QColor color;
    if(isChecked())
    {
        color = this->palette().color(QPalette::HighlightedText);
    }
    else
    {
        color = this->palette().color(QPalette::ButtonText);
    }
    return color;
}
//======================================================
QPalette X_ChemicalElementButton::zh_currentPalette() const
{
    QPalette palette = this->palette();
    if(isChecked())
    {
        palette.setColor(QPalette::Window, palette.color(QPalette::Highlight));
    }

    return palette;
}
//======================================================

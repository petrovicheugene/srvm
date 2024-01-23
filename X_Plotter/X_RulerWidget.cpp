//=========================================================
#include "X_RulerWidget.h"
#include <QLabel>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QPixmap>
#include <math.h>
//=========================================================
X_RulerWidget::X_RulerWidget(QWidget *parent) : QWidget(parent)
{
    xv_plot = nullptr;
    xv_rulersAndGreedManager = nullptr;
    xv_formatSet << 'f' << 'g' << 'e' << 'E' << 'G';
    //    xv_rightRulerProperty.visibility = false;
    //    xv_topRulerProperty.visibility = false;
    xv_lineColor = palette().color(QPalette::Text);
    xv_lineWidth = 1;

    xv_infoLabel = new QLabel(this);
    xh_recalcRuleSizes();
}
//=========================================================
void X_RulerWidget::xp_setPlotView(QWidget* plot)
{
    if(plot != nullptr)
    {
        plot->setParent(this);
        xv_plot = plot;
    }
    update();
}
//=========================================================
bool X_RulerWidget::xp_setLineWidth(int width)
{
    if(width < 1)
    {
        return false;
    }
    xv_lineWidth = width;
    update();
    return true;
}
//=========================================================
void X_RulerWidget::xp_setLineColor(QColor color)
{
    xv_lineColor = color;
    update();
}
//=========================================================
void X_RulerWidget::xp_setLeftScratchLineSize(int size)
{
    xv_leftRulerProperty.scratchLineSize = size;
    update();
}
//=========================================================
void X_RulerWidget::xp_setRightScratchLineSize(int size)
{
    xv_rightRulerProperty.scratchLineSize = size;
    update();
}
//=========================================================
void X_RulerWidget::xp_setTopScratchLineSize(int size)
{
    xv_topRulerProperty.scratchLineSize = size;
    update();
}
//=========================================================
void X_RulerWidget::xp_setBottomScratchLineSize(int size)
{
    xv_bottomRulerProperty.scratchLineSize = size;
    update();
}
//=========================================================
void X_RulerWidget::xp_setScratchLineSize(int size)
{
    xv_leftRulerProperty.scratchLineSize = size;
    xv_rightRulerProperty.scratchLineSize = size;
    xv_topRulerProperty.scratchLineSize = size;
    xv_bottomRulerProperty.scratchLineSize = size;
    update();
}
//=========================================================
bool X_RulerWidget::xp_setLeftFontSize(int size)
{
    if(size < 2)
    {
        return false;
    }
    xv_leftRulerProperty.fontSize = size;
    return true;
}
//=========================================================
bool X_RulerWidget::xp_setRightFontSize(int size)
{
    if(size < 2)
    {
        return false;
    }
    xv_rightRulerProperty.fontSize = size;
    return true;
}
//=========================================================
bool X_RulerWidget::xp_setTopFontSize(int size)
{
    if(size < 2)
    {
        return false;
    }
    xv_topRulerProperty.fontSize = size;
    return true;
}
//=========================================================
bool X_RulerWidget::xp_setBottomFontSize(int size)
{
    if(size < 2)
    {
        return false;
    }
    xv_bottomRulerProperty.fontSize = size;
    return true;
}
//=========================================================
int X_RulerWidget::xp_leftFontSize()
{
    return xv_leftRulerProperty.fontSize;
}
//=========================================================
int X_RulerWidget::xp_rightFontSize()
{
    return xv_rightRulerProperty.fontSize;
}
//=========================================================
int X_RulerWidget::xp_topFontSize()
{
    return xv_topRulerProperty.fontSize;
}
//=========================================================
int X_RulerWidget::xp_bottomFontSize()
{
    return xv_bottomRulerProperty.fontSize;
}
//=========================================================
bool X_RulerWidget::xp_setLeftMarkFormat(char format)
{
    if(!xv_formatSet.contains(format))
    {
        return false;
    }
    xv_leftRulerProperty.format = format;
    return true;
}
//=========================================================
bool X_RulerWidget::xp_setRightMarkFormat(char format)
{
    if(!xv_formatSet.contains(format))
    {
        return false;
    }
    xv_rightRulerProperty.format = format;
    return true;
}
//=========================================================
bool X_RulerWidget::xp_setTopMarkFormat(char format)
{
    if(!xv_formatSet.contains(format))
    {
        return false;
    }
    xv_topRulerProperty.format = format;
    return true;
}
//=========================================================
bool X_RulerWidget::xp_setBottomMarkFormat(char format)
{
    if(!xv_formatSet.contains(format))
    {
        return false;
    }
    xv_bottomRulerProperty.format = format;
    return true;
}

//=========================================================
bool X_RulerWidget::xp_setLeftMarkPrecision(int precision)
{
    if(precision < 0)
    {
        return false;
    }
    xv_leftRulerProperty.precision = precision;
    return true;
}
//=========================================================
bool X_RulerWidget::xp_setRightMarkPrecision(int precision)
{
    if(precision < 0)
    {
        return false;
    }
    xv_rightRulerProperty.precision = precision;
    return true;
}
//=========================================================
bool X_RulerWidget::xp_setTopMarkPrecision(int precision)
{
    if(precision < 0)
    {
        return false;
    }
    xv_topRulerProperty.precision = precision;
    return true;
}
//=========================================================
bool X_RulerWidget::xp_setBottomMarkPrecision(int precision)
{
    if(precision < 0)
    {
        return false;
    }
    xv_bottomRulerProperty.precision = precision;
    return true;
}
//=========================================================
void X_RulerWidget::xp_setLeftMarkRecalcMetrix(const QString& label, bool labelVisibility,double K0, double K1, double K2)
{
    xv_leftRulerProperty.ruleLabel = label;
    xv_leftRulerProperty.ruleLabelVisibility = labelVisibility;
    xh_recalcRuleSizes();
    xv_leftRulerProperty.K0 = K0;
    xv_leftRulerProperty.K1 = K1;
    xv_leftRulerProperty.K2 = K2;
    update();
}
//=========================================================
void X_RulerWidget::xp_setRightMarkRecalcMetrix(const QString& label, bool labelVisibility,double K0, double K1, double K2)
{
    xv_rightRulerProperty.ruleLabel = label;
    xv_rightRulerProperty.ruleLabelVisibility = labelVisibility;
    xh_recalcRuleSizes();
    xv_rightRulerProperty.K0 = K0;
    xv_rightRulerProperty.K1 = K1;
    xv_rightRulerProperty.K2 = K2;
    update();
}
//=========================================================
void X_RulerWidget::xp_setTopMarkRecalcMetrix(const QString& label, bool labelVisibility,double K0, double K1, double K2)
{
    xv_topRulerProperty.ruleLabel = label;
    xv_topRulerProperty.ruleLabelVisibility = labelVisibility;
    xh_recalcRuleSizes();
    xv_topRulerProperty.K0 = K0;
    xv_topRulerProperty.K1 = K1;
    xv_topRulerProperty.K2 = K2;
    update();
}
//=========================================================
void X_RulerWidget::xp_setBottomMarkRecalcMetrix(const QString& label, bool labelVisibility,double K0, double K1, double K2)
{
    xv_bottomRulerProperty.ruleLabel = label;
    xv_bottomRulerProperty.ruleLabelVisibility = labelVisibility;
    xh_recalcRuleSizes();
    xv_bottomRulerProperty.K0 = K0;
    xv_bottomRulerProperty.K1 = K1;
    xv_bottomRulerProperty.K2 = K2;
    update();
}
//=========================================================
void X_RulerWidget::xp_setLeftMarkRecalcFlag(bool flag)
{
    if(xv_leftRulerProperty.recalcMarkLabel == flag)
    {
        return;
    }

    xv_leftRulerProperty.recalcMarkLabel = flag;
    update();
}
//=========================================================
void X_RulerWidget::xp_setRightMarkRecalcFlag(bool flag)
{
    if(xv_rightRulerProperty.recalcMarkLabel == flag)
    {
        return;
    }

    xv_rightRulerProperty.recalcMarkLabel = flag;
    update();
}
//=========================================================
void X_RulerWidget::xp_setTopMarkRecalcFlag(bool flag)
{
    if(xv_topRulerProperty.recalcMarkLabel == flag)
    {
        return;
    }

    xv_topRulerProperty.recalcMarkLabel = flag;
    update();
}
//=========================================================
void X_RulerWidget::xp_setBottomMarkRecalcFlag(bool flag)
{
    if(xv_bottomRulerProperty.recalcMarkLabel == flag)
    {
        return;
    }

    xv_bottomRulerProperty.recalcMarkLabel = flag;
    update();
}
//=========================================================
void X_RulerWidget::xp_setHorizontalMarkRecalcFactors(const QString& label, double K0, double K1, double K2)
{
    xv_topRulerProperty.ruleLabel = label;
    xv_topRulerProperty.K0 = K0;
    xv_topRulerProperty.K1 = K1;
    xv_topRulerProperty.K2 = K2;

    xv_bottomRulerProperty.ruleLabel = label;
    xv_bottomRulerProperty.K0 = K0;
    xv_bottomRulerProperty.K1 = K1;
    xv_bottomRulerProperty.K2 = K2;
    update();
}
//=========================================================
void X_RulerWidget::xp_horizontalMarkRecalcFactors(QString& label, double& K0, double& K1, double& K2) const
{
    label = xv_topRulerProperty.ruleLabel;
    K0 = xv_topRulerProperty.K0;
    K1 = xv_topRulerProperty.K1;
    K2 = xv_topRulerProperty.K2;

}
//=========================================================
void X_RulerWidget::xp_setVerticalMarkRecalcFactors(const QString& label, double K0, double K1, double K2)
{
    xv_leftRulerProperty.ruleLabel = label;
    xv_leftRulerProperty.K0 = K0;
    xv_leftRulerProperty.K1 = K1;
    xv_leftRulerProperty.K2 = K2;

    xv_rightRulerProperty.ruleLabel = label;
    xv_rightRulerProperty.K0 = K0;
    xv_rightRulerProperty.K1 = K1;
    xv_rightRulerProperty.K2 = K2;
    update();
}
//=========================================================
void X_RulerWidget::xp_setTopRuleVisibility(bool visibility)
{
    xv_topRulerProperty.visibility = visibility;
    update();
}
//=========================================================
void X_RulerWidget::xp_setBottomRuleVisibility(bool visibility)
{
    xv_bottomRulerProperty.visibility = visibility;
    update();
}
//=========================================================
void X_RulerWidget::xp_setLeftRuleVisibility(bool visibility)
{
    xv_leftRulerProperty.visibility = visibility;
    update();
}
//=========================================================
void X_RulerWidget::xp_setRightRuleVisibility(bool visibility)
{
    xv_rightRulerProperty.visibility = visibility;
    update();
}
//=========================================================
void X_RulerWidget::xp_setTopRuleLabel(const QString &label)
{
    xv_topRulerProperty.ruleLabel = label;
    update();
}
//=========================================================
void X_RulerWidget::xp_setBottomRuleLabel(const QString &label)
{
    xv_bottomRulerProperty.ruleLabel = label;
    update();
}
//=========================================================
void X_RulerWidget::xp_setLeftRuleLabel(const QString &label)
{
    xv_leftRulerProperty.ruleLabel = label;
    update();
}
//=========================================================
void X_RulerWidget::xp_setRightRuleLabel(const QString &label)
{
    xv_rightRulerProperty.ruleLabel = label;
    update();
}
//=========================================================
void X_RulerWidget::xp_setTopRuleLabelVisible(bool visible)
{
    if(xv_topRulerProperty.ruleLabelVisibility == visible)
    {
        return;
    }
    xv_topRulerProperty.ruleLabelVisibility = visible;
    update();
}
//=========================================================
void X_RulerWidget::xp_setBottomRuleLabelVisible(bool visible)
{
    if(xv_bottomRulerProperty.ruleLabelVisibility == visible)
    {
        return;
    }
    xv_bottomRulerProperty.ruleLabelVisibility = visible;
    update();
}
//=========================================================
void X_RulerWidget::xp_setLeftRuleLabelVisible(bool visible)
{
    if(xv_leftRulerProperty.ruleLabelVisibility == visible)
    {
        return;
    }
    xv_leftRulerProperty.ruleLabelVisibility = visible;
    update();
}
//=========================================================
void X_RulerWidget::xp_setRightRuleLabelVisible(bool visible)
{
    if(xv_rightRulerProperty.ruleLabelVisibility == visible)
    {
        return;
    }
    xv_rightRulerProperty.ruleLabelVisibility = visible;
    update();
}
//=========================================================
void X_RulerWidget::xp_setHorizontalRulePointList(QList<RulePoint> * XRuleList)
{
    xv_XRuleList = XRuleList;
}
//=========================================================
void X_RulerWidget::xp_setVerticalRulePointList(QList<RulePoint> * YRuleList)
{
    xv_YRuleList = YRuleList;
}
//=========================================================
int X_RulerWidget::xp_maxMarkWidth()
{
    return xv_maxMarkWidth;
}
//=========================================================
int X_RulerWidget::xp_maxMarkHeight()
{
    return xv_maxMarkHeight;
}
//=========================================================
void X_RulerWidget::xp_setInfoLabelText(const QString& text)
{
    xv_infoLabel->setText(text);
}
//=========================================================
void X_RulerWidget::xp_setInfoLabelVisible(bool visible)
{
    xv_infoLabel->setVisible(visible);
}
//=========================================================
void X_RulerWidget::resizeEvent(QResizeEvent* event)
{
    if(xv_plot != 0)
    {
        // plot layout
        int newPlotWidth = event->size().width() - xv_leftRulerProperty.rulerSize() -
                           xv_rightRulerProperty.rulerSize();

        int newPliotHeight = event->size().height() - xv_topRulerProperty.rulerSize() -
                             xv_bottomRulerProperty.rulerSize();

        if(newPlotWidth < 50 || newPliotHeight < 20)
        {
            QWidget::resizeEvent(event);
            xv_dontRepaint = true;
            xv_plot->setHidden(true);
            return;
        }
        else
        {
            xv_dontRepaint = false;
            xv_plot->setHidden(false);
        }

        QSize plotSize(newPlotWidth, newPliotHeight);
        QPoint plotPos(xv_leftRulerProperty.rulerSize(), xv_topRulerProperty.rulerSize());

        QRect geometryRect(plotPos, plotSize);

        // scratch line rects
        // QRect boundRect = geometryRect.adjusted(-1, -1, 0, 0);
        // left
        QPoint scratchPoint(geometryRect.left() - xv_leftRulerProperty.scratchLineSize, geometryRect.top() - 1);
        QSize scratchRectSize(xv_leftRulerProperty.scratchLineSize, geometryRect.height() + 2);
        xv_leftRulerProperty.scratchRect = QRect(scratchPoint, scratchRectSize);
        // right
        scratchPoint = QPoint(geometryRect.right() + 1, geometryRect.top() - 1);
        scratchRectSize = QSize(xv_rightRulerProperty.scratchLineSize, geometryRect.height() + 2);
        xv_rightRulerProperty.scratchRect = QRect(scratchPoint, scratchRectSize);
        // top
        scratchPoint = QPoint(geometryRect.left() - 1, geometryRect.top() - xv_topRulerProperty.scratchLineSize);
        scratchRectSize = QSize(geometryRect.width() + 2, xv_topRulerProperty.scratchLineSize);
        xv_topRulerProperty.scratchRect = QRect(scratchPoint, scratchRectSize);
        // bottom
        scratchPoint = QPoint(geometryRect.left() - 1, geometryRect.bottom() +1);
        scratchRectSize = QSize(geometryRect.width() + 2, xv_bottomRulerProperty.scratchLineSize);
        xv_bottomRulerProperty.scratchRect = QRect(scratchPoint, scratchRectSize);

        // mark lines rects and rule labelRect
        xv_topRulerProperty.markRect = QRect(QPoint(0,0), QSize(event->size().width(), xv_topRulerProperty.markLineSize));
        if(xv_topRulerProperty.ruleLabelVisibility)
        {
            int dy2 = (xv_topRulerProperty.markRect.height()  - xv_maxMarkHeight*1.5) * -1;
            xv_topRulerProperty.ruleLabelRect = xv_topRulerProperty.markRect.adjusted(0,0,0,dy2);
        }
        else
        {
            xv_topRulerProperty.ruleLabelRect = QRect();
        }

        xv_bottomRulerProperty.markRect = QRect(QPoint(0,event->size().height() - xv_bottomRulerProperty.markLineSize),
                                                QSize(event->size().width(), xv_bottomRulerProperty.markLineSize));
        if(xv_bottomRulerProperty.ruleLabelVisibility)
        {
            int dy1 = (xv_bottomRulerProperty.markRect.height()  - xv_maxMarkHeight*1.5);
            xv_bottomRulerProperty.ruleLabelRect = xv_bottomRulerProperty.markRect.adjusted(0, dy1, 0,0);
        }
        else
        {
            xv_bottomRulerProperty.ruleLabelRect = QRect();
        }

        xv_leftRulerProperty.markRect = QRect(QPoint(0, geometryRect.top() - xv_maxMarkHeight),
                                              QSize(xv_leftRulerProperty.markLineSize, geometryRect.height() + (xv_maxMarkHeight*2)));
        if(xv_leftRulerProperty.ruleLabelVisibility)
        {
            int dx2 = (xv_leftRulerProperty.markRect.width() - xv_maxMarkHeight*1.5) * -1;
            xv_leftRulerProperty.ruleLabelRect = xv_leftRulerProperty.markRect.adjusted(0,0, dx2, 0);
        }
        else
        {
            xv_leftRulerProperty.ruleLabelRect = QRect();
        }

        xv_rightRulerProperty.markRect = QRect(QPoint(geometryRect.right()+1 + xv_rightRulerProperty.scratchLineSize, geometryRect.top() - xv_maxMarkHeight),
                                               QSize(xv_rightRulerProperty.markLineSize, geometryRect.height() + (xv_maxMarkHeight*2)));
        if(xv_rightRulerProperty.ruleLabelVisibility)
        {
            int dx1 = xv_leftRulerProperty.markRect.width() - xv_maxMarkHeight*1.5;
            xv_rightRulerProperty.ruleLabelRect = xv_rightRulerProperty.markRect.adjusted(dx1, 0, 0, 0 );
        }
        else
        {
            xv_rightRulerProperty.ruleLabelRect = QRect();
        }

        xv_plot->setGeometry(geometryRect);

        QRect infoLabelRect = xv_bottomRulerProperty.ruleLabelRect;
        infoLabelRect.setLeft(geometryRect.left());
        QFontMetrics fm(this->font());
        int ruleLabelWidth = fm.horizontalAdvance(xv_bottomRulerProperty.ruleLabel);
        infoLabelRect.setRight(geometryRect.center().x() - ruleLabelWidth);
        xv_infoLabel->setGeometry(infoLabelRect);
    }

    QWidget::resizeEvent(event);
}
//=========================================================
void X_RulerWidget::paintEvent(QPaintEvent* event)
{
    if(xv_plot != 0 && !xv_dontRepaint)
    {
        QPainter painter(this);
        painter.setWindow(event->rect());
        QPen pen(QBrush(xv_lineColor), xv_lineWidth);
        painter.setPen(pen);

        xh_paintLeftRule(&painter);
        xh_paintRightRule(&painter);
        xh_paintTopRule(&painter);
        xh_paintBottomRule(&painter);


        QRect boundRect = xv_plot->geometry().adjusted(-1, -1, 0, 0);
        painter.drawRect(boundRect);

        return;
    }

    QWidget::paintEvent(event);
}
//=========================================================
void X_RulerWidget::xh_paintLeftRule(QPainter* painter)
{
    if(!xv_leftRulerProperty.visibility || xv_YRuleList == 0)
    {
        return;
    }
    painter->save();

    QFont font = this->font();
    font.setPixelSize(xv_leftRulerProperty.fontSize);
    QFontMetrics fontMetrics(font);
    QRectF markRect;
    painter->setFont(font);

    double y;
    for(int i = 0; i < xv_YRuleList->count(); i++)
    {
        // scratch
        y = mapFromGlobal(QPointF(0.0, qRound(xv_YRuleList->value(i).globalPos))).y();

        if(xv_leftRulerProperty.scratchRect.isValid())
        {
            painter->drawLine(QPointF(xv_leftRulerProperty.scratchRect.left() , y),
                              QPointF(xv_leftRulerProperty.scratchRect.right(), y));

        }
        // Mark
        if(xv_leftRulerProperty.markRect.isValid() && xv_YRuleList->value(i).markType != RulePoint::MT_SCRATCH)
        {
            // scratch
            painter->drawLine(QPointF(xv_leftRulerProperty.markRect.right() - xv_leftRulerProperty.scratchLineSize, y),
                              QPointF(xv_leftRulerProperty.markRect.right(), y));


            // mark
            if(xv_YRuleList->value(i).markType == RulePoint::MT_MARK)
            {
                double markValue = xv_YRuleList->value(i).markValue * -1;
                if(xv_leftRulerProperty.recalcMarkLabel)
                {
                    markValue = pow(markValue, 2.0)*xv_leftRulerProperty.K2
                                + markValue*xv_leftRulerProperty.K1
                                + xv_leftRulerProperty.K0;
                }
                QString mark = QString::number(markValue, xv_leftRulerProperty.format, xv_leftRulerProperty.precision);
                markRect = QRectF(QPointF(0.0, 0.0),
                                 QSizeF(fontMetrics.horizontalAdvance(mark), xv_maxMarkHeight));
                markRect.moveCenter(QPointF(xv_leftRulerProperty.markRect.right()
                                               - xv_leftRulerProperty.scratchLineSize
                                               - xv_leftRulerProperty.markOffsetFromScratch
                                               - markRect.width()/2.0, y));
                painter->drawText(markRect, mark);
            }
        }
    }

    if(xv_leftRulerProperty.ruleLabelVisibility)
    {
        painter->rotate(-90);
        QRectF ruleLabelRectForRotatedPainter;
        ruleLabelRectForRotatedPainter.setBottom(xv_leftRulerProperty.ruleLabelRect.right());
        ruleLabelRectForRotatedPainter.setTop(xv_leftRulerProperty.ruleLabelRect.left());
        ruleLabelRectForRotatedPainter.setRight(xv_leftRulerProperty.ruleLabelRect.top() * -1.0);
        ruleLabelRectForRotatedPainter.setLeft(xv_leftRulerProperty.ruleLabelRect.bottom() * -1.0);
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawText(ruleLabelRectForRotatedPainter, Qt::AlignCenter, xv_leftRulerProperty.ruleLabel);
    }

    painter->restore();
}
//=========================================================
void X_RulerWidget::xh_paintRightRule(QPainter* painter)
{
    if(!xv_rightRulerProperty.visibility || xv_YRuleList == 0)
    {
        return;
    }
    painter->save();

    QFont font = this->font();
    font.setPixelSize(xv_rightRulerProperty.fontSize);
    QFontMetrics fontMetrics(font);
    QRectF markRect;
    painter->setFont(font);

    double y;
    for(int i = 0; i < xv_YRuleList->count(); i++)
    {
        // scratch
        y = mapFromGlobal(QPointF(0.0, qRound(xv_YRuleList->value(i).globalPos))).y();

        if(xv_rightRulerProperty.scratchRect.isValid())
        {
            painter->drawLine(QPointF(xv_rightRulerProperty.scratchRect.left() , y),
                              QPointF(xv_rightRulerProperty.scratchRect.right(), y));

        }
        // Mark
        if(xv_rightRulerProperty.markRect.isValid() && xv_YRuleList->value(i).markType != RulePoint::MT_SCRATCH)
        {
            // scratch
            painter->drawLine(QPointF(xv_rightRulerProperty.markRect.left() + xv_rightRulerProperty.scratchLineSize, y),
                              QPointF(xv_rightRulerProperty.markRect.left(), y));


            // mark
            if(xv_YRuleList->value(i).markType == RulePoint::MT_MARK)
            {
                double markValue = xv_YRuleList->value(i).markValue * -1;
                if(xv_rightRulerProperty.recalcMarkLabel)
                {
                    markValue = pow(markValue, 2.0)*xv_rightRulerProperty.K2
                                + markValue*xv_rightRulerProperty.K1
                                + xv_rightRulerProperty.K0;
                }

                QString mark = QString::number(markValue, xv_rightRulerProperty.format, xv_rightRulerProperty.precision);
                markRect = QRectF(QPointF(0.0, 0.0),
                                 QSizeF(fontMetrics.horizontalAdvance(mark), xv_maxMarkHeight));
                markRect.moveCenter(QPointF(xv_rightRulerProperty.markRect.left()
                                               + xv_rightRulerProperty.scratchLineSize
                                               + xv_rightRulerProperty.markOffsetFromScratch
                                               + markRect.width()/2.0, y));
                painter->drawText(markRect, mark);
            }
        }
    }

    if(xv_rightRulerProperty.ruleLabelVisibility)
    {
        painter->rotate(-90);
        QRectF ruleLabelRectForRotatedPainter;
        ruleLabelRectForRotatedPainter.setBottom(xv_rightRulerProperty.ruleLabelRect.right());
        ruleLabelRectForRotatedPainter.setTop(xv_rightRulerProperty.ruleLabelRect.left());
        ruleLabelRectForRotatedPainter.setRight(xv_rightRulerProperty.ruleLabelRect.top() * -1.0);
        ruleLabelRectForRotatedPainter.setLeft(xv_rightRulerProperty.ruleLabelRect.bottom() * -1.0);
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawText(ruleLabelRectForRotatedPainter, Qt::AlignCenter, xv_rightRulerProperty.ruleLabel);
    }
    painter->restore();
}
//=========================================================
void X_RulerWidget::xh_paintTopRule(QPainter* painter)
{
    if(!xv_topRulerProperty.visibility || xv_XRuleList == 0)
    {
        return;
    }
    painter->save();

    double x;
    for(int i = 0; i < xv_XRuleList->count(); i++)
    {
        // scratch
        x = mapFromGlobal(QPointF(qRound(xv_XRuleList->value(i).globalPos), 0.0)).x();

        if(xv_topRulerProperty.scratchRect.isValid())
        {
            painter->drawLine(QPointF(x, xv_topRulerProperty.scratchRect.top()),
                              QPointF(x, xv_topRulerProperty.scratchRect.bottom()));
        }
        // Mark
        if(xv_topRulerProperty.markRect.isValid() && xv_XRuleList->value(i).markType != RulePoint::MT_SCRATCH)
        {
            QFont font = this->font();
            font.setPixelSize(xv_topRulerProperty.fontSize);
            QFontMetrics fontMetrics(font);
            QRectF markRect;
            painter->setFont(font);
            // scratch
            painter->drawLine(QPointF(x, xv_topRulerProperty.markRect.bottom() -  xv_topRulerProperty.scratchLineSize),
                              QPointF(x, xv_topRulerProperty.markRect.bottom()));


            // mark
            if(xv_XRuleList->value(i).markType == RulePoint::MT_MARK)
            {
                double markValue = xv_XRuleList->value(i).markValue;
                if(xv_topRulerProperty.recalcMarkLabel)
                {
                    markValue = pow(markValue, 2.0)*xv_topRulerProperty.K2
                                + markValue*xv_topRulerProperty.K1
                                + xv_topRulerProperty.K0;
                }

                QString mark = QString::number(markValue, xv_topRulerProperty.format, xv_topRulerProperty.precision);
                markRect = QRectF(QPointF(0.0, 0.0),
                                 QSizeF(fontMetrics.horizontalAdvance(mark), xv_maxMarkHeight));
                markRect.moveCenter(QPoint(x, xv_topRulerProperty.markRect.bottom()
                                                  - xv_topRulerProperty.scratchLineSize
                                                  - xv_topRulerProperty.markOffsetFromScratch
                                                  - markRect.height()/2.0));
                painter->drawText(markRect, mark);
            }
        }
    }

    if(xv_topRulerProperty.ruleLabelVisibility)
    {
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawText(xv_topRulerProperty.ruleLabelRect, Qt::AlignCenter, xv_topRulerProperty.ruleLabel);
        //painter->fillRect(xv_topRulerProperty.ruleLabelRect, QBrush(QColor(Qt::blue)));
    }

    painter->restore();
}
//=========================================================
void X_RulerWidget::xh_paintBottomRule(QPainter* painter)
{
    if(!xv_bottomRulerProperty.visibility || xv_XRuleList == 0)
    {
        return;
    }
    painter->save();

    double x;
    for(int i = 0; i < xv_XRuleList->count(); i++)
    {
        // scratch
        x = mapFromGlobal(QPointF(qRound(xv_XRuleList->value(i).globalPos), 0.0)).x();

        if(xv_bottomRulerProperty.scratchRect.isValid())
        {
            painter->drawLine(QPointF(x, xv_bottomRulerProperty.scratchRect.top()),
                              QPointF(x, xv_bottomRulerProperty.scratchRect.bottom()));
        }
        // Mark
        if(xv_bottomRulerProperty.markRect.isValid() && xv_XRuleList->value(i).markType != RulePoint::MT_SCRATCH)
        {
            QFont font = this->font();
            font.setPixelSize(xv_bottomRulerProperty.fontSize);
            QFontMetrics fontMetrics(font);
            QRectF markRect;
            painter->setFont(font);
            // scratch
            painter->drawLine(QPointF(x, xv_bottomRulerProperty.markRect.top() + xv_bottomRulerProperty.scratchLineSize),
                              QPointF(x, xv_bottomRulerProperty.markRect.top()));


            // mark
            if(xv_XRuleList->value(i).markType == RulePoint::MT_MARK)
            {
                double markValue = xv_XRuleList->value(i).markValue;
                if(xv_bottomRulerProperty.recalcMarkLabel)
                {
                    markValue = pow(markValue, 2.0)*xv_bottomRulerProperty.K2
                                + markValue*xv_bottomRulerProperty.K1
                                + xv_bottomRulerProperty.K0;
                }

                QString mark = QString::number(markValue, xv_bottomRulerProperty.format, xv_bottomRulerProperty.precision);
                markRect = QRectF(QPointF(0.0, 0.0),
                                 QSizeF(fontMetrics.horizontalAdvance(mark), xv_maxMarkHeight));
                markRect.moveCenter(QPointF(x, xv_bottomRulerProperty.markRect.top()
                                                  + xv_bottomRulerProperty.scratchLineSize
                                                  + xv_bottomRulerProperty.markOffsetFromScratch
                                                  + markRect.height()/2.0));
                painter->drawText(markRect, mark);
            }
        }
    }

    if(xv_bottomRulerProperty.ruleLabelVisibility)
    {
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawText(xv_bottomRulerProperty.ruleLabelRect, Qt::AlignCenter, xv_bottomRulerProperty.ruleLabel);
        //painter->fillRect(xv_topRulerProperty.ruleLabelRect, QBrush(QColor(Qt::blue)));
    }

    painter->restore();
}
//=========================================================
void X_RulerWidget::xh_recalcRuleSizes()
{
    int fontSize = qMax( qMax(xv_leftRulerProperty.fontSize, xv_rightRulerProperty.fontSize),
                        qMax(xv_topRulerProperty.fontSize, xv_bottomRulerProperty.fontSize));
    int precision = qMax( qMax(xv_leftRulerProperty.precision, xv_rightRulerProperty.precision),
                         qMax(xv_topRulerProperty.precision, xv_bottomRulerProperty.precision));

    QFont f = font();
    f.setPixelSize(fontSize);
    QString maxString = QString::number(-5.555555555555E-99, 'f', precision);

    QFontMetrics fm(font());
    xv_maxMarkWidth = fm.horizontalAdvance(maxString);
    xv_maxMarkHeight = fm.height();

    // horizontal offset
    xv_topRulerProperty.markOffsetFromScratch = 1;
    xv_bottomRulerProperty.markOffsetFromScratch = 1;

    f.setPixelSize(xv_leftRulerProperty.fontSize);
    fm = QFontMetrics(font());
    xv_leftRulerProperty.markOffsetFromScratch = fm.horizontalAdvance("8");

    f.setPixelSize(xv_rightRulerProperty.fontSize);
    fm = QFontMetrics(font());
    xv_rightRulerProperty.markOffsetFromScratch = fm.horizontalAdvance("8");

    // mark line sizes
    xv_topRulerProperty.markLineSize = xv_maxMarkHeight * 1.5;
    xv_bottomRulerProperty.markLineSize = xv_maxMarkHeight * 1.5;
    xv_leftRulerProperty.markLineSize = xv_maxMarkWidth + xv_leftRulerProperty.markOffsetFromScratch + xv_leftRulerProperty.scratchLineSize;
    xv_rightRulerProperty.markLineSize = xv_maxMarkWidth  + xv_rightRulerProperty.markOffsetFromScratch + xv_rightRulerProperty.scratchLineSize;

    // adding space for rule name
    if(xv_leftRulerProperty.ruleLabelVisibility)
    {
        xv_leftRulerProperty.markLineSize += xv_maxMarkHeight * 1.5;
    }
    if(xv_rightRulerProperty.ruleLabelVisibility)
    {
        xv_rightRulerProperty.markLineSize += xv_maxMarkHeight * 1.5;
    }
    if(xv_topRulerProperty.ruleLabelVisibility)
    {
        xv_topRulerProperty.markLineSize += xv_maxMarkHeight * 1.5;
    }
    if(xv_bottomRulerProperty.ruleLabelVisibility)
    {
        xv_bottomRulerProperty.markLineSize += xv_maxMarkHeight * 1.5;
    }
}
//=========================================================

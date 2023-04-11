//=========================================================
#ifndef X_RULERWIDGET_H
#define X_RULERWIDGET_H
//=========================================================
#include <QWidget>
#include <QSet>
#include "X_RulersAndGridManager.h"
//=========================================================
class QLabel;
//=========================================================
class X_RulerProperty
{
public:
    X_RulerProperty()
    {
         scratchLineSize = 3;
         markLineSize = 0;
         precision = 6;
         format = 'g';
         fontSize = 10;
         visibility = true;
         scratchRect = QRect();
         markRect = QRect();
         ruleLabelRect = QRect();
         markOffsetFromScratch = 3;
         recalcMarkLabel = true;
         ruleLabelVisibility = true;
         ruleLabel = QString();

         K0 = 0.0;
         K1 = 1.0;
         K2 = 0.0;
    }

    int rulerSize() const
    {
        if(!visibility || scratchLineSize + markLineSize < 1)
        {
            return 1;
        }
        return scratchLineSize + markLineSize;
    }

    int scratchLineSize;
    int markLineSize;
    int precision;
    char format;
    int fontSize;
    bool visibility;
    QRect scratchRect;
    QRect markRect;
    QRect ruleLabelRect;
    int markOffsetFromScratch;
    bool recalcMarkLabel;
    bool ruleLabelVisibility;
    QString ruleLabel;

    // recalc marks factors
    double K0; // free member
    double K1;
    double K2; // square member factor
};
//=========================================================
class X_RulerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit X_RulerWidget(QWidget *parent = 0);

    void xp_setPlotView(QWidget* plot);

    bool xp_setLineWidth(int);
    void xp_setLineColor(QColor);

    void xp_setLeftScratchLineSize(int);
    void xp_setRightScratchLineSize(int);
    void xp_setTopScratchLineSize(int);
    void xp_setBottomScratchLineSize(int);
    void xp_setScratchLineSize(int);

    bool xp_setLeftFontSize(int size);
    bool xp_setRightFontSize(int size);
    bool xp_setTopFontSize(int size);
    bool xp_setBottomFontSize(int size);

    int xp_leftFontSize();
    int xp_rightFontSize();
    int xp_topFontSize();
    int xp_bottomFontSize();

    bool xp_setLeftMarkFormat(char format);
    bool xp_setRightMarkFormat(char format);
    bool xp_setTopMarkFormat(char format);
    bool xp_setBottomMarkFormat(char format);

    bool xp_setLeftMarkPrecision(int precision);
    bool xp_setRightMarkPrecision(int precision);
    bool xp_setTopMarkPrecision(int precision);
    bool xp_setBottomMarkPrecision(int precision);

    void xp_setLeftMarkRecalcMetrix(const QString&, bool labelVisibility,double K0, double K1, double K2);
    void xp_setRightMarkRecalcMetrix(const QString&, bool labelVisibility,double K0, double K1, double K2);
    void xp_setTopMarkRecalcMetrix(const QString&, bool labelVisibility,double K0, double K1, double K2);
    void xp_setBottomMarkRecalcMetrix(const QString&, bool labelVisibility,double K0, double K1, double K2);

    void xp_setLeftMarkRecalcFlag(bool flag);
    void xp_setRightMarkRecalcFlag(bool flag);
    void xp_setTopMarkRecalcFlag(bool flag);
    void xp_setBottomMarkRecalcFlag(bool flag);

    void xp_setHorizontalMarkRecalcFactors(const QString &label, double K0, double K1, double K2);
    void xp_horizontalMarkRecalcFactors(QString& label, double& K0, double& K1, double& K2) const;
    void xp_setVerticalMarkRecalcFactors(const QString &label, double K0, double K1, double K2);

    void xp_setTopRuleVisibility(bool visibility);
    void xp_setBottomRuleVisibility(bool visibility);
    void xp_setLeftRuleVisibility(bool visibility);
    void xp_setRightRuleVisibility(bool visibility);

    void xp_setTopRuleLabel(const QString &label);
    void xp_setBottomRuleLabel(const QString &label);
    void xp_setLeftRuleLabel(const QString &label);
    void xp_setRightRuleLabel(const QString &label);

    void xp_setTopRuleLabelVisible(bool visible);
    void xp_setBottomRuleLabelVisible(bool visible);
    void xp_setLeftRuleLabelVisible(bool visible);
    void xp_setRightRuleLabelVisible(bool visible);

    void xp_setHorizontalRulePointList(QList<RulePoint> *);
    void xp_setVerticalRulePointList(QList<RulePoint> *);

    int xp_maxMarkWidth();
    int xp_maxMarkHeight();

    void xp_setInfoLabelText(const QString& text);
    void xp_setInfoLabelVisible(bool visible);

signals:

public slots:

protected:

    // VARS
    QWidget* xv_plot;
    X_RulerProperty xv_topRulerProperty;
    X_RulerProperty xv_bottomRulerProperty;
    X_RulerProperty xv_leftRulerProperty;
    X_RulerProperty xv_rightRulerProperty;

    QColor xv_lineColor;
    int xv_lineWidth;
    bool xv_dontRepaint;

    int xv_maxMarkHeight;
    int xv_maxMarkWidth;

    QLabel* xv_infoLabel;

    X_RulersAndGridManager* xv_rulersAndGreedManager;

    QList<RulePoint> * xv_XRuleList;
    QList<RulePoint> * xv_YRuleList;
    QSet<char> xv_formatSet;

    // FUNCS
    void resizeEvent(QResizeEvent* event);
    void paintEvent(QPaintEvent* event);

    void xh_paintLeftRule(QPainter* painter);
    void xh_paintRightRule(QPainter* painter);
    void xh_paintTopRule(QPainter* painter);
    void xh_paintBottomRule(QPainter* painter);

    void xh_recalcRuleSizes();
};
//=========================================================
#endif // X_RULERWIDGET_H

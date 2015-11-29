//====================================================
#ifndef ZPLOTTER_H
#define ZPLOTTER_H
//====================================================
#include <QWidget>
//====================================================
class ZMarkLine;
class ZVerticalMarkLine;
class ZScratchLine;
class ZVerticalScratchLine;
class ZPlotGraphicsView;
class ZPlotGraphicsScene;
class QScrollBar;
class ZRulersAndGridManager;
class ZRulerWidget;
class ZDashBoard;
//====================================================
class ZPlotter : public QWidget
{
    Q_OBJECT
public:
    explicit ZPlotter(QWidget *parent = 0);

    ZPlotGraphicsScene* zp_plotScene();

    bool zp_setMinimalVerticalScaleInterval(qreal);
    qreal zp_minimalVerticalScaleInterval();
    bool zp_setMinimalHorizontalScaleInterval(qreal);
    qreal zp_minimalHorizontalScaleInterval();

    bool zp_setLeftMarkFontSize(int size);
    bool zp_setRightMarkFontSize(int size);
    bool zp_setTopMarkFontSize(int size);
    bool zp_setBottomMarkFontSize(int size);

    int zp_leftMarkFontSize();
    int zp_rightMarkFontSize();
    int zp_topMarkFontSize();
    int zp_bottomMarkFontSize();

    bool zp_setLeftMarkFormat(char);
    bool zp_setRightMarkFormat(char);
    bool zp_setTopMarkFormat(char);
    bool zp_setBottomMarkFormat(char);

    bool zp_setLeftMarkPrecision(int);
    bool zp_setRightMarkPrecision(int);
    bool zp_setTopMarkPrecision(int);
    bool zp_setBottomMarkPrecision(int);

signals:

public slots:

    void zp_setLeftRuleVisible(bool visible);
    void zp_setRightRuleVisible(bool visible);
    void zp_setTopRuleVisible(bool visible);
    void zp_setBottomRuleVisible(bool visible);

protected:

    // FUNCS
    void resizeEvent(QResizeEvent* event);
    bool eventFilter(QObject *obj, QEvent *event);

private:

    // VARS
    ZRulersAndGridManager* zv_rulersAndGreedManager;

    ZRulerWidget* zv_rulerWidget;
    ZPlotGraphicsView* zv_plotView;
    ZPlotGraphicsScene* zv_plotScene;
    ZDashBoard* zv_dashBoard;

    // scrollbar
    QScrollBar* zv_verticalScrollBar;
    QScrollBar* zv_horizontalScrollBar;
    bool zv_mouseButtonDown;
    bool zv_userResizesWidget;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_connectScrollBars();
    void zh_updateScrollBarsVisible();

    void zh_scrollBarVisibleControl(int min , int max);

};
//====================================================
#endif // ZPLOTTER_H

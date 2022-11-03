//===========================================================
#ifndef X_DASHBOARD_H
#define X_DASHBOARD_H
//===========================================================
#include <QFrame>
//===========================================================
class QToolButton;
class X_PlotGraphicsView;
class X_PlotGraphicsScene;
class QHBoxLayout;
class QSlider;
//===========================================================
class X_HorizontalDashBoard : public QFrame
{
    Q_OBJECT
public:

    enum AuxWidgetPositon {AWP_LEFT_OF_BUTTONS,
                          AWP_RIGHT_OF_BUTTONS};

    explicit X_HorizontalDashBoard(QWidget *parent = 0);

    void xp_setPlotGraphicsView(X_PlotGraphicsView*);
    void xp_setPlotGraphicsScene(X_PlotGraphicsScene*);
    void xp_appendButtonsToDashboard(QList<QAction*>& actionList,
                                     AuxWidgetPositon position,
                                     Qt::AlignmentFlag alignent,
                                     int extraSperatorSpacing = 0);
    void xp_appendWidgetToDashboard(QWidget* widget,
                                    AuxWidgetPositon position,
                                    Qt::AlignmentFlag alignment,
                                    int extraSeparatorSpacing = 0);
    int xp_distortionValue();
    void xp_setDistortionSliderVisible(bool visible);

signals:

    void xg_distortionFactorChanged(int);

public slots:

protected:
    // FUNCS
    void timerEvent(QTimerEvent* event);

private:

    // VARS
    QHBoxLayout* xv_mainLayout;
    QToolButton* xv_hideGridButton;
    QToolButton* xv_increaseVerticalButton;
    QToolButton* xv_increaseHorizontalButton;
    QToolButton* xv_decreaseVerticalButton;
    QToolButton* xv_decreaseHorizontalButton;
    QToolButton* xv_fitViewInSceneButton;

    QSlider* xv_distortionSlider;

    X_PlotGraphicsView* xv_plotGraphicsView;
    X_PlotGraphicsScene* xv_plotGraphicsScene;
    int xv_timer;
    Qt::Orientation xv_orientation;
    bool xv_increase;
    // FUNCS
    void xh_createComponents();
    void xh_createConnections();
    void xh_startTimer();

private slots:

    void xh_increaseVertical();
    void xh_increaseHorizontal();
    void xh_decreaseVertical();
    void xh_decreaseHorizontal();
    void xh_fitViewInScene() const;
    void xh_hideGrid(bool) const;
    void xh_killTimer();


};
//===========================================================
#endif // X_DASHBOARD_H

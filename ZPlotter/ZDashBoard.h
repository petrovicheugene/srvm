//===========================================================
#ifndef ZDASHBOARD_H
#define ZDASHBOARD_H
//===========================================================
#include <QFrame>
//===========================================================
class QToolButton;
class ZPlotGraphicsView;
//===========================================================
class ZDashBoard : public QFrame
{
    Q_OBJECT
public:
    explicit ZDashBoard(QWidget *parent = 0);

    void zp_setPlotGraphicsView(ZPlotGraphicsView*);
signals:

public slots:

protected:
    // FUNCS
    void timerEvent(QTimerEvent* event);

private:

    // VARS
    QToolButton* zv_hideGridButton;
    QToolButton* zv_increaseVerticalButton;
    QToolButton* zv_increaseHorizontalButton;
    QToolButton* zv_decreaseVerticalButton;
    QToolButton* zv_decreaseHorizontalButton;
    QToolButton* zv_fitViewInSceneButton;

    ZPlotGraphicsView* zv_plotGraphicsView;
    int zv_timer;
    Qt::Orientation zv_orientation;
    bool zv_increase;
    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_startTimer();

private slots:

    void zh_increaseVertical();
    void zh_increaseHorizontal();
    void zh_decreaseVertical();
    void zh_decreaseHorizontal();
    void zh_fitViewinScene();
    void zh_hideGrid(bool);
    void zh_killTimer();
};
//===========================================================
#endif // ZDASHBOARD_H

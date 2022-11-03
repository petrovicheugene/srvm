//======================================================
#ifndef X_ADVANCEDACTION_H
#define X_ADVANCEDACTION_H
//======================================================
#include <QAction>
#include <QEvent>
//======================================================

//======================================================
class X_ControlAction : public QAction
{
    Q_OBJECT
public:
    explicit X_ControlAction(QObject *parent = 0);

signals:
    
    void xg_enableChanged(bool);

protected:

    // VARS
    bool xv_enabled;
    // FUNCS
    bool event(QEvent *event);


};
//======================================================
#endif // X_ADVANCEDACTION_H

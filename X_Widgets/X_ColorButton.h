//===========================================================
#ifndef X_COLORBUTTON_H
#define X_COLORBUTTON_H
//===========================================================
#include <QWidget>
#include <QColor>
//===========================================================

//===========================================================
class X_ColorButton : public QWidget
{
    Q_OBJECT
public:
    explicit X_ColorButton(QWidget *parent = 0);

    void xp_setSizeHint(int);
    void xp_setColor(QColor);
    QColor xp_color() const;

public slots:

    void xp_runColorSelectionDialog();

protected:

    // FUNCS
    void paintEvent(QPaintEvent* event);
    QSize sizeHint() const;
    void mouseReleaseEvent(QMouseEvent *event);


    // VARS
    QSize xv_sizeHint;
    QColor xv_color;

signals:

    void xg_colorChangedNotification();
    void xg_colorChanged(QColor);

public slots:
};
//===========================================================
#endif // X_COLORBUTTON_H

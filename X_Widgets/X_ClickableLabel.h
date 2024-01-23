//=================================================
#ifndef X_CLICKABLELABEL_H
#define X_CLICKABLELABEL_H
//=================================================
#include <QLabel>
//=================================================

//=================================================
class X_ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit X_ClickableLabel(QWidget *parent);


signals:

    void clicked();

protected:

    void mouseReleaseEvent(QMouseEvent*);

};
//=================================================
#endif // X_CLICKABLELABEL_H

//===============================================================
#ifndef X_VISIBILITYPOINTF_H
#define X_VISIBILITYPOINTF_H
//===============================================================
#include <QPointF>
//===============================================================
class X_VisibilityPointF : public QPointF
{
public:
    X_VisibilityPointF();
    X_VisibilityPointF(const QPoint &point, bool visible = true);
    X_VisibilityPointF(qreal xpos, qreal ypos, bool visible = true);

    bool xp_isVisible() const;
    void xp_setVisible(bool visible = true);

private:

    bool xv_visibility;

};
//===============================================================
#endif // X_VISIBILITYPOINTF_H

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
    X_VisibilityPointF(const QPoint &point, bool visible = true, Qt::CheckState checkState = Qt::Checked);
    X_VisibilityPointF(qreal xpos, qreal ypos, bool visible = true, Qt::CheckState checkState = Qt::Checked);
    X_VisibilityPointF(const QPoint &point, bool visible = true, bool checkState = true);
    X_VisibilityPointF(qreal xpos, qreal ypos, bool visible = true, bool checkState = true);

    bool xp_isVisible() const;
    void xp_setVisible(bool visible = true);

    Qt::CheckState xp_isChecked() const;
    void xp_setChecked(Qt::CheckState checkState = Qt::Checked);

private:

    bool xv_visibility = true;
    Qt::CheckState xv_checkState = Qt::Checked;

};
//===============================================================
#endif // X_VISIBILITYPOINTF_H

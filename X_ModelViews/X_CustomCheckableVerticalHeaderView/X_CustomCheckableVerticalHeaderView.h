//=========================================================
#ifndef X_CUSTOMCHECKABLEHEADERVIEW_H
#define X_CUSTOMCHECKABLEHEADERVIEW_H
//=========================================================
#include <QHeaderView>
//=========================================================

//=========================================================
class X_CustomCheckableVerticalHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit X_CustomCheckableVerticalHeaderView(QWidget * parent = 0);


protected:

    // VARS
    int xv_pressedIndex;
    const int xv_labelTextOffset = 3;

    // FUNCS

    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
    QSize sectionSizeFromContents(int logicalIndex) const;
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* e);

    QRect xh_decorationRect(const QRect &sectionRect) const;
    bool xh_isFirstVisibleSection(int section) const;
    bool xh_isLastVisibleSection(int section) const;

signals:

    void xg_userChangesTermState(int section);

public slots:
};
//=========================================================
#endif // X_CUSTOMCHECKABLEHEADERVIEW_H

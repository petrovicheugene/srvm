//=========================================================
#ifndef ZCUSTOMCHECKABLEHEADERVIEW_H
#define ZCUSTOMCHECKABLEHEADERVIEW_H
//=========================================================
#include <QHeaderView>
//=========================================================

//=========================================================
class ZCustomCheckableVerticalHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit ZCustomCheckableVerticalHeaderView(QWidget * parent = 0);


protected:

    // VARS
    int zv_pressedIndex;
    const int zv_labelTextOffset = 3;

    // FUNCS

    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
    QSize sectionSizeFromContents(int logicalIndex) const;
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    QRect zh_decorationRect(const QRect &sectionRect) const;
    bool zh_isFirstVisibleSection(int section) const;
    bool zh_isLastVisibleSection(int section) const;

signals:

    void zg_userChangesTermState(int section);

public slots:
};
//=========================================================
#endif // ZCUSTOMCHECKABLEHEADERVIEW_H

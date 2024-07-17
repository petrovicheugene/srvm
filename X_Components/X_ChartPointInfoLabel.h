//======================================================
#ifndef X_CHARTPOINTINFOLABEL_H
#define X_CHARTPOINTINFOLABEL_H
//======================================================
#include <QLabel>
//======================================================
class X_ChartPointInfoLabel : public QLabel
{
    Q_OBJECT
public:
    explicit X_ChartPointInfoLabel(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    explicit X_ChartPointInfoLabel(const QString &text, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~X_ChartPointInfoLabel();

    void xp_correctBottomRightShift();

protected:
    bool eventFilter(QObject* object, QEvent* event);

signals:



private:
    void xh_initLabel();

};
//======================================================
#endif // X_CHARTPOINTINFOLABEL_H

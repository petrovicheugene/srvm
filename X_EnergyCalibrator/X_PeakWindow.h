//==================================================
#ifndef X_PEAKWINDOW_H
#define X_PEAKWINDOW_H
//==================================================
#include <QObject>
//==================================================

//==================================================
class X_PeakWindow : public QObject
{
    Q_OBJECT
public:
    explicit X_PeakWindow(QObject *parent = 0);

    bool xp_setPeakName(const QString& peakName);
    QString xp_peakName() const;

signals:

public slots:


protected:
    // VARS
    QString xv_peakName;

    // FUNCS

};
//==================================================
#endif // X_PEAKWINDOW_H

//======================================================
#ifndef X_ENERGYCALIBRATIONLINE_H
#define X_ENERGYCALIBRATIONLINE_H
//======================================================
#include <QObject>
#include <QColor>
#include <QString>
//======================================================
class X_EnergyCalibrationLine : public QObject
{
    Q_OBJECT
public:
    explicit X_EnergyCalibrationLine(QObject* parent = 0);

    int xp_channelNumber() const;
    double xp_energyValue() const;
    QString xp_lineName() const;
    QColor xp_lineColor() const;
    bool xp_isEnabled() const;
    void xp_setEnabled(bool enabled);

public slots:

    void xp_setChannelNumber(int channelNumber);
    void xp_setEnergyValue(double energyValue);
    void xp_setEnergyValueString(const QString& value);
    void xp_setLineName(const QString& lineName);
    void xp_setLineColor(QColor color);

signals:

    void xg_channelNumber(int channelNumber);
    void xg_energyValue(double energyValue);
    void xg_lineName(const QString& lineName);
    void xg_lineColor(QColor color);

private:

    // VARS
    int xv_channelNumber;
    double xv_energyValue;
    QString xv_lineName;
    QColor xv_lineColor;
    bool xv_enabled;

};
//======================================================
#endif // X_ENERGYCALIBRATIONLINE_H

//==========================================================
// BASE CLASS FOR ALL SPECTRUM CLASS
#ifndef X_ABSTRACTSPECTRUM_H
#define X_ABSTRACTSPECTRUM_H
//==========================================================
#include "X_AbstractSpectrumAuxData.h"

#include <QObject>
#include <QMetaType>
#include <QString>
#include <QStringList>
#include <QDate>
#include <QTime>
#include <QList>
#include <QMap>
#include <QColor>
#include <QPainterPath>
//==========================================================
class X_CalibrationWindow;
//==========================================================
// Inheritage: ADD  SpectrumType !!!
//==========================================================
class X_AbstractSpectrum : public QObject
{
    Q_OBJECT
public:
    explicit X_AbstractSpectrum(const QList<quint32> &intensityList,
                               const QString& path,
                               QColor color,
                               bool completed,
                               QObject* parent);
    virtual ~X_AbstractSpectrum();

    // VARS
    enum SpectrumType {ST_NA, ST_SPE};

    // FUNCS
    QString xp_name() const;
    void xp_setSpectrumName(const QString& name);
    QString xp_path() const;
    SpectrumType xp_type() const;
    bool xp_isSpectrumVisible() const;
    void xp_setSpectrumVisible(bool);

    bool xp_isSpectrumChecked() const;
    bool xp_setSpectrumChecked(bool);

    void xp_insertConcentration(qint64 chemElementId, const QString& concentration);
    bool xp_removeConcentration(qint64 chemElementId);
    QString xp_concentrationString(qint64 chemElementId) const;
    qreal xp_concentrationValue(qint64 chemElementId) const;
    bool xp_setConcentration(qint64 chemElementId, const QString& concentration);
    QList<quint32> xp_spectrumData() const;
    void xp_setSpectrumData(QList<quint32> data);

    virtual const X_AbstractSpectrumAuxData* xp_auxData() const = 0;
    bool xp_intensityInWindow(int startChannel, int lastChannel, qreal& intensity) const;
    bool xp_intensityInWindow(const X_CalibrationWindow*, qreal& intensity) const;

    int xp_channelCount() const;
    int xp_maxIntensity() const;
    QColor xp_color() const;
    void xp_setColor(QColor);
    qint64 xp_spectrumId() const;
    QPainterPath xp_spectrumPainterPath() const;

    void xp_setCompleted(bool completed);
    bool xp_isCompleted() const;

protected:

    // VARS
    SpectrumType xv_type;
    QString xv_name;
    QString xv_path;
    QList<quint32> xv_spectrumData;

    QPainterPath xv_spectrumPaintPath;
    QMap<qint64, QString> xv_concentrationMap;
    bool xv_visible;
    bool xv_checked;
    QColor xv_color;
    qint64 xv_spectrumId;

    int xv_channelCount;
    quint64 xv_maxIntensity;
    bool xv_completed;

    // FUNCS
    void zh_recalcSpectrumPainterPath();

    // STATIC
    // VARS
    static qint64 xv_lastSpectrumId;

};
//==========================================================
#endif // X_ABSTRACTSPECTRUM_H

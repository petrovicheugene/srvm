//==========================================================
// BASE CLASS FOR ALL SPECTRUM CLASS
#ifndef ZABSTRACTSPECTRUM_H
#define ZABSTRACTSPECTRUM_H
//==========================================================
#include "ZAbstractSpectrumAuxData.h"

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
// Inheritage: ADD  SpectrumType !!!
//==========================================================
class ZAbstractSpectrum : public QObject
{
    Q_OBJECT
public:
    explicit ZAbstractSpectrum(const QList<int> &intensityList, const QString& path, QColor color, QObject* parent);
    virtual ~ZAbstractSpectrum();

    // VARS
    enum SpectrumType {ST_NA, ST_SPE};

    // FUNCS
    QString zp_name() const;
    QString zp_path() const ;
    SpectrumType zp_type() const;
    bool zp_isSpectrumVisible() const;
    void zp_setSpectrumVisible(bool);

    bool zp_isSpectrumChecked() const;
    bool zp_setSpectrumChecked(bool);

    void zp_insertConcentration(qint64 chemElementId, const QString& concentration);
    QString zp_concentration(qint64 chemElementId) const;
    bool zp_setConcentration(qint64 chemElementId, const QString& concentration);
    QList<int> zp_spectrumData() const;
    virtual const ZAbstractSpectrumAuxData* zp_auxData() const = 0;
    int zp_intensityInWindow(int startChannel, int lastChannel, bool *ok = 0) const;

    int zp_channelCount() const;
    int zp_maxIntensity() const;
    QColor zp_color() const;
    void zp_setColor(QColor);
    qint64 zp_spectrumId() const;
    QPainterPath zp_spectrumPainterPath() const;

protected:

    // VARS
    SpectrumType zv_type;
    QString zv_name;
    QString zv_path;
    QList<int> zv_spectrumData;

    QPainterPath zv_spectrumPaintPath;

    QMap<qint64, QString> zv_concentrationMap;
    bool zv_visible;
    bool zv_checked;
    QColor zv_color;
    qint64 zv_spectrumId;

    int zv_channelCount;
    int zv_maxIntensity;

    static qint64 zv_lastSpectrumId;


    // FUNCS
    void zh_recalcSpectrumPainterPath();

};
//==========================================================
#endif // ZABSTRACTSPECTRUM_H

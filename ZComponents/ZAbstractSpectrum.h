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

    void zp_insertConcentration(const QString& chemElement, const QString& concentration);
    QStringList zp_chemElementList() const;
    QString zp_concentration(const QString& chemElement) const;
    bool zp_setConcentration(const QString& chemElement, const QString& concentration);
    QList<int> zp_spectrumData();
    virtual ZAbstractSpectrumAuxData zp_auxData() const = 0;
    int zp_intensityInWindow(int startChannel, int lastChannel, bool *ok = 0);

    int zp_channelCount();
    int zp_maxIntensity();
    QColor zp_color();

protected:

    // VARS
    SpectrumType zv_type;
    QString zv_name;
    QString zv_path;
    QList<int> zv_spectrumData;
    QMap<QString, QString> zv_concentrationMap;
    bool zv_visible;
    QColor zv_color;

    int zv_channelCount;
    int zv_maxIntensity;

};
//==========================================================
#endif // ZABSTRACTSPECTRUM_H

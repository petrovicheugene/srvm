//==========================================================
// BASE CLASS FOR ALL SPECTRUM CLASS
#ifndef ZABSTRACTSPECTRUM_H
#define ZABSTRACTSPECTRUM_H
//==========================================================
#include <QObject>
#include <QMetaType>
#include <QString>
#include <QStringList>
#include <QDate>
#include <QTime>
#include <QList>
#include <QMap>
#include "ZAbstractSpectrumAuxData.h"
//==========================================================
// Inheritage: ADD  SpectrumType !!!
//==========================================================
class ZAbstractSpectrum : public QObject
{
    Q_OBJECT
public:
    explicit ZAbstractSpectrum(const QList<int> &intensityList, const QString& path, QObject* parent);
    virtual ~ZAbstractSpectrum();

    // VARS
    enum SpectrumType {ST_NA, ST_SPE};

    // FUNCS
    QString zp_name() const;
    QString zp_path() const ;
    SpectrumType zp_type() const;
    void zp_insertConcentration(const QString& chemElement, const QString& concentration);
    QStringList zp_chemElementList() const;
    QString zp_concentration(const QString& chemElement) const;
    bool zp_setConcentration(const QString& chemElement, const QString& concentration);
    QList<int> zp_spectrumIntensityArray();
    virtual ZAbstractSpectrumAuxData zp_auxData() const = 0;


protected:

    // VARS
    SpectrumType zv_type;
    QString zv_name;
    QString zv_path;
    QList<int> zv_intensityList;
    QMap<QString, QString> zv_concentrationMap;

};
//==========================================================
#endif // ZABSTRACTSPECTRUM_H

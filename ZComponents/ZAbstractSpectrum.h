//==========================================================
// BASE CLASS FOR ALL SPECTRUM CLASS
#ifndef ZABSTRACTSPECTRUM_H
#define ZABSTRACTSPECTRUM_H
//==========================================================
#include <QMetaType>
#include <QString>
#include <QDate>
#include <QTime>
#include <QList>
#include "ZAbstractSpectrumAuxData.h"

//==========================================================
class ZAbstractSpectrum
{
public:
    ZAbstractSpectrum();
    ZAbstractSpectrum(const QString& name);
    virtual ~ZAbstractSpectrum();

    // VARS
    enum SpectrumType {ST_NA, ST_SPE};

    // FUNCS
    QString zp_name();
    SpectrumType zp_type();
    // virtual ZAbstractSpectrumAuxData zp_auxData() = 0;

protected:

    // VARS
    SpectrumType zv_type;
    QString zv_name;
    QList<double> zv_intensityList;

};
//==========================================================
#endif // ZABSTRACTSPECTRUM_H

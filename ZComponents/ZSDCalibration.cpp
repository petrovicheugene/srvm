//=================================================
#include "ZSDCalibration.h"
#include "ZAbstractSpectrum.h"
//=================================================
ZSDCalibration::ZSDCalibration(const QString& path, QObject *parent)
    : ZAbstractCalibration(path, parent)
{

}
//=================================================
ZSDCalibration::~ZSDCalibration()
{

}
//=================================================
double ZSDCalibration::zp_calcConcentration(ZAbstractSpectrum* const spectrum, bool *ok)
{
    if(spectrum == 0)
    {
        if(ok != 0)
        {
            *ok = false;
            return 0;
        }
    }

    double fullIntensity = static_cast<double>(spectrum->zp_intensityInWindow(0, 2048, ok));

    return fullIntensity;
}
//=================================================

//==========================================================
#include "ZXMLCalibration.h"
//==========================================================
ZXMLCalibration::ZXMLCalibration(const QString& path, QObject *parent)
    : ZAbstractCalibration(path, parent)
{

}
//==========================================================
ZXMLCalibration::~ZXMLCalibration()
{

}
//==========================================================
double ZXMLCalibration::zp_calcConcentration(ZAbstractSpectrum* const spectrum, bool *ok)
{
    if(spectrum == 0)
    {
        if(ok != 0)
        {
            *ok = false;
            return 0;
        }
    }

    * ok = true;
    return 0;
}
//==========================================================


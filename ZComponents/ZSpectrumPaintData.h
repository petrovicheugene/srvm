//==========================================================
#ifndef ZSPECTRUMPAINTDATA
#define ZSPECTRUMPAINTDATA
//==========================================================
#include <QVector>
#include <QPointF>
#include <QMetaType>
//==========================================================
struct ZSpectrumPaintData
{
    QList<int> spectrumData;
    int maxChannel;
    int maxIntensity;

};
Q_DECLARE_METATYPE(ZSpectrumPaintData)
//==========================================================
#endif // ZSPECTRUMPAINTDATA


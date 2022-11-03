//==========================================================
#ifndef X_SPECTRUMPAINTDATA
#define X_SPECTRUMPAINTDATA
//==========================================================
#include <QVector>
#include <QPointF>
#include <QMetaType>
#include <QColor>
//==========================================================
struct X_SpectrumPaintData
{
    QList<quint32> spectrumData;
    int maxChannel;
    int maxIntensity;
    QColor color;

};
Q_DECLARE_METATYPE(X_SpectrumPaintData)
//==========================================================
#endif // X_SPECTRUMPAINTDATA


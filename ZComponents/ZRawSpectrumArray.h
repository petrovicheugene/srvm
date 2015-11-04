//========================================================
#ifndef ZRAWARRAY
#define ZRAWARRAY
//========================================================
#include <QStringList>
#include <QMap>
//========================================================
struct ZRawSpectrum
{
    ZRawSpectrum()
    {
        path = QString();
        concentrationMap = QMap<QString, QString>();
    }

    QString path;
    QMap<QString, QString> concentrationMap; // QMap<Chem element, Concentration value>
};

//========================================================
struct ZRawSpectrumArray
{
    ZRawSpectrumArray()
    {
        name = QString();
        spectrumList = QList<ZRawSpectrum>();
    }

    QString name;
    QList<ZRawSpectrum> spectrumList;
};
//========================================================
#endif // ZRAWARRAY


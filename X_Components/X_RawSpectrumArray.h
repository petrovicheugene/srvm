//========================================================
#ifndef X_RAWARRAY
#define X_RAWARRAY
//========================================================
#include <QStringList>
#include <QMap>
//========================================================
struct X_RawSpectrum
{
    X_RawSpectrum()
    {
        path = QString();
        concentrationMap = QMap<QString, QString>();
        checked = true;
        visible = true;
    }

    QString path;
    QMap<QString, QString> concentrationMap; // QMap<Chem element, Concentration value>
    bool checked;
    bool visible;
};

//========================================================
struct X_RawSpectrumArray
{
    X_RawSpectrumArray()
    {
        name = QString();
        gainFactor = 0;
        spectrumList = QList<X_RawSpectrum>();
    }

    QString name;
    int gainFactor;
    QList<X_RawSpectrum> spectrumList;
};
//========================================================
#endif // X_RAWARRAY


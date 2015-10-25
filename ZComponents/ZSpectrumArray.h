//===============================================
#ifndef ZSPECTRUMARRAY_H
#define ZSPECTRUMARRAY_H
//===============================================
#include <QObject>
#include <QList>
#include <QString>
#include "ZAbstractSpectrum.h"
#include "ZRawArray.h"
//===============================================
class ZSpectrumArray : public QObject
{
    Q_OBJECT
public:

    //explicit ZSpectrumArray(QObject* parent);
    explicit ZSpectrumArray(const QString&, QObject* parent);
    QString zp_arrayName() const;
    void zp_setArrayName(const QString&);

    int zp_spectrumCount() const;
    QString zp_spectrumFileName(int index) const;
    QList<int> zp_spectrumData(int index) const;
    bool zp_removeSpectrum(int);
    void zp_clearArray();
    // bool zp_setSpectrumAtIndex(int index, const ZAbstractSpectrum& spectrum);
    // bool zp_applySpectrum(const QString &fileName, QString &error);
    // bool zp_appendSpectrum(const QString& fileName);
    bool  zp_appendSpectrum(const ZRawSpectrum&);

signals:

    void zg_message(QString) const;

private:

    // VARS
    QString zv_arrayName;
    QList<ZAbstractSpectrum*> zv_spectrumList;

};
//===============================================
#endif // ZSPECTRUMARRAY_H

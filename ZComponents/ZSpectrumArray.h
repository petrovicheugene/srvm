//===============================================
#ifndef ZSPECTRUMARRAY_H
#define ZSPECTRUMARRAY_H
//===============================================
#include "ZAbstractSpectrum.h"
#include <QList>
#include <QString>
//===============================================
class ZSpectrumArray
{
public:

    ZSpectrumArray();
    ~ZSpectrumArray();


    QString zp_arrayName() const;
    void zp_setArrayName(const QString&);

    int zp_spectrumCount() const;
    QString zp_spectrumFileName(int index) const;
    bool zp_removeSpectrum(int);
    //bool zp_setSpectrumAtIndex(int index, const ZAbstractSpectrum& spectrum);
    // bool zp_applySpectrum(const QString &fileName, QString &error);
    // bool zp_appendSpectrum(const QString& fileName);
    void zp_appendSpectrum(ZAbstractSpectrum*);


private:

    // copy and assignment not allowed
    ZSpectrumArray( const ZSpectrumArray& ) = delete;
    void operator=( const ZSpectrumArray& ) = delete;

    // VARS
    QString zv_arrayName;
    QList<ZAbstractSpectrum*> zv_spectrumList;

};
//===============================================
#endif // ZSPECTRUMARRAY_H

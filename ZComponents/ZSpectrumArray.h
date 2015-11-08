//===============================================
#ifndef ZSPECTRUMARRAY_H
#define ZSPECTRUMARRAY_H
//===============================================
#include <QObject>
#include <QList>
#include <QString>
#include "ZAbstractSpectrum.h"
#include "ZRawSpectrumArray.h"
#include "ZChemElementList.h"

//===============================================
class ZSpectrumArray : public QObject
{
    Q_OBJECT
public:

    explicit ZSpectrumArray(const QString&, QObject* parent);
    QString zp_arrayName() const;
    void zp_setArrayName(const QString&);
    ZRawSpectrumArray zp_createRawSpectrumArray();
    int zp_spectrumCount() const;

    int zp_chemElementCount() const;
    int zp_chemElementIndex(const QString& chemElement) const;
    int zp_numberVisibleChemElementsBeforeIndex(int index) const;
    int zp_visibleChemElementCount() const;
    QStringList zp_chemElementList() const;
    QString zp_visibleChemElementName(int visibleIndex) const;
    QString zp_chemElementName(int index) const;
    bool zp_chemElementIsVisible(int index) const;
    bool zp_setChemElementVisible(int row, bool visible);
    bool zp_setChemElementName(int row, const QString&);

    QString zp_chemConcentration(const QString& chemElement,
                                 int spectrumIndex) const;
    bool zp_setChemConcentration(const QString& chemElement,
                                 int spectrumIndex, const QString& concentration);

    QString zp_spectrumFileName(int index) const;
    QList<int> zp_spectrumIntensityArray(int index) const;
    bool zp_removeSpectrum(int);
    void zp_clearArray();
    bool zp_appendSpectrum(const ZRawSpectrum& rawSpectrum);
    bool zp_appendNewChemElement();
    bool zp_removeChemElement(int);

signals:

    void zg_message(QString) const;
    void zg_chemElementOperation(ZChemElementList::OperationType, int, int) const;

private:

    // VARS
    QString zv_arrayName;
    QList<ZAbstractSpectrum*> zv_spectrumList;
    ZChemElementList zv_chemElementList;

    // FUNCS

    void zh_createConnections();

private slots:

};
//===============================================
#endif // ZSPECTRUMARRAY_H

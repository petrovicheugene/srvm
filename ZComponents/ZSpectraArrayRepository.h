//==================================================================
#ifndef ZSPECTRAARRAYREPOSITORY_H
#define ZSPECTRAARRAYREPOSITORY_H
//==================================================================
#include <QObject>
#include <QList>
#include "ZRawArray.h"
#include "ZSpectrumArray.h"
//==================================================================
class ZSpectraArrayRepository : public QObject
{
    Q_OBJECT
public:
    explicit ZSpectraArrayRepository(QObject *parent = 0);

    // VARS
    enum OperationType {OT_INSERT_ARRAYS,
                        OT_END_INSERT_ARRAYS,
                        OT_REMOVE_ARRAYS,
                        OT_END_REMOVE_ARRAYS
                       };

    // FUNCS
    bool zp_isEmpty();
    void zp_clear();

    int zp_arrayCount() const;
    int zp_spectrumCount(int arrayIndex) const;
    QString zp_arrayName(int arrayIndex) const;
    QString zp_spectrumName(int arrayIndex, int spectrumIndex) const;

    QList<int> zp_spectrum(int arrayIndex, int spectrumIndex) const;

signals:

    void zg_message(QString) const;
    void zg_currentOperation(OperationType, int, int) const;
    void zg_currentFile(bool dirty, QString fileName) const;

public slots:

    void zp_appendArrays(QString path, QList<ZRawArray>);
    void zp_createArray(const ZRawArray&);
    void zp_getArrayCount(int&) const;
    void zp_getSpectrumCount(int arrayIndex, int&) const;
    void zp_getArrayName(int arrayIndex, QString&) const;
    void zp_getSpectrumName(int arrayIndex, int spectrumIndex, QString&) const;

private:

    // VARS
    QList<ZSpectrumArray*> zv_arrayList;
    QString zv_path;
    bool zv_dirty;
    // FUNCS

    // void zp_saveArrayListToFile();

};
//==================================================================
#endif // ZSPECTRAARRAYREPOSITORY_H

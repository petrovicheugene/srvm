//==================================================================
#ifndef ZSPECTRAARRAYREPOSITORY_H
#define ZSPECTRAARRAYREPOSITORY_H
//==================================================================
#include <QObject>
#include <QList>
#include <QMenu>
#include "ZRawArray.h"
#include "ZSpectrumArray.h"

//==================================================================
class QAction;
//==================================================================
class ZSpectraArrayRepository : public QObject
{
    Q_OBJECT
public:
    explicit ZSpectraArrayRepository(QObject *parent = 0);

    // VARS
    enum ArrayOperationType {AOT_INSERT_ARRAYS,
                             AOT_END_INSERT_ARRAYS,
                             AOT_REMOVE_ARRAYS,
                             AOT_END_REMOVE_ARRAYS
                            };

    enum SpectrumOperationType {SOT_INSERT_SPECTRA,
                                SOT_END_INSERT_SPECTRA,
                                SOT_REMOVE_SPECTRA,
                                SOT_END_REMOVE_SPECTRA
                               };

    // FUNCS
    void zp_appendActionsToMenu(QMenu *menu) const;
    QList<QAction*> zp_arrayActions() const;
    QList<QAction*>zp_spectrumActions() const;

    bool zp_isEmpty() const;
    void zp_clear();

    int zp_arrayCount() const;
    int zp_spectrumCount(int arrayIndex) const;
    QString zp_arrayName(int arrayIndex) const;
    QString zp_spectrumName(int arrayIndex, int spectrumIndex) const;

    QList<int> zp_spectrum(int arrayIndex, int spectrumIndex) const;

signals:

    void zg_message(QString) const;
    void zg_currentArrayOperation(ArrayOperationType, int, int) const;
    void zg_currentSpectrumOperation(SpectrumOperationType, int, int, int) const;
    void zg_currentFile(bool dirty, QString fileName) const;

    void zg_setCurrentArrayIndex(int arrayIndex);
    void zg_requestCurrentArrayIndex(int& arrayIndex);

    void zg_initSpectraAppending(int);

public slots:

    void zp_appendArrays(QString path, QList<ZRawArray>);
    void zp_appendSpectraToArray(int, QStringList);
    void zp_getArrayCount(int&) const;
    void zp_getSpectrumCount(int arrayIndex, int&) const;
    void zp_getArrayName(int arrayIndex, QString&) const;
    void zp_getSpectrumName(int arrayIndex, int spectrumIndex, QString&) const;

private slots:

    void zh_onAppendArrayAction();
    void zh_onRemoveArrayAction();
    void zh_onAppendSpectraToArrayAction();
    void zh_onRemoveSpectrumFromArrayAction();


private:

    // VARS
    QList<ZSpectrumArray*> zv_arrayList;
    QString zv_path;
    bool zv_dirty;

    QAction* zv_appendArrayAction;
    QAction* zv_removeArrayAction;
    QAction* zv_appendSpectrumToArrayAction;
    QAction* zv_removeSpectrumFromArrayAction;

    // FUNCS
    void zh_createActions();
    void zh_createConnections();
    void zh_createArray(const ZRawArray&);
    void zh_removeArray(int);
    //  void zp_saveArrayListToFile();

};
//==================================================================
#endif // ZSPECTRAARRAYREPOSITORY_H

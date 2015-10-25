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
    enum OperationType {OT_INSERT_ARRAYS,
                        OT_END_INSERT_ARRAYS,
                        OT_REMOVE_ARRAYS,
                        OT_END_REMOVE_ARRAYS
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
    void zg_currentOperation(OperationType, int, int) const;
    void zg_currentFile(bool dirty, QString fileName) const;

    void zg_setCurrentArrayIndex(int arrayIndex);
    void zg_requestCurrentArrayIndex(int& arrayIndex);

public slots:

    void zp_appendArrays(QString path, QList<ZRawArray>);
    void zp_getArrayCount(int&) const;
    void zp_getSpectrumCount(int arrayIndex, int&) const;
    void zp_getArrayName(int arrayIndex, QString&) const;
    void zp_getSpectrumName(int arrayIndex, int spectrumIndex, QString&) const;

private slots:

    void zh_onAppendArrayAction();
    void zh_onRemoveArrayAction();
    void zh_onAppendSpectrumToArrayAction();
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
    void zp_createArray(const ZRawArray&);
    void zh_removeArray(int);
    //  void zp_saveArrayListToFile();

};
//==================================================================
#endif // ZSPECTRAARRAYREPOSITORY_H

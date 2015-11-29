//===========================================================
#ifndef ZPLOTTERDATAMANAGER_H
#define ZPLOTTERDATAMANAGER_H
//===========================================================
#include <QObject>
#include <QGraphicsItem>
#include "ZSpectraArrayRepository.h"
//===========================================================

//===========================================================
class ZPlotterDataManager : public QObject
{
    Q_OBJECT
public:
    explicit ZPlotterDataManager(QObject *parent = 0);

    // FUNCS
    void zp_setSpectraArrayRepository(ZSpectraArrayRepository*);

signals:

public slots:

    void zp_currentArrayChanged(int current, int previous);

private:

    // VARS
    int zv_currentArrayIndex;
    ZSpectraArrayRepository* zv_spectrumArrayRepositiry;
    QList<QGraphicsItem*> zv_spectrumGraphicsItemList;

    // FUNCS

private slots:

    void zh_onRepositoryArrayOperation(ZSpectraArrayRepository::SpectrumOperationType type,
                                  int arrayIndex, int first, int last);
};
//===========================================================
#endif // ZPLOTTERDATAMANAGER_H

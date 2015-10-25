//==================================================================
#ifndef ZSPECTRAJOINTDATAMANAGER_H
#define ZSPECTRAJOINTDATAMANAGER_H
//==================================================================
#include <QObject>
#include <QMOdelIndex>
#include "ZSpectraArrayRepository.h"
//==================================================================
class ZJointSpectraDataManager : public QObject
{
    Q_OBJECT
public:
    explicit ZJointSpectraDataManager(QObject *parent = 0);

    enum OperationType {OT_RESET_DATA,
                        OT_END_RESET_DATA,
                       OT_INSERT_ROW,
                       OT_END_INSERT_ROW,
                       OT_REMOVE_ROW,
                       OT_END_REMOVE_ROW};
    // FUNCS
    void zp_setSpectraArrayRepository(ZSpectraArrayRepository*);

    int zp_rowCount() const;
    int zp_columnCount() const;
    QVariant zp_data(QModelIndex index) const;
    QString zp_columnName(int) const;

signals:

    void zg_currentOperation(OperationType, int, int) const;

public slots:

    void zp_currentArrayChanged(int, int);

private slots:

    void zh_onRepositoryOperation(ZSpectraArrayRepository::SpectrumOperationType,
                                  int arrayIndex, int first, int last);

private:

    // VARS
    int zv_currentArrayIndex;
    ZSpectraArrayRepository* zv_repositiry;
    QString zv_currentChemElement;
};
//==================================================================
#endif // ZSPECTRAJOINTDATAMANAGER_H

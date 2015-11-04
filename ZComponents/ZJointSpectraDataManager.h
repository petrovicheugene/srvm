//==================================================================
#ifndef ZSPECTRAJOINTDATAMANAGER_H
#define ZSPECTRAJOINTDATAMANAGER_H
//==================================================================
#include <QObject>
#include <QModelIndex>
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
                        OT_END_REMOVE_ROW,
                        OT_INSERT_COLUMN,
                        OT_END_INSERT_COLUMN,
                        OT_REMOVE_COLUMN,
                        OT_END_REMOVE_COLUMN};
    // FUNCS
    void zp_setSpectraArrayRepository(ZSpectraArrayRepository*);

    int zp_rowCount() const;
    int zp_columnCount() const;
    int zp_visibleChemElementCount() const;
    bool zp_isColumnChemElementColumn(int column) const;
    QVariant zp_data(QModelIndex index) const;
    QString zp_columnName(int) const;

    bool zp_setChemConcentration(int row, int column, const QString& consentration);

signals:

    void zg_currentOperation(OperationType, int, int) const;

public slots:

    void zp_currentArrayChanged(int, int);

private slots:

    void zh_onRepositoryArrayOperation(ZSpectraArrayRepository::SpectrumOperationType,
                                  int arrayIndex, int first, int last);

    void zh_onRepositoryChemElementOperation(ZSpectraArrayRepository::ChemElementOperationType,
                                  int arrayIndex, int first, int last);


private:

    // VARS
    const int zv_spectrumDataColumnCount = 2;
    int zv_currentArrayIndex;
    ZSpectraArrayRepository* zv_repositiry;
    QString zv_currentChemElement;


    // FUNCS
    int zh_convertChemElementIndexToVisibleChemElementIndex(int arrayIndex, int originalIndex);

};

//==================================================================
#endif // ZSPECTRAJOINTDATAMANAGER_H

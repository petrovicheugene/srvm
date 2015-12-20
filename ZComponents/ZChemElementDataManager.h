//=========================================================
#ifndef ZJOINTCHEMELEMENTDATAMANAGER_H
#define ZJOINTCHEMELEMENTDATAMANAGER_H
//=========================================================
#include <QObject>
#include "ZSpectraArrayRepository.h"
#include <QModelIndex>
//=========================================================

//=========================================================
class ZChemElementDataManager : public QObject
{
    Q_OBJECT
public:
    explicit ZChemElementDataManager(QObject *parent = 0);

    enum OperationType {OT_RESET_DATA,
                        OT_END_RESET_DATA,
                        OT_INSERT_ROW,
                        OT_END_INSERT_ROW,
                        OT_REMOVE_ROW,
                        OT_END_REMOVE_ROW,
                        OT_INSERT_COLUMN,
                        OT_END_INSERT_COLUMN,
                        OT_REMOVE_COLUMN,
                        OT_END_REMOVE_COLUMN,
                       OT_DATA_CHANGED};
    // FUNCS

    void zp_connectToSpectraArrayRepository(ZSpectrumArrayRepository*);

    int zp_rowCount() const;
    int zp_columnCount() const;
    QString zp_chemElementName(int row) const;
    bool zp_chemElementIsVisible(int row) const;

    QString zp_columnName(int) const;

    bool zp_setVisible(int row, bool visible) const;
    bool zp_setChemElementName(int chemElementIndex, const QString& chemElementName) const;

signals:

    void zg_currentOperation(OperationType, int, int) const;

public slots:

    void zp_currentArrayChanged(qint64, int);

private:

    // VARS
    qint64 zv_currentArrayId;
    int zv_currentArrayIndex;
    ZSpectrumArrayRepository* zv_repositiry;

private slots:

    void zh_onRepositoryChemElementOperation(ZSpectrumArrayRepository::ChemElementOperationType,
                                             int arrayIndex, int first, int last);


};
//=========================================================
#endif // ZJOINTCHEMELEMENTDATAMANAGER_H

//=========================================================
#ifndef X_JOINTCHEMELEMENTDATAMANAGER_H
#define X_JOINTCHEMELEMENTDATAMANAGER_H
//=========================================================
#include <QObject>
#include "X_SpectrumArrayRepository.h"
#include <QModelIndex>
//=========================================================

//=========================================================
class X_ChemElementDataManager : public QObject
{
    Q_OBJECT
public:
    explicit X_ChemElementDataManager(QObject *parent = 0);

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

    void xp_connectToSpectraArrayRepository(X_SpectrumArrayRepository*);

    int xp_rowCount() const;
    int xp_columnCount() const;
    QString xp_chemElementName(int row) const;
    bool xp_chemElementIsVisible(int row) const;

    QString xp_columnName(int) const;

    bool xp_setVisible(int row, bool visible) const;
    bool xp_setChemElementName(int chemElementIndex, const QString& chemElementName) const;

signals:

    void xg_currentOperation(OperationType, int, int) const;

public slots:

    void xp_currentArrayChanged(qint64, int);

private:

    // VARS
    qint64 xv_currentArrayId;
    int xv_currentArrayIndex;
    X_SpectrumArrayRepository* xv_repositiry;

private slots:

    void xh_onRepositoryChemElementOperation(X_SpectrumArrayRepository::ChemElementOperationType,
                                             int arrayIndex, int first, int last);


};
//=========================================================
#endif // X_JOINTCHEMELEMENTDATAMANAGER_H

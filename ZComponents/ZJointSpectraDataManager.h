//==================================================================
#ifndef ZSPECTRAJOINTDATAMANAGER_H
#define ZSPECTRAJOINTDATAMANAGER_H
//==================================================================
#include <QObject>
#include <QModelIndex>
#include <QMap>
#include <QList>
#include "ZSpectraArrayRepository.h"
#include "ZCalibrationRepository.h"
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
                        OT_END_REMOVE_COLUMN,
                        OT_COLUMN_HEADER_CHANGED,
                        OT_SEPECTRUM_DATA_CHANGED};
    // FUNCS
    void zp_connectToSpectraArrayRepository(ZSpectrumArrayRepository*);
    void zp_connectToCalibrationRepository(ZCalibrationRepository*);

    int zp_rowCount() const;
    int zp_columnCount() const;
    int zp_visibleChemElementCount() const;
    bool zp_isColumnChemElement(int column) const;
    QVariant zp_data(QModelIndex index) const;
    QString zp_columnName(int) const;
    QColor zp_spectrumColor(int) const;
    bool zp_isSpectrumVisible(int) const;
    bool zp_setSpectrumVisible(int, bool);
    bool zp_setChemConcentration(int row, int column, const QString& consentration);
    int zp_spectrumDataColumnCount();

signals:

    void zg_currentOperation(OperationType, int, int) const;

public slots:

    void zp_currentArrayChanged(qint64, int);

private slots:

    void zh_onRepositoryArrayOperation(ZSpectrumArrayRepository::SpectrumOperationType,
                                       int arrayIndex, int first, int last);

    void zh_onRepositoryChemElementOperation(ZSpectrumArrayRepository::ChemElementOperationType,
                                             int arrayIndex, int first, int last);
    void zh_onRepositoryCalibrationOperation(ZCalibrationRepository::OperationType, int, int);

private:

    // VARS
    const int zv_spectrumDataColumnCount = 2;
    char zv_concentrationFormat;
    int zv_concentrationPrecision;

    int zv_currentArrayIndex;
    qint64 zv_currentArrayId;
    ZSpectrumArrayRepository* zv_spectrumArrayRepositiry;
    ZCalibrationRepository* zv_calibrationRepository;
    QString zv_currentChemElement;

    int zv_visibleChemElementCount;
    int zv_visibleCalibrationCount;
    QMap<QString, QStringList> zv_calculatedConcentrationMap; // QMap<CalibName, QPair<specIndex, Concentration>>

    // FUNCS
    int zh_convertChemElementIndexToVisibleChemElementIndex(int arrayIndex, int originalIndex);
    void zh_defineColumnCounts();
    void zh_calculateConcentrationsForCalibration();

};
//==================================================================
#endif // ZSPECTRAJOINTDATAMANAGER_H

//==================================================================
#ifndef ZPREDICTORTABLEMANAGER_H
#define ZPREDICTORTABLEMANAGER_H
//==================================================================
#include <QObject>
#include <QModelIndex>

#include "ZCalibrationRepository.h"

//==================================================================

//==================================================================
class ZPredictorTableManager : public QObject
{
   Q_OBJECT
public:
   explicit ZPredictorTableManager(QObject *parent = 0);

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
   void zp_connectToCalibrationRepository(ZCalibrationRepository*);


   int zp_rowCount() const;
   int zp_columnCount() const;

   QVariant zp_data(QModelIndex index) const;
   QString zp_columnName(Qt::Orientation orientation, int) const;


signals:

   void zg_currentOperation(OperationType, int, int) const;

public slots:


private slots:

   void zh_currentCalibrationChanged(qreal calibrationId, int calibrationIndex);
   void zh_onRepositoryPredictorOperation(ZCalibrationRepository::PredictorOperationType, int, int, int);

private:

    // VARS
    ZCalibrationRepository* zv_calibrationRepository;
    qint64 zv_currentCalibrationId;

    qreal zv_averageChemConcentration;

    // FUNCS



};
//==================================================================
#endif // ZPREDICTORTABLEMANAGER_H

//==================================================================
#ifndef ZPREDICTORTABLEMANAGER_H
#define ZPREDICTORTABLEMANAGER_H
//==================================================================
#include <QObject>
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

signals:

   void zg_currentOperation(OperationType, int, int) const;

public slots:


private slots:

   void zh_currentCalibrationChanged(qreal calibrationId, int calibrationIndex);


private:

    // VARS
    ZCalibrationRepository* zv_calibrationRepository;
    qint64 zv_currentCalibrationId;





};
//==================================================================
#endif // ZPREDICTORTABLEMANAGER_H

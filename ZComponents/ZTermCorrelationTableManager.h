//==================================================================
#ifndef ZTERMTABLEMANAGER_H
#define ZTERMTABLEMANAGER_H
//==================================================================
#include <QObject>
#include <QModelIndex>

#include "ZCalibrationRepository.h"

//==================================================================

//==================================================================
class ZTermCorrelationTableManager : public QObject
{
   Q_OBJECT
public:
   explicit ZTermCorrelationTableManager(QObject *parent = 0);

   enum OperationType {TOT_BEGIN_RESET,
                       TOT_END_RESET,
                       TOT_BEGIN_INSERT_ROW,
                       TOT_END_INSERT_ROW,
                       TOT_BEGIN_REMOVE_ROW,
                       TOT_END_REMOVE_ROW,
                       TOT_BEGIN_INSERT_COLUMN,
                       TOT_END_INSERT_COLUMN,
                       TOT_BEGIN_REMOVE_COLUMN,
                       TOT_END_REMOVE_COLUMN,
                       TOT_DATA_CHANGED};

   // FUNCS
   void zp_connectToCalibrationRepository(ZCalibrationRepository*);


   int zp_rowCount() const;
   int zp_columnCount() const;

   QVariant zp_data(QModelIndex index) const;
   QString zp_columnName(int) const;


signals:

   void zg_currentOperation(OperationType, int, int) const;

public slots:


private slots:

   void zh_currentCalibrationChanged(qreal calibrationId, int calibrationIndex);
   void zh_onRepositoryTermOperation(ZCalibrationRepository::TermOperationType, int, int, int);

private:

    // VARS
    ZCalibrationRepository* zv_calibrationRepository;
    qint64 zv_currentCalibrationId;
    qreal zv_averageChemConcentration;
    const int zv_firstNonTermColumnCount = 2;
    const QString zv_defaultChemElementString = tr("No chemical");
    int zv_columnCountCorrector;
    // FUNCS



};
//==================================================================
#endif // ZTERMTABLEMANAGER_H

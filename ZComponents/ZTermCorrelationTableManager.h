//==================================================================
#ifndef ZTERMTABLEMANAGER_H
#define ZTERMTABLEMANAGER_H
//==================================================================
#include <QObject>
#include <QModelIndex>
#include <QVector2D>

#include "ZCalibrationRepository.h"
#include "ZSpectrumArrayRepository.h"
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
                        TOT_DATA_CHANGED,
                        TOT_HORIZONTAL_HEADER_CHANGED,
                        TOT_VERTICAL_HEADER_CHANGED
                       };

    // FUNCS
    void zp_connectToCalibrationRepository(ZCalibrationRepository*);
    void zp_connectToSpectrumArrayRepository(ZSpectrumArrayRepository*);


    int zp_rowCount() const;
    int zp_columnCount() const;

    QVariant zp_data(QModelIndex index) const;
    bool zp_setData(QModelIndex index, QVariant vFactor);
    QVariant zp_cellColor(QModelIndex index) const;
    QString zp_horizontalColumnName(int) const;
    QString zp_verticalColumnName(int) const;

    QPixmap zp_termStateIcon(int) const;
    void zp_setNextUsersTermState(int);

signals:

    void zg_currentOperation(OperationType, int, int) const;

public slots:


private slots:


    void zh_currentSpectrumArrayChanged(qint64, int);
    void zh_onRepositoryChemElementOperation(ZSpectrumArrayRepository::ChemElementOperationType type,
                                             int arrayIndex, int first, int last);
    void zh_currentCalibrationChanged(qreal calibrationId, int calibrationIndex);
    void zh_onRepositoryTermOperation(ZCalibrationRepository::TermOperationType, int, int, int);
    void zh_onCalibrationRepositoryOperation(ZCalibrationRepository::CalibrationOperationType, int, int);
    void zh_onSpectrumOperation(ZSpectrumArrayRepository::SpectrumOperationType, int, int, int);
    void zh_onCalibrationNormalizerChange(qint64 calibrationId);

private:

    // VARS
    ZCalibrationRepository* zv_calibrationRepository;
    ZSpectrumArrayRepository* zv_spectrumArrayRepository;
    qint64 zv_currentCalibrationId;
    qint64 zv_currentArrayId;
    int zv_currentArrayIndex;

    QStringList zv_chemElementCorrelationList;
    QMap<QPair<int, int>, QString> zv_termIntercorrelationMap;

    // QList< QPair<qreal, QList<qreal> > >chemConcentrationAndTermValueList;

    const int zv_firstNonTermColumnCount = 2;
    const QString zv_defaultChemElementString = tr("No Element");
    int zv_columnCountCorrector;

    QColor zv_greenCell;
    QColor zv_orangeCell;
    QColor zv_yellowCell;
    QColor zv_redCell;

    // FUNCS

//    bool zh_calcChemElementCorrelation(int termIndex, qreal &correlationValue) const;
//    void zh_createTermValueAndChemConcentrationList(int termIndex,
//                                                    qint64 chemElementId,
//                                                    QList<QPair<qint64, qreal> > &termAndConcentrationList) const;

    void zh_calcCorrelations();
    void zh_calcChemElementCorrelations(const QList< QPair<qreal, QList<qreal> > >& chemConcentrationAndTermValueList,
                                        qreal averageChemElementValue,
                                        const QMap<int, qreal>& averageTermValueMap);
    void zh_calcIntercorrelations(const QList< QPair<qreal, QList<qreal> > >& chemConcentrationAndTermValueList,
                                  const QMap<int, qreal>& averageTermValueMap);
};
//==================================================================
#endif // ZTERMTABLEMANAGER_H

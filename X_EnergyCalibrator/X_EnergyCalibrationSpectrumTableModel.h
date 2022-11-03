//=========================================================
#ifndef X_ENERGYCALIBRATIONSPECTRUMMOPDEL_H
#define X_ENERGYCALIBRATIONSPECTRUMMOPDEL_H
//=========================================================
#include <QAbstractTableModel>

#include <QList>
#include <QMap>
//=========================================================
class X_SpeSpectrum;
//=========================================================
class X_EnergyCalibrationSpectrumTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit X_EnergyCalibrationSpectrumTableModel(QObject *parent = nullptr);
    ~X_EnergyCalibrationSpectrumTableModel();

    // override FUNCS
    virtual Qt::ItemFlags flags(const QModelIndex & index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void xp_loadSpectrumData(QMap<quint8, QList<X_SpeSpectrum *> > &spectrumMap);

    X_SpeSpectrum* xp_spectrumForRow(int row);

    void xp_writeEnergyCalibrationToCheckedSpectra(QList<double> &factorList);

signals:


public slots:

    void xp_setGainFactorFilter(quint8 currentGainFactor);

private:

    // VARS
    QMap<quint8, QList<X_SpeSpectrum*> > xv_spectrumMap;
    QMap<quint8, QList<QPair<X_SpeSpectrum*, QPair<bool, bool> > > > xv_spectrumVisibilityMap;

    quint8 xv_gainFactorFilter;
    int xv_maxChannelNumber;
    int xv_maxIntensity;

    // FUNCS
    void zh_recalcMaxSpectrumParameters();
    void zh_saveSpectrumVisibleAndCheckedState();
    void zh_restoreSpectrumVisibleAndCheckedState();

};
//=========================================================
#endif // X_ENERGYCALIBRATIONSPECTRUMMOPDEL_H

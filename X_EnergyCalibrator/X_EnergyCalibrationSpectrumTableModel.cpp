//=========================================================
#include "X_General.h"
#include "X_EnergyCalibrationSpectrumTableModel.h"
#include "X_SpectrumPaintData.h"
#include "X_SpeSpectrum.h"

#include <QDebug>
//=========================================================
X_EnergyCalibrationSpectrumTableModel::X_EnergyCalibrationSpectrumTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    xv_gainFactorFilter = 0;
}
//=========================================================
X_EnergyCalibrationSpectrumTableModel::~X_EnergyCalibrationSpectrumTableModel()
{
    zh_restoreSpectrumVisibleAndCheckedState();
}
//=========================================================
Qt::ItemFlags X_EnergyCalibrationSpectrumTableModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags;
    flags = Qt::ItemIsEnabled;

    if(index.column() == 0)
    {
        flags |= Qt::ItemIsUserCheckable;
    }
    return flags;
}
//=========================================================
QModelIndex X_EnergyCalibrationSpectrumTableModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row < 0 || row >= rowCount() || column < 0 || column >= columnCount() )
    {
        return QModelIndex();
    }

    return createIndex(row, column, xv_spectrumMap.value(xv_gainFactorFilter).at(row));
}
//=========================================================
int X_EnergyCalibrationSpectrumTableModel::columnCount(const QModelIndex & parent) const
{
    return 2;
}
//=========================================================
int X_EnergyCalibrationSpectrumTableModel::rowCount(const QModelIndex & parent) const
{
    return xv_spectrumMap.value(xv_gainFactorFilter).count();
}
//=========================================================
QVariant X_EnergyCalibrationSpectrumTableModel::data(const QModelIndex & index, int role) const
{
    if(!index.isValid() || index.row() < 0 || index.row() >= rowCount() ||
            index.column() < 0 || index.column() >= columnCount())
    {
        return QVariant();
    }

    QList<X_SpeSpectrum*> spectrumList = xv_spectrumMap.value(xv_gainFactorFilter);
    X_SpeSpectrum* spectrum = spectrumList.value(index.row(), nullptr);
    if(!spectrum)
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            QString speName = spectrum->xp_name();
            return QVariant(speName);
        }

        if(index.column() == 1)
        {
            X_SpectrumPaintData paintData;
            paintData.spectrumData = spectrum->xp_spectrumData();
            paintData.maxChannel = xv_maxChannelNumber;
            paintData.maxIntensity = xv_maxIntensity;

            return QVariant::fromValue(paintData);
        }
    }

    if(role == Qt::DecorationRole)
    {
        if(index.column() == 1)
        {
            QColor spectrumColor = spectrum->xp_color();
            if(!spectrumColor.isValid())
            {
                return QVariant();
            }

            return QVariant(spectrumColor); // QVariant(xv_dataManager->xp_spectrumColor(index.row()));
        }
    }

    if(role == NS_DataRole::VisibleRole)
    {
        if(index.column() == 1)
        {
            bool visibility = spectrum->xp_isSpectrumVisible();
            return QVariant(visibility);
        }
    }

    if(role == NS_DataRole::DataTypeRole)
    {
        if(index.column() == 1)
        {
            return QVariant(glSpectrumDataType);
        }
    }

    if(role == Qt::CheckStateRole)
    {
        if(index.column() == 0)
        {
            if(spectrum->xp_isSpectrumChecked())
            {
                return Qt::Checked;
            }
            else
            {
                return Qt::Unchecked;
            }
        }
    }

    return QVariant();
}
//=========================================================
bool X_EnergyCalibrationSpectrumTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!index.isValid() || index.row() < 0 || index.row() >= rowCount() ||
            index.column() < 0 || index.column() >= columnCount() || !value.isValid())
    {
        return false;
    }

    QList<X_SpeSpectrum*> spectrumList = xv_spectrumMap.value(xv_gainFactorFilter);
    X_SpeSpectrum* spectrum = spectrumList.value(index.row(), nullptr);
    if(!spectrum)
    {
        return false;
    }

    if(role == Qt::CheckStateRole)
    {
        if(index.column() == 0)
        {
            bool checked = spectrum->xp_isSpectrumChecked();
            spectrum->xp_setSpectrumChecked(!checked);
            emit dataChanged(index, index);
            return true;
        }
    }

    if(role == NS_DataRole::VisibleRole)
    {
        if(index.column() == 1)
        {
            bool visibility = spectrum->xp_isSpectrumVisible();
            spectrum->xp_setSpectrumVisible(!visibility);
            emit dataChanged(index, index);
            return true;
        }
    }

    return false;
}
//=========================================================
QVariant X_EnergyCalibrationSpectrumTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Vertical)
        {
            return QVariant(QString::number(section + 1));
        }
        else // Horizontal
        {
            return tr("Name");
        }
    }
    return QVariant();
}
//=========================================================
void X_EnergyCalibrationSpectrumTableModel::xp_loadSpectrumData(QMap<quint8, QList<X_SpeSpectrum*> >& spectrumMap)
{
    beginResetModel();
    xv_spectrumMap = spectrumMap;
    zh_recalcMaxSpectrumParameters();
    zh_saveSpectrumVisibleAndCheckedState();
    endResetModel();
}
//=========================================================
X_SpeSpectrum* X_EnergyCalibrationSpectrumTableModel::xp_spectrumForRow(int row)
{
    if(row < 0 || row >= rowCount())
    {
        return nullptr;
    }

    return xv_spectrumMap.value(xv_gainFactorFilter).at(row);
}
//=========================================================
void X_EnergyCalibrationSpectrumTableModel::xp_writeEnergyCalibrationToCheckedSpectra(QList<double>& factorList)
{
    QList<X_SpeSpectrum*> spectrumList = xv_spectrumMap.value(xv_gainFactorFilter, QList<X_SpeSpectrum*>());
    foreach(X_SpeSpectrum* spectrum, spectrumList)
    {
        if(spectrum->xp_isSpectrumChecked())
        {
            spectrum->xp_setEnergyCalibration(factorList);
            QString msg = tr("Energy calibration in spectrum %1 has been changed").arg(spectrum->xp_name());
            qInfo().noquote() << msg;
        }
    }
}
//=========================================================
void X_EnergyCalibrationSpectrumTableModel::xp_setGainFactorFilter(quint8 currentGainFactor)
{
    beginResetModel();
    xv_gainFactorFilter = currentGainFactor;
    zh_recalcMaxSpectrumParameters();
    endResetModel();
}
//=========================================================
void X_EnergyCalibrationSpectrumTableModel::zh_recalcMaxSpectrumParameters()
{
    xv_maxIntensity = 0;
    xv_maxChannelNumber = 0;
    for(auto & spectrum : xv_spectrumMap.value(xv_gainFactorFilter))
    {
        if(xv_maxIntensity < spectrum->xp_maxIntensity())
        {
            xv_maxIntensity = spectrum->xp_maxIntensity();
        }

        if(xv_maxChannelNumber < spectrum->xp_spectrumData().count())
        {
            xv_maxChannelNumber = spectrum->xp_spectrumData().count();
        }
    }
}
//=========================================================
void X_EnergyCalibrationSpectrumTableModel::zh_saveSpectrumVisibleAndCheckedState()
{
    for(auto gainFactor :  xv_spectrumMap.keys())
    {
        for(auto spectrum : xv_spectrumMap.value(gainFactor))
        {
            xv_spectrumVisibilityMap[gainFactor].append(QPair<X_SpeSpectrum*, QPair<bool, bool> >(spectrum,
                                                                                               QPair<bool, bool>(spectrum->xp_isSpectrumChecked(), spectrum->xp_isSpectrumVisible())));
        }
    }
}
//=========================================================
void X_EnergyCalibrationSpectrumTableModel::zh_restoreSpectrumVisibleAndCheckedState()
{
    int spectrumIndex = 0;
    for(auto gainFactor :  xv_spectrumVisibilityMap.keys())
    {
        if(!xv_spectrumMap.keys().contains(gainFactor))
        {
            continue;
        }
        for(auto spectrumProperties : xv_spectrumVisibilityMap.value(gainFactor))
        {
            spectrumIndex = xv_spectrumMap[gainFactor].indexOf(spectrumProperties.first);
            if(spectrumIndex < 0 || spectrumIndex >= xv_spectrumMap[gainFactor].count())
            {
                continue;
            }
            xv_spectrumMap[gainFactor][spectrumIndex]->xp_setSpectrumChecked(spectrumProperties.second.first);
            xv_spectrumMap[gainFactor][spectrumIndex]->xp_setSpectrumVisible(spectrumProperties.second.second);
        }
    }
}
//=========================================================

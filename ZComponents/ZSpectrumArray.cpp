//===============================================
#include "ZSpectrumArray.h"
#include "ZSpeSpectrum.h"
#include "ZSpeIOHandler.h"
#include "ZGeneral.h"

#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QMessageBox>
////===============================================
QList<QColor> ZSpectrumArray::zv_colorList = ZSpectrumArray::zp_createColorList();
qint64 ZSpectrumArray::zv_lastArrayId = 0;
//===============================================
ZSpectrumArray::ZSpectrumArray(QObject* parent)
{
    zv_maxArrayChannelCount = 0;
    zv_maxArrayIntensity = 0;

    zv_arrayName = QString();
    zv_lastColorIndex = 0;

    zv_energyK0 = 0;
    zv_energyK1 = 0;
    zv_energyK2 = 0;
    zv_exposition = 0;
    zv_gainFactor = 0;
    zv_energyUnit = QString();

    zv_arrayId = zv_lastArrayId++;
    zh_createConnections();
    zh_recalcArrayMaxParameters();}
//===============================================
ZSpectrumArray::ZSpectrumArray(const QString& name, QObject* parent)  : QObject(parent)
{
    zv_arrayName = name;
    zv_lastColorIndex = 0;

    zv_energyK0 = 0;
    zv_energyK1 = 0;
    zv_energyK2 = 0;
    zv_exposition = 0;
    zv_gainFactor = 0;
    zv_energyUnit = QString();

    zv_arrayId = zv_lastArrayId++;
    zh_createConnections();
    zh_recalcArrayMaxParameters();
}
//===============================================
ZSpectrumArray::~ZSpectrumArray()
{
    zp_clearArray();
    zv_chemElementList.zp_clearList();
}
//===============================================
bool ZSpectrumArray::checkColor(QColor color)
{
    if(!color.isValid() || zv_colorList.contains(color))
    {
        return false;
    }

    //    if(color.red() == 255 &&  color.green() < 10 && color.blue() < 10  ||
    //            color.red() < 10 &&  color.green() == 255 && color.blue() < 10  ||
    //            color.red() < 10 &&  color.green() < 10 && color.blue() == 255  )
    //    {
    //        return false;
    //    }

    int colorSumm = color.red() + color.green() + color.blue();
    int averageVol = colorSumm / 3;

    if(colorSumm > 450)
    {
        return false;
    }

    if(abs(averageVol - color.red()) + abs(averageVol - color.green()) + abs(averageVol - color.blue()) < 90)
    {
        return false;
    }

    return true;
}
//===============================================
QList<QColor> ZSpectrumArray::zp_createColorList()
{
    int red;
    int green;
    int blue;
    QColor color;

    QList<QColor> colorList;

    while(colorList.count() < 1500)
    {
        qsrand(QDateTime::currentMSecsSinceEpoch());
        red = ((qrand() % 24) * 10) + 15;
        green = ((qrand() % 24) * 10) + 15;
        blue = ((qrand() % 24) * 10) +15;

        color = QColor(red, green, blue);
        if(ZSpectrumArray::checkColor(color))
        {
            colorList << color;
        }
    }
    return colorList;
}
//===============================================
QString ZSpectrumArray::zp_arrayName() const
{
    return zv_arrayName;
}
//===============================================
void ZSpectrumArray::zp_setArrayName(const QString& name)
{
    zv_arrayName = name;
}
//===============================================
ZRawSpectrumArray ZSpectrumArray::zp_createRawSpectrumArray()
{
    ZRawSpectrumArray rawSpectrumArray;
    rawSpectrumArray.name = zv_arrayName;
    rawSpectrumArray.gainFactor = zv_gainFactor;

    for(int s = 0; s < zv_spectrumList.count(); s++)
    {
        ZRawSpectrum rawSpectrum;
        rawSpectrum.path = zv_spectrumList.value(s)->zp_path();
        // concentration list
        for(int e = 0; e < zv_chemElementList.zp_chemElementCount(); e++)
        {
            QString chemElement = zv_chemElementList.zp_chemElementName(e);
            if(chemElement.isEmpty())
            {
                continue;
            }
            qint64 chemElementId = zv_chemElementList.zp_chemElementId(e);
            if(chemElementId < 0)
            {
                continue;
            }
            QString concentration = zv_spectrumList.value(s)->zp_concentrationString(chemElementId);
            rawSpectrum.concentrationMap.insert(chemElement, concentration);
        }

        rawSpectrumArray.spectrumList << rawSpectrum;
    }

    return rawSpectrumArray;
}
//===============================================
int ZSpectrumArray::zp_spectrumCount() const
{
    return zv_spectrumList.count();
}
//===============================================
int ZSpectrumArray::zp_chemElementCount() const
{
    return zv_chemElementList.zp_chemElementCount();
}
//===============================================
int ZSpectrumArray::zp_chemElementIndexForName(const QString& chemElement) const
{
    for(int i = 0; i < zv_chemElementList.zp_chemElementCount(); i++)
    {
        if(zv_chemElementList.zp_chemElementName(i) == chemElement)
        {
            return i;
        }
    }
    return -1;
}
//===============================================
int ZSpectrumArray::zp_chemElementIndexForId(qint64 chemElementId) const
{
    for(int i = 0; i < zv_chemElementList.zp_chemElementCount(); i++)
    {
        if(zv_chemElementList.zp_chemElementId(i) == chemElementId)
        {
            return i;
        }
    }
    return -1;
}
//===============================================
int ZSpectrumArray::zp_numberVisibleChemElementsBeforeIndex(int index) const
{
    return zv_chemElementList.zp_numberVisibleChemElementsBeforeIndex(index);
}
//===============================================
int ZSpectrumArray::zp_visibleChemElementCount() const
{
    return zv_chemElementList.zp_visibleElementCount();
}
//===============================================
QStringList ZSpectrumArray::zp_chemElementList() const
{
    return zv_chemElementList.zp_chemElementList();
}
//===============================================
QString ZSpectrumArray::zp_visibleChemElementName(int visibleIndex) const
{
    return zv_chemElementList.zp_visibleChemElementName(visibleIndex);
}
//===============================================
qint64 ZSpectrumArray::zp_visibleChemElementId(int visibleIndex) const
{
    return zv_chemElementList.zp_visibleChemElementId(visibleIndex);
}
//===============================================
QString ZSpectrumArray::zp_chemElementName(int index) const
{
    return zv_chemElementList.zp_chemElementName(index);
}
//===============================================
qint64 ZSpectrumArray::zp_chemElementId(int index) const
{
    return zv_chemElementList.zp_chemElementId(index);
}
//===============================================
bool ZSpectrumArray::zp_containsChemElementId(qint64 chemElementId) const
{
    return zv_chemElementList.zp_containsElementId(chemElementId);
}
//===============================================
bool ZSpectrumArray::zp_chemElementIsVisible(int index) const
{
    return zv_chemElementList.zp_chemElementIsVisible(index);
}
//===============================================
bool ZSpectrumArray::zp_setChemElementVisible(int row, bool visible)
{
    return zv_chemElementList.zp_setChemElementVisible(row, visible);
}
//===============================================
bool ZSpectrumArray::zp_setChemElementName(int row, const QString& name)
{
    return zv_chemElementList.zp_setChemElementName(row, name);
}
//===============================================
QString ZSpectrumArray::zp_chemConcentration(qint64 chemElementId,
                                             int spectrumIndex) const
{
    if(spectrumIndex < 0 || spectrumIndex >= zp_spectrumCount()
            || !zv_chemElementList.zp_containsElementId(chemElementId))
    {
        return QString();
    }

    return zv_spectrumList.at(spectrumIndex)->zp_concentrationString(chemElementId);
}
//===============================================
qreal ZSpectrumArray::zp_chemConcentrationValue(qint64 chemElementId,
                                                int spectrumIndex) const
{
    bool ok;
    qreal value = zp_chemConcentration(chemElementId, spectrumIndex).toDouble(&ok);
    if(!ok)
    {
        return 0.0;
    }

    return value;
}
//===============================================
bool ZSpectrumArray::zp_setChemConcentration(qint64 chemElementId,
                                             int spectrumIndex, const QString& concentration)
{
    if(spectrumIndex < 0 || spectrumIndex >= zp_spectrumCount()
            || !zv_chemElementList.zp_containsElementId(chemElementId))
    {
        return false;
    }

    bool res = zv_spectrumList.at(spectrumIndex)->zp_setConcentration(chemElementId, concentration);
    if(res)
    {
        emit zg_spectrumOperation(ZSpectrumArray::OT_CONCENTRATION_CHANGED, spectrumIndex, spectrumIndex);
    }

    return res;
}
//===============================================
bool ZSpectrumArray::zp_energyCalibration(int spectrumIndex, QList<double>& energyCalibration, QString& energyUnit) const
{
    if(spectrumIndex < 0 || spectrumIndex >= zv_spectrumList.count())
    {
        return false;
    }

    ZSpeSpectrum* spectrum = qobject_cast<ZSpeSpectrum*>(zv_spectrumList.at(spectrumIndex));
    if(!spectrum)
    {
        return false;
    }

    energyCalibration = spectrum->zp_energyCalibration();
    energyUnit = zv_energyUnit;
    return true;
}
//===============================================
int ZSpectrumArray::zp_gainFactor() const
{
    return zv_gainFactor;
}
//===============================================
bool ZSpectrumArray::zp_setGainFactor(int gainFactor)
{
    if(gainFactor < 0 || gainFactor > glMaxGainFactor)
    {
        return false;
    }
    zv_gainFactor = gainFactor;
    return true;
}
//===============================================
int ZSpectrumArray::zp_exposition() const
{
    return zv_exposition;
}
//===============================================
QString ZSpectrumArray::zp_spectrumFileName(int index) const
{
    if(index < 0 || index >= zv_spectrumList.count())
    {
        return QString();
    }
    return zv_spectrumList.at(index)->zp_name();
}
//===============================================
qint64 ZSpectrumArray::zp_spectrumId(int index) const
{
    if(index < 0 || index >= zv_spectrumList.count())
    {
        return -1;
    }
    return zv_spectrumList.at(index)->zp_spectrumId();
}
//===============================================
QColor ZSpectrumArray::zp_spectrumColor(int index) const
{
    if(index < 0 || index >= zv_spectrumList.count())
    {
        return QColor();
    }
    return zv_spectrumList.at(index)->zp_color();
}
//===============================================
QList<quint32> ZSpectrumArray::zp_spectrumData(int index) const
{
    if(index < 0 || index >= zv_spectrumList.count())
    {
        return QList<quint32>();
    }
    return zv_spectrumList.at(index)->zp_spectrumData();
}
//===============================================
bool ZSpectrumArray::zp_isSpectrumVisible(int index) const
{
    if(index < 0 || index >= zv_spectrumList.count())
    {
        return false;
    }

    return zv_spectrumList.at(index)->zp_isSpectrumVisible();
}
//===============================================
bool ZSpectrumArray::zp_setSpectrumVisible(int index, bool visible)
{
    if(index < 0 || index >= zv_spectrumList.count())
    {
        return false;
    }

    zv_spectrumList[index]->zp_setSpectrumVisible(visible);
    return true;
}
//===============================================
bool ZSpectrumArray::zp_isSpectrumChecked(int index) const
{
    if(index < 0 || index >= zv_spectrumList.count())
    {
        return false;
    }

    return zv_spectrumList.at(index)->zp_isSpectrumChecked();
}
//===============================================
bool ZSpectrumArray::zp_setSpectrumChecked(int index, bool checked)
{
    if(index < 0 || index >= zv_spectrumList.count())
    {
        return false;
    }

    bool res = zv_spectrumList.at(index)->zp_setSpectrumChecked(checked);
    if(res)
    {
        emit zg_spectrumOperation(OT_CHANGED, index, index);
        //        zh_calcAverageChemConcentrations();
    }
    return res;
}
//===============================================
bool ZSpectrumArray::zp_removeSpectrum(int index)
{
    if(index < 0 || index >= zv_spectrumList.count())
    {
        return false;
    }
    emit zg_spectrumOperation(OT_REMOVE_SPECTRA, index, index);
    delete zv_spectrumList.at(index);
    zv_spectrumList.removeAt(index);
    emit zg_spectrumOperation(OT_END_REMOVE_SPECTRA, index, index);

    zh_recalcArrayMaxParameters();
    //   zh_calcAverageChemConcentrations();

    if(zv_spectrumList.count() < 1 )
    {
        zv_energyUnit = QString();
        zv_energyK0 = 0.0;
        zv_energyK1 = 0.0;
        zv_energyK2 = 0.0;
        zv_exposition = -1;
    }
    return true;
}
//===============================================
void ZSpectrumArray::zp_clearArray()
{
    if(!zv_spectrumList.isEmpty())
    {
        int lastSpectrumIndex = zv_spectrumList.count() - 1;
        emit zg_spectrumOperation(OT_REMOVE_SPECTRA, 0, lastSpectrumIndex);
        //        qDeleteAll(zv_spectrumList);
        //        zv_spectrumList.clear();
        for(int i = zv_spectrumList.count()-1; i >= 0; i-- )
        {
            delete zv_spectrumList.takeAt(i);
        }

        emit zg_spectrumOperation(OT_END_REMOVE_SPECTRA,  0, lastSpectrumIndex);

        zh_recalcArrayMaxParameters();
        //        zh_calcAverageChemConcentrations();
    }
}
//===============================================
void ZSpectrumArray::zp_clearChemElements()
{
    zv_chemElementList.zp_clearList();
}
//===============================================
bool ZSpectrumArray::zp_appendSpectrum(const ZRawSpectrum& rawSpectrum, bool last, bool& dontAsk, bool& continueAppend)
{
    QFileInfo fileInfo(rawSpectrum.path);
    ZAbstractSpectrumIOHandler* ioHandler;

    QString suffix = fileInfo.suffix();
    if(!fileInfo.exists() || !fileInfo.isFile())
    {
        QString error = tr("Error: \"%1\" is not a file!").arg(rawSpectrum.path);
        emit zg_message(error);
        qCritical().noquote() <<  error;
        return false;
    }
    else if(suffix == "spe")
    {
        ioHandler = new ZSpeIOHandler(this, this);
    }
    else
    {
        QString error = QObject::tr("Cannot handle file of type \"%1\"!").arg(suffix);
        emit zg_message(error);
        qCritical().noquote() <<  error;
        return false;
    }

    ZAbstractSpectrum* abstractSpectrum;
    bool res = ioHandler->zp_getSpectrumFromFile(rawSpectrum.path, zv_colorList.value(zv_lastColorIndex), abstractSpectrum);
    if(res)
    {
        ZSpeSpectrum* speSpectrum = qobject_cast<ZSpeSpectrum*>(abstractSpectrum);
        if(zp_isEnergyCalibrationValid() && !dontAsk)
        {
            QStringList inconsistenciesList = speSpectrum->zp_isEnergyCalibrationAndExpositionSuitable(zv_energyUnit, zv_energyK0, zv_energyK1, zv_energyK2, zv_exposition);
            if(!inconsistenciesList.isEmpty())
            {
                // QString question = tr("Energy calibration in spectrum %1% is different to calibration that have other spectra in the array.\r\n").arg(rawSpectrum.path);
                QString question = tr("The following inconsistencies have been found: \r\n");
                for(int i = 0; i < inconsistenciesList.count(); i++)
                {
                    question += "- "+inconsistenciesList.value(i);
                    if(i < inconsistenciesList.count() - 1)
                    {
                        question += ";";
                    }
                    else
                    {
                        question += ".";
                    }
                    question += "\r\n";
                }
                question += "Do you want to continue loading?";

                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Question);
                msgBox.setText(tr("Energy calibration and exposition in spectrum \"%1\" is different to calibration that have other spectra in the array.\r\n").arg(rawSpectrum.path));
                msgBox.setInformativeText(question);

                if(last)
                {
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                }
                else
                {
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll);
                }
                msgBox.setDefaultButton(QMessageBox::Yes );
                int ret = msgBox.exec();
                switch (ret)
                {
                case QMessageBox::Yes:
                    res = true;
                    continueAppend = true;
                    break;
                case QMessageBox::YesToAll:
                    dontAsk = true;
                    continueAppend = true;
                    res = true;
                    break;
                case QMessageBox::No:
                    res = false;
                    continueAppend = true;
                    break;
                case QMessageBox::NoToAll:
                default:
                    dontAsk = true;
                    continueAppend = false;
                    res = false;
                    // should never be reached
                    break;
                }
            }
        }

        if(res)
        {
            if(zv_spectrumList.count() < 1)
            {
                // first spectrum. storing energy calibration

                const ZSpeAuxData* auxData = static_cast<const ZSpeAuxData*>(speSpectrum->zp_auxData());
                zv_energyUnit = auxData->zp_energyUnit();
                zv_energyK0 = auxData->zp_energyK0();
                zv_energyK1 = auxData->zp_energyK1();
                zv_energyK2 = auxData->zp_energyK2();

                zv_exposition = auxData->zp_exposition();
                zv_gainFactor = static_cast<int>(auxData->zp_gainFactor());

            }

            int spectrumIndex = zv_spectrumList.count();
            emit zg_spectrumOperation(OT_INSERT_SPECTRA, spectrumIndex, spectrumIndex);
            zv_spectrumList.append(speSpectrum);
            emit zg_spectrumOperation(OT_END_INSERT_SPECTRA, spectrumIndex, spectrumIndex);

            // color index increment
            if(++zv_lastColorIndex >= zv_colorList.count())
            {
                zv_lastColorIndex = 0;
            }

            QMap<QString, QString>::const_iterator it;
            qint64 chemElementId = -1;

            for(it = rawSpectrum.concentrationMap.begin(); it != rawSpectrum.concentrationMap.end(); it++)
            {
                if(!zv_chemElementList.zp_containsElement(it.key()))
                {
                    zv_chemElementList.zp_appendElement(it.key(), chemElementId);
                }
                else
                {
                    for(int i = 0; i < zv_chemElementList.zp_chemElementCount(); i++)
                    {
                        if(zv_chemElementList.zp_chemElementName(i) == it.key())
                        {
                            chemElementId = zp_chemElementId(i);
                            break;
                        }
                    }
                }

                speSpectrum->zp_insertConcentration(chemElementId, it.value());
            }

            //            connect(speSpectrum, &ZSpeSpectrum::zg_energyCalibrationChanged,
            //                    this, &ZSpectrumArray::zh_saveSpectrumToFile);
            connect(speSpectrum, &ZSpeSpectrum::zg_energyCalibrationChanged,
                    this, &ZSpectrumArray::zh_onSpectrumEnergyCalibrationChange);
            connect(speSpectrum, &ZSpeSpectrum::zg_gainFactorChanged,
                    this, &ZSpectrumArray::zh_onSpectrumGainFactorChange);
        }
    }

    delete ioHandler;
    zh_recalcArrayMaxParameters();
    return res;
}
//===============================================
void ZSpectrumArray::zh_onSpectrumEnergyCalibrationChange() const
{
    ZSpeSpectrum* spectrum = qobject_cast<ZSpeSpectrum*>(sender());
    if(!spectrum)
    {
        return;
    }

    zh_saveSpectrumToFile(spectrum);

    bool ok = false;
    int spectrumIndex = zh_indexForSpectrum(spectrum, &ok);

    if(ok)
    {
        emit zg_spectrumOperation(ZSpectrumArray::OT_SPECTRUM_ENERGY_CALIBRATION_CHANGED, spectrumIndex, spectrumIndex);
    }
}
//===============================================
void ZSpectrumArray::zh_onSpectrumGainFactorChange() const
{
    ZSpeSpectrum* spectrum = qobject_cast<ZSpeSpectrum*>(sender());
    if(!spectrum)
    {
        return;
    }

    zh_saveSpectrumToFile(spectrum);
}
//===============================================
void ZSpectrumArray::zh_saveSpectrumToFile(ZSpeSpectrum* spectrum) const
{
    ZSpeIOHandler speIOHandler(nullptr);

    QFileInfo fileInfo(spectrum->zp_path());
    QString folderPath = fileInfo.absolutePath();
    QString fileName = fileInfo.fileName();

    speIOHandler.zp_saveSpectrumToFile(folderPath, fileName, spectrum);
}
//===============================================
int ZSpectrumArray::zh_indexForSpectrum(ZSpeSpectrum* spectrum, bool* ok) const
{
    for(int i = 0; i < zv_spectrumList.count(); i++)
    {
        if(zv_spectrumList.at(i) == spectrum)
        {
            if(ok)
            {
                *ok = true;
            }
            return i;
        }
    }

    if(ok)
    {
        *ok = false;
    }

    return -1;
}
//===============================================
bool ZSpectrumArray::zp_appendNewChemElement(QString chemElement)
{
    int nextElementIndex = zv_chemElementList.zp_chemElementCount() + 1;
    if(chemElement.isEmpty())
    {
        chemElement = "Element #";
        while(zv_chemElementList.zp_containsElement(chemElement+QString::number(nextElementIndex)))
        {
            nextElementIndex++;
        }
        chemElement += QString::number(nextElementIndex);
    }
    else if(zv_chemElementList.zp_containsElement(chemElement))
    {
        chemElement += " #";
        while(zv_chemElementList.zp_containsElement(chemElement+QString::number(nextElementIndex)))
        {
            nextElementIndex++;
        }
        chemElement += QString::number(nextElementIndex);
    }

    qint64 chemElementId;
    bool res = zv_chemElementList.zp_appendElement(chemElement, chemElementId);
    if(res)
    {
        for(int s = 0; s < zv_spectrumList.count(); s++)
        {
            zv_spectrumList[s]->zp_insertConcentration(chemElementId, QString::number(0));
        }
    }
    return res;
}
//===============================================
bool ZSpectrumArray::zp_removeChemElement(int chemElementIndex)
{
    if(chemElementIndex < 0 || chemElementIndex >= zv_chemElementList.zp_chemElementCount())
    {
        return false;
    }

    QString chemElement = zv_chemElementList.zp_chemElementName(chemElementIndex);
    if(chemElement.isEmpty())
    {
        return false;
    }

    qint64 chemElementId;
    bool res = zv_chemElementList.zp_removeElement(chemElement, chemElementId);
    if(res)
    {
        for(int s  = 0; s < zv_spectrumList.count(); s++)
        {
            zv_spectrumList[s]->zp_removeConcentration(chemElementId);
        }
    }
    return res;
}
//===============================================
const ZAbstractSpectrum *ZSpectrumArray::zp_spectrum(int index) const
{
    if(index < 0 || index >= zv_spectrumList.count())
    {
        return nullptr;
    }

    return zv_spectrumList.at(index);
}
//===============================================
QList<ZAbstractSpectrum*> ZSpectrumArray::zp_spectrumList() const
{
    QList<ZAbstractSpectrum*> spectrumList;

    for(int s = 0; s < zv_spectrumList.count(); s++)
    {
        spectrumList << zv_spectrumList.value(s);
    }
    return spectrumList;
}
//===============================================
int ZSpectrumArray::zp_maxIntensity() const
{
    return zv_maxArrayIntensity;
}
//===============================================
int ZSpectrumArray::zp_maxChannelCount() const
{
    return zv_maxArrayChannelCount;
}
//===============================================
bool ZSpectrumArray::zp_isEnergyCalibrationValid() const
{
    return !(zv_energyK1 == 0 && zv_energyK2 == 0);
}
//===============================================
qint64 ZSpectrumArray::zp_arrayId() const
{
    return zv_arrayId;
}
//===============================================
//bool ZSpectrumArray::zp_averageChemConcentrationForChemElementId(qint64 chemElementId, qreal& averageValue) const
//{
//    return zv_chemElementList.zp_averageChemConcentrationForChemElementId(chemElementId, averageValue);
//}
////===============================================
//bool ZSpectrumArray::zp_averageChemConcentration(const QString& chemElement, qreal& averageValue) const
//{
//    return zv_chemElementList.zp_averageChemConcentration(chemElement, averageValue);
//}
////===============================================
//bool ZSpectrumArray::zp_averageChemConcentration(int chemElementIndex, qreal& averageValue) const
//{
//    return zv_chemElementList.zp_averageChemConcentration(chemElementIndex, averageValue);
//}
//===============================================
void ZSpectrumArray::zh_createConnections()
{
    connect(&zv_chemElementList, &ZChemElementList::zg_operationType,
            this, &ZSpectrumArray::zg_chemElementOperation);
}
//===============================================
void ZSpectrumArray::zh_recalcArrayMaxParameters()
{
    zv_maxArrayChannelCount = 0;
    zv_maxArrayIntensity = 0;

    if(!zv_spectrumList.isEmpty())
    {
        for(int s = 0; s < zv_spectrumList.count(); s++)
        {
            if(zv_maxArrayChannelCount < zv_spectrumList.at(s)->zp_channelCount())
            {
                zv_maxArrayChannelCount = zv_spectrumList.at(s)->zp_channelCount();
            }

            if(zv_maxArrayIntensity < zv_spectrumList.at(s)->zp_maxIntensity())
            {
                zv_maxArrayIntensity = zv_spectrumList.at(s)->zp_maxIntensity();
            }
        }
    }

    emit zg_arrayMaxParametersChanged(zv_arrayId,  zv_maxArrayIntensity,  zv_maxArrayChannelCount);
}
//===============================================
//bool ZSpectrumArray::zh_calcAverageChemConcentration(qint64 chemElementId)
//{
//    if(!zv_chemElementList.zp_containsElementId(chemElementId))
//    {
//#ifdef DBG
//        qDebug() << "ZSpectrumArray: AVERAGE NOT RECALCED FOR CHEM ELEMENT ID" << chemElementId;
//#endif

//        return false;
//    }

//    if(zv_spectrumList.isEmpty())
//    {
//#ifdef DBG
//        qDebug() << "ZSpectrumArray: AVERAGE NOT RECALCED FOR CHEM ELEMENT ID" << chemElementId << "SPE LIST IS EMPTY";
//#endif

//        zv_chemElementList.zp_setAverageChemConcentration(chemElementId, 0.0);
//        return true;
//    }

//    qreal averageValue = 0;
//    bool ok;
//    qreal currentConcentration;
//    int checkedSpectrumCount = 0;
//    for(int s  = 0; s < zv_spectrumList.count(); s++)
//    {
//        if(!zv_spectrumList.at(s)->zp_isSpectrumChecked())
//        {
//            continue;
//        }
//        currentConcentration = zv_spectrumList.at(s)->zp_concentration(chemElementId).toDouble(&ok);
//        if(!ok)
//        {
//            currentConcentration = 0.0;
//        }

//        averageValue += currentConcentration;
//        checkedSpectrumCount++;
//    }

//    if(checkedSpectrumCount == 0)
//    {
//#ifdef DBG
//        qDebug() << "ZSpectrumArray: AVERAGE NOT RECALCED FOR CHEM ELEMENT ID" << chemElementId << "CHECKED SPE COUNT IS ZERO";
//#endif

//        zv_chemElementList.zp_setAverageChemConcentration(chemElementId, 0.0);
//        return true;
//    }
//    averageValue /= checkedSpectrumCount;
//    zv_chemElementList.zp_setAverageChemConcentration(chemElementId, averageValue);

//#ifdef DBG
//    qDebug() << "ZSpectrumArray: AVERAGE RECALCED FOR CHEM ELEMENT ID" << chemElementId << averageValue;
//#endif

//    return true;
//}
////===============================================
//void ZSpectrumArray::zh_calcAverageChemConcentrations()
//{
//#ifdef DBG
//    qDebug() << "ZSpectrumArray: START FULL CHEM AVERAGE RECALC ";
//#endif

//    if(zv_chemElementList.zp_isEmpty())
//    {
//        return;
//    }

//    for(int e = 0; e < zv_chemElementList.zp_chemElementCount(); e++)
//    {
//        zh_calcAverageChemConcentration(zv_chemElementList.zp_chemElementId(e));
//    }
//}
//===============================================


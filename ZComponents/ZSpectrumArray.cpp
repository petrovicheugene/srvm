//===============================================
#include "ZSpectrumArray.h"
#include "ZSpeSpectrum.h"
#include "ZSpeIOHandler.h"
#include "globalVariables.h"

#include <QFileInfo>
#include <QFile>
#include <QMessageBox>
////===============================================
QList<QColor> ZSpectrumArray::zv_colorList = ZSpectrumArray::zp_createColorList();
qint64 ZSpectrumArray::zv_lastArrayId = 0;
//===============================================
ZSpectrumArray::ZSpectrumArray(const QString& name, QObject* parent)  : QObject(parent)
{
    zv_arrayName = name;
    zv_colorIndex = 0;

    zv_energyK0 = 0;
    zv_energyK1 = 0;
    zv_energyK2 = 0;
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
            QString concentration = zv_spectrumList.value(s)->zp_concentration(chemElement);
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
int ZSpectrumArray::zp_chemElementIndex(const QString& chemElement) const
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
QString ZSpectrumArray::zp_chemElementName(int index) const
{
    return zv_chemElementList.zp_chemElementName(index);
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
QString ZSpectrumArray::zp_chemConcentration(const QString& chemElement,
                                             int spectrumIndex) const
{
    if(spectrumIndex < 0 || spectrumIndex >= zp_spectrumCount()
            || !zv_chemElementList.zp_containsElement(chemElement))
    {
        return QString();
    }

    return zv_spectrumList.at(spectrumIndex)->zp_concentration(chemElement);
}
//===============================================
bool ZSpectrumArray::zp_setChemConcentration(const QString& chemElement,
                                             int spectrumIndex, const QString& concentration)
{
    if(spectrumIndex < 0 || spectrumIndex >= zp_spectrumCount()
            || !zv_chemElementList.zp_containsElement(chemElement))
    {
        return false;
    }

    return zv_spectrumList.at(spectrumIndex)->zp_setConcentration(chemElement, concentration);
}
//===============================================
bool ZSpectrumArray::zp_energyCalibration(qreal& K0, qreal& K1, qreal& K2, QString& energyUnit)
{
    if(!zp_isEnergyCalibrationValid())
    {
        return false;
    }

    K0 = zv_energyK0;
    K1 = zv_energyK1;
    K2 = zv_energyK2;
    energyUnit = zv_energyUnit;
    return true;
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
QList<int> ZSpectrumArray::zp_spectrumData(int index) const
{
    if(index < 0 || index >= zv_spectrumList.count())
    {
        return QList<int>();
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
bool ZSpectrumArray::zp_removeSpectrum(int index)
{
    if(index < 0 || index >= zv_spectrumList.count())
    {
        return false;
    }
    emit zg_spectrumOperation(OT_REMOVE_SPECTRA, index, index);
    delete zv_spectrumList.takeAt(index);
    emit zg_spectrumOperation(OT_END_REMOVE_SPECTRA, index, index);

    zh_recalcArrayMaxParameters();
    if(zv_spectrumList.count() < 1 )
    {
        zv_energyUnit = QString();
        zv_energyK0 = 0;
        zv_energyK1 = 0;
        zv_energyK2 = 0;
        emit zg_energyCalibrationChanged(zv_arrayId);
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
    }
}
//===============================================
void ZSpectrumArray::zp_clearChemElements()
{
    zv_chemElementList.zp_clearList();
}
//===============================================
bool ZSpectrumArray::zp_appendSpectrum(const ZRawSpectrum& rawSpectrum, bool last, bool& dontAsk)
{
    QFileInfo fileInfo(rawSpectrum.path);
    ZAbstractSpectrumIOHandler* ioHandler;

    QString suffix = fileInfo.suffix();
    if(!fileInfo.exists() || !fileInfo.isFile())
    {
        QString error = tr("Error: \"%1\" is not a file!").arg(rawSpectrum.path);
        emit zg_message(error);
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
        return false;
    }

    ZAbstractSpectrum* abstractSpectrum;
    bool res = ioHandler->zp_getSpectrumFromFile(rawSpectrum.path, zv_colorList.value(zv_colorIndex), abstractSpectrum);
    if(res)
    {
        ZSpeSpectrum* speSpectrum = qobject_cast<ZSpeSpectrum*>(abstractSpectrum);

        if(zp_isEnergyCalibrationValid() && !dontAsk)
        {
            QStringList inconsistenciesList = speSpectrum->zp_isEnergyCalibrationSuitable(zv_energyUnit, zv_energyK0, zv_energyK1, zv_energyK2);
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
                msgBox.setText(tr("Energy calibration in spectrum \"%1\" is different to calibration that have other spectra in the array.\r\n").arg(rawSpectrum.path));
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
                    break;
                case QMessageBox::YesToAll:
                    dontAsk = true;
                    res = true;
                    break;
                case QMessageBox::No:
                    res = false;
                    break;
                case QMessageBox::NoToAll:
                default:
                    dontAsk = true;
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
                emit zg_energyCalibrationChanged(zv_arrayId);
            }

            int spectrumIndex = zv_spectrumList.count();
            emit zg_spectrumOperation(OT_INSERT_SPECTRA, spectrumIndex, spectrumIndex);
            zv_spectrumList.append(speSpectrum);
            emit zg_spectrumOperation(OT_END_INSERT_SPECTRA, spectrumIndex, spectrumIndex);
            // color index increment
            if(++zv_colorIndex >= zv_colorList.count())
            {
                zv_colorIndex = 0;
            }

            QMap<QString, QString>::const_iterator it;

            for(it = rawSpectrum.concentrationMap.begin(); it != rawSpectrum.concentrationMap.end(); it++)
            {
                speSpectrum->zp_insertConcentration(it.key(), it.value());
                if(!zv_chemElementList.zp_containsElement(it.key()))
                {
                    // TODO signals column number increased
                    zv_chemElementList.zp_appendElement(it.key());
                }
            }
        }
    }

    delete ioHandler;
    zh_recalcArrayMaxParameters();
    return res;
}
//===============================================
bool ZSpectrumArray::zp_appendNewChemElement()
{
    QString chemElement = "Element #";
    int nextElementIndex = zv_chemElementList.zp_chemElementCount() + 1;
    while(zv_chemElementList.zp_containsElement(chemElement+QString::number(nextElementIndex)))
    {
        nextElementIndex++;
    }

    return zv_chemElementList.zp_appendElement(chemElement+QString::number(nextElementIndex));
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

    return zv_chemElementList.zp_removeElement(chemElement);
}
//===============================================
const ZAbstractSpectrum* ZSpectrumArray::zp_spectrum(int index) const
{
    if(index < 0 || index >= zv_spectrumList.count())
    {
        return 0;
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
int ZSpectrumArray::zp_maxIntensity()
{
    return zv_maxArrayIntensity;
}
//===============================================
int ZSpectrumArray::zp_maxChannelCount()
{
    return zv_maxArrayChannelCount;
}
//===============================================
bool ZSpectrumArray::zp_isEnergyCalibrationValid()
{
    return !(zv_energyK1 == 0 && zv_energyK2 == 0);
}
//===============================================
qint64 ZSpectrumArray::zp_arrayId()
{
    return zv_arrayId;
}
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

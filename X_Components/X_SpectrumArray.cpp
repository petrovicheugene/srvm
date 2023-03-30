//===============================================
#include "X_SpectrumArray.h"
#include "X_SpeSpectrum.h"
#include "X_SpeIOHandler.h"
#include "X_General.h"

#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QMessageBox>
////===============================================
QList<QColor> X_SpectrumArray::xv_colorList = X_SpectrumArray::xp_createColorList();
qint64 X_SpectrumArray::xv_lastArrayId = 0;
//===============================================
X_SpectrumArray::X_SpectrumArray(QObject* parent)
{
    xv_maxArrayChannelCount = 0;
    xv_maxArrayIntensity = 0;

    xv_arrayName = QString();
    xv_lastColorIndex = 0;

    xv_energyK0 = 0;
    xv_energyK1 = 0;
    xv_energyK2 = 0;
    xv_exposition = 0;
    xv_gainFactor = 0;
    xv_energyUnit = QString();

    xv_arrayId = xv_lastArrayId++;
    xh_createConnections();
    xh_recalcArrayMaxParameters();}
//===============================================
X_SpectrumArray::X_SpectrumArray(const QString& name, QObject* parent)  : QObject(parent)
{
    xv_arrayName = name;
    xv_lastColorIndex = 0;

    xv_energyK0 = 0;
    xv_energyK1 = 0;
    xv_energyK2 = 0;
    xv_exposition = 0;
    xv_gainFactor = 0;
    xv_energyUnit = QString();

    xv_arrayId = xv_lastArrayId++;
    xh_createConnections();
    xh_recalcArrayMaxParameters();
}
//===============================================
X_SpectrumArray::~X_SpectrumArray()
{
    xp_clearArray();
    xv_chemElementList.xp_clearList();
}
//===============================================
bool X_SpectrumArray::checkColor(QColor color)
{
    if(!color.isValid() || xv_colorList.contains(color))
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
QList<QColor> X_SpectrumArray::xp_createColorList()
{
    int red;
    int green;
    int blue;
    QColor color;

    QList<QColor> colorList;

    while(colorList.count() < 1500)
    {
        srand(QDateTime::currentMSecsSinceEpoch());
        red = ((rand() % 24) * 10) + 15;
        green = ((rand() % 24) * 10) + 15;
        blue = ((rand() % 24) * 10) +15;

        color = QColor(red, green, blue);
        if(X_SpectrumArray::checkColor(color))
        {
            colorList << color;
        }
    }

    return colorList;
}
//===============================================
QString X_SpectrumArray::xp_arrayName() const
{
    return xv_arrayName;
}
//===============================================
void X_SpectrumArray::xp_setArrayName(const QString& name)
{
    xv_arrayName = name;
}
//===============================================
X_RawSpectrumArray X_SpectrumArray::xp_createRawSpectrumArray()
{
    X_RawSpectrumArray rawSpectrumArray;
    rawSpectrumArray.name = xv_arrayName;
    rawSpectrumArray.gainFactor = xv_gainFactor;

    for(int s = 0; s < xv_spectrumList.count(); s++)
    {
        X_RawSpectrum rawSpectrum;
        rawSpectrum.path = xv_spectrumList.value(s)->xp_path();
        // concentration list
        for(int e = 0; e < xv_chemElementList.xp_chemElementCount(); e++)
        {
            QString chemElement = xv_chemElementList.xp_chemElementName(e);
            if(chemElement.isEmpty())
            {
                continue;
            }
            qint64 chemElementId = xv_chemElementList.xp_chemElementId(e);
            if(chemElementId < 0)
            {
                continue;
            }
            QString concentration = xv_spectrumList.value(s)->xp_concentrationString(chemElementId);
            rawSpectrum.concentrationMap.insert(chemElement, concentration);
        }
        rawSpectrum.checked = xv_spectrumList.value(s)->xp_isSpectrumChecked();
        rawSpectrum.visible = xv_spectrumList.value(s)->xp_isSpectrumVisible();

        rawSpectrumArray.spectrumList << rawSpectrum;
    }

    return rawSpectrumArray;
}
//===============================================
int X_SpectrumArray::xp_spectrumCount() const
{
    return xv_spectrumList.count();
}
//===============================================
int X_SpectrumArray::xp_chemElementCount() const
{
    return xv_chemElementList.xp_chemElementCount();
}
//===============================================
int X_SpectrumArray::xp_chemElementIndexForName(const QString& chemElement) const
{
    for(int i = 0; i < xv_chemElementList.xp_chemElementCount(); i++)
    {
        if(xv_chemElementList.xp_chemElementName(i) == chemElement)
        {
            return i;
        }
    }
    return -1;
}
//===============================================
int X_SpectrumArray::xp_chemElementIndexForId(qint64 chemElementId) const
{
    for(int i = 0; i < xv_chemElementList.xp_chemElementCount(); i++)
    {
        if(xv_chemElementList.xp_chemElementId(i) == chemElementId)
        {
            return i;
        }
    }
    return -1;
}
//===============================================
int X_SpectrumArray::xp_numberVisibleChemElementsBeforeIndex(int index) const
{
    return xv_chemElementList.xp_numberVisibleChemElementsBeforeIndex(index);
}
//===============================================
int X_SpectrumArray::xp_visibleChemElementCount() const
{
    return xv_chemElementList.xp_visibleElementCount();
}
//===============================================
QStringList X_SpectrumArray::xp_chemElementList() const
{
    return xv_chemElementList.xp_chemElementList();
}
//===============================================
QString X_SpectrumArray::xp_visibleChemElementName(int visibleIndex) const
{
    return xv_chemElementList.xp_visibleChemElementName(visibleIndex);
}
//===============================================
qint64 X_SpectrumArray::xp_visibleChemElementId(int visibleIndex) const
{
    return xv_chemElementList.xp_visibleChemElementId(visibleIndex);
}
//===============================================
QString X_SpectrumArray::xp_chemElementName(int index) const
{
    return xv_chemElementList.xp_chemElementName(index);
}
//===============================================
qint64 X_SpectrumArray::xp_chemElementId(int index) const
{
    return xv_chemElementList.xp_chemElementId(index);
}
//===============================================
bool X_SpectrumArray::xp_containsChemElementId(qint64 chemElementId) const
{
    return xv_chemElementList.xp_containsElementId(chemElementId);
}
//===============================================
bool X_SpectrumArray::xp_chemElementIsVisible(int index) const
{
    return xv_chemElementList.xp_chemElementIsVisible(index);
}
//===============================================
bool X_SpectrumArray::xp_setChemElementVisible(int row, bool visible)
{
    return xv_chemElementList.xp_setChemElementVisible(row, visible);
}
//===============================================
bool X_SpectrumArray::xp_setChemElementName(int row, const QString& name)
{
    return xv_chemElementList.xp_setChemElementName(row, name);
}
//===============================================
QString X_SpectrumArray::xp_chemConcentration(qint64 chemElementId,
                                             int spectrumIndex) const
{
    if(spectrumIndex < 0 || spectrumIndex >= xp_spectrumCount()
            || !xv_chemElementList.xp_containsElementId(chemElementId))
    {
        return QString();
    }

    return xv_spectrumList.at(spectrumIndex)->xp_concentrationString(chemElementId);
}
//===============================================
qreal X_SpectrumArray::xp_chemConcentrationValue(qint64 chemElementId,
                                                int spectrumIndex) const
{
    bool ok;
    QLocale locale;
    qreal value = locale.toDouble(xp_chemConcentration(chemElementId, spectrumIndex), &ok);
    if(!ok)
    {
        return 0.0;
    }

    return value;
}
//===============================================
bool X_SpectrumArray::xp_setChemConcentration(qint64 chemElementId,
                                             int spectrumIndex, const QString& concentration)
{
    if(spectrumIndex < 0 || spectrumIndex >= xp_spectrumCount()
            || !xv_chemElementList.xp_containsElementId(chemElementId))
    {
        return false;
    }

    bool res = xv_spectrumList.at(spectrumIndex)->xp_setConcentration(chemElementId, concentration);
    if(res)
    {
        emit xg_spectrumOperation(X_SpectrumArray::OT_CONCENTRATION_CHANGED, spectrumIndex, spectrumIndex);
    }

    return res;
}
//===============================================
bool X_SpectrumArray::xp_energyCalibration(int spectrumIndex, QList<double>& energyCalibration, QString& energyUnit) const
{
    if(spectrumIndex < 0 || spectrumIndex >= xv_spectrumList.count())
    {
        return false;
    }

    X_SpeSpectrum* spectrum = qobject_cast<X_SpeSpectrum*>(xv_spectrumList.at(spectrumIndex));
    if(!spectrum)
    {
        return false;
    }

    energyCalibration = spectrum->xp_energyCalibration();
    energyUnit = xv_energyUnit;
    return true;
}
//===============================================
int X_SpectrumArray::xp_gainFactor() const
{
    return xv_gainFactor;
}
//===============================================
bool X_SpectrumArray::xp_setGainFactor(int gainFactor)
{
    if(gainFactor < 0 || gainFactor > glMaxGainFactor)
    {
        return false;
    }
    xv_gainFactor = gainFactor;
    return true;
}
//===============================================
int X_SpectrumArray::xp_exposition() const
{
    return xv_exposition;
}
//===============================================
QString X_SpectrumArray::xp_spectrumFileName(int index) const
{
    if(index < 0 || index >= xv_spectrumList.count())
    {
        return QString();
    }
    return xv_spectrumList.at(index)->xp_name();
}
//===============================================
qint64 X_SpectrumArray::xp_spectrumId(int index) const
{
    if(index < 0 || index >= xv_spectrumList.count())
    {
        return -1;
    }
    return xv_spectrumList.at(index)->xp_spectrumId();
}
//===============================================
QColor X_SpectrumArray::xp_spectrumColor(int index) const
{
    if(index < 0 || index >= xv_spectrumList.count())
    {
        return QColor();
    }
    return xv_spectrumList.at(index)->xp_color();
}
//===============================================
QList<quint32> X_SpectrumArray::xp_spectrumData(int index) const
{
    if(index < 0 || index >= xv_spectrumList.count())
    {
        return QList<quint32>();
    }
    return xv_spectrumList.at(index)->xp_spectrumData();
}
//===============================================
bool X_SpectrumArray::xp_isSpectrumVisible(int index) const
{
    if(index < 0 || index >= xv_spectrumList.count())
    {
        return false;
    }

    return xv_spectrumList.at(index)->xp_isSpectrumVisible();
}
//===============================================
bool X_SpectrumArray::xp_setSpectrumVisible(int index, bool visible)
{
    if(index < 0 || index >= xv_spectrumList.count())
    {
        return false;
    }

    xv_spectrumList[index]->xp_setSpectrumVisible(visible);
    return true;
}
//===============================================
bool X_SpectrumArray::xp_isSpectrumChecked(int index) const
{
    if(index < 0 || index >= xv_spectrumList.count())
    {
        return false;
    }

    return xv_spectrumList.at(index)->xp_isSpectrumChecked();
}
//===============================================
bool X_SpectrumArray::xp_setSpectrumChecked(int index, bool checked)
{
    if(index < 0 || index >= xv_spectrumList.count())
    {
        return false;
    }

    bool res = xv_spectrumList.at(index)->xp_setSpectrumChecked(checked);
    if(res)
    {
        emit xg_spectrumOperation(OT_CHANGED, index, index);
        //        xh_calcAverageChemConcentrations();
    }
    return res;
}
//===============================================
bool X_SpectrumArray::xp_removeSpectrum(int index)
{
    if(index < 0 || index >= xv_spectrumList.count())
    {
        return false;
    }
    emit xg_spectrumOperation(OT_REMOVE_SPECTRA, index, index);
    delete xv_spectrumList.at(index);
    xv_spectrumList.removeAt(index);
    emit xg_spectrumOperation(OT_END_REMOVE_SPECTRA, index, index);

    xh_recalcArrayMaxParameters();
    //   xh_calcAverageChemConcentrations();

    if(xv_spectrumList.count() < 1 )
    {
        xv_energyUnit = QString();
        xv_energyK0 = 0.0;
        xv_energyK1 = 0.0;
        xv_energyK2 = 0.0;
        xv_exposition = -1;
    }
    return true;
}
//===============================================
void X_SpectrumArray::xp_clearArray()
{
    if(!xv_spectrumList.isEmpty())
    {
        int lastSpectrumIndex = xv_spectrumList.count() - 1;
        emit xg_spectrumOperation(OT_REMOVE_SPECTRA, 0, lastSpectrumIndex);
        //        qDeleteAll(xv_spectrumList);
        //        xv_spectrumList.clear();
        for(int i = xv_spectrumList.count()-1; i >= 0; i-- )
        {
            delete xv_spectrumList.takeAt(i);
        }

        emit xg_spectrumOperation(OT_END_REMOVE_SPECTRA,  0, lastSpectrumIndex);

        xh_recalcArrayMaxParameters();
        //        xh_calcAverageChemConcentrations();
    }
}
//===============================================
void X_SpectrumArray::xp_clearChemElements()
{
    xv_chemElementList.xp_clearList();
}
//===============================================
bool X_SpectrumArray::xp_appendSpectrum(const X_RawSpectrum& rawSpectrum, bool last, bool& dontAsk, bool& continueAppend)
{
    QFileInfo fileInfo(rawSpectrum.path);
    X_AbstractSpectrumIOHandler* ioHandler;

    QString suffix = fileInfo.suffix();
    if(!fileInfo.exists() || !fileInfo.isFile())
    {
        QString error = tr("Error: \"%1\" is not a file!").arg(rawSpectrum.path);
        emit xg_message(error);
        qCritical().noquote() <<  error;
        return false;
    }
    else if(suffix == "spe")
    {
        ioHandler = new X_SpeIOHandler(this, this);
    }
    else
    {
        QString error = QObject::tr("Cannot handle file of type \"%1\"!").arg(suffix);
        emit xg_message(error);
        qCritical().noquote() <<  error;
        return false;
    }

    X_AbstractSpectrum* abstractSpectrum;
    bool res = ioHandler->xp_getSpectrumFromFile(rawSpectrum.path, xv_colorList.value(xv_lastColorIndex), abstractSpectrum);
    if(res)
    {
        X_SpeSpectrum* speSpectrum = qobject_cast<X_SpeSpectrum*>(abstractSpectrum);
        if(xp_isEnergyCalibrationValid() && !dontAsk)
        {
            QStringList inconsistenciesList = speSpectrum->xp_isEnergyCalibrationAndExpositionSuitable(xv_energyUnit, xv_energyK0, xv_energyK1, xv_energyK2, xv_exposition);
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
            if(xv_spectrumList.count() < 1)
            {
                // first spectrum. storing energy calibration

                const X_SpeAuxData* auxData = static_cast<const X_SpeAuxData*>(speSpectrum->xp_auxData());
                xv_energyUnit = auxData->xp_energyUnit();
                xv_energyK0 = auxData->xp_energyK0();
                xv_energyK1 = auxData->xp_energyK1();
                xv_energyK2 = auxData->xp_energyK2();

                xv_exposition = auxData->xp_exposition();
                xv_gainFactor = static_cast<int>(auxData->xp_gainFactor());

            }

            int spectrumIndex = xv_spectrumList.count();
            emit xg_spectrumOperation(OT_INSERT_SPECTRA, spectrumIndex, spectrumIndex);
            xv_spectrumList.append(speSpectrum);
            emit xg_spectrumOperation(OT_END_INSERT_SPECTRA, spectrumIndex, spectrumIndex);

            speSpectrum->xp_setSpectrumChecked(rawSpectrum.checked);
            speSpectrum->xp_setSpectrumVisible(rawSpectrum.visible);

            // color index increment
            if(++xv_lastColorIndex >= xv_colorList.count())
            {
                xv_lastColorIndex = 0;
            }

            QMap<QString, QString>::const_iterator it;
            qint64 chemElementId = -1;

            for(it = rawSpectrum.concentrationMap.begin(); it != rawSpectrum.concentrationMap.end(); it++)
            {
                if(!xv_chemElementList.xp_containsElement(it.key()))
                {
                    xv_chemElementList.xp_appendElement(it.key(), chemElementId);
                }
                else
                {
                    for(int i = 0; i < xv_chemElementList.xp_chemElementCount(); i++)
                    {
                        if(xv_chemElementList.xp_chemElementName(i) == it.key())
                        {
                            chemElementId = xp_chemElementId(i);
                            break;
                        }
                    }
                }

                speSpectrum->xp_insertConcentration(chemElementId, it.value());
            }

            //            connect(speSpectrum, &X_SpeSpectrum::xg_energyCalibrationChanged,
            //                    this, &X_SpectrumArray::xh_saveSpectrumToFile);
            connect(speSpectrum, &X_SpeSpectrum::xg_energyCalibrationChanged,
                    this, &X_SpectrumArray::xh_onSpectrumEnergyCalibrationChange);
            connect(speSpectrum, &X_SpeSpectrum::xg_gainFactorChanged,
                    this, &X_SpectrumArray::xh_onSpectrumGainFactorChange);
        }
    }

    delete ioHandler;
    xh_recalcArrayMaxParameters();
    return res;
}
//===============================================
void X_SpectrumArray::xh_onSpectrumEnergyCalibrationChange() const
{
    X_SpeSpectrum* spectrum = qobject_cast<X_SpeSpectrum*>(sender());
    if(!spectrum)
    {
        return;
    }

    xh_saveSpectrumToFile(spectrum);

    bool ok = false;
    int spectrumIndex = xh_indexForSpectrum(spectrum, &ok);

    if(ok)
    {
        emit xg_spectrumOperation(X_SpectrumArray::OT_SPECTRUM_ENERGY_CALIBRATION_CHANGED, spectrumIndex, spectrumIndex);
    }
}
//===============================================
void X_SpectrumArray::xh_onSpectrumGainFactorChange() const
{
    X_SpeSpectrum* spectrum = qobject_cast<X_SpeSpectrum*>(sender());
    if(!spectrum)
    {
        return;
    }

    xh_saveSpectrumToFile(spectrum);
}
//===============================================
void X_SpectrumArray::xh_saveSpectrumToFile(X_SpeSpectrum* spectrum) const
{
    X_SpeIOHandler speIOHandler(nullptr);

    QFileInfo fileInfo(spectrum->xp_path());
    QString folderPath = fileInfo.absolutePath();
    QString fileName = fileInfo.fileName();

    speIOHandler.xp_saveSpectrumToFile(folderPath, fileName, spectrum);
}
//===============================================
int X_SpectrumArray::xh_indexForSpectrum(X_SpeSpectrum* spectrum, bool* ok) const
{
    for(int i = 0; i < xv_spectrumList.count(); i++)
    {
        if(xv_spectrumList.at(i) == spectrum)
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
bool X_SpectrumArray::xp_appendNewChemElement(QString chemElement)
{
    int nextElementIndex = xv_chemElementList.xp_chemElementCount() + 1;
    if(chemElement.isEmpty())
    {
        chemElement = "Element #";
        while(xv_chemElementList.xp_containsElement(chemElement+QString::number(nextElementIndex)))
        {
            nextElementIndex++;
        }
        chemElement += QString::number(nextElementIndex);
    }
    else if(xv_chemElementList.xp_containsElement(chemElement))
    {
        chemElement += " #";
        while(xv_chemElementList.xp_containsElement(chemElement+QString::number(nextElementIndex)))
        {
            nextElementIndex++;
        }
        chemElement += QString::number(nextElementIndex);
    }

    qint64 chemElementId;
    bool res = xv_chemElementList.xp_appendElement(chemElement, chemElementId);
    if(res)
    {
        for(int s = 0; s < xv_spectrumList.count(); s++)
        {
            xv_spectrumList[s]->xp_insertConcentration(chemElementId, QString::number(0));
        }
    }
    return res;
}
//===============================================
bool X_SpectrumArray::xp_removeChemElement(int chemElementIndex)
{
    if(chemElementIndex < 0 || chemElementIndex >= xv_chemElementList.xp_chemElementCount())
    {
        return false;
    }

    QString chemElement = xv_chemElementList.xp_chemElementName(chemElementIndex);
    if(chemElement.isEmpty())
    {
        return false;
    }

    qint64 chemElementId;
    bool res = xv_chemElementList.xp_removeElement(chemElement, chemElementId);
    if(res)
    {
        for(int s  = 0; s < xv_spectrumList.count(); s++)
        {
            xv_spectrumList[s]->xp_removeConcentration(chemElementId);
        }
    }
    return res;
}
//===============================================
const X_AbstractSpectrum *X_SpectrumArray::xp_spectrum(int index) const
{
    if(index < 0 || index >= xv_spectrumList.count())
    {
        return nullptr;
    }

    return xv_spectrumList.at(index);
}
//===============================================
QList<X_AbstractSpectrum*> X_SpectrumArray::xp_spectrumList() const
{
    QList<X_AbstractSpectrum*> spectrumList;

    for(int s = 0; s < xv_spectrumList.count(); s++)
    {
        spectrumList << xv_spectrumList.value(s);
    }
    return spectrumList;
}
//===============================================
int X_SpectrumArray::xp_maxIntensity() const
{
    return xv_maxArrayIntensity;
}
//===============================================
int X_SpectrumArray::xp_maxChannelCount() const
{
    return xv_maxArrayChannelCount;
}
//===============================================
bool X_SpectrumArray::xp_isEnergyCalibrationValid() const
{
    return !(xv_energyK1 == 0.0 && xv_energyK2 == 0.0);
}
//===============================================
qint64 X_SpectrumArray::xp_arrayId() const
{
    return xv_arrayId;
}
//===============================================
//bool X_SpectrumArray::xp_averageChemConcentrationForChemElementId(qint64 chemElementId, qreal& averageValue) const
//{
//    return xv_chemElementList.xp_averageChemConcentrationForChemElementId(chemElementId, averageValue);
//}
////===============================================
//bool X_SpectrumArray::xp_averageChemConcentration(const QString& chemElement, qreal& averageValue) const
//{
//    return xv_chemElementList.xp_averageChemConcentration(chemElement, averageValue);
//}
////===============================================
//bool X_SpectrumArray::xp_averageChemConcentration(int chemElementIndex, qreal& averageValue) const
//{
//    return xv_chemElementList.xp_averageChemConcentration(chemElementIndex, averageValue);
//}
//===============================================
void X_SpectrumArray::xh_createConnections()
{
    connect(&xv_chemElementList, &X_ChemElementList::xg_operationType,
            this, &X_SpectrumArray::xg_chemElementOperation);
}
//===============================================
void X_SpectrumArray::xh_recalcArrayMaxParameters()
{
    xv_maxArrayChannelCount = 0;
    xv_maxArrayIntensity = 0;

    if(!xv_spectrumList.isEmpty())
    {
        for(int s = 0; s < xv_spectrumList.count(); s++)
        {
            if(xv_maxArrayChannelCount < xv_spectrumList.at(s)->xp_channelCount())
            {
                xv_maxArrayChannelCount = xv_spectrumList.at(s)->xp_channelCount();
            }

            if(xv_maxArrayIntensity < xv_spectrumList.at(s)->xp_maxIntensity())
            {
                xv_maxArrayIntensity = xv_spectrumList.at(s)->xp_maxIntensity();
            }
        }
    }

    emit xg_arrayMaxParametersChanged(xv_arrayId,  xv_maxArrayIntensity,  xv_maxArrayChannelCount);
}
//===============================================
//bool X_SpectrumArray::xh_calcAverageChemConcentration(qint64 chemElementId)
//{
//    if(!xv_chemElementList.xp_containsElementId(chemElementId))
//    {
//#ifdef DBG
//        qDebug() << "X_SpectrumArray: AVERAGE NOT RECALCED FOR CHEM ELEMENT ID" << chemElementId;
//#endif

//        return false;
//    }

//    if(xv_spectrumList.isEmpty())
//    {
//#ifdef DBG
//        qDebug() << "X_SpectrumArray: AVERAGE NOT RECALCED FOR CHEM ELEMENT ID" << chemElementId << "SPE LIST IS EMPTY";
//#endif

//        xv_chemElementList.xp_setAverageChemConcentration(chemElementId, 0.0);
//        return true;
//    }

//    qreal averageValue = 0;
//    bool ok;
//    qreal currentConcentration;
//    int checkedSpectrumCount = 0;
//    for(int s  = 0; s < xv_spectrumList.count(); s++)
//    {
//        if(!xv_spectrumList.at(s)->xp_isSpectrumChecked())
//        {
//            continue;
//        }
//        currentConcentration = xv_spectrumList.at(s)->xp_concentration(chemElementId).toDouble(&ok);
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
//        qDebug() << "X_SpectrumArray: AVERAGE NOT RECALCED FOR CHEM ELEMENT ID" << chemElementId << "CHECKED SPE COUNT IS X_ERO";
//#endif

//        xv_chemElementList.xp_setAverageChemConcentration(chemElementId, 0.0);
//        return true;
//    }
//    averageValue /= checkedSpectrumCount;
//    xv_chemElementList.xp_setAverageChemConcentration(chemElementId, averageValue);

//#ifdef DBG
//    qDebug() << "X_SpectrumArray: AVERAGE RECALCED FOR CHEM ELEMENT ID" << chemElementId << averageValue;
//#endif

//    return true;
//}
////===============================================
//void X_SpectrumArray::xh_calcAverageChemConcentrations()
//{
//#ifdef DBG
//    qDebug() << "X_SpectrumArray: START FULL CHEM AVERAGE RECALC ";
//#endif

//    if(xv_chemElementList.xp_isEmpty())
//    {
//        return;
//    }

//    for(int e = 0; e < xv_chemElementList.xp_chemElementCount(); e++)
//    {
//        xh_calcAverageChemConcentration(xv_chemElementList.xp_chemElementId(e));
//    }
//}
//===============================================


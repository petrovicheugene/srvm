//===========================================================
#include "ZSpeIOHandler.h"
#include "ZConstants.h"

#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDevice>
#include "ZSpeSpectrum.h"
//===========================================================
ZSpeIOHandler::ZSpeIOHandler(QObject *spectrumParent, QObject* parent)
    : ZAbstractSpectrumIOHandler(spectrumParent, parent)
{
    zv_whatToDoAnswer = -1;
}
//===========================================================
ZSpeIOHandler::~ZSpeIOHandler()
{

}
//===========================================================
bool ZSpeIOHandler::zp_getSpectrumFromFile(const QString& path, QColor color, ZAbstractSpectrum*& spectrum) const
{
    QFileInfo fileInfo(path);
    if(!fileInfo.exists() || !fileInfo.isFile())
    {
        QString errorString = tr("File \"%1\" doesn't exist!").arg(path);
        emit zg_message(errorString);
        return false;
    }

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        QString errorMsg;
        if(file.error() != QFile::NoError)
        {
            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
        }
        else
        {
            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), tr("Unknown error"));
        }
        emit zg_message(errorMsg);
        return false;
    }

    return zp_getSpectrumFromFile(file, color, spectrum);
}
//===========================================================
bool ZSpeIOHandler::zp_getSpectrumFromFile(QFile& file, QColor color, ZAbstractSpectrum*& spectrum) const
{
    if(!file.isOpen())
    {
        QString errorMsg;
        errorMsg = tr("Cannot read file \"%1\"! The file is not open!").arg(file.fileName());
        emit zg_message(errorMsg);
        return false;
    }
    if(!(file.openMode() & QIODevice::ReadOnly))
    {
        QString errorMsg;
        errorMsg = tr("Cannot read file \"%1\"! The file is not in \"Read\" mode!").arg(file.fileName());
        emit zg_message(errorMsg);
        return false;
    }

    QTextStream ts(&file);
    int lineNumber = 0;
    QList<quint32> intensityList;
    ZSpeAuxData speAuxData;
    bool ok;
    while (!ts.atEnd())
    {
        QString line = ts.readLine();
        if(lineNumber >= zv_intensityStartLine)
        {
            int intensity = line.toInt(&ok);
            if(!ok)
            {
                intensity = 0;
            }
            intensityList.append(intensity);
        }
        else // auxData
        {
            if(lineNumber == 1)
            {
                speAuxData.zp_setDate(line);
            }
            else if(lineNumber == 2)
            {
                speAuxData.zp_setTime(line);
            }
            else if(lineNumber == 3)
            {
                speAuxData.zp_setExposition(line);
            }
            else if(lineNumber == 4)
            {
                speAuxData.zp_setAliveTime(line);
            }
            else if(lineNumber == 5)
            {
                speAuxData.zp_setGainFactor(line);
            }
            else if(lineNumber == 10)
            {
                speAuxData.zp_setEnergyUnit(line);
            }
            else if(lineNumber == 11)
            {
                speAuxData.zp_setEnergyK0(line);
            }
            else if(lineNumber == 12)
            {
                speAuxData.zp_setEnergyK1(line);
            }
            else if(lineNumber == 13)
            {
                speAuxData.zp_setEnergyK2(line);
            }
            else if(lineNumber == 14)
            {
                speAuxData.zp_setPeakWidth(line);
            }
            else if(lineNumber == 15)
            {
                speAuxData.zp_setPeakWidthK0(line);
            }
            else if(lineNumber == 16)
            {
                speAuxData.zp_setPeakWidthK1(line);
            }
            else if(lineNumber == 17)
            {
                speAuxData.zp_setPeakWidthK2(line);
            }
        }
        lineNumber++;
    }

    if(intensityList.isEmpty())
    {
        QString errorMsg;
        errorMsg = tr("Cannot get intensity from file \"%1\"!").arg(file.fileName());
        emit zg_message(errorMsg);
        return false;
    }

    spectrum = new ZSpeSpectrum(intensityList,
                                speAuxData,
                                file.fileName(),
                                color,
                                true,
                                zv_spectrumParent);
    return true;
}
//===========================================================
bool ZSpeIOHandler::zp_saveSpectrumToFile(const QString& path,
                                          const QString& fileName,
                                          ZSpeSpectrum*& spectrum)
{
    // if directory doesn't exist create it
    if(!QDir(path).exists())
    {
        if(!QDir().mkpath(path))
        {
            QString errorMsg;
            errorMsg = tr("Cannot create directory \"%1\"!").arg(path);
            emit zg_message(errorMsg);
            return false;
        }
    }

    QFileInfo fileInfo(QDir(path),fileName);
    if(fileInfo.exists() && zv_whatToDoAnswer != QMessageBox::YesToAll )
    {
        if(zv_whatToDoAnswer == QMessageBox::NoToAll)
        {
            return true;
        }

        QMessageBox msgBox;
        msgBox.setText(tr("The file \"%1\" is already exists.").arg(fileInfo.absoluteFilePath()));
        msgBox.setInformativeText("Do you want to update it?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::YesToAll
                                  | QMessageBox::No | QMessageBox::NoToAll);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        zv_whatToDoAnswer = ret;

        switch(ret)
        {
        case QMessageBox::Yes:
            break;
        case QMessageBox::YesToAll:
            break;
        case QMessageBox::No:
            return true;
            break;
        case QMessageBox::NoToAll:
            return true;
            break;
        }
    }

    QFile file(fileInfo.absoluteFilePath());
    if(!file.open(QIODevice::WriteOnly))
    {
        QString errorMsg;
        if(file.error() != QFile::NoError)
        {
            errorMsg = tr("Cannot write to file \"%1\"! %2").arg(fileInfo.absoluteFilePath(), file.errorString());
        }
        else
        {
            errorMsg = tr("Cannot write to file \"%1\"! %2").arg(fileInfo.absoluteFilePath(), tr("Unknown error"));
        }
        emit zg_message(errorMsg);
        return false;
    }

    return zp_saveSpectrumToFile(file, spectrum);

}
//===========================================================
bool ZSpeIOHandler::zp_saveSpectrumToFile(QFile& file, ZSpeSpectrum*& spectrum)
{
    if(!file.isOpen())
    {
        QString errorMsg;
        errorMsg = tr("Cannot write to file \"%1\"! The file is not open!").arg(file.fileName());
        emit zg_message(errorMsg);
        return false;
    }
    if(!(file.openMode() & QIODevice::WriteOnly))
    {
        QString errorMsg;
        errorMsg = tr("Cannot write to file \"%1\"! The file is not in \"Write\" mode!").arg(file.fileName());
        emit zg_message(errorMsg);
        return false;
    }

    QTextStream ts(&file);
    // spe header
    for(int i = 0; i < 20; i++)
    {
        switch(i)
        {
        case 0:
            ts << QString::number(0);
            break;
        case 1:
            ts << endl << spectrum->zp_speAuxData()->zp_date().toString("dd.MM.yyyy");
            break;
        case 2:
            ts << endl << spectrum->zp_speAuxData()->zp_time().toString("hh.mm.ss");
            break;
        case 3:
            ts << endl << QString::number(spectrum->zp_speAuxData()->zp_exposition());
            break;
        case 4:
            ts << endl << QString::number(spectrum->zp_speAuxData()->zp_aliveTime());
            break;
        case 5:
            ts << endl << QString::number(spectrum->zp_speAuxData()->zp_gainFactor());
            break;
        case 6:
            ts << endl << QString("comment");
            break;
        case 7:
            ts << endl << QString("comment");
            break;
        case 8:
            ts << endl << QString("comment");
            break;
        case 9:
            ts << endl << QString("comment");
            break;
        case 10:
            ts << endl << spectrum->zp_speAuxData()->zp_energyUnit();
            break;
        case 11:
            ts << endl << QString::number(spectrum->zp_speAuxData()->zp_energyK0());
            break;
        case 12:
            ts << endl << QString::number(spectrum->zp_speAuxData()->zp_energyK1());
            break;
        case 13:
            ts << endl << QString::number(spectrum->zp_speAuxData()->zp_energyK2());
            break;
        case 14:
            ts << endl << QString::number(spectrum->zp_speAuxData()->zp_peakWidth());
            break;
        case 15:
            ts << endl << QString::number(spectrum->zp_speAuxData()->zp_peakWidthK0());
            break;
        case 16:
            ts << endl << QString::number(spectrum->zp_speAuxData()->zp_peakWidthK1());
            break;
        case 17:
            ts << endl << QString::number(spectrum->zp_speAuxData()->zp_peakWidthK2());
            break;
        case 18:
            ts << endl << QString("info");
            break;
        case 19:
            ts << endl << QString("info");
            break;
         }
    }

    // spectrum data
    QList<quint32> spectrumData;
    spectrum->zp_setSpectrumData(spectrumData);

    foreach(quint32 intensity, spectrumData)
    {
        ts << QString::number(intensity, 10);
    }

    file.close();
    return true;
//    int lineNumber = 0;
//    bool ok;
//    while (!ts.atEnd())
//    {
//        QString line = ts.readLine();
//        if(lineNumber >= zv_intensityStartLine)
//        {
//            int intensity = line.toInt(&ok);
//            if(!ok)
//            {
//                intensity = 0;
//            }
//            intensityList.append(intensity);
//        }
//        else // auxData



}
//===========================================================

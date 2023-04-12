//===========================================================
#include "X_SpeIOHandler.h"
#include "X_General.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QFileDevice>
#include "X_SpeSpectrum.h"
#include <QMessageBox>
//===========================================================
X_SpeIOHandler::X_SpeIOHandler(QObject *spectrumParent, QObject* parent)
    : X_AbstractSpectrumIOHandler(spectrumParent, parent)
{
    xv_whatToDoAnswer = -1;
}
//===========================================================
X_SpeIOHandler::~X_SpeIOHandler()
{

}
//===========================================================
bool X_SpeIOHandler::xp_getSpectrumFromFile(const QString& path, QColor color, X_AbstractSpectrum*& spectrum) const
{
    QFileInfo fileInfo(path);
    if(!fileInfo.exists() || !fileInfo.isFile())
    {
        QString errorString = tr("File \"%1\" doesn't exist!").arg(path);
        emit xg_message(errorString);
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
        emit xg_message(errorMsg);
        return false;
    }

    return xp_getSpectrumFromFile(file, color, spectrum);
}
//===========================================================
bool X_SpeIOHandler::xp_getSpectrumFromFile(QFile& file, QColor color, X_AbstractSpectrum*& spectrum) const
{
    if(!file.isOpen())
    {
        QString errorMsg;
        errorMsg = tr("Cannot read file \"%1\"! The file is not open!").arg(file.fileName());
        emit xg_message(errorMsg);
        return false;
    }
    if(!(file.openMode() & QIODevice::ReadOnly))
    {
        QString errorMsg;
        errorMsg = tr("Cannot read file \"%1\"! The file is not in \"Read\" mode!").arg(file.fileName());
        emit xg_message(errorMsg);
        return false;
    }

    QTextStream ts(&file);
    int lineNumber = 0;
    QList<quint32> intensityList;
    X_SpeAuxData speAuxData;
    bool ok;
    while (!ts.atEnd())
    {
        QString line = ts.readLine();
        if(lineNumber >= xv_intensityStartLine)
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
            if(lineNumber == 0)
            {
                speAuxData.xp_setSrvN(line);
            }
            else if(lineNumber == 1)
            {
                speAuxData.xp_setDate(line);
            }
            else if(lineNumber == 2)
            {
                speAuxData.xp_setTime(line);
            }
            else if(lineNumber == 3)
            {
                speAuxData.xp_setExposition(line);
            }
            else if(lineNumber == 4)
            {
                speAuxData.xp_setAliveTime(line);
            }
            else if(lineNumber == 5)
            {
                speAuxData.xp_setGainFactor(line);
            }

            // comments
            else if(lineNumber == 6)
            {
                speAuxData.xp_setComment(0, line);
            }
            else if(lineNumber == 7)
            {
                speAuxData.xp_setComment(1, line);
            }
            else if(lineNumber == 8)
            {
                speAuxData.xp_setComment(2, line);
            }
            else if(lineNumber == 9)
            {
                speAuxData.xp_setComment(3, line);
            }
            // end comments

            else if(lineNumber == 10)
            {
                speAuxData.xp_setEnergyUnit(line);
            }
            else if(lineNumber == 11)
            {
                speAuxData.xp_setEnergyK0(line);
            }
            else if(lineNumber == 12)
            {
                speAuxData.xp_setEnergyK1(line);
            }
            else if(lineNumber == 13)
            {
                speAuxData.xp_setEnergyK2(line);
            }
            else if(lineNumber == 14)
            {
                speAuxData.xp_setPeakWidth(line);
            }
            else if(lineNumber == 15)
            {
                speAuxData.xp_setPeakWidthK0(line);
            }
            else if(lineNumber == 16)
            {
                speAuxData.xp_setPeakWidthK1(line);
            }
            else if(lineNumber == 17)
            {
                speAuxData.xp_setPeakWidthK2(line);
            }
        }
        lineNumber++;
    }

    spectrum = new X_SpeSpectrum(intensityList,
                                speAuxData,
                                file.fileName(),
                                color,
                                true,
                                xv_spectrumParent);
    return true;
}
//===========================================================
bool X_SpeIOHandler::xp_saveSpectrumToFile(const QString& path,
                                          const QString& fileName,
                                          X_SpeSpectrum* spectrum,
                                          bool dontAsk)
{
    // if directory doesn't exist create it
    if(!QDir(path).exists())
    {
        if(!QDir().mkpath(path))
        {
            QString errorMsg;
            errorMsg = tr("Cannot create directory \"%1\"!").arg(path);
            emit xg_message(errorMsg);
            return false;
        }
    }

    QString fileNameWithSuffix = fileName;
    if(QFileInfo(fileName).suffix() != "spe")
    {
        fileNameWithSuffix += ".spe";
    }

    QFileInfo fileInfo(QDir(path),fileNameWithSuffix);
    if(fileInfo.exists() && xv_whatToDoAnswer != QMessageBox::YesToAll && !dontAsk)
    {
        if(xv_whatToDoAnswer == QMessageBox::NoToAll)
        {
            return true;
        }

        QMessageBox msgBox;
        msgBox.setText(tr("The file \"%1\" is already exists.").arg(fileInfo.absoluteFilePath()));
        msgBox.setInformativeText(tr("Do you want to update it?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::YesToAll
                                  | QMessageBox::No | QMessageBox::NoToAll);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        xv_whatToDoAnswer = ret;

        switch(ret)
        {
        case QMessageBox::Yes:
            break;
        case QMessageBox::YesToAll:
            break;
        case QMessageBox::No:
            return true;
        case QMessageBox::NoToAll:
            return true;
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
        emit xg_message(errorMsg);
        return false;
    }

    return xp_saveSpectrumToFile(file, spectrum);
}
//===========================================================
bool X_SpeIOHandler::xp_saveSpectrumToFile(QFile& file, X_SpeSpectrum* spectrum)
{
    if(!file.isOpen())
    {
        QString errorMsg;
        errorMsg = tr("Cannot write to file \"%1\"! The file is not open!").arg(file.fileName());
        emit xg_message(errorMsg);
        return false;
    }
    if(!(file.openMode() & QIODevice::WriteOnly))
    {
        QString errorMsg;
        errorMsg = tr("Cannot write to file \"%1\"! The file is not in \"Write\" mode!").arg(file.fileName());
        emit xg_message(errorMsg);
        return false;
    }

    QTextStream ts(&file);
    // spe header
    for(int i = 0; i < 20; i++)
    {
        switch(i)
        {
        case 0:
            ts << spectrum->xp_speAuxData()->xp_srvN();
            break;
        case 1:
            ts << Qt::endl << spectrum->xp_speAuxData()->xp_date().toString("dd.MM.yyyy");
            break;
        case 2:
            ts << Qt::endl << spectrum->xp_speAuxData()->xp_time().toString("hh:mm:ss");
            break;
        case 3:
            ts << Qt::endl << QString::number(spectrum->xp_speAuxData()->xp_exposition());
            break;
        case 4:
            ts << Qt::endl << QString::number(spectrum->xp_speAuxData()->xp_aliveTime());
            break;
        case 5:
            ts << Qt::endl << QString::number(spectrum->xp_speAuxData()->xp_gainFactor());
            break;

        case 6:
            ts << Qt::endl << spectrum->xp_speAuxData()->xp_comment(0);
            break;
        case 7:
            ts << Qt::endl << spectrum->xp_speAuxData()->xp_comment(1);
            break;
        case 8:
            ts << Qt::endl << spectrum->xp_speAuxData()->xp_comment(2);
            break;
        case 9:
            ts << Qt::endl << spectrum->xp_speAuxData()->xp_comment(3);
            break;

        case 10:
            ts << Qt::endl << spectrum->xp_speAuxData()->xp_energyUnit();
            break;
        case 11:
            ts << Qt::endl << QString::number(spectrum->xp_speAuxData()->xp_energyK0(), 'E', 14);
            break;
        case 12:
            ts << Qt::endl << QString::number(spectrum->xp_speAuxData()->xp_energyK1(), 'E', 14);
            break;
        case 13:
            ts << Qt::endl << QString::number(spectrum->xp_speAuxData()->xp_energyK2(), 'E', 14);
            break;
        case 14:
            ts << Qt::endl << QString::number(spectrum->xp_speAuxData()->xp_peakWidth());
            break;
        case 15:
            ts << Qt::endl << QString::number(spectrum->xp_speAuxData()->xp_peakWidthK0(), 'E', 14);
            break;
        case 16:
            ts << Qt::endl << QString::number(spectrum->xp_speAuxData()->xp_peakWidthK1(), 'E', 14);
            break;
        case 17:
            ts << Qt::endl << QString::number(spectrum->xp_speAuxData()->xp_peakWidthK2(), 'E', 14);
            break;
        case 18:
            ts << Qt::endl << QString("");
            break;
        case 19:
            ts << Qt::endl << QString("");
            break;
        }
    }

    // spectrum data
    QList<quint32> spectrumData = spectrum->xp_spectrumData();

    foreach(quint32 intensity, spectrumData)
    {
        ts << Qt::endl << QString::number(intensity, 10);
    }

    file.close();
    return true;
}
//===========================================================

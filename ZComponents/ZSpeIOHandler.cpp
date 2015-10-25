//===========================================================
#include "ZSpeIOHandler.h"
#include "glVariables.h"
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QFileDevice>
#include "ZSpeSpectrum.h"
//===========================================================
ZSpeIOHandler::ZSpeIOHandler(QObject *spectrumParent, QObject* parent)
    : ZAbstractSpectrumIOHandler(spectrumParent, parent)
{

}
//===========================================================
bool ZSpeIOHandler::zp_getSpectrumFromFile(const QString& path, ZAbstractSpectrum*& spectrum) const
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

    return zp_getSpectrumFromFile(file, spectrum);
}
//===========================================================
bool ZSpeIOHandler::zp_getSpectrumFromFile(QFile& file, ZAbstractSpectrum*& spectrum) const
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
    QList<int> intensityList;
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

//#ifdef DBG
//        qDebug() << "LN"<< lineNumber << line;
//#endif
        lineNumber++;
    }

    spectrum = new ZSpeSpectrum(intensityList, file.fileName(), zv_spectrumParent);
    return true;
}
//===========================================================
//bool ZSpeIOHandler::zp_getSpectrumFromFile(QFile& file, ZSpectrumArray&) const
//{

//}
//===========================================================

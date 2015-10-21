//===========================================================
#include "ZSpeIOHandler.h"
#include "glVariables.h"
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QFileDevice>
#include "ZSpeSpectrum.h"
//===========================================================
ZSpeIOHandler::ZSpeIOHandler(QObject* parent) : ZAbstractSpectrumIOHandler(parent)
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
    while (!ts.atEnd())
    {
        QString line = ts.readLine();
#ifdef DBG
        qDebug() << "LN"<< lineNumber << line;
#endif
        lineNumber++;
    }

    spectrum = new ZSpeSpectrum();
    return true;
}
//===========================================================
//bool ZSpeIOHandler::zp_getSpectrumFromFile(QFile& file, ZSpectrumArray&) const
//{

//}
//===========================================================

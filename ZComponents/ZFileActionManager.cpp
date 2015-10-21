//======================================================
#include "ZFileActionManager.h"
#include "ZAbstractArrayIOHandler.h"
#include "ZXMLArrayIOHandler.h"
#include "ZSpectrumArray.h"
#include "ZAbstractSpectrumIOHandler.h"
#include "ZSpeIOHandler.h"
#include "glVariables.h"

#include <QMenu>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
//======================================================
ZFileActionManager::ZFileActionManager(QObject *parent) : QObject(parent)
{
    zh_createActions();
    zh_createConnections();
}
//======================================================
void ZFileActionManager::zp_appendActionsToMenu(QMenu* menu) const
{
    if(menu->objectName() == "File")
    {
        menu->addAction(zv_openArrayFromFileAction);
        menu->addAction(zv_saveArrayToFileAction);
        menu->addAction(zv_saveArrayAsFileAction);
        menu->addSeparator();

        menu->addAction(zv_openCalibrationFromFileAction);
        menu->addAction(zv_saveCalibrationToFileAction);
        menu->addAction(zv_saveCalibrationAsFileAction);
        menu->addSeparator();

        menu->addAction(zv_loadSpectrumFromFileAction);
        menu->addSeparator();
    }
}
//======================================================
QList<QAction*> ZFileActionManager::zp_actionList() const
{
    QList<QAction*> actionList;

    actionList <<  zv_openArrayFromFileAction;
    actionList <<  zv_saveArrayToFileAction;
    actionList <<  zv_saveArrayAsFileAction;

    actionList <<  zv_openCalibrationFromFileAction;
    actionList <<  zv_saveCalibrationToFileAction;
    actionList <<  zv_saveCalibrationAsFileAction;

    actionList <<  zv_loadSpectrumFromFileAction;

    return actionList;
}
//======================================================
void ZFileActionManager::zh_createActions()
{
    zv_openArrayFromFileAction = new QAction(QIcon(":/images/document-open.png"), tr("&Open array"), this);
    zv_saveArrayToFileAction = new QAction(QIcon(":/images/document-open.png"), tr("&Save array"), this);
    zv_saveArrayAsFileAction = new QAction(QIcon(":/images/document-open.png"), tr("Save array &as..."), this);

    zv_openCalibrationFromFileAction = new QAction(QIcon(":/images/document-open.png"), tr("Open calibration"), this);
    zv_saveCalibrationToFileAction = new QAction(QIcon(":/images/document-open.png"), tr("Save calibration"), this);
    zv_saveCalibrationAsFileAction = new QAction(QIcon(":/images/document-open.png"), tr("Save calibration as..."), this);

    zv_loadSpectrumFromFileAction = new QAction(QIcon(":/images/document-open.png"), tr("Append &spectra to array"), this);
}
//======================================================
void ZFileActionManager::zh_createConnections()
{
    connect(zv_openArrayFromFileAction, &QAction::triggered,
            this, &ZFileActionManager::zh_onOpenArrayAction);
    connect(zv_saveArrayToFileAction, &QAction::triggered,
            this, &ZFileActionManager::zh_onSaveArrayAction);
    connect(zv_saveArrayAsFileAction, &QAction::triggered,
            this, &ZFileActionManager::zh_onSaveArrayAsAction);

    connect(zv_openCalibrationFromFileAction, &QAction::triggered,
            this, &ZFileActionManager::zh_onOpenCalibrationAction);
    connect(zv_saveCalibrationToFileAction, &QAction::triggered,
            this, &ZFileActionManager::zh_onSaveCalibrationAction);
    connect(zv_saveCalibrationAsFileAction, &QAction::triggered,
            this, &ZFileActionManager::zh_onSaveCalibrationAsAction);

    connect(zv_loadSpectrumFromFileAction, &QAction::triggered,
            this, &ZFileActionManager::zh_onOpenSpectraAction);
}
//======================================================
bool ZFileActionManager::zh_defineArrayFileName(QString& fileName) const
{
    // opening
    fileName = QFileDialog::getOpenFileName(0, tr("Select file to open"),
                                            zv_arrayFilePath,
                                            tr("XML files(*.xml);;All files(*.*)"));
    if(fileName.isEmpty())
    {
        return false;
    }

    QFileInfo fileInfo(fileName);
    if(!fileInfo.exists())
    {
        QString msg = tr("File \"%1\" does not exist!").arg(fileName);
        QMessageBox::critical(0, tr("File open error"), msg);
        emit zg_message(msg);
        return false;
    }

    if(fileInfo.isDir())
    {
        QString msg = tr("\"%1\" is a folder!").arg(fileName);
        QMessageBox::critical(0, tr("File open error"), msg);
        emit zg_message(msg);
        return false;
    }

    return true;
}
//======================================================
bool ZFileActionManager::zh_getRawSpectrumArrayFromFile(const QString& fileName, QList<ZRawArray>& rawArray)
{
    QFileInfo fileInfo(fileName);
    ZAbstractArrayIOHandler* ioHandler;

    // ioHandler subclass definition
    // here is an ability to using the other classes, that handle files of different formats
    if(!fileInfo.exists() || !fileInfo.isFile())
    {
        QString msg = tr("Error: \"%1\" is not a file!").arg(fileName);
        // QMessageBox::critical(0, tr("File handling error"), msg);
        emit zg_message(msg);
        return false;
    }
    else if(fileInfo.suffix() == "xml")
    {
        ioHandler = new ZXMLArrayIOHandler(this);
    }
    else // other suffixes
    {
        QString msg = tr("Error handling file \"%1\"! Cannot handle \"%2\" files.").arg(fileName, fileInfo.suffix());
        QMessageBox::critical(0, tr("File handling error"), msg);
        emit zg_message(msg);
        return false;
    }

    // file opening
    QFile file(fileName);
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

    // current directory  saving
    zv_arrayFilePath = fileInfo.absolutePath();

    // ability of message receiving
    connect(ioHandler, &ZAbstractArrayIOHandler::zg_message,
            this, &ZFileActionManager::zg_message);

    // array loading
    bool readArrayRes = ioHandler->zp_readSpectrumArray(rawArray, file);

    if(file.isOpen())
    {
        file.close();
    }
    delete ioHandler;

    return readArrayRes;
}
//======================================================
bool ZFileActionManager::zh_getSpectrumFromFile(const QString& fileName,
ZAbstractSpectrum*& spectrum)
{
#ifdef DBG
    qDebug() << "SP PATH:" << fileName;
#endif

    ZAbstractSpectrumIOHandler* ioHandler;
    QFileInfo fileInfo(fileName);
    // here is an ability to using the other classes, that handle files of different formats
    if(!fileInfo.exists() || !fileInfo.isFile())
    {
        QString msg = tr("Error: \"%1\" is not a file!").arg(fileName);
        emit zg_message(msg);
        return false;
    }
    else if(fileInfo.suffix() == "spe")
    {
        ioHandler = new ZSpeIOHandler(this);
    }
    else // other suffixes
    {
        QString msg = tr("Error handling file \"%1\"! Cannot handle \"%2\" files.").arg(fileName, fileInfo.suffix());
        emit zg_message(msg);
        return false;
    }

    QFile file(fileName);
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

    connect(ioHandler, &ZAbstractSpectrumIOHandler::zg_message,
            this, &ZFileActionManager::zg_message);

    // specrum loading
    bool readSpectrumRes = ioHandler->zp_getSpectrumFromFile(file, spectrum);

    if(file.isOpen())
    {
        file.close();
    }
    delete ioHandler;

    //    if(!readspectrumRes)
    //    {
    //        QString msg = tr("Data loading from file \"%1\" failed.").arg(fileName);
    //        QMessageBox::critical(0, tr("Loading error"), msg, QMessageBox::Ok);
    //        emit zg_message(msg);
    //        return false;
    //    }

    return readSpectrumRes;
}
//======================================================
void ZFileActionManager::zp_restoreSettings() const
{

}
//======================================================
void ZFileActionManager::zp_saveSettings() const
{

}
//======================================================
void ZFileActionManager::zh_onOpenArrayAction() // outputs RawArray
{
    QString fileName;
    if(!zh_defineArrayFileName(fileName))
    {
        return;
    }

    QList<ZRawArray> rawArrayList;
    if(!zh_getRawSpectrumArrayFromFile(fileName, rawArrayList))
    {
        QString msg = tr("Data loading from file \"%1\" failed.").arg(fileName);
        QMessageBox::critical(0, tr("Loading error"), msg, QMessageBox::Ok);
        emit zg_message(msg);
        return;
    }

#ifdef DBG
    foreach(ZRawArray ra, rawArrayList)
    {
        qDebug() << ra.name;
        for(int i = 0; i < ra.spectrumList.count(); i++)
        {
           // qDebug() << ra.spectrumList.value(i);
        }

        qDebug() << "********";

    }
#endif

//    // spectrum array list creation
//    QList<ZSpectrumArray> spectrumArrayList;
//    foreach(ZRawArray rawArray, rawArrayList)
//    {
//        ZSpectrumArray array;
//        array.zp_setArrayName(rawArray.name);

//        for(int s = 0; s < rawArray.pathList.count(); s++)
//        {
//            ZAbstractSpectrum* spectrum;
//            if(!zh_getSpectrumFromFile(rawArray.pathList.value(s), spectrum))
//            {
//                array.zp_appendSpectrum(spectrum);
//                // spectrum.clear();
//            }
//        }
//        spectrumArrayList << array;
//    }

//#ifdef DBG
//    qDebug() << "STOP";
//#endif

    //    ZSpectrumArray spectrumArray;
    //    if(!zh_getSpectrumArrayFromFile(fileName, spectrumArray))
    //    {
    //        QString msg = tr("Data loading from file \"%1\" failed.").arg(fileName);
    //        QMessageBox::critical(0, tr("Loading error"), msg, QMessageBox::Ok);
    //        emit zg_message(msg);
    //        return;
    //    }

    //    // spectra loading
    //    //ZAbstractSpectrum spectrum;
    //    for(int i = spectrumArray.zp_spectrumCount() -1; i >= 0; i--)
    //    {

    //#ifdef DBG
    //        qDebug() << spectrumArray.zp_spectrumFileName(i);
    //#endif
    //        QSharedPointer<ZAbstractSpectrum> spectrum;
    //        if(!zh_getSpectrumFromFile(spectrumArray.zp_spectrumFileName(i), spectrum))
    //        {
    //            spectrumArray.zp_removeSpectrum(i);
    //            continue;
    //        }

    //        // setting a loaded spectrum into current place
    //        if(!spectrum.isNull())
    //        {
    //            spectrumArray.zp_setSpectrumAtIndex(i, *spectrum);
    //            spectrum.clear();
    //        }
    //    }

    //    // TODO loading a loaded array to model
}
//======================================================
//bool ZFileActionManager::zh_getSpectrumArrayFromFile(const QString &fileName,
//                                                     ZSpectrumArray& spectrumArray)
//{
//    QFileInfo fileInfo(fileName);
//    ZAbstractArrayIOHandler* ioHandler;

//    // here is an ability to using the other classes, that handle files of different formats
//    if(!fileInfo.exists() || !fileInfo.isFile())
//    {
//        QString msg = tr("Error: \"%1\" is not a file!").arg(fileName);
//        // QMessageBox::critical(0, tr("File handling error"), msg);
//        emit zg_message(msg);
//        return false;
//    }
//    else if(fileInfo.suffix() == "xml")
//    {
//        ioHandler = new ZXMLArrayIOHandler(this);
//    }
//    else // other suffixes
//    {
//        QString msg = tr("Error handling file \"%1\"! Cannot handle \"%2\" files.").arg(fileName, fileInfo.suffix());
//        QMessageBox::critical(0, tr("File handling error"), msg);
//        emit zg_message(msg);
//        return false;
//    }

//    QFile file(fileName);
//    if(!file.open(QIODevice::ReadOnly))
//    {
//        QString errorMsg;
//        if(file.error() != QFile::NoError)
//        {
//            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
//        }
//        else
//        {
//            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), tr("Unknown error"));
//        }
//        emit zg_message(errorMsg);
//        return false;
//    }

//    zv_arrayFilePath = fileInfo.absolutePath();

//    connect(ioHandler, &ZAbstractArrayIOHandler::zg_message,
//            this, &ZFileActionManager::zg_message);

//    // array loading
//    bool readArrayRes = ioHandler->zp_readSpectrumArray(spectrumArray, file);

//    if(file.isOpen())
//    {
//        file.close();
//    }
//    delete ioHandler;

//    return readArrayRes;
//}
//======================================================
void ZFileActionManager::zh_onSaveArrayAction() const
{

}
//======================================================
void ZFileActionManager::zh_onSaveArrayAsAction() const
{

}
//======================================================
void ZFileActionManager::zh_onOpenCalibrationAction() const
{

}
//======================================================
void ZFileActionManager::zh_onSaveCalibrationAction() const
{

}
//======================================================
void ZFileActionManager::zh_onSaveCalibrationAsAction() const
{

}
//======================================================
void ZFileActionManager::zh_onOpenSpectraAction() const
{

}
//======================================================

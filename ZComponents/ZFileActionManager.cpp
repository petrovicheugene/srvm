//======================================================
#include "ZFileActionManager.h"
#include "ZXMLSpectrumArrayIOHandler.h"
#include "ZSpectrumArray.h"
#include "ZAbstractSpectrumIOHandler.h"
#include "ZXMLCalibrationProjectIOHandler.h"
#include "ZXMLCalibrationIOHandler.h"
#include "ZSpeIOHandler.h"
#include "ZCalibration.h"
#include "ZGeneral.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QMenu>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QSettings>
//======================================================
ZFileActionManager::ZFileActionManager(QObject *parent) : QObject(parent)
{
    zh_createActions();
    zh_createConnections();
    zh_restoreSettings();
}
//======================================================
void ZFileActionManager::zp_appendActionsToMenu(QMenu* menu) const
{
    if(menu->objectName() == "File")
    {
        menu->addAction(zv_openCalibrationProjectAction);
        menu->addSeparator();

        menu->addAction(zv_openArrayFromFileAction);
        menu->addAction(zv_saveArrayToFileAction);
        menu->addAction(zv_saveArrayAsFileAction);
        menu->addSeparator();

        menu->addAction(zv_openCalibrationFromFileAction);
        menu->addAction(zv_saveCalibrationToFileAction);
        menu->addAction(zv_saveCalibrationAsFileAction);
        menu->addSeparator();

    }
}
//======================================================
QList<QAction*> ZFileActionManager::zp_actionList() const
{
    QList<QAction*> actionList;

    actionList << zv_openCalibrationProjectAction;
    actionList <<  zv_openArrayFromFileAction;
    actionList <<  zv_saveArrayToFileAction;
    actionList <<  zv_saveArrayAsFileAction;

    actionList <<  zv_openCalibrationFromFileAction;
    actionList <<  zv_saveCalibrationToFileAction;
    actionList <<  zv_saveCalibrationAsFileAction;

    // actionList <<  zv_loadSpectrumFromFileAction;

    return actionList;
}
//======================================================
void ZFileActionManager::zp_initOpenCalibrationFromFileAction(QAction*& action) const
{
    action->setIcon(zv_openCalibrationFromFileAction->icon());
    action->setText(zv_openCalibrationFromFileAction->text());
    action->setIconText(zv_openCalibrationFromFileAction->iconText());

    action->setToolTip(zv_openCalibrationFromFileAction->toolTip());
    action->setCheckable(zv_openCalibrationFromFileAction->isCheckable());
    action->setChecked(zv_openCalibrationFromFileAction->isChecked());
    action->setEnabled(zv_openCalibrationFromFileAction->isEnabled());

    connect (action, &QAction::triggered,
             zv_openCalibrationFromFileAction, &QAction::trigger);
}
//======================================================
void ZFileActionManager::zp_initSaveCalibrationFromFileAction(QAction*& action) const
{
    action->setIcon(zv_saveCalibrationToFileAction->icon());
    action->setText(zv_saveCalibrationToFileAction->text());
    action->setIconText(zv_saveCalibrationToFileAction->iconText());

    action->setToolTip(zv_saveCalibrationToFileAction->toolTip());
    action->setCheckable(zv_saveCalibrationToFileAction->isCheckable());
    action->setChecked(zv_saveCalibrationToFileAction->isChecked());
    action->setEnabled(zv_saveCalibrationToFileAction->isEnabled());

    connect (action, &QAction::triggered,
             zv_saveCalibrationToFileAction, &QAction::trigger);
}
//======================================================
void ZFileActionManager::zp_initSaveAsCalibrationFromFileAction(QAction*& action) const
{
    action->setIcon(zv_saveCalibrationAsFileAction->icon());
    action->setText(zv_saveCalibrationAsFileAction->text());
    action->setIconText(zv_saveCalibrationAsFileAction->iconText());

    action->setToolTip(zv_saveCalibrationAsFileAction->toolTip());
    action->setCheckable(zv_saveCalibrationAsFileAction->isCheckable());
    action->setChecked(zv_saveCalibrationAsFileAction->isChecked());
    action->setEnabled(zv_saveCalibrationAsFileAction->isEnabled());

    connect (action, &QAction::triggered,
             zv_saveCalibrationAsFileAction, &QAction::trigger);

}
//======================================================
void ZFileActionManager::zp_initOpenSpectrumArrayAction(QAction*& action) const
{
    action->setIcon(zv_openArrayFromFileAction->icon());
    action->setText(zv_openArrayFromFileAction->text());
    action->setIconText(zv_openArrayFromFileAction->iconText());

    action->setToolTip(zv_openArrayFromFileAction->toolTip());
    action->setCheckable(zv_openArrayFromFileAction->isCheckable());
    action->setChecked(zv_openArrayFromFileAction->isChecked());
    action->setEnabled(zv_openArrayFromFileAction->isEnabled());

    connect (action, &QAction::triggered,
             zv_openArrayFromFileAction, &QAction::trigger);

}
//======================================================
void ZFileActionManager::zp_initSaveSpectrumArrayAction(QAction*& action) const
{
    action->setIcon(zv_saveArrayToFileAction->icon());
    action->setText(zv_saveArrayToFileAction->text());
    action->setIconText(zv_saveArrayToFileAction->iconText());

    action->setToolTip(zv_saveArrayToFileAction->toolTip());
    action->setCheckable(zv_saveArrayToFileAction->isCheckable());
    action->setChecked(zv_saveArrayToFileAction->isChecked());
    action->setEnabled(zv_saveArrayToFileAction->isEnabled());

    connect (action, &QAction::triggered,
             zv_saveArrayToFileAction, &QAction::trigger);

}
//======================================================
void ZFileActionManager::zp_initSaveAsSpectrumArrayAction(QAction*& action) const
{
    action->setIcon(zv_saveArrayAsFileAction->icon());
    action->setText(zv_saveArrayAsFileAction->text());
    action->setIconText(zv_saveArrayAsFileAction->iconText());

    action->setToolTip(zv_saveArrayAsFileAction->toolTip());
    action->setCheckable(zv_saveArrayAsFileAction->isCheckable());
    action->setChecked(zv_saveArrayAsFileAction->isChecked());
    action->setEnabled(zv_saveArrayAsFileAction->isEnabled());

    connect (action, &QAction::triggered,
             zv_saveArrayAsFileAction, &QAction::trigger);

}
//======================================================
void ZFileActionManager::zp_triggerSaveArrayToFileAction()
{
    zv_saveArrayToFileAction->trigger();
}
//======================================================
void ZFileActionManager::zp_triggerSaveCalibrationsToFile()
{
    zv_saveCalibrationToFileAction->trigger();
}
//======================================================
void ZFileActionManager::zh_createActions()
{
    zv_openArrayFromFileAction = new QAction(QIcon(NS_Icons::glIconOpenArray), tr("&Open spectrum array list"), this);
    zv_saveArrayToFileAction = new QAction(QIcon(NS_Icons::glIconSaveArray), tr("&Save spectrum array list"), this);
    zv_saveArrayToFileAction->setEnabled(false);
    zv_saveArrayAsFileAction = new QAction(QIcon(NS_Icons::glIconSaveAsArray), tr("Save spectrum array list &as..."), this);
    zv_saveArrayAsFileAction->setEnabled(false);

    zv_openCalibrationFromFileAction = new QAction(QIcon(NS_Icons::glIconOpenCalibration), tr("Open &calibrations"), this);
    zv_saveCalibrationToFileAction = new QAction(QIcon(NS_Icons::glIconSaveCalibration), tr("Save calibration"), this);
    zv_saveCalibrationToFileAction->setEnabled(false);
    zv_saveCalibrationAsFileAction = new QAction(QIcon(NS_Icons::glIconSaveAsCalibration), tr("Save calibration as..."), this);
    zv_saveCalibrationAsFileAction->setEnabled(false);
    // zv_loadSpectrumFromFileAction = new QAction(QIcon(":/images/ZImages/document-open.png"), tr("Append &spectra to array"), this);

    zv_openCalibrationProjectAction = new QAction(QIcon(), tr("Open calibration &project"), this);

}
//======================================================
void ZFileActionManager::zh_createConnections()
{
    connect(zv_openArrayFromFileAction, &QAction::triggered,
            this, &ZFileActionManager::zh_onOpenSpectrumArrayAction);
    connect(zv_saveArrayToFileAction, &QAction::triggered,
            this, &ZFileActionManager::zh_onSaveSpectrumArrayAction);
    connect(zv_saveArrayAsFileAction, &QAction::triggered,
            this, &ZFileActionManager::zh_onSaveSpectrumArrayAsAction);

    connect(zv_openCalibrationFromFileAction, &QAction::triggered,
            this, &ZFileActionManager::zh_onOpenCalibrationAction);
    connect(zv_saveCalibrationToFileAction, &QAction::triggered,
            this, &ZFileActionManager::zh_onSaveCalibrationAction);
    connect(zv_saveCalibrationAsFileAction, &QAction::triggered,
            this, &ZFileActionManager::zh_onSaveCalibrationAsAction);

    connect(zv_openCalibrationProjectAction, &QAction::triggered,
            this, &ZFileActionManager::zh_onOpenCalibrationProjectAction);
}
//======================================================
bool ZFileActionManager::zh_defineSpectrumArrayFileNameToOpen(QString& fileName) const
{
    // opening
    fileName = QFileDialog::getOpenFileName(nullptr, tr("Select file to open"),
                                            zv_spectrumArrayFolderPath,
                                            tr("Spectrum array files(*.%1);;XML files(%2);;All files(%3)").arg(zv_spectrumArrayFileSuffix, "*.xml", "*.*"));
    return zh_checkFile(fileName);
}
//======================================================
bool ZFileActionManager::zh_defineSpectrumArrayFileNameToSave(QString& fileName) const
{
    // opening
    fileName = QFileDialog::getSaveFileName(nullptr, tr("Select file to save"),
                                            zv_spectrumArrayFolderPath,
                                            tr("Spectrum array files(*.%1);;XML files(%2);;All files(%3)").arg(zv_spectrumArrayFileSuffix, "*.xml", "*.*"));
    return !fileName.isEmpty();
}
//======================================================
bool ZFileActionManager::zh_defineCalibrationFileNamesToOpen(QStringList& fileNames) const
{
    // opening
    //    fileNames = QFileDialog::getOpenFileNames(0, tr("Select file to open"),
    //                                              zv_calibrationFolderPath,
    //                                              tr("XML Calibration files(*.clbx);;XML files(*.xml);;All files(*.*)"));

    fileNames = ZXMLCalibrationIOHandler::zp_getCalibrationOpenFiles(zv_calibrationFolderPath);

    if(fileNames.isEmpty())
    {
        return false;
    }

    return true;
}
//======================================================
bool ZFileActionManager::zh_defineCalibrationFileNameToSave(QString& fileName) const
{
    //    fileName = QFileDialog::getSaveFileName(0, tr("Select file to save"),
    //                                              fileName,
    //                                              tr("XML Calibration files(*.clbx);;XML files(*.xml);;All files(*.*)"));

    fileName = ZXMLCalibrationIOHandler::zp_getCalibrationSaveFile(fileName);

    if(fileName.isEmpty())
    {
        return false;
    }

    return true;
}
//======================================================
bool ZFileActionManager::zh_checkFile(const QString& fileName) const
{
    if(fileName.isEmpty())
    {
        return false;
    }

    QFileInfo fileInfo(fileName);
    if(!fileInfo.exists())
    {
        QString msg = tr("File \"%1\" does not exist!").arg(fileName);
        QMessageBox::critical(nullptr, tr("File open error"), msg);
        emit zg_message(msg);
        qCritical().noquote() <<  msg;
        return false;
    }

    if(fileInfo.isDir())
    {
        QString msg = tr("\"%1\" is a folder!").arg(fileName);
        QMessageBox::critical(nullptr, tr("File open error"), msg);
        emit zg_message(msg);
        qCritical().noquote() <<  msg;
        return false;
    }

    return true;
}
//======================================================
bool ZFileActionManager::zh_getRawSpectrumArrayFromFile(const QString& fileName, QList<ZRawSpectrumArray>& rawArray)
{
    QFileInfo fileInfo(fileName);


    // ioHandler subclass definition
    // here is an ability to using the other classes, that handle files of different formats
    if(!fileInfo.exists() || !fileInfo.isFile())
    {
        QString msg = tr("Error: \"%1\" is not a file!").arg(fileName);
        // QMessageBox::critical(0, tr("File handling error"), msg);
        emit zg_message(msg);
        qCritical().noquote() <<  msg;
        return false;
    }
    else if(fileInfo.suffix() != "xml" && fileInfo.suffix() != zv_spectrumArrayFileSuffix)
    {
        QString msg = tr("Error handling file \"%1\"! Cannot handle \"%2\" files.").arg(fileName, fileInfo.suffix());
        QMessageBox::critical(nullptr, tr("File handling error"), msg);
        emit zg_message(msg);
        qCritical().noquote() <<  msg;
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
        qCritical().noquote() <<  errorMsg;
        return false;
    }

    ZXMLSpectrumArrayIOHandler* ioHandler = new ZXMLSpectrumArrayIOHandler(this);

    // current directory  saving
    zv_spectrumArrayFolderPath = fileInfo.absolutePath();

    // ability of message receiving
    connect(ioHandler, &ZXMLSpectrumArrayIOHandler::zg_message,
            this, &ZFileActionManager::zg_message);

    // array loading
    bool readArrayRes = ioHandler->zp_readSpectrumArray(file, rawArray);

    if(file.isOpen())
    {
        file.close();
    }
    delete ioHandler;

    return readArrayRes;
}
//======================================================
//bool ZFileActionManager::zh_getRawCalibrationArrayFromFile(const QString& fileName, QList<ZRawCalibrationArray>& rawArray)
//{
//    QFileInfo fileInfo(fileName);
//    ZAbstractCalibrationArrayIOHandler* ioHandler;

//    // ioHandler subclass definition
//    // here is an ability to using the other classes, that handle files of different formats
//    if(!fileInfo.exists() || !fileInfo.isFile())
//    {
//        QString msg = tr("Error: \"%1\" is not a file!").arg(fileName);
//        // QMessageBox::critical(0, tr("File handling error"), msg);
//        emit zg_message(msg);
//        return false;
//    }
//    else if(fileInfo.suffix() == "xml" || fileInfo.suffix() == "clar")
//    {
//        ioHandler = new ZXMLCalibrationArrayIOHandler(this);
//    }
//    else // other suffixes
//    {
//        QString msg = tr("Error handling file \"%1\"! Cannot handle \"%2\" files.").arg(fileName, fileInfo.suffix());
//        QMessageBox::critical(0, tr("File handling error"), msg);
//        emit zg_message(msg);
//        return false;
//    }

//    // file opening
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

//    // current directory  saving
//    zv_calibrationFolderPath = fileInfo.absolutePath();

//    // ability of message receiving
//    connect(ioHandler, &ZAbstractCalibrationArrayIOHandler::zg_message,
//            this, &ZFileActionManager::zg_message);

//    // array loading
//    bool readArrayRes = ioHandler->zp_readCalibrationArray(rawArray, file);

//    if(file.isOpen())
//    {
//        file.close();
//    }
//    delete ioHandler;

//    return readArrayRes;
//}
//======================================================
void ZFileActionManager::zh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("Common");

    vData = settings.value("spectrumArrayFolderPath");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
    {
        zv_spectrumArrayFolderPath = vData.toString();
    }

    vData = settings.value("calibrationArrayFolderPath");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
    {
        zv_calibrationFolderPath = vData.toString();
    }

    vData = settings.value("spectrumFolderPath");
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
    {
        zv_spectrumFolderPath = vData.toString();
    }

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void ZFileActionManager::zp_saveSettings() const
{
    QSettings settings;

    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("Common");

    settings.setValue("spectrumArrayFolderPath", QVariant(zv_spectrumArrayFolderPath));
    settings.setValue("calibrationArrayFolderPath", QVariant(zv_calibrationFolderPath));
    settings.setValue("spectrumFolderPath", QVariant(zv_spectrumFolderPath));

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void ZFileActionManager::zp_openCalibrations() const
{
    zv_openCalibrationFromFileAction->trigger();
}
//======================================================
void ZFileActionManager::zp_saveSpectraArrayListToFile(QString& filePath, QList<ZRawSpectrumArray> rawArrayList, bool& res)
{
    if(filePath.isEmpty())
    {
        if(!zh_defineSpectrumArrayFileNameToSave(filePath))
        {
            return;
        }
    }

    QFileInfo fileInfo(filePath);
    if(fileInfo.suffix() != "xml" && fileInfo.suffix() != zv_spectrumArrayFileSuffix)
    {
        QString msg = tr("Error handling file \"%1\"! Cannot handle \"%2\" files.").arg(filePath, fileInfo.suffix());
        QMessageBox::critical(nullptr, tr("File handling error"), msg);
        qCritical().noquote() <<  msg;
        emit zg_message(msg);
        res = false;
        return;
    }

    // file opening
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly))
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
        qCritical().noquote() <<  errorMsg;
        res = false;
        return;
    }

    ZXMLSpectrumArrayIOHandler* ioHandler = new ZXMLSpectrumArrayIOHandler(this);

    // saving current directory
    zv_spectrumArrayFolderPath = fileInfo.absolutePath();

    // ability of message receiving
    connect(ioHandler, &ZXMLSpectrumArrayIOHandler::zg_message,
            this, &ZFileActionManager::zg_message);

    // array loading
    bool writeArrayRes = ioHandler->zp_writeSpectrumArray(file, rawArrayList);

    if(file.isOpen())
    {
        file.close();
    }
    delete ioHandler;

    if(writeArrayRes)
    {
        emit zg_spectrumArraySaved(filePath);
    }

    res = writeArrayRes;

    // TODO signal that saving has been completed
}
//======================================================
void ZFileActionManager::zp_saveCalibrationToFile(const ZCalibration* calibration, QString filePath, QString name)
{
    QString absFileName;
    if(filePath.isEmpty())
    {
        absFileName = QFileInfo(QDir(zv_calibrationFolderPath), name).absoluteFilePath(); ;
        if(!zh_defineCalibrationFileNameToSave(absFileName))
        {
            return;
        }
    }
    else
    {
        absFileName = QFileInfo(QDir(filePath), name).absoluteFilePath();
    }

    //    QFileInfo fileInfo(absFileName);
    //    if(fileInfo.suffix() != "xml" && fileInfo.suffix() != "spar")
    //    {
    //        QString msg = tr("Error handling file \"%1\"! Cannot handle \"%2\" files.").arg(filePath, fileInfo.suffix());
    //        QMessageBox::critical(0, tr("File handling error"), msg);
    //        emit zg_message(msg);
    //        return;
    //    }

    // file opening
    QFile file(absFileName);
    if(!file.open(QIODevice::WriteOnly))
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
        qCritical().noquote() <<  errorMsg;
        return;
    }

    // ZXMLCalibrationIOHandler* ioHandler = new ZXMLCalibrationIOHandler(this);
    ZXMLCalibrationIOHandler ioHandler;
    // saving current directory
    zv_calibrationFolderPath = QFileInfo(absFileName).absolutePath();

    // ability of message receiving
    connect(&ioHandler, &ZXMLCalibrationIOHandler::zg_message,
            this, &ZFileActionManager::zg_message);

    // saving
    //bool res = ioHandler->zp_writeCalibrationToFile(file, calibration);
    bool res = ioHandler.zp_writeCalibrationToFile(file, calibration);
    if(file.isOpen())
    {
        file.close();
    }
    // delete ioHandler;

    if(res)
    {
        emit zg_calibrationSaved(calibration, absFileName);
        zh_saveCalibrationProjectFile(absFileName);
    }

}
//======================================================
void ZFileActionManager::zh_saveCalibrationProjectFile(const QString& calibrationFilePath)
{
    QFileInfo fileInfo(calibrationFilePath);

    QDir dir(fileInfo.absolutePath());
    QString projectFileName = dir.absoluteFilePath(fileInfo.baseName() + "." +zv_projectFileSuffix);

    QFile file(projectFileName);
    if(!file.open(QIODevice::WriteOnly))
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
        qCritical().noquote() <<  errorMsg;
        return;
    }

    QString msg = tr("Do you want to save calibration project?");
    if(QMessageBox::question(nullptr, tr("Calibration saving"), msg, QMessageBox::Yes | QMessageBox::No)
            == QMessageBox::No)
    {
        return;
    }

    // init to save spectrumArray
    QString spectrumArrayFilePath;
    bool ok = false;
    emit zg_requestRawArrayListAndInitSaving(spectrumArrayFilePath, ok);

    if(!ok)
    {
        return;
    }

    ZXMLCalibrationProjectIOHandler XMLCalibrationProjectIOHandler;
    XMLCalibrationProjectIOHandler.zp_writeProjectData(file, calibrationFilePath, spectrumArrayFilePath);

    if(file.isOpen())
    {
        file.close();
    }
}
//======================================================
void ZFileActionManager::zp_onArrayListDirtyChange(bool dirty, bool currentArrayExists)
{
    zv_saveArrayToFileAction->setEnabled(dirty);
    zv_saveArrayAsFileAction->setEnabled(dirty || currentArrayExists);
}
//======================================================
void ZFileActionManager::zp_onCurrentCalibrationDirtyChange(bool dirty, bool currentCalibrationExists)
{
    zv_saveCalibrationToFileAction->setEnabled(dirty);
    zv_saveCalibrationAsFileAction->setEnabled(dirty || currentCalibrationExists);
}
//======================================================
void ZFileActionManager::zp_defineSpectrumFilesAndInitAppending(int arrayIndex)
{
    QStringList fileNameList = QFileDialog::getOpenFileNames(nullptr, tr("Select spectrum file"),
                                                             zv_spectrumFolderPath,
                                                             tr("SRV spectrum files(%1);;All files(%2)").arg("*.spe", "*.*"));
    if(fileNameList.isEmpty())
    {
        return;
    }

    QFileInfo fileInfo(fileNameList.first()) ;
    zv_spectrumFolderPath = fileInfo.absolutePath();
    emit zg_spectrumFileListToOpen(arrayIndex, fileNameList);
}
//======================================================
void ZFileActionManager::zh_onOpenSpectrumArrayAction() // outputs RawArray
{
    QString fileName;
    if(!zh_defineSpectrumArrayFileNameToOpen(fileName))
    {
        return;
    }

    QList<ZRawSpectrumArray> rawArrayList;
    if(!zh_getRawSpectrumArrayFromFile(fileName, rawArrayList))
    {
        QString msg = tr("Data loading from file \"%1\" failed.").arg(fileName);
        QMessageBox::critical(nullptr, tr("Loading error"), msg, QMessageBox::Ok);
        emit zg_message(msg);
        qCritical().noquote() << msg;
        return;
    }

    emit zg_spectrumRawArrayList(fileName, rawArrayList);
}
//======================================================
void ZFileActionManager::zh_onSaveSpectrumArrayAction() const
{
    QString filePath;
    bool ok = false;
    emit zg_requestRawArrayListAndInitSaving(filePath, ok);
}
//======================================================
void ZFileActionManager::zh_onSaveSpectrumArrayAsAction() const
{
    QString filePath;
    bool ok = false;
    if(zh_defineSpectrumArrayFileNameToSave(filePath))
    {
        emit zg_requestRawArrayListAndInitSaving(filePath, ok);
    }
}
//======================================================
void ZFileActionManager::zh_onOpenCalibrationAction()
{
    QStringList fileNames;
    if(!zh_defineCalibrationFileNamesToOpen(fileNames))
    {
        return;
    }

    zv_calibrationFolderPath = QFileInfo(fileNames.at(0)).absolutePath();
    emit zg_calibrationFileListToOpen(fileNames);

    //    QList<ZRawCalibrationArray> rawArrayList;
    //    if(!zh_getRawCalibrationArrayFromFile(fileName, rawArrayList))
    //    {
    //        QString msg = tr("Data loading from file \"%1\" failed.").arg(fileName);
    //        QMessageBox::critical(0, tr("Loading error"), msg, QMessageBox::Ok);
    //        emit zg_message(msg);
    //        return;
    //    }
}
//======================================================
void ZFileActionManager::zh_onSaveCalibrationAction() const
{
    emit zg_requestCalibrationDataAndInitSaving(QString(), QString());
}
//======================================================
void ZFileActionManager::zh_onSaveCalibrationAsAction() const
{
    QString filePath = QFileInfo(QDir(zv_calibrationFolderPath), "*."+zv_calibrationFileSuffix).absoluteFilePath();
    if(zh_defineCalibrationFileNameToSave(filePath))
    {
        emit zg_requestCalibrationDataAndInitSaving(QFileInfo(filePath).absolutePath(), QFileInfo(filePath).fileName());
    }
}
//======================================================
void ZFileActionManager::zh_onOpenCalibrationProjectAction()
{
    QString fileName = QFileDialog::getOpenFileName(
        nullptr,
        tr("Select file to open"),
        zv_calibrationFolderPath,
        tr("Calibration project files(%1);;XML files(%2);;All files(%3)")
            .arg("*." + zv_projectFileSuffix, "*.xml", "*.*"));

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
        qCritical().noquote() <<  errorMsg;
        return;
    }


    QString calibrationFilePath;
    QString spectrumArrayFilePath;
    ZXMLCalibrationProjectIOHandler XMLCalibrationProjectIOHandler;
    connect(&XMLCalibrationProjectIOHandler, &ZXMLCalibrationProjectIOHandler::zg_message,
            this, &ZFileActionManager::zg_message);

    if(!XMLCalibrationProjectIOHandler.zp_readProjectData(file, calibrationFilePath,
                                                      spectrumArrayFilePath))

    {
        return;
    }

    // Spectrum array list
    QList<ZRawSpectrumArray> rawArrayList;
    if(!zh_getRawSpectrumArrayFromFile(spectrumArrayFilePath, rawArrayList))
    {
        QString msg = tr("Data loading from file \"%1\" failed.").arg(fileName);
        QMessageBox::critical(nullptr, tr("Loading error"), msg, QMessageBox::Ok);
        emit zg_message(msg);
        qCritical().noquote() << msg;
        return;
    }

    emit zg_spectrumRawArrayList(fileName, rawArrayList);

    // calibrations

    emit zg_calibrationFileListToOpen(QStringList(calibrationFilePath));
}
//======================================================

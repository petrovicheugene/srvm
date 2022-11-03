//======================================================
#include "X_FileActionManager.h"
#include "X_Calibration.h"
#include "X_General.h"
#include "X_XMLCalibrationIOHandler.h"
#include "X_XMLCalibrationProjectIOHandler.h"
#include "X_XMLSpectrumArrayIOHandler.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
//======================================================
X_FileActionManager::X_FileActionManager(QObject* parent) : QObject(parent)
{
    xh_createActions();
    xh_createConnections();
    xh_restoreSettings();
}
//======================================================
void X_FileActionManager::xp_appendActionsToMenu(QMenu* menu) const
{
    if (menu->objectName() == "File")
    {
        menu->addAction(xv_openCalibrationProjectAction);
        menu->addSeparator();

        menu->addAction(xv_openArrayFromFileAction);
        menu->addAction(xv_saveArrayToFileAction);
        menu->addAction(xv_saveArrayAsFileAction);
        menu->addSeparator();

        menu->addAction(xv_openCalibrationFromFileAction);
        menu->addAction(xv_saveCalibrationToFileAction);
        menu->addAction(xv_saveCalibrationAsFileAction);
        menu->addSeparator();
    }
}
//======================================================
QList<QAction*> X_FileActionManager::xp_actionList() const
{
    QList<QAction*> actionList;

    actionList << xv_openCalibrationProjectAction;
    actionList << xv_openArrayFromFileAction;
    actionList << xv_saveArrayToFileAction;
    actionList << xv_saveArrayAsFileAction;

    actionList << xv_openCalibrationFromFileAction;
    actionList << xv_saveCalibrationToFileAction;
    actionList << xv_saveCalibrationAsFileAction;

    // actionList <<  xv_loadSpectrumFromFileAction;

    return actionList;
}
//======================================================
void X_FileActionManager::xp_initOpenCalibrationFromFileAction(QAction*& action) const
{
    action->setIcon(xv_openCalibrationFromFileAction->icon());
    action->setText(xv_openCalibrationFromFileAction->text());
    action->setIconText(xv_openCalibrationFromFileAction->iconText());

    action->setToolTip(xv_openCalibrationFromFileAction->toolTip());
    action->setCheckable(xv_openCalibrationFromFileAction->isCheckable());
    action->setChecked(xv_openCalibrationFromFileAction->isChecked());
    action->setEnabled(xv_openCalibrationFromFileAction->isEnabled());

    connect(action, &QAction::triggered, xv_openCalibrationFromFileAction, &QAction::trigger);
}
//======================================================
void X_FileActionManager::xp_initSaveCalibrationFromFileAction(QAction*& action) const
{
    action->setIcon(xv_saveCalibrationToFileAction->icon());
    action->setText(xv_saveCalibrationToFileAction->text());
    action->setIconText(xv_saveCalibrationToFileAction->iconText());

    action->setToolTip(xv_saveCalibrationToFileAction->toolTip());
    action->setCheckable(xv_saveCalibrationToFileAction->isCheckable());
    action->setChecked(xv_saveCalibrationToFileAction->isChecked());
    action->setEnabled(xv_saveCalibrationToFileAction->isEnabled());

    connect(action, &QAction::triggered, xv_saveCalibrationToFileAction, &QAction::trigger);
    //    connect(xv_saveCalibrationToFileAction,
    //            &QAction::changed,
    //            this,
    //            &X_FileActionManager::xh_synchronizeSaveCalibrationActions);
    //    connect(this,
    //            &X_FileActionManager::xg_saveCalibrationActionEnabledChanged,
    //            action,
    //            &QAction::setEnabled);
}
//======================================================
//void X_FileActionManager::xh_synchronizeSaveCalibrationActions()
//{
//    QAction* action = qobject_cast<QAction*>(sender());
//    if (!action)
//    {
//        return;
//    }

//    qDebug() << "synchronizeSaveCalibration"
//             << "ENABLED" << action->isEnabled();

//    emit xg_saveCalibrationActionEnabledChanged(action->isEnabled());
//}
//======================================================
void X_FileActionManager::xp_initSaveAsCalibrationFromFileAction(QAction*& action) const
{
    action->setIcon(xv_saveCalibrationAsFileAction->icon());
    action->setText(xv_saveCalibrationAsFileAction->text());
    action->setIconText(xv_saveCalibrationAsFileAction->iconText());

    action->setToolTip(xv_saveCalibrationAsFileAction->toolTip());
    action->setCheckable(xv_saveCalibrationAsFileAction->isCheckable());
    action->setChecked(xv_saveCalibrationAsFileAction->isChecked());
    action->setEnabled(xv_saveCalibrationAsFileAction->isEnabled());

    connect(action, &QAction::triggered, xv_saveCalibrationAsFileAction, &QAction::trigger);
    //    connect(xv_saveCalibrationAsFileAction,
    //            &QAction::changed,
    //            this,
    //            &X_FileActionManager::xh_synchronizeSaveAsCalibrationActions);
    //    connect(this,
    //            &X_FileActionManager::xg_saveCalibrationAsActionEnabledChanged,
    //            action,
    //            &QAction::setEnabled);
}
//======================================================
//void X_FileActionManager::xh_synchronizeSaveAsCalibrationActions()
//{
//    QAction* action = qobject_cast<QAction*>(sender());
//    if (!action)
//    {
//        return;
//    }

//    emit xg_saveCalibrationAsActionEnabledChanged(action->isEnabled());
//}
//======================================================
void X_FileActionManager::xp_initOpenSpectrumArrayAction(QAction*& action) const
{
    action->setIcon(xv_openArrayFromFileAction->icon());
    action->setText(xv_openArrayFromFileAction->text());
    action->setIconText(xv_openArrayFromFileAction->iconText());

    action->setToolTip(xv_openArrayFromFileAction->toolTip());
    action->setCheckable(xv_openArrayFromFileAction->isCheckable());
    action->setChecked(xv_openArrayFromFileAction->isChecked());
    action->setEnabled(xv_openArrayFromFileAction->isEnabled());

    connect(action, &QAction::triggered, xv_openArrayFromFileAction, &QAction::trigger);
}
//======================================================
void X_FileActionManager::xp_initSaveSpectrumArrayAction(QAction*& action) const
{
    action->setIcon(xv_saveArrayToFileAction->icon());
    action->setText(xv_saveArrayToFileAction->text());
    action->setIconText(xv_saveArrayToFileAction->iconText());

    action->setToolTip(xv_saveArrayToFileAction->toolTip());
    action->setCheckable(xv_saveArrayToFileAction->isCheckable());
    action->setChecked(xv_saveArrayToFileAction->isChecked());
    action->setEnabled(xv_saveArrayToFileAction->isEnabled());

    connect(action, &QAction::triggered, xv_saveArrayToFileAction, &QAction::trigger);
}
//======================================================
void X_FileActionManager::xp_initSaveAsSpectrumArrayAction(QAction*& action) const
{
    action->setIcon(xv_saveArrayAsFileAction->icon());
    action->setText(xv_saveArrayAsFileAction->text());
    action->setIconText(xv_saveArrayAsFileAction->iconText());

    action->setToolTip(xv_saveArrayAsFileAction->toolTip());
    action->setCheckable(xv_saveArrayAsFileAction->isCheckable());
    action->setChecked(xv_saveArrayAsFileAction->isChecked());
    action->setEnabled(xv_saveArrayAsFileAction->isEnabled());

    connect(action, &QAction::triggered, xv_saveArrayAsFileAction, &QAction::trigger);
}
//======================================================
void X_FileActionManager::xp_triggerSaveArrayToFileAction()
{
    xv_saveArrayToFileAction->trigger();
}
//======================================================
void X_FileActionManager::xp_triggerSaveCalibrationsToFile()
{
    xv_saveCalibrationToFileAction->trigger();
}
//======================================================
void X_FileActionManager::xh_createActions()
{
    xv_openArrayFromFileAction = new QAction(QIcon(NS_Icons::glIconOpenArray),
                                             tr("&Open spectrum array list"),
                                             this);
    xv_saveArrayToFileAction = new QAction(QIcon(NS_Icons::glIconSaveArray),
                                           tr("&Save spectrum array list"),
                                           this);
    xv_saveArrayToFileAction->setEnabled(false);
    xv_saveArrayAsFileAction = new QAction(QIcon(NS_Icons::glIconSaveAsArray),
                                           tr("Save spectrum array list &as..."),
                                           this);
    xv_saveArrayAsFileAction->setEnabled(false);

    xv_openCalibrationFromFileAction = new QAction(QIcon(NS_Icons::glIconOpenCalibration),
                                                   tr("Open &calibrations"),
                                                   this);
    xv_saveCalibrationToFileAction = new QAction(QIcon(NS_Icons::glIconSaveCalibration),
                                                 tr("Save calibration"),
                                                 this);
    xv_saveCalibrationToFileAction->setEnabled(false);
    xv_saveCalibrationAsFileAction = new QAction(QIcon(NS_Icons::glIconSaveAsCalibration),
                                                 tr("Save calibration as..."),
                                                 this);
    xv_saveCalibrationAsFileAction->setEnabled(false);
    // xv_loadSpectrumFromFileAction = new QAction(QIcon(":/images/X_Images/document-open.png"), tr("Append &spectra to array"), this);

    xv_openCalibrationProjectAction = new QAction(QIcon(), tr("Open calibration &project"), this);
}
//======================================================
void X_FileActionManager::xh_createConnections()
{
    connect(xv_openArrayFromFileAction,
            &QAction::triggered,
            this,
            &X_FileActionManager::xh_onOpenSpectrumArrayAction);
    connect(xv_saveArrayToFileAction,
            &QAction::triggered,
            this,
            &X_FileActionManager::xh_onSaveSpectrumArrayAction);
    connect(xv_saveArrayAsFileAction,
            &QAction::triggered,
            this,
            &X_FileActionManager::xh_onSaveSpectrumArrayAsAction);

    connect(xv_openCalibrationFromFileAction,
            &QAction::triggered,
            this,
            &X_FileActionManager::xh_onOpenCalibrationAction);
    connect(xv_saveCalibrationToFileAction,
            &QAction::triggered,
            this,
            &X_FileActionManager::xh_onSaveCalibrationAction);
    connect(xv_saveCalibrationAsFileAction,
            &QAction::triggered,
            this,
            &X_FileActionManager::xh_onSaveCalibrationAsAction);

    connect(xv_openCalibrationProjectAction,
            &QAction::triggered,
            this,
            &X_FileActionManager::xh_onOpenCalibrationProjectAction);
}
//======================================================
bool X_FileActionManager::xh_defineSpectrumArrayFileNameToOpen(QString& fileName) const
{
    // opening
    fileName = QFileDialog::getOpenFileName(
        nullptr,
        tr("Select file to open"),
        xv_spectrumArrayFolderPath,
        tr("Spectrum array files(*.%1);;XML files(%2);;All files(%3)")
            .arg(xv_spectrumArrayFileSuffix, "*.xml", "*.*"));
    return xh_checkFile(fileName);
}
//======================================================
bool X_FileActionManager::xh_defineSpectrumArrayFileNameToSave(QString& fileName) const
{
    // opening
    fileName = QFileDialog::getSaveFileName(
        nullptr,
        tr("Select file to save"),
        xv_spectrumArrayFolderPath,
        tr("Spectrum array files(*.%1);;XML files(%2);;All files(%3)")
            .arg(xv_spectrumArrayFileSuffix, "*.xml", "*.*"));
    return !fileName.isEmpty();
}
//======================================================
bool X_FileActionManager::xh_defineCalibrationFileNamesToOpen(QStringList& fileNames) const
{
    // opening
    //    fileNames = QFileDialog::getOpenFileNames(0, tr("Select file to open"),
    //                                              xv_calibrationFolderPath,
    //                                              tr("XML Calibration files(*.clbx);;XML files(*.xml);;All files(*.*)"));

    // QString fileName = QFileDialog::getSaveFileName();

    fileNames = X_XMLCalibrationIOHandler::xp_getCalibrationOpenFiles(xv_calibrationFolderPath);

    if (fileNames.isEmpty())
    {
        return false;
    }

    return true;
}
//======================================================
bool X_FileActionManager::xh_defineCalibrationFileNameToSave(QString& fileName) const
{
    //    fileName = QFileDialog::getSaveFileName(0, tr("Select file to save"),
    //                                              fileName,
    //                                              tr("XML Calibration files(*.clbx);;XML files(*.xml);;All files(*.*)"));

    fileName = X_XMLCalibrationIOHandler::xp_getCalibrationSaveFile(fileName);

    if (fileName.isEmpty())
    {
        return false;
    }

    return true;
}
//======================================================
bool X_FileActionManager::xh_checkFile(const QString& fileName) const
{
    if (fileName.isEmpty())
    {
        return false;
    }

    QFileInfo fileInfo(fileName);
    if (!fileInfo.exists())
    {
        QString msg = tr("File \"%1\" does not exist!").arg(fileName);
        QMessageBox::critical(nullptr, tr("File open error"), msg);
        emit xg_message(msg);
        qCritical().noquote() << msg;
        return false;
    }

    if (fileInfo.isDir())
    {
        QString msg = tr("\"%1\" is a folder!").arg(fileName);
        QMessageBox::critical(nullptr, tr("File open error"), msg);
        emit xg_message(msg);
        qCritical().noquote() << msg;
        return false;
    }

    return true;
}
//======================================================
bool X_FileActionManager::xh_getRawSpectrumArrayFromFile(const QString& fileName,
                                                         QList<X_RawSpectrumArray>& rawArray)
{
    QFileInfo fileInfo(fileName);

    // ioHandler subclass definition
    // here is an ability to using the other classes, that handle files of different formats
    if (!fileInfo.exists() || !fileInfo.isFile())
    {
        QString msg = tr("Error: \"%1\" is not a file!").arg(fileName);
        // QMessageBox::critical(0, tr("File handling error"), msg);
        emit xg_message(msg);
        qCritical().noquote() << msg;
        return false;
    }
    else if (fileInfo.suffix() != "xml" && fileInfo.suffix() != xv_spectrumArrayFileSuffix)
    {
        QString msg = tr("Error handling file \"%1\"! Cannot handle \"%2\" files.")
                          .arg(fileName, fileInfo.suffix());
        QMessageBox::critical(nullptr, tr("File handling error"), msg);
        emit xg_message(msg);
        qCritical().noquote() << msg;
        return false;
    }

    // file opening
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QString errorMsg;
        if (file.error() != QFile::NoError)
        {
            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
        }
        else
        {
            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), tr("Unknown error"));
        }
        emit xg_message(errorMsg);
        qCritical().noquote() << errorMsg;
        return false;
    }

    X_XMLSpectrumArrayIOHandler* ioHandler = new X_XMLSpectrumArrayIOHandler(this);

    // current directory  saving
    xv_spectrumArrayFolderPath = fileInfo.absolutePath();

    // ability of message receiving
    connect(ioHandler,
            &X_XMLSpectrumArrayIOHandler::xg_message,
            this,
            &X_FileActionManager::xg_message);

    // array loading
    bool readArrayRes = ioHandler->xp_readSpectrumArray(file, rawArray);

    if (file.isOpen())
    {
        file.close();
    }
    delete ioHandler;

    return readArrayRes;
}
//======================================================
//bool X_FileActionManager::xh_getRawCalibrationArrayFromFile(const QString& fileName, QList<X_RawCalibrationArray>& rawArray)
//{
//    QFileInfo fileInfo(fileName);
//    X_AbstractCalibrationArrayIOHandler* ioHandler;

//    // ioHandler subclass definition
//    // here is an ability to using the other classes, that handle files of different formats
//    if(!fileInfo.exists() || !fileInfo.isFile())
//    {
//        QString msg = tr("Error: \"%1\" is not a file!").arg(fileName);
//        // QMessageBox::critical(0, tr("File handling error"), msg);
//        emit xg_message(msg);
//        return false;
//    }
//    else if(fileInfo.suffix() == "xml" || fileInfo.suffix() == "clar")
//    {
//        ioHandler = new X_XMLCalibrationArrayIOHandler(this);
//    }
//    else // other suffixes
//    {
//        QString msg = tr("Error handling file \"%1\"! Cannot handle \"%2\" files.").arg(fileName, fileInfo.suffix());
//        QMessageBox::critical(0, tr("File handling error"), msg);
//        emit xg_message(msg);
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
//        emit xg_message(errorMsg);
//        return false;
//    }

//    // current directory  saving
//    xv_calibrationFolderPath = fileInfo.absolutePath();

//    // ability of message receiving
//    connect(ioHandler, &X_AbstractCalibrationArrayIOHandler::xg_message,
//            this, &X_FileActionManager::xg_message);

//    // array loading
//    bool readArrayRes = ioHandler->xp_readCalibrationArray(rawArray, file);

//    if(file.isOpen())
//    {
//        file.close();
//    }
//    delete ioHandler;

//    return readArrayRes;
//}
//======================================================
void X_FileActionManager::xh_restoreSettings()
{
    QSettings settings;
    QVariant vData;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("Common");

    vData = settings.value("spectrumArrayFolderPath");
    if (vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
    {
        xv_spectrumArrayFolderPath = vData.toString();
    }

    vData = settings.value("calibrationArrayFolderPath");
    if (vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
    {
        xv_calibrationFolderPath = vData.toString();
    }

    vData = settings.value("spectrumFolderPath");
    if (vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
    {
        xv_spectrumFolderPath = vData.toString();
    }

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void X_FileActionManager::xp_saveSettings() const
{
    QSettings settings;

    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup("Common");

    settings.setValue("spectrumArrayFolderPath", QVariant(xv_spectrumArrayFolderPath));
    settings.setValue("calibrationArrayFolderPath", QVariant(xv_calibrationFolderPath));
    settings.setValue("spectrumFolderPath", QVariant(xv_spectrumFolderPath));

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void X_FileActionManager::xp_openCalibrations() const
{
    xv_openCalibrationFromFileAction->trigger();
}
//======================================================
void X_FileActionManager::xp_saveSpectraArrayListToFile(QString& filePath,
                                                        QList<X_RawSpectrumArray> rawArrayList,
                                                        bool& res)
{
    if (filePath.isEmpty())
    {
        xh_onSaveSpectrumArrayAsAction();
        return;
    }

    QFileInfo fileInfo(filePath);
    if (fileInfo.suffix() != "xml" && fileInfo.suffix() != xv_spectrumArrayFileSuffix)
    {
        QString msg = tr("Error handling file \"%1\"! Cannot handle \"%2\" files.")
                          .arg(filePath, fileInfo.suffix());
        QMessageBox::critical(nullptr, tr("File handling error"), msg);
        qCritical().noquote() << msg;
        emit xg_message(msg);
        res = false;
        return;
    }

    // file opening
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        QString errorMsg;
        if (file.error() != QFile::NoError)
        {
            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
        }
        else
        {
            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), tr("Unknown error"));
        }
        emit xg_message(errorMsg);
        qCritical().noquote() << errorMsg;
        res = false;
        return;
    }

    X_XMLSpectrumArrayIOHandler* ioHandler = new X_XMLSpectrumArrayIOHandler(this);

    // saving current directory
    xv_spectrumArrayFolderPath = fileInfo.absolutePath();

    // ability of message receiving
    connect(ioHandler,
            &X_XMLSpectrumArrayIOHandler::xg_message,
            this,
            &X_FileActionManager::xg_message);

    // array loading
    bool writeArrayRes = ioHandler->xp_writeSpectrumArray(file, rawArrayList);

    if (file.isOpen())
    {
        file.close();
    }
    delete ioHandler;

    if (writeArrayRes)
    {
        emit xg_spectrumArraySaved(filePath);
    }

    res = writeArrayRes;

    // TODO signal that saving has been completed
}
//======================================================
//void X_FileActionManager::xp_initSavingSpectraArrayListToFile(QString &filePath,
//                                         QList<X_RawSpectrumArray> rawArrayList,
//                                         bool &res)
//{
//    if (filePath.isEmpty())
//    {
//        xh_onSaveSpectraArrayListAsAction();
//        return;
//    }

//    QFileInfo fileInfo(filePath);
//    if (fileInfo.suffix() != "xml" && fileInfo.suffix() != xv_spectrumArrayFileSuffix)
//    {
//        QString msg = tr("Error handling file \"%1\"! Cannot handle \"%2\" files.")
//                          .arg(filePath, fileInfo.suffix());
//        QMessageBox::critical(nullptr, tr("File handling error"), msg);
//        qCritical().noquote() << msg;
//        emit xg_message(msg);
//        res = false;
//        return;
//    }

//    // file opening
//    QFile file(filePath);
//    if (!file.open(QIODevice::WriteOnly))
//    {
//        QString errorMsg;
//        if (file.error() != QFile::NoError)
//        {
//            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
//        }
//        else
//        {
//            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), tr("Unknown error"));
//        }
//        emit xg_message(errorMsg);
//        qCritical().noquote() << errorMsg;
//        res = false;
//        return;
//    }

//    X_XMLSpectrumArrayIOHandler* ioHandler = new X_XMLSpectrumArrayIOHandler(this);

//    // saving current directory
//    xv_spectrumArrayFolderPath = fileInfo.absolutePath();

//    // ability of message receiving
//    connect(ioHandler,
//            &X_XMLSpectrumArrayIOHandler::xg_message,
//            this,
//            &X_FileActionManager::xg_message);

//    // array loading
//    bool writeArrayRes = ioHandler->xp_writeSpectrumArray(file, rawArrayList);

//    if (file.isOpen())
//    {
//        file.close();
//    }
//    delete ioHandler;

//    if (writeArrayRes)
//    {
//        emit xg_spectrumArraySaved(filePath);
//    }

//    res = writeArrayRes;

//}
//======================================================
void X_FileActionManager::xh_saveCalibrationToFile(const X_Calibration* calibration,
                                                   QString absFileName)
{
    // file opening
    QFile file(absFileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        QString errorMsg;
        if (file.error() != QFile::NoError)
        {
            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
        }
        else
        {
            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), tr("Unknown error"));
        }
        emit xg_message(errorMsg);
        qCritical().noquote() << errorMsg;
        return;
    }

    // X_XMLCalibrationIOHandler* ioHandler = new X_XMLCalibrationIOHandler(this);
    X_XMLCalibrationIOHandler ioHandler;
    // saving current directory
    xv_calibrationFolderPath = QFileInfo(absFileName).absolutePath();

    // ability of message receiving
    connect(&ioHandler,
            &X_XMLCalibrationIOHandler::xg_message,
            this,
            &X_FileActionManager::xg_message);

    // saving
    //bool res = ioHandler->xp_writeCalibrationToFile(file, calibration);
    bool res = ioHandler.xp_writeCalibrationToFile(file, calibration);
    if (file.isOpen())
    {
        file.close();
    }
    // delete ioHandler;

    if (res)
    {
        emit xg_calibrationSaved(calibration, absFileName);
        xh_saveCalibrationProjectFile(absFileName);
    }
}
//======================================================
void X_FileActionManager::xp_initSavingCalibrationToFile(const X_Calibration *calibration,
                                                         QString filePath,
                                                         QString name)
{
    QString absFileName;
    if (filePath.isEmpty())
    {
        //        absFileName = QFileInfo(QDir(xv_calibrationFolderPath), name).absoluteFilePath();

        //        if (!xh_defineCalibrationFileNameToSave(absFileName))
        //        {
        //            return;
        //        }
        xh_onSaveCalibrationAsAction();
        return;
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
    //        emit xg_message(msg);
    //        return;
    //    }
    xh_saveCalibrationToFile(calibration, absFileName);

//    // file opening
//    QFile file(absFileName);
//    if (!file.open(QIODevice::WriteOnly))
//    {
//        QString errorMsg;
//        if (file.error() != QFile::NoError)
//        {
//            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
//        }
//        else
//        {
//            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), tr("Unknown error"));
//        }
//        emit xg_message(errorMsg);
//        qCritical().noquote() << errorMsg;
//        return;
//    }

//    // X_XMLCalibrationIOHandler* ioHandler = new X_XMLCalibrationIOHandler(this);
//    X_XMLCalibrationIOHandler ioHandler;
//    // saving current directory
//    xv_calibrationFolderPath = QFileInfo(absFileName).absolutePath();

//    // ability of message receiving
//    connect(&ioHandler,
//            &X_XMLCalibrationIOHandler::xg_message,
//            this,
//            &X_FileActionManager::xg_message);

//    // saving
//    //bool res = ioHandler->xp_writeCalibrationToFile(file, calibration);
//    bool res = ioHandler.xp_writeCalibrationToFile(file, calibration);
//    if (file.isOpen())
//    {
//        file.close();
//    }
//    // delete ioHandler;

//    if (res)
//    {
//        emit xg_calibrationSaved(calibration, absFileName);
//        xh_saveCalibrationProjectFile(absFileName);
//    }

}
//======================================================
void X_FileActionManager::xh_saveCalibrationProjectFile(const QString& calibrationFilePath)
{
    QString msg = tr("Do you want to save calibration project?");
    if (QMessageBox::question(nullptr,
                              tr("Calibration saving"),
                              msg,
                              QMessageBox::Yes | QMessageBox::No)
        == QMessageBox::No)
    {
        return;
    }

    QFileInfo fileInfo(calibrationFilePath);

    QDir dir(fileInfo.absolutePath());
    QString projectFilePath = dir.absoluteFilePath(fileInfo.baseName() + "." + xv_projectFileSuffix);

    QFile file(projectFilePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        QString errorMsg;
        if (file.error() != QFile::NoError)
        {
            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
        }
        else
        {
            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), tr("Unknown error"));
        }
        emit xg_message(errorMsg);
        qCritical().noquote() << errorMsg;
        return;
    }

    // init to save spectrumArray
    QString spectrumArrayFilePath = dir.absoluteFilePath(fileInfo.baseName() + "."
                                                         + xv_spectrumArrayFileSuffix);
    bool ok = false;
    emit xg_requestRawArrayListAndInitSaving(spectrumArrayFilePath, ok);

    if (!ok)
    {
        return;
    }

    X_XMLCalibrationProjectIOHandler XMLCalibrationProjectIOHandler;
    XMLCalibrationProjectIOHandler.xp_writeProjectData(file,
                                                       calibrationFilePath,
                                                       spectrumArrayFilePath);

    if (file.isOpen())
    {
        file.close();
    }
}
//======================================================
void X_FileActionManager::xp_onArrayListDirtyChange(bool dirty, bool currentArrayExists)
{
    xv_saveArrayToFileAction->setEnabled(dirty);
    xv_saveArrayAsFileAction->setEnabled(dirty || currentArrayExists);
}
//======================================================
void X_FileActionManager::xp_onCurrentCalibrationDirtyChange(bool dirty,
                                                             bool currentCalibrationExists)
{
    xv_saveCalibrationToFileAction->setEnabled(dirty);
    xv_saveCalibrationAsFileAction->setEnabled(dirty || currentCalibrationExists);
}
//======================================================
void X_FileActionManager::xp_defineSpectrumFilesAndInitAppending()
{
    QFileDialog* fileDialog = new QFileDialog(nullptr,
                                              tr("Select spectra file"),
                                              xv_spectrumFolderPath,
                                              tr("SRV spectrum files(%1);;All files(%2)").arg("*.spe", "*.*"));
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    connect(fileDialog, &QFileDialog::accepted,
            this, &X_FileActionManager::xh_handleOpenSpectra);
    fileDialog->setAttribute(Qt::WA_DeleteOnClose);
    fileDialog->open();
}
//======================================================
void X_FileActionManager::xh_onOpenSpectrumArrayAction() // outputs RawArray
{
    QString locationDirString = X_XMLCalibrationIOHandler::xp_checkDirPath(xv_calibrationFolderPath);
    QFileDialog* fileDialog = new QFileDialog(nullptr,
                                              tr("Open file"),
                                              xv_spectrumArrayFolderPath,
                                              tr("Spectrum array files(*.%1);;XML files(%2);;All files(%3)")
                                                  .arg(xv_spectrumArrayFileSuffix, "*.xml", "*.*"));
    //fileDialog->setOptions(QFileDialog::DontUseNativeDialog);
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    connect(fileDialog, &QFileDialog::accepted,
            this, &X_FileActionManager::xh_handleOpenSpectrumArray);
    fileDialog->setAttribute(Qt::WA_DeleteOnClose);
    fileDialog->open();
}
//======================================================
void X_FileActionManager::xh_onSaveSpectrumArrayAction() const
{
    QString filePath;
    bool ok = false;
    emit xg_requestRawArrayListAndInitSaving(filePath, ok);
}
//======================================================
void X_FileActionManager::xh_onSaveSpectrumArrayAsAction() const
{
    //QString filePath;
    QFileDialog* fileDialog = new QFileDialog(nullptr,
                                              tr("Select file to save"),
                                              xv_spectrumArrayFolderPath,
                                              tr("Spectrum array files(*.%1);;XML files(%2);;All files(%3)")
                                                  .arg(xv_spectrumArrayFileSuffix, "*.xml", "*.*"));

    fileDialog->setFileMode(QFileDialog::AnyFile);
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    connect(fileDialog, &QFileDialog::accepted,
            this, &X_FileActionManager::xh_handleSaveSpectrumArrayFiles);
    fileDialog->setAttribute(Qt::WA_DeleteOnClose);
    fileDialog->open();


    // opening
//    fileName = QFileDialog::getSaveFileName(
//        nullptr,
//        tr("Select file to save"),
//        xv_spectrumArrayFolderPath,
//        tr("Spectrum array files(*.%1);;XML files(%2);;All files(%3)")
//            .arg(xv_spectrumArrayFileSuffix, "*.xml", "*.*"));


//    bool ok = false;
//    if (xh_defineSpectrumArrayFileNameToSave(filePath))
//    {
//        emit xg_requestRawArrayListAndInitSaving(filePath, ok);
//    }
}
//======================================================
void X_FileActionManager::xh_onOpenCalibrationAction()
{
    QString locationDirString = X_XMLCalibrationIOHandler::xp_checkDirPath(xv_calibrationFolderPath);
    QFileDialog* fileDialog = new QFileDialog(nullptr,
                                              tr("Open file"),
                                              locationDirString,
                                              tr("XML Calibration files(*.clbx);;XML files(*.xml);;All files(*.*)"));
    //fileDialog->setOptions(QFileDialog::DontUseNativeDialog);
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    connect(fileDialog, &QFileDialog::accepted,
            this, &X_FileActionManager::xh_handleOpenCalibrationFiles);
    fileDialog->setAttribute(Qt::WA_DeleteOnClose);
    fileDialog->open();
}
//======================================================
void X_FileActionManager::xh_onSaveCalibrationAction() const
{
    emit xg_requestCalibrationDataAndInitSaving(QString(), QString());
}
//======================================================
void X_FileActionManager::xh_onSaveCalibrationAsAction() const
{
    QString filePath = QFileInfo(QDir(xv_calibrationFolderPath), "*." + xv_calibrationFileSuffix)
                           .absoluteFilePath();

    QFileDialog* fileDialog = new QFileDialog(nullptr,
                                              tr("Save file"),
                                              filePath,
                                              tr("XML Calibration files(*.clbx);;XML files(*.xml);;All files(*.*)"));
    fileDialog->setFileMode(QFileDialog::AnyFile);
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    connect(fileDialog, &QFileDialog::accepted,
            this, &X_FileActionManager::xh_handleSaveCalibrationFiles);
    fileDialog->setAttribute(Qt::WA_DeleteOnClose);
    fileDialog->open();


//    if (xh_defineCalibrationFileNameToSave(filePath))
//    {
//        emit xg_requestCalibrationDataAndInitSaving(QFileInfo(filePath).absolutePath(),
//                                                    QFileInfo(filePath).fileName());
//    }
}
//======================================================
void X_FileActionManager::xh_onOpenCalibrationProjectAction()
{
    QFileDialog* fileDialog = new QFileDialog(nullptr,
                                              tr("Select file to open"),
                                              xv_calibrationFolderPath,
                                              tr("Calibration project files(%1);;XML files(%2);;All files(%3)")
                                                  .arg("*." + xv_projectFileSuffix, "*.xml", "*.*"));

    //fileDialog->setOptions(QFileDialog::DontUseNativeDialog);
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    connect(fileDialog, &QFileDialog::accepted,
            this, &X_FileActionManager::xh_handleOpenCalibrationProject);
    fileDialog->setAttribute(Qt::WA_DeleteOnClose);
    fileDialog->open();

}
//======================================================
void X_FileActionManager::xh_handleOpenCalibrationFiles()
{
    if(!sender())
    {
        return;
    }

    QFileDialog* dialog = dynamic_cast<QFileDialog*>(sender());
    if(!dialog)
    {
        return;
    }

    QStringList fileNames = dialog->selectedFiles();
    if(fileNames.isEmpty())
    {
        return;
    }

    xv_calibrationFolderPath = QFileInfo(fileNames.at(0)).absolutePath();
    emit xg_calibrationFileListToOpen(fileNames);
}
//======================================================
void X_FileActionManager::xh_handleOpenSpectrumArray()
{
    if(!sender())
    {
        return;
    }

    QFileDialog* dialog = dynamic_cast<QFileDialog*>(sender());
    if(!dialog)
    {
        return;
    }

    QStringList fileNames = dialog->selectedFiles();
    if(fileNames.isEmpty())
    {
        return;
    }

    QString fileName = fileNames.at(0);
    if(!xh_checkFile(fileName))
    {
        return;
    }

    QList<X_RawSpectrumArray> rawArrayList;
    if (!xh_getRawSpectrumArrayFromFile(fileName, rawArrayList))
    {
        QString msg = tr("Data loading from file \"%1\" failed.").arg(fileName);
        QMessageBox::critical(nullptr, tr("Loading error"), msg, QMessageBox::Ok);
        emit xg_message(msg);
        qCritical().noquote() << msg;
        return;
    }

    emit xg_spectrumRawArrayList(fileName, rawArrayList);

}
//======================================================
void X_FileActionManager::xh_handleOpenSpectra()
{
    if(!sender())
    {
        return;
    }

    QFileDialog* dialog = dynamic_cast<QFileDialog*>(sender());
    if(!dialog)
    {
        return;
    }

    QStringList fileNameList = dialog->selectedFiles();
    if(fileNameList.isEmpty())
    {
        return;
    }

    QFileInfo fileInfo(fileNameList.first());
    xv_spectrumFolderPath = fileInfo.absolutePath();
    emit xg_spectrumFileListToOpen(fileNameList);
}
//======================================================
void X_FileActionManager::xh_handleOpenCalibrationProject()
{

    if(!sender())
    {
        return;
    }

    QFileDialog* dialog = dynamic_cast<QFileDialog*>(sender());
    if(!dialog)
    {
        return;
    }

    QStringList fileNameList = dialog->selectedFiles();
    if(fileNameList.isEmpty())
    {
        return;
    }

    QString fileName = fileNameList.at(0);

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QString errorMsg;
        if (file.error() != QFile::NoError)
        {
            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
        }
        else
        {
            errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), tr("Unknown error"));
        }
        emit xg_message(errorMsg);
        qCritical().noquote() << errorMsg;
        return;
    }

    QString calibrationFilePath;
    QString spectrumArrayFilePath;
    X_XMLCalibrationProjectIOHandler XMLCalibrationProjectIOHandler;
    connect(&XMLCalibrationProjectIOHandler,
            &X_XMLCalibrationProjectIOHandler::xg_message,
            this,
            &X_FileActionManager::xg_message);

    if (!XMLCalibrationProjectIOHandler.xp_readProjectData(file,
                                                           calibrationFilePath,
                                                           spectrumArrayFilePath))

    {
        return;
    }

    // Spectrum array list
    QList<X_RawSpectrumArray> rawArrayList;
    if (!xh_getRawSpectrumArrayFromFile(spectrumArrayFilePath, rawArrayList))
    {
        QString msg = tr("Data loading from file \"%1\" failed.").arg(fileName);
        QMessageBox::critical(nullptr, tr("Loading error"), msg, QMessageBox::Ok);
        emit xg_message(msg);
        qCritical().noquote() << msg;
        return;
    }

    emit xg_spectrumRawArrayList(spectrumArrayFilePath, rawArrayList);

    // calibrations
    emit xg_calibrationFileListToOpen(QStringList(calibrationFilePath));
}
//======================================================
void X_FileActionManager::xh_handleSaveCalibrationFiles()
{
    if(!sender())
    {
        return;
    }

    QFileDialog* dialog = dynamic_cast<QFileDialog*>(sender());
    if(!dialog)
    {
        return;
    }

    QStringList fileNameList = dialog->selectedFiles();
    if(fileNameList.isEmpty())
    {
        return;
    }

    QString fileName = fileNameList.at(0);

    emit xg_requestCalibrationDataAndInitSaving(QFileInfo(fileName).absolutePath(),
                                                QFileInfo(fileName).fileName());
}
//======================================================
void X_FileActionManager::xh_handleSaveSpectrumArrayFiles()
{
    if(!sender())
    {
        return;
    }

    QFileDialog* dialog = dynamic_cast<QFileDialog*>(sender());
    if(!dialog)
    {
        return;
    }

    QStringList fileNameList = dialog->selectedFiles();
    if(fileNameList.isEmpty())
    {
        return;
    }

    QString fileName = fileNameList.at(0);

    bool res;
    emit xg_requestRawArrayListAndInitSaving(fileName, res);
}
//======================================================

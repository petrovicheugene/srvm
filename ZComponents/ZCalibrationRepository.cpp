//======================================================
#include "ZCalibrationRepository.h"
#include "glVariables.h"
#include "ZXMLCalibrationIOHandler.h"
#include "ZSDCalibrationIOHandler.h"
#include "ZSDCalibration.h"
#include "ZXMLCalibration.h"
#include "ZFileActionManager.h"
#include "ZCalibrationEditDialog.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QFile>
#include <QtAlgorithms>
//======================================================
ZCalibrationRepository::ZCalibrationRepository(QObject *parent) : QObject(parent)
{
    zh_createActions();
    zh_createConnections();
    zh_actionAvailabilityControl(-1);
}
//==================================================================
void ZCalibrationRepository::zp_appendActionsToMenu(QMenu *menu) const
{
    if(menu->objectName() == "Edit")
    {
        menu->addAction(zv_newCalibrationAction);
        menu->addAction(zv_removeCalibrationAction);
        menu->addSeparator();
    }
}
//==================================================================
void ZCalibrationRepository::zp_connectToFileManager(ZFileActionManager* manager)
{
    connect(manager, &ZFileActionManager::zg_calibrationFileListToOpen,
            this, &ZCalibrationRepository::zp_appendCalibrationsToArray);

    connect(this, &ZCalibrationRepository::zg_openCalibrationsActionTriggered,
            manager, &ZFileActionManager::zp_openCalibrations);
}
//==================================================================
QList<QAction*> ZCalibrationRepository::zp_arrayActions() const
{
    QList<QAction*> actionList;
    actionList << zv_newCalibrationAction;
    actionList << zv_openCalibrationsAction;
    actionList << zv_editCalibrationsAction;
    actionList << zv_removeCalibrationAction;
    return actionList;
}
//==================================================================
bool ZCalibrationRepository::zp_isEmpty() const
{
    return zv_caibrationList.isEmpty();
}
//==================================================================
void ZCalibrationRepository::zp_clear()
{
    if(!zv_caibrationList.isEmpty())
    {
        emit zg_currentOperation(OT_REMOVE_CALIBRATIONS, 0, zv_caibrationList.count() - 1);
        qDeleteAll(zv_caibrationList);
        zv_caibrationList.clear();
        emit zg_currentOperation(OT_END_REMOVE_CALIBRATIONS, 0, 0);
    }
}
//======================================================
int ZCalibrationRepository::zp_calibrationCount() const
{
    return zv_caibrationList.count();
}
//======================================================
QString ZCalibrationRepository::zp_calibrationName(int calibrationIndex) const
{
    if(calibrationIndex < 0 || calibrationIndex >= zv_caibrationList.count())
    {
        return QString();
    }


    return zv_caibrationList.value(calibrationIndex)->zp_name();
}
//======================================================
bool ZCalibrationRepository::zp_isChecked(int row)
{
    if(row < 0 || row >= zv_caibrationList.count())
    {
        return false;
    }

    return zv_caibrationList.value(row)->zp_isChecked();
}
//======================================================
void ZCalibrationRepository::zp_setChecked(int row, bool checked)
{
    if(row < 0 || row >= zv_caibrationList.count())
    {
        return;
    }

    zv_caibrationList[row]->zp_setChecked(checked);
}
//======================================================
bool ZCalibrationRepository::zp_isDirty(int row)
{
    if(row < 0 || row >= zv_caibrationList.count())
    {
        return false;
    }

    return zv_caibrationList.value(row)->zp_isDirty();
}
//======================================================
QString ZCalibrationRepository::zp_suffix(int row)
{
    if(row < 0 || row >= zv_caibrationList.count())
    {
        return QString();
    }

    return zv_caibrationList.value(row)->zp_suffix();
}
//======================================================
void ZCalibrationRepository::zp_appendCalibrationsToArray(const QStringList& fileNameList)
{
    foreach(QString fileName, fileNameList)
    {
        zh_createCalibrationFromFile(fileName);
    }
}
//======================================================
void ZCalibrationRepository::zp_onCurrentCalibrationChanged(int current, int previous)
{
    zh_actionAvailabilityControl(current);
}
//======================================================
void ZCalibrationRepository::zh_onNewCalibrationAction()
{
    ZCalibrationEditDialog dialog;
    if(!dialog.exec())
    {
#ifdef DBG
        qDebug() << "REJECT";
#endif

        return;
    }

    zh_createNewCalibration(dialog.zp_calibrationFullName());

#ifdef DBG
    qDebug() << "ACCEPT";
#endif
}
//======================================================
void ZCalibrationRepository::zh_onEditCalibrationsAction()
{
    int currentCalibrationIndex = -1;
    emit zg_requestCurrentCalibrationIndex(currentCalibrationIndex);
    if(currentCalibrationIndex < 0
            || currentCalibrationIndex >= zv_caibrationList.count())
    {
        return;
    }

    ZCalibrationEditDialog dialog;
    if(!dialog.exec())
    {
#ifdef DBG
        qDebug() << "REJECT";
#endif
        return;
    }

#ifdef DBG
    qDebug() << "ACCEPT";
#endif

}
//======================================================
void ZCalibrationRepository::zh_onRemoveCalibrationsAction()
{
    QList<int> selectedCalibrationIndexList;
    emit zg_requestSelectedCalibrationIndexList(selectedCalibrationIndexList);
    if(selectedCalibrationIndexList.isEmpty())
    {
        return;
    }

    qSort(selectedCalibrationIndexList);

    for(int i = selectedCalibrationIndexList.count() - 1; i >= 0; i--)
    {
        zh_removeCalibration(selectedCalibrationIndexList.value(i));
    }
}
//======================================================
void ZCalibrationRepository::zh_removeCalibration(int index)
{
    if(index < 0 || index >= zv_caibrationList.count())
    {
        return;
    }

    emit zg_currentOperation(OT_REMOVE_CALIBRATIONS, index, index);
    delete zv_caibrationList.takeAt(index);
    emit zg_currentOperation(OT_END_REMOVE_CALIBRATIONS, index, index);

}
//======================================================
bool ZCalibrationRepository::zh_createCalibrationFromFile(const QString& fileName)
{
    QFileInfo fileInfo(fileName);
    ZAbstractCalibrationIOHandler* ioHandler;
    QString suffix = fileInfo.suffix();

    if(!fileInfo.exists() || !fileInfo.isFile())
    {
        QString error = tr("Error: \"%1\" is not a file!").arg(fileName);
        emit zg_message(error);
        return false;
    }
    else if(suffix == "clbr")
    {
        ioHandler = new ZSDCalibrationIOHandler(this, this);
    }
    else if(suffix == "clbx")
    {
        ioHandler = new ZXMLCalibrationIOHandler(this, this);
    }
    else
    {
        QString error = QObject::tr("Cannot handle file of type \"%1\"!").arg(suffix);
        emit zg_message(error);
        return false;
    }

    ZAbstractCalibration* calibration;
    bool res = ioHandler->zp_getCalibrationFromFile(fileName, calibration);

    if(res)
    {
        res = zh_appendCalibrationToList(calibration);
    }

    delete ioHandler;
    return res;
}
//======================================================
bool ZCalibrationRepository::zh_createNewCalibration(const QString& name)
{
    if(name.isEmpty())
    {
        return false;
    }
    QFileInfo fileInfo(name);
    QString suffix = fileInfo.suffix();

    if(fileInfo.baseName().isEmpty() || suffix.isEmpty() )
    {
        return false;
    }

    ZAbstractCalibration* calibration;
    if(suffix == "clbr")
    {
        calibration = new ZSDCalibration(name, this);
    }
    else if(suffix == "clbx")
    {
        calibration = new ZXMLCalibration(name, this);
    }
    else
    {
        return false;
    }

    return zh_appendCalibrationToList(calibration);
}
//======================================================
bool ZCalibrationRepository::zh_appendCalibrationToList(ZAbstractCalibration* calibration)
{
    if(!calibration)
    {
        return false;
    }

    int insertIndex = zv_caibrationList.count();
    emit zg_currentOperation(OT_INSERT_CALIBRATIONS, insertIndex, insertIndex);
    zv_caibrationList << calibration;
    emit zg_currentOperation(OT_END_INSERT_CALIBRATIONS, -1, -1);
    return true;
}
//======================================================
void ZCalibrationRepository::zh_actionAvailabilityControl(int current)
{
    zv_editCalibrationsAction->setDisabled(current < 0);
    zv_removeCalibrationAction->setDisabled(current < 0);
#ifdef DBG
    qDebug() << "Availability Control" << current;
#endif
}
//======================================================
void ZCalibrationRepository::zh_createActions()
{
    zv_newCalibrationAction = new QAction(this);
    zv_newCalibrationAction->setIcon(QIcon(":/images/addGreen.png"));
    zv_newCalibrationAction->setText(tr("Append a new calibration"));
    zv_newCalibrationAction->setToolTip(tr("Append a new calibration to the list"));

    zv_openCalibrationsAction = new QAction(this);
    zv_openCalibrationsAction->setIcon(QIcon(":/images/addGreen.png"));
    zv_openCalibrationsAction->setText(tr("Open calibrations"));
    zv_openCalibrationsAction->setToolTip(tr("Append a new calibration to the list"));

    zv_editCalibrationsAction  = new QAction(this);
    zv_editCalibrationsAction->setIcon(QIcon(":/images/addGreen.png"));
    zv_editCalibrationsAction->setText(tr("Edit calibrations"));
    zv_editCalibrationsAction->setToolTip(tr("Edit selected calibration"));

    zv_removeCalibrationAction = new QAction(this);
    zv_removeCalibrationAction->setIcon(QIcon(":/images/CRemove3Blue.png"));
    zv_removeCalibrationAction->setText(tr("Remove selected calibrations"));
    zv_removeCalibrationAction->setToolTip(tr("Remove selected calibrations from the list"));

}
//======================================================
void ZCalibrationRepository::zh_createConnections()
{
    connect(zv_newCalibrationAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onNewCalibrationAction);
    connect(zv_openCalibrationsAction, &QAction::triggered,
            this, &ZCalibrationRepository::zg_openCalibrationsActionTriggered);
    connect(zv_editCalibrationsAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onEditCalibrationsAction);
    connect(zv_removeCalibrationAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onRemoveCalibrationsAction);
}
//======================================================

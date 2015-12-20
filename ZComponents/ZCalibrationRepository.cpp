//======================================================
#include "ZCalibrationRepository.h"
#include "globalVariables.h"
#include "ZXMLCalibrationIOHandler.h"
#include "ZCalibration.h"
#include "ZFileActionManager.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QFile>
#include <QtAlgorithms>

//======================================================
ZCalibrationRepository::ZCalibrationRepository(QObject *parent) : QObject(parent)
{
    zv_defaultCalibrationName = tr("Calibration #");
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
        for(int i = zv_caibrationList.count()-1; i >= 0; i-- )
        {
            delete zv_caibrationList.takeAt(i);
        }
        emit zg_currentOperation(OT_END_REMOVE_CALIBRATIONS, 0, 0);
    }
}
//======================================================
int ZCalibrationRepository::zp_calibrationCount() const
{
    return zv_caibrationList.count();
}
//======================================================
int ZCalibrationRepository::zp_visibleCalibrationCount() const
{
    int visible = 0;
    for(int i = 0; i < zv_caibrationList.count(); i++)
    {
        if(zv_caibrationList.at(i)->zp_isVisible())
        {
            visible++;
        }
    }

    return visible;
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
bool ZCalibrationRepository::zp_setCalibrationName(int row, const QString& name)
{
    if(row < 0 || row >= zv_caibrationList.count())
    {
        return false;
    }

    if(name.isEmpty() || name == zv_caibrationList.value(row)->zp_name())
    {
        return false;
    }

    zv_caibrationList[row]->zp_setBaseName(name);
    emit zg_currentOperation(OT_CALIBRATION_CHANGED, row, row);
    return true;
}
//======================================================
QString ZCalibrationRepository::zp_visibleCalibrationName(int visibleCalibrationIndex) const
{
    if(visibleCalibrationIndex < 0 || visibleCalibrationIndex >= zp_visibleCalibrationCount())
    {
        return QString();
    }

    int visible = -1;
    for(int i = 0; i < zv_caibrationList.count(); i++)
    {
        if(!zv_caibrationList.at(i)->zp_isVisible())
        {
            continue;
        }

        visible++;
        if(visible == visibleCalibrationIndex)
        {
            return zv_caibrationList.at(i)->zp_name();
        }
    }

    return QString();
}
//======================================================
QString ZCalibrationRepository::zp_calibrationChemicalElement(int row) const
{
    if(row < 0 || row >= zv_caibrationList.count())
    {
        return QString();
    }

    return zv_caibrationList.at(row)->zp_chemElement();
}
//======================================================
bool ZCalibrationRepository::zp_setCalibrationChemicalElement(int row, const QString& chemElement)
{
    if(row < 0 || row >= zv_caibrationList.count())
    {
        return false;
    }

    if(chemElement.isEmpty() || chemElement == zv_caibrationList.at(row)->zp_chemElement())
    {
        return false;
    }

    zv_caibrationList[row]->zp_setChemElement(chemElement);
    emit zg_currentOperation(OT_CALIBRATION_CHANGED, row, row);
    return true;
}
//======================================================
bool ZCalibrationRepository::zp_calibrationIsVisible(int row)
{
    if(row < 0 || row >= zv_caibrationList.count())
    {
        return false;
    }

    return zv_caibrationList.value(row)->zp_isVisible();
}
//======================================================
QColor ZCalibrationRepository::zp_calibrationColor(int row)
{
    if(row < 0 || row >= zv_caibrationList.count())
    {
        return QColor();
    }

    return zv_caibrationList.value(row)->zp_color();
}
//======================================================
bool ZCalibrationRepository::zp_setVisible(int row, bool visibility)
{
    if(row < 0 || row >= zv_caibrationList.count())
    {
        return false;
    }

    if(visibility == zv_caibrationList.at(row)->zp_isVisible())
    {
        return false;
    }

    emit zg_currentOperation(OT_CALIBRATION_VISIBILITY_CHANGE, row, row);
    zv_caibrationList[row]->zp_setVisible(visibility);
    emit zg_currentOperation(OT_END_CALIBRATION_VISIBILITY_CHANGE, row, row);
    return true;
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
//QString ZCalibrationRepository::zp_fileSuffix(int row)
//{
//    if(row < 0 || row >= zv_caibrationList.count())
//    {
//        return QString();
//    }

//    return zv_caibrationList.value(row)->zp_suffix();
//}
//======================================================
double ZCalibrationRepository::zp_calculateConcentration(int row, const ZAbstractSpectrum* const spectrum, bool* ok) const
{
    if(row < 0 || row >= zv_caibrationList.count()
            || spectrum == 0)
    {
        if(ok != 0)
        {
            *ok = false;
        }
        return 0;
    }

    return zv_caibrationList.at(row)->zp_calcConcentration(spectrum, ok);
}
//======================================================
void ZCalibrationRepository::zp_appendCalibrationsToArray(const QStringList& fileNameList)
{
    bool res = false;
    foreach(QString fileName, fileNameList)
    {
        res = res || zh_createCalibrationFromFile(fileName);
    }

    if(res)
    {
        emit zg_setCurrentCalibrationIndex(zv_caibrationList.count() - 1);
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
    // new Calibration Name
    int maxCalibrationNumber = 1;
    for(int i = 0; i < zv_caibrationList.count(); i++)
    {
        QString name = zv_caibrationList.at(i)->zp_name();
        if(name.startsWith(zv_defaultCalibrationName))
        {
            QString digitalPart = name.mid(zv_defaultCalibrationName.count());
            bool ok;
            int number = digitalPart.toInt(&ok);
            if(!ok)
            {
                continue;
            }
            if(number >= maxCalibrationNumber)
            {
                maxCalibrationNumber = ++number;
            }
        }
    }

    QString calibrationName = zv_defaultCalibrationName + QString::number(maxCalibrationNumber);
    if(zh_createNewCalibration(calibrationName))
    {
        emit zg_setCurrentCalibrationIndex(zv_caibrationList.count() - 1);
        emit zg_startCurrentCalibrationEdition();
    }
}
//======================================================
void ZCalibrationRepository::zh_onEditCalibrationsAction()
{
//    int currentCalibrationIndex = -1;
//    emit zg_requestCurrentCalibrationIndex(currentCalibrationIndex);
//    if(currentCalibrationIndex < 0
//            || currentCalibrationIndex >= zv_caibrationList.count())
//    {
//        return;
//    }

//    ZCalibrationEditDialog dialog;
//    if(!dialog.exec())
//    {
//        return;
//    }
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
    QString suffix = fileInfo.suffix();

    if(!fileInfo.exists() || !fileInfo.isFile())
    {
        QString error = tr("Error: \"%1\" is not a file!").arg(fileName);
        emit zg_message(error);
        return false;
    }
    else if(suffix != "clbx" && suffix != "xml")
    {
        QString error = QObject::tr("Cannot handle file of type \"%1\"!").arg(suffix);
        emit zg_message(error);
        return false;
    }

    ZXMLCalibrationIOHandler* ioHandler = new ZXMLCalibrationIOHandler(this, this);
    ZCalibration* calibration;
    bool res = ioHandler->zp_getCalibrationFromFile(fileName, calibration);

    if(res)
    {
        calibration->zp_setVisible(true);
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

    ZCalibration* calibration = new ZCalibration(name, this);
    calibration->zp_setVisible(true);
    return zh_appendCalibrationToList(calibration);
}
//======================================================
bool ZCalibrationRepository::zh_appendCalibrationToList(ZCalibration* calibration)
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

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
    actionList << zv_removeCalibrationAction;
    return actionList;
}
//==================================================================
QList<QAction*> ZCalibrationRepository::zp_windowActions() const
{
    QList<QAction*> actionList;
    actionList << zv_newWindowAction;
    actionList << zv_removeWindowAction;
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
        emit zg_calibrationOperation(COT_REMOVE_CALIBRATIONS, 0, zv_caibrationList.count() - 1);
        for(int i = zv_caibrationList.count()-1; i >= 0; i-- )
        {
            delete zv_caibrationList.takeAt(i);
        }
        emit zg_calibrationOperation(COT_END_REMOVE_CALIBRATIONS, 0, 0);
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
qint64 ZCalibrationRepository::zp_calibrationIdForCalibrationIndex(int calibrationIndex) const
{
    if(calibrationIndex < 0 || calibrationIndex >= zv_caibrationList.count())
    {
        return -1;
    }

    return zv_caibrationList.at(calibrationIndex)->zp_calibrationId();
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
    emit zg_calibrationOperation(COT_CALIBRATION_CHANGED, row, row);
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
QString ZCalibrationRepository::zp_calibrationChemicalElementForId(qint64 id) const
{
    if(id < 0)
    {
        return QString();
    }

    for(int i = 0; i < zv_caibrationList.count(); i++)
    {
        if(zv_caibrationList.at(i)->zp_calibrationId() == id)
        {
            return zv_caibrationList.at(i)->zp_chemElement();
        }
    }

    return QString();
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
    emit zg_calibrationOperation(COT_CALIBRATION_CHANGED, row, row);
    return true;
}
//======================================================
bool ZCalibrationRepository::zp_isCalibrationVisible(int row)
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

    emit zg_calibrationOperation(COT_CALIBRATION_VISIBILITY_CHANGE, row, row);
    zv_caibrationList[row]->zp_setVisible(visibility);
    emit zg_calibrationOperation(COT_END_CALIBRATION_VISIBILITY_CHANGE, row, row);
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
bool ZCalibrationRepository::zp_isCalibrationWindowVisible(qint64 calibrationId, int windowIndex) const
{
    if(windowIndex < 0 || calibrationId < 0 )
    {
        return false;
    }

    for(int c = 0; c < zv_caibrationList.count(); c++)
    {
        if(zv_caibrationList.at(c)->zp_calibrationId() != calibrationId)
        {
            continue;
        }

        return zv_caibrationList.at(c)->zp_isCalibrationWindowVisible(windowIndex);
    }

    return false;
}
//======================================================
bool ZCalibrationRepository::zp_isCalibrationWindowVisible(qint64 windowId) const
{
   for(int c = 0; c < zv_caibrationList.count(); c++)
   {
      int windowCount = zv_caibrationList.at(c)->zp_calibrationWindowCount();
      for(int w = 0; w < windowCount; w++)
      {
         if(windowId == zv_caibrationList.at(c)->zp_calibrationWindowId(w))
         {
            return zv_caibrationList.at(c)->zp_isCalibrationWindowVisible(w);
         }
      }
   }
   return false;
}
//======================================================
bool ZCalibrationRepository::zp_setCalibrationWindowVisible(qreal calibrationId, int windowIndex, bool visibility)
{
    if(windowIndex < 0 || calibrationId < 0 )
    {
        return false;
    }

    for(int c = 0; c < zv_caibrationList.count(); c++)
    {
        if(zv_caibrationList.at(c)->zp_calibrationId() != calibrationId)
        {
            continue;
        }

        return zv_caibrationList.at(c)->zp_setCalibrationWindowVisible(windowIndex, visibility);
    }

    return false;
}
//======================================================
int ZCalibrationRepository::zp_calibrationWindowCount(qint64 calibrationId) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(calibration)
    {
        return calibration->zp_calibrationWindowCount();
    }


//    if(calibrationId < 0)
//    {
//        return 0;
//    }

//    for(int c = 0; c < zv_caibrationList.count(); c++)
//    {
//        if(zv_caibrationList.at(c)->zp_calibrationId() != calibrationId)
//        {
//            continue;
//        }

//        return zv_caibrationList.at(c)->zp_calibrationWindowCount();
//    }

    return 0;
}
//======================================================
QString ZCalibrationRepository::zp_calibrationWindowName(qint64 calibrationId, int windowIndex) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(calibration)
    {
        return calibration->zp_calibrationWindowName(windowIndex);
    }


//    if(calibrationId < 0)
//    {
//        return 0;
//    }

//    for(int c = 0; c < zv_caibrationList.count(); c++)
//    {
//        if(zv_caibrationList.at(c)->zp_calibrationId() != calibrationId)
//        {
//            continue;
//        }

//        return zv_caibrationList.at(c)->zp_calibrationWindowName(windowIndex);
//    }

    return 0;
}
//======================================================
bool ZCalibrationRepository::zp_setCalibrationWindowName(qint64 calibrationId, int windowIndex, const QString& name)
{
    ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(calibration)
    {
        return calibration->zp_setCalibrationWindowName(windowIndex, name);
    }


//    if(calibrationId < 0)
//    {
//        return false;
//    }

//    for(int c = 0; c < zv_caibrationList.count(); c++)
//    {
//        if(zv_caibrationList.at(c)->zp_calibrationId() != calibrationId)
//        {
//            continue;
//        }

//        return zv_caibrationList.at(c)->zp_setCalibrationWindowName(windowIndex, name);
//    }

    return false;
}
//======================================================
ZCalibrationWindow::WindowType ZCalibrationRepository::zp_calibrationWindowType(qint64 calibrationId, int windowIndex) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(calibration)
    {
        return calibration->zp_calibrationWindowType(windowIndex);
    }


//    if(calibrationId < 0)
//    {
//        return ZCalibrationWindow::WT_NOT_DEFINED;
//    }

//    for(int c = 0; c < zv_caibrationList.count(); c++)
//    {
//        if(zv_caibrationList.at(c)->zp_calibrationId() != calibrationId)
//        {
//            continue;
//        }

//        return zv_caibrationList.at(c)->zp_calibrationWindowType(windowIndex);
//    }

    return ZCalibrationWindow::WT_NOT_DEFINED;
}
//======================================================
bool ZCalibrationRepository::zp_setCalibrationWindowType(qint64 calibrationId, int windowIndex, ZCalibrationWindow::WindowType type)
{
    ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(calibration)
    {
        return calibration->zp_setCalibrationWindowType(windowIndex, type);
    }

//    if(calibrationId < 0)
//    {
//        return false;
//    }

//    for(int c = 0; c < zv_caibrationList.count(); c++)
//    {
//        if(zv_caibrationList.at(c)->zp_calibrationId() != calibrationId)
//        {
//            continue;
//        }

//        return zv_caibrationList.at(c)->zp_setCalibrationWindowType(windowIndex, type);
//    }

    return false;
}
//======================================================
int ZCalibrationRepository::zp_calibrationWindowFirstChannel(qint64 calibrationId, int windowIndex) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(calibration)
    {
        return calibration->zp_calibrationWindowFirstChannel(windowIndex);
    }

//    if(calibrationId < 0)
//    {
//        return 0;
//    }

//    for(int c = 0; c < zv_caibrationList.count(); c++)
//    {
//        if(zv_caibrationList.at(c)->zp_calibrationId() != calibrationId)
//        {
//            continue;
//        }

//        return zv_caibrationList.at(c)->zp_calibrationWindowFirstChannel(windowIndex);
//    }

    return 0;
}
//======================================================
int ZCalibrationRepository::zp_calibrationWindowLastChannel(qint64 calibrationId, int windowIndex) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(calibration)
    {
        return calibration->zp_calibrationWindowLastChannel(windowIndex);
    }

//    if(calibrationId < 0)
//    {
//        return 0;
//    }

//    for(int c = 0; c < zv_caibrationList.count(); c++)
//    {
//        if(zv_caibrationList.at(c)->zp_calibrationId() != calibrationId)
//        {
//            continue;
//        }

//        return zv_caibrationList.at(c)->zp_calibrationWindowLastChannel(windowIndex);
//    }

    return 0;
}
//======================================================
bool ZCalibrationRepository::zp_setCalibrationWindowFirstChannel(qint64 calibrationId, int windowIndex, int channel)
{
    ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(calibration)
    {
        return calibration->zp_setCalibrationWindowFirstChannel(windowIndex, channel);
    }

//    if(calibrationId < 0)
//    {
//        return false;
//    }

//    for(int c = 0; c < zv_caibrationList.count(); c++)
//    {
//        if(zv_caibrationList.at(c)->zp_calibrationId() != calibrationId)
//        {
//            continue;
//        }

//        return zv_caibrationList.at(c)->zp_setCalibrationWindowFirstChannel(windowIndex, channel);
//    }

    return false;
}
//======================================================
bool ZCalibrationRepository::zp_setCalibrationWindowLastChannel(qint64 calibrationId, int windowIndex, int channel)
{
    ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(calibration)
    {
        return calibration->zp_setCalibrationWindowLastChannel(windowIndex, channel);
    }

//    if(calibrationId < 0)
//    {
//        return false;
//    }

//    for(int c = 0; c < zv_caibrationList.count(); c++)
//    {
//        if(zv_caibrationList.at(c)->zp_calibrationId() != calibrationId)
//        {
//            continue;
//        }

//        return zv_caibrationList.at(c)->zp_setCalibrationWindowLastChannel(windowIndex, channel);
//    }

    return false;
}
//======================================================
QColor ZCalibrationRepository::zp_calibrationWindowColor(qint64 calibrationId, int windowIndex) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(calibration)
    {
        return calibration->zp_color();
    }


//    if(calibrationId < 0)
//    {
//        return QColor();
//    }

//    for(int c = 0; c < zv_caibrationList.count(); c++)
//    {
//        if(zv_caibrationList.at(c)->zp_calibrationId() != calibrationId)
//        {
//            continue;
//        }

//        return zv_caibrationList.at(c)->zp_color();
//    }

    return QColor();
}
//======================================================
qint64 ZCalibrationRepository::zp_calibrationWindowId(qint64 calibrationId,
                                                      int windowIndex) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(calibration)
    {
        return calibration->zp_calibrationWindowId(windowIndex);
    }

//   if(calibrationId < 0)
//   {
//       return -1;
//   }

//   for(int c = 0; c < zv_caibrationList.count(); c++)
//   {
//       if(zv_caibrationList.at(c)->zp_calibrationId() != calibrationId)
//       {
//           continue;
//       }

//       return zv_caibrationList.at(c)->zp_calibrationWindowId(windowIndex);
//   }

   return -1;
}
//======================================================
const ZCalibrationWindow* ZCalibrationRepository::zp_calibrationWindow(qint64 calibrationId,
                                                                       int windowIndex) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(calibration)
    {
        return calibration->zp_calibrationWindow(windowIndex);
    }

//   if(calibrationId < 0)
//   {
//       return 0;
//   }

//   for(int c = 0; c < zv_caibrationList.count(); c++)
//   {
//       if(zv_caibrationList.at(c)->zp_calibrationId() != calibrationId)
//       {
//           continue;
//       }

//       return zv_caibrationList.at(c)->zp_calibrationWindow(windowIndex);
//   }

   return 0;
}
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
int ZCalibrationRepository::zp_termCount(qint64 calibrationId) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(calibration)
    {
        return calibration->zp_termCount();
    }

//    if(calibrationId < 0)
//    {
//        return 0;
//    }

//    for(int c = 0; c < zv_caibrationList.count(); c++)
//    {
//        if(zv_caibrationList.at(c)->zp_calibrationId() != calibrationId)
//        {
//            continue;
//        }

//        return zv_caibrationList.at(c)->zp_termCount();
//    }

    return 0;
}
//======================================================
QString ZCalibrationRepository::zp_termName(qint64 calibrationId, int termIndex) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(calibration)
    {
        return calibration->zp_termName(termIndex);
    }

    return QString();

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
    qint64 calibrationId;
    if(current < 0 || current >= zv_caibrationList.count() )
    {
        calibrationId = -1;
    }
    else
    {
        calibrationId = zv_caibrationList.at(current)->zp_calibrationId();
    }

    emit zg_currentCalibrationChanged(calibrationId, current);
}
//======================================================
void ZCalibrationRepository::zp_onCurrentCalibrationWindowChanged(int currentIndex, int previousIndex)
{
   // current calibration
   int currentCalibrationIndex;

   emit zg_requestCurrentCalibrationIndex(currentCalibrationIndex);
   qint64 currentCalibrationWindowId;
   qint64 previousCalibrationWindowId;

   if(currentCalibrationIndex < 0 || currentCalibrationIndex >= zv_caibrationList.count())
   {
      currentCalibrationWindowId = -1;
      previousCalibrationWindowId = -1;
   }
   else
   {
      if(currentIndex < 0 || currentIndex >= zv_caibrationList.at(currentCalibrationIndex)->zp_calibrationWindowCount())
      {
         currentCalibrationWindowId = -1;
      }
      else
      {
         currentCalibrationWindowId = zv_caibrationList.at(currentCalibrationIndex)->zp_calibrationWindowId(currentIndex);
      }

      if(previousIndex < 0 || previousIndex >= zv_caibrationList.at(currentCalibrationIndex)->zp_calibrationWindowCount())
      {
         previousCalibrationWindowId = -1;
      }
      else
      {
         previousCalibrationWindowId = zv_caibrationList.at(currentCalibrationIndex)->zp_calibrationWindowId(previousIndex);
      }

   }
   emit zg_currentCalibrationWindowChanged(currentCalibrationWindowId, currentIndex,
                                           previousCalibrationWindowId, previousIndex);
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
void ZCalibrationRepository::zh_onNewWindowAction()
{
    int currentCalibrationIndex;
    emit zg_requestCurrentCalibrationIndex(currentCalibrationIndex);

    QRectF currentVisibleSceneRect;
    emit zg_requestCurrentVisibleSceneRect(currentVisibleSceneRect);

    if(currentCalibrationIndex < 0 || currentCalibrationIndex >= zv_caibrationList.count())
    {
        return;
    }

    int offset = qRound(currentVisibleSceneRect.width()/2.5);
    int firstChannel;
    int lastChannel;
    if(currentVisibleSceneRect.width() > 0)
    {
        firstChannel = qRound(currentVisibleSceneRect.left()) + offset;
        lastChannel = qRound(currentVisibleSceneRect.right()) - offset;
    }
    else
    {
        firstChannel = 0;
        lastChannel = 1;
    }
    zv_caibrationList.at(currentCalibrationIndex)->zp_createNewCalibrationWindow(firstChannel,
                                                                                 lastChannel);
}
//======================================================
void ZCalibrationRepository::zh_onRemoveWindowAction()
{
    int currentCalibrationIndex;
    emit zg_requestCurrentCalibrationIndex(currentCalibrationIndex);

    if(currentCalibrationIndex < 0 || currentCalibrationIndex >= zv_caibrationList.count())
    {
        return;
    }

    QList<int> selectedWindowList;
    emit zg_requestSelectedWindowIndexList(selectedWindowList);
    if(selectedWindowList.isEmpty())
    {
        QString string = tr("There is no window to remove!");
        QMessageBox::critical(0, tr("Window removing"), string, QMessageBox::Ok);
        return;
    }

    QString question = tr("Do you want to remove the selected windows from the list?");
    if(QMessageBox::question(0, tr("Window removing"), question, QMessageBox::Yes, QMessageBox::No)
            == QMessageBox::No)
    {
        return;
    }

    qSort(selectedWindowList);
    for(int i = selectedWindowList.count() - 1; i >= 0; i--)
    {
        zh_removeCalibrationWindow(currentCalibrationIndex, selectedWindowList.value(i));
    }
}
//======================================================
void ZCalibrationRepository::zh_removeCalibrationWindow(int currentCalibrationIndex, int spectrumWindowIndex)
{
    if(currentCalibrationIndex < 0 || currentCalibrationIndex >= zv_caibrationList.count())
    {
        return;
    }

    if(spectrumWindowIndex < 0 || spectrumWindowIndex >= zv_caibrationList.at(currentCalibrationIndex)->zp_calibrationWindowCount())
    {
        return;
    }

    zv_caibrationList[currentCalibrationIndex]->zp_removeSpectrumWindow(spectrumWindowIndex);
}
//======================================================
void ZCalibrationRepository::zh_onWindowOperation(ZCalibration::WindowOperationType type, int first, int last)
{
    if(!sender())
    {
        return;
    }

    ZCalibration* calibration = static_cast<ZCalibration*>(sender());
    for(int calibrationIndex = 0; calibrationIndex < zv_caibrationList.count(); calibrationIndex++)
    {
        if(zv_caibrationList.at(calibrationIndex) != calibration)
        {
            continue;
        }

        if(type == ZCalibration::WOT_BRGIN_INSERT_WINDOWS)
        {
            emit zg_calibrationWindowOperation(WOT_INSERT_WINDOWS, calibrationIndex, first, last);
        }
        else if(type == ZCalibration::WOT_END_INSERT_WINDOWS)
        {
            emit zg_calibrationWindowOperation(WOT_END_INSERT_WINDOWS, calibrationIndex, first, last);
        }
        else if(type == ZCalibration::WOT_BEGIN_REMOVE_WINDOWS)
        {
            emit zg_calibrationWindowOperation(WOT_REMOVE_WINDOWS, calibrationIndex, first, last);
        }
        else if(type == ZCalibration::WOT_END_REMOVE_WINDOWS)
        {
            emit zg_calibrationWindowOperation(WOT_END_REMOVE_WINDOWS, calibrationIndex, first, last);
        }
        else if(type == ZCalibration::WOT_WINDOW_CHANGED)
        {
            emit zg_calibrationWindowOperation(WOT_WINDOW_CHANGED, calibrationIndex, first, last);
        }

        break;
    }
}
//======================================================
void ZCalibrationRepository::zh_onTermOperation(ZCalibration::TremOperationType type, int first, int last)
{
    if(!sender())
    {
        return;
    }

    ZCalibration* calibration = static_cast<ZCalibration*>(sender());
    for(int calibrationIndex = 0; calibrationIndex < zv_caibrationList.count(); calibrationIndex++)
    {
        if(zv_caibrationList.at(calibrationIndex) != calibration)
        {
            continue;
        }

        if(type == ZCalibration::POT_BEGIN_INSERT_TERM)
        {
            emit zg_termOperation(POT_BEGIN_INSERT_TERM, calibrationIndex, first, last);
        }
        else if(type == ZCalibration::POT_END_INSERT_TERM)
        {
            emit zg_termOperation(POT_END_INSERT_TERM, calibrationIndex, first, last);
        }
        else if(type == ZCalibration::POT_BEGIN_REMOVE_TERM)
        {
            emit zg_termOperation(POT_BEGIN_REMOVE_TERM, calibrationIndex, first, last);
        }
        else if(type == ZCalibration::POT_END_REMOVE_TERM)
        {
            emit zg_termOperation(POT_END_REMOVE_TERM, calibrationIndex, first, last);
        }
        else if(type == ZCalibration::POT_TERM_CHANGED)
        {
            emit zg_termOperation(POT_TERM_CHANGED, calibrationIndex, first, last);
        }

        break;
    }
}
//======================================================
void ZCalibrationRepository::zh_removeCalibration(int index)
{
    if(index < 0 || index >= zv_caibrationList.count())
    {
        return;
    }

    emit zg_calibrationOperation(COT_REMOVE_CALIBRATIONS, index, index);
    delete zv_caibrationList.takeAt(index);
    emit zg_calibrationOperation(COT_END_REMOVE_CALIBRATIONS, index, index);
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

    connect(calibration, &ZCalibration::zg_message,
                this, &ZCalibrationRepository::zg_message);


    connect(calibration, &ZCalibration::zg_windowOperation,
            this, &ZCalibrationRepository::zh_onWindowOperation);
    connect(calibration, &ZCalibration::zg_termOperation,
            this, &ZCalibrationRepository::zh_onTermOperation);


     //    connect(calibration, &ZCalibration::zg_visibilityChanged,
    //            this, &ZCalibrationRepository::zh_onCalibrationOperation);
    //    connect(calibration, &ZCalibration::zg_dirtyChanged,
    //            this, &ZCalibrationRepository::zh_onCalibrationOperation);



    int insertIndex = zv_caibrationList.count();
    emit zg_calibrationOperation(COT_INSERT_CALIBRATIONS, insertIndex, insertIndex);
    zv_caibrationList << calibration;
    emit zg_calibrationOperation(COT_END_INSERT_CALIBRATIONS, -1, -1);
    return true;
}
//======================================================
void ZCalibrationRepository::zh_actionAvailabilityControl(int current)
{
    zv_removeCalibrationAction->setDisabled(current < 0);
}
//======================================================
void ZCalibrationRepository::zh_createActions()
{
    // calibration actions
    zv_newCalibrationAction = new QAction(this);
    zv_newCalibrationAction->setIcon(QIcon(":/images/addGreen.png"));
    zv_newCalibrationAction->setText(tr("Append a new calibration"));
    zv_newCalibrationAction->setToolTip(tr("Append a new calibration to the list"));

    zv_openCalibrationsAction = new QAction(this);
    zv_openCalibrationsAction->setIcon(QIcon(":/images/addGreen.png"));
    zv_openCalibrationsAction->setText(tr("Open calibrations"));
    zv_openCalibrationsAction->setToolTip(tr("Append a new calibration to the list"));

    zv_removeCalibrationAction = new QAction(this);
    zv_removeCalibrationAction->setIcon(QIcon(":/images/CRemove3Blue.png"));
    zv_removeCalibrationAction->setText(tr("Remove selected calibrations"));
    zv_removeCalibrationAction->setToolTip(tr("Remove selected calibrations from the list"));

    // window actions
    zv_newWindowAction = new QAction(this);
    zv_newWindowAction->setIcon(QIcon(":/images/CRemove3Blue.png"));
    zv_newWindowAction->setText(tr("Remove selected calibrations"));
    zv_newWindowAction->setToolTip(tr("Remove selected calibrations from the list"));

    zv_removeWindowAction = new QAction(this);
    zv_removeWindowAction->setIcon(QIcon(":/images/CRemove3Blue.png"));
    zv_removeWindowAction->setText(tr("Remove selected calibrations"));
    zv_removeWindowAction->setToolTip(tr("Remove selected calibrations from the list"));

}
//======================================================
void ZCalibrationRepository::zh_createConnections()
{
    connect(zv_newCalibrationAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onNewCalibrationAction);
    connect(zv_openCalibrationsAction, &QAction::triggered,
            this, &ZCalibrationRepository::zg_openCalibrationsActionTriggered);
    connect(zv_removeCalibrationAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onRemoveCalibrationsAction);

    connect(zv_newWindowAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onNewWindowAction);
    connect(zv_removeWindowAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onRemoveWindowAction);

}
//======================================================
ZCalibration* ZCalibrationRepository::zh_calibrationForId(qint64 calibrationId) const
{
    if(calibrationId < 0)
    {
        return 0;
    }

    for(int c = 0; c < zv_caibrationList.count(); c++)
    {
        if(zv_caibrationList.at(c)->zp_calibrationId() != calibrationId)
        {
            continue;
        }

        return zv_caibrationList.value(c);
    }

    return 0;
}
//======================================================

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
#include <QClipboard>
#include <QMimeData>
//======================================================
ZCalibrationRepository::ZCalibrationRepository(QObject *parent) : QObject(parent)
{
    zv_spectrumArrayRepository = 0;
    zv_newCalibrationAction = 0;
    zv_openCalibrationsAction = 0;
    zv_removeCalibrationAction = 0;

    zv_newWindowAction = 0;
    zv_removeWindowAction = 0;

    zv_recalcEquationFactorsAction = 0;

    zv_defaultCalibrationName = tr("Calibration #");
    zv_defaultCalibrationWindowType = ZCalibrationWindow::WT_PEAK;
    zv_currentArrayId = -1;
    zv_currentArrayIndex = 0;
    zv_currentCalibrationIndex = -1;
    zv_currentTermIndex = -1;

    zh_createActions();
    zh_createConnections();
    zh_actionEnablingControl();
}
//==================================================================
void ZCalibrationRepository::zp_appendActionsToMenu(QMenu *menu) const
{
    if(menu->objectName() == "Edit")
    {
        QMenu* calibrationMenu = new QMenu(tr("Calibrations"));
        calibrationMenu->setIcon(QIcon(glCalibrationIconString));
        calibrationMenu->addAction(zv_newCalibrationAction);
        calibrationMenu->addAction(zv_removeCalibrationAction);
        menu->addMenu(calibrationMenu);

        QMenu* windowsMenu = new QMenu(tr("Calibration windows"));
        windowsMenu->setIcon(QIcon(glWindowIconString));
        windowsMenu->addAction(zv_newWindowAction);
        windowsMenu->addAction(zv_removeWindowAction);
        menu->addMenu(windowsMenu);

        QMenu* termMenu = new QMenu(tr("Equation terms"));
        termMenu->setIcon(QIcon());
        termMenu->addAction(zv_createMixedTermsAction);
        termMenu->addAction(zv_removeMixedTermsAction);
        termMenu->addSeparator();
        termMenu->addAction(zv_createCustomTermAction);
        termMenu->addAction(zv_removeCustomTermAction);

        menu->addMenu(termMenu);
    }
    else if(menu->objectName() == "Actions")
    {
        menu->addAction(zv_recalcEquationFactorsAction);
        menu->addSeparator();
    }
}
//==================================================================
void ZCalibrationRepository::zp_connectToFileManager(ZFileActionManager* manager)
{
    connect(manager, &ZFileActionManager::zg_calibrationFileListToOpen,
            this, &ZCalibrationRepository::zh_appendCalibrationsToArray);
    connect(manager, &ZFileActionManager::zg_requestCalibrationDataAndInitSaving,
            this, &ZCalibrationRepository::zh_createCalibrationAndStartSaving);

    connect(this, &ZCalibrationRepository::zg_openCalibrationsActionTriggered,
            manager, &ZFileActionManager::zp_openCalibrations);
    connect(this, &ZCalibrationRepository::zg_currentCalibrationDirtyChanged,
            manager, &ZFileActionManager::zp_onCurrentCalibrationDirtyChange);
    connect(this, &ZCalibrationRepository::zg_saveCalibration,
            manager, &ZFileActionManager::zp_saveCalibrationToFile);
    connect(manager, &ZFileActionManager::zg_calibrationSaved,
            this, &ZCalibrationRepository::zh_onCalibrationSaving);

}
//==================================================================
QList<QAction*> ZCalibrationRepository::zp_calibrationActions() const
{
    QList<QAction*> actionList;
    actionList << zv_newCalibrationAction;
    actionList << zv_openCalibrationsAction;
    actionList << zv_removeCalibrationAction;
    return actionList;
}
//==================================================================
QList<QAction*> ZCalibrationRepository::zp_calibrationContextMenuActions() const
{
    QList<QAction*> actionList;
    actionList << zv_newCalibrationAction;
    actionList << zv_openCalibrationsAction;
    actionList << zv_removeCalibrationAction;
    actionList << 0;
    actionList << zv_copySelectedCalibrationAction;
    actionList << 0;
    actionList << zv_setCalibrationVisibleAction;
    actionList << zv_setCalibrationInvisibleAction;
    actionList << zv_invertCalibrationVisibilityAction;

    return actionList;
}
//==================================================================
QList<QAction*> ZCalibrationRepository::zp_calibrationVisibilityActions() const
{
    QList<QAction*> actionList;
    actionList << zv_setCalibrationVisibleAction;
    actionList << zv_setCalibrationInvisibleAction;
    actionList << zv_invertCalibrationVisibilityAction;
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
QList<QAction*> ZCalibrationRepository::zp_windowContextMenuActions() const
{
    QList<QAction*> actionList;
    actionList << zv_newWindowAction;
    actionList << zv_removeWindowAction;
    //    actionList << 0;
    //    actionList << zv_copySelectedWindowsAction;
    //    actionList << zv_pasteWindowsAction;
    actionList << 0;
    actionList << zv_setWindowsVisibleAction;
    actionList << zv_setWindowsInvisibleAction;
    actionList << zv_invertWindowsVisibilityAction;

    return actionList;
}
//==================================================================
QList<QAction*> ZCalibrationRepository::zp_termActions() const
{
    QList<QAction*> actionList;
    actionList << zv_createMixedTermsAction;
    actionList << zv_removeMixedTermsAction;
    //    actionList << 0;
    //    actionList << zv_createCustomTermAction;
    //    actionList << zv_removeCustomTermAction;
    return actionList;
}
//==================================================================
QList<QAction*> ZCalibrationRepository::zp_termContextMenuActions() const
{
    QList<QAction*> actionList;
    actionList << zv_resetTermStateAction;
    actionList << 0;
    actionList << zv_createMixedTermsAction;
    actionList << zv_removeMixedTermsAction;
    actionList << 0;
    actionList << zv_createCustomTermAction;
    actionList << zv_removeCustomTermAction;
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


    return zv_caibrationList.value(calibrationIndex)->zp_baseName();
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

    if(name.isEmpty() || name == zv_caibrationList.value(row)->zp_baseName())
    {
        return false;
    }

    zv_caibrationList[row]->zp_setBaseName(name);
    emit zg_calibrationOperation(COT_CALIBRATION_NAME_CHANGED, row, row);
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
            return zv_caibrationList.at(i)->zp_baseName();
        }
    }

    return QString();
}
//======================================================
qint64 ZCalibrationRepository::zp_visibleCalibrationId(int visibleCalibrationIndex) const
{
    if(visibleCalibrationIndex < 0 || visibleCalibrationIndex >= zp_visibleCalibrationCount())
    {
        return -1;
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
            return zv_caibrationList.at(i)->zp_calibrationId();
        }
    }

    return -1;
}
//======================================================
int ZCalibrationRepository::zp_calibrationIndexForVisibleIndex(int visibleCalibrationIndex) const
{
    if(visibleCalibrationIndex < 0 || visibleCalibrationIndex >= zp_visibleCalibrationCount())
    {
        return -1;
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
            return i;
        }
    }

    return -1;
}
//======================================================
QString ZCalibrationRepository::zp_chemElement(int row) const
{
    if(row < 0 || row >= zv_caibrationList.count())
    {
        return QString();
    }

    return zv_caibrationList.at(row)->zp_chemElement();
}
//======================================================
QString ZCalibrationRepository::zp_chemElementForCalibrationId(qint64 id) const
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
bool ZCalibrationRepository::zp_setChemElement(int row, const QString& chemElement)
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
    emit zg_calibrationOperation(COT_CALIBRATION_CHEM_ELEMENT_CHANGED, row, row);
    return true;
}
//======================================================
QString ZCalibrationRepository::zp_baseTermString(int row) const
{
    if(row < 0 || row >= zv_caibrationList.count())
    {
        return QString();
    }

    return zv_caibrationList.at(row)->zp_baseTermDisplayString();
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
bool ZCalibrationRepository::zp_setCalibrationVisible(int row, bool visibility)
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
    if(!calibration)
    {
        return 0;
    }

    return calibration->zp_calibrationWindowCount();
}
//======================================================
QString ZCalibrationRepository::zp_calibrationWindowName(qint64 calibrationId, int windowIndex) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return QString();
    }

    return calibration->zp_calibrationWindowName(windowIndex);
}
//======================================================
bool ZCalibrationRepository::zp_setCalibrationWindowName(qint64 calibrationId, int windowIndex, const QString& name)
{
    ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return false;
    }

    return calibration->zp_setCalibrationWindowName(windowIndex, name);
}
//======================================================
ZCalibrationWindow::WindowType ZCalibrationRepository::zp_calibrationWindowType(qint64 calibrationId, int windowIndex) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return ZCalibrationWindow::WT_NOT_DEFINED;
    }

    return calibration->zp_calibrationWindowType(windowIndex);
}
//======================================================
bool ZCalibrationRepository::zp_setCalibrationWindowType(qint64 calibrationId, int windowIndex, ZCalibrationWindow::WindowType type)
{
    ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return false;
    }

    return calibration->zp_setCalibrationWindowType(windowIndex, type);
}
//======================================================
int ZCalibrationRepository::zp_calibrationWindowFirstChannel(qint64 calibrationId, int windowIndex) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return 0;
    }

    return calibration->zp_calibrationWindowFirstChannel(windowIndex);

}
//======================================================
int ZCalibrationRepository::zp_calibrationWindowLastChannel(qint64 calibrationId, int windowIndex) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return 0;
    }

    return calibration->zp_calibrationWindowLastChannel(windowIndex);

}
//======================================================
bool ZCalibrationRepository::zp_setCalibrationWindowFirstChannel(qint64 calibrationId, int windowIndex, int channel)
{
    ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return false;
    }

    return calibration->zp_setCalibrationWindowFirstChannel(windowIndex, channel);

}
//======================================================
bool ZCalibrationRepository::zp_setCalibrationWindowLastChannel(qint64 calibrationId, int windowIndex, int channel)
{
    ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return false;
    }

    return calibration->zp_setCalibrationWindowLastChannel(windowIndex, channel);

}
//======================================================
QColor ZCalibrationRepository::zp_calibrationWindowColor(qint64 calibrationId, int windowIndex) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return QColor();
    }

    return calibration->zp_color();
}
//======================================================
qint64 ZCalibrationRepository::zp_calibrationWindowId(qint64 calibrationId,
                                                      int windowIndex) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return -1;
    }

    return calibration->zp_calibrationWindowId(windowIndex);

}
//======================================================
const ZCalibrationWindow* ZCalibrationRepository::zp_calibrationWindow(qint64 calibrationId,
                                                                       int windowIndex) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return 0;
    }

    return calibration->zp_calibrationWindow(windowIndex);
}
//======================================================
bool ZCalibrationRepository::zp_calculateConcentration(int row, const ZAbstractSpectrum* spectrum, qreal& concentration) const
{
    if(row < 0 || row >= zv_caibrationList.count()
            || spectrum == 0)
    {
        concentration = 0;
        return false;
    }

    return zv_caibrationList.at(row)->zp_calcConcentration(spectrum, concentration);
}
//======================================================
bool ZCalibrationRepository::zp_calculateConcentrationForId(qint64 calibrationId, const ZAbstractSpectrum* spectrum, qreal& concentration) const
{
    if(calibrationId < 0)
    {
        return false;
    }

    ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return false;
    }

    return calibration->zp_calcConcentration(spectrum, concentration);
}
//======================================================
int ZCalibrationRepository::zp_termCount(qint64 calibrationId) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return 0;
    }

    return calibration->zp_termCount();
}
//======================================================
QString ZCalibrationRepository::zp_termName(qint64 calibrationId, int termIndex) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return QString();
    }
    return calibration->zp_termDisplayName(termIndex);
}
//======================================================
ZAbstractTerm::TermState ZCalibrationRepository::zp_termState(qint64 calibrationId, int termIndex)
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return ZAbstractTerm::TS_NOT_DEFINED;
    }
    return calibration->zp_termStateForDisplay(termIndex);
}
//======================================================
void ZCalibrationRepository::zp_setNextUsersTermState(qint64 calibrationId, int termLogIndex)
{
    ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return;
    }

    calibration->zp_setNextUsersTermState(termLogIndex);
}
//======================================================
ZTermNormalizer::NormaType ZCalibrationRepository::zp_normaTypeForCalibrationId(qint64 calibrationId) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return ZTermNormalizer::NT_NONE;
    }

    return calibration->zp_normaType();
}
//======================================================
bool ZCalibrationRepository::zp_setNormaTypeForCalibrationId(qint64 calibrationId, ZTermNormalizer::NormaType type)
{
    ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return false;
    }

    return calibration->zp_setNormaType(type);
}
//======================================================
ZTermNormalizer::NormaType ZCalibrationRepository::zp_normaType(int calibrationIndex) const
{
    if(calibrationIndex < 0 || calibrationIndex >= zv_caibrationList.count())
    {
        return ZTermNormalizer::NT_NONE;
    }

    return zv_caibrationList.at(calibrationIndex)->zp_normaType();
}
//======================================================
bool ZCalibrationRepository::zp_setNormaType(int calibrationIndex, ZTermNormalizer::NormaType type)
{
    if(calibrationIndex < 0 || calibrationIndex >= zv_caibrationList.count())
    {
        return false;
    }

    return zv_caibrationList.at(calibrationIndex)->zp_setNormaType(type);
}
//======================================================
bool ZCalibrationRepository::zp_equationTypeForCalibrationId(qint64 calibrationId, ZCalibration::EquationType& type) const
{
    ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return false;
    }

    type = calibration->zp_equationType();
    return true;
}
//======================================================
bool ZCalibrationRepository::zp_setEquationTypeForCalibrationId(qint64 calibrationId, ZCalibration::EquationType type)
{
    ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return false;
    }

    return calibration->zp_setEquationType(type);
}
//======================================================
ZCalibration::EquationType ZCalibrationRepository::zp_equationType(int calibrationIndex) const
{
    if(calibrationIndex < 0 || calibrationIndex >= zv_caibrationList.count())
    {
        return ZCalibration::ET_NOT_DEFINED;
    }

    return zv_caibrationList.at(calibrationIndex)->zp_equationType();
}
//======================================================
bool ZCalibrationRepository::zp_setEquationType(int calibrationIndex, ZCalibration::EquationType type)
{
    if(calibrationIndex < 0 || calibrationIndex >= zv_caibrationList.count())
    {
        return false;
    }

    return zv_caibrationList.at(calibrationIndex)->zp_setEquationType(type);
}
//======================================================
ZNormaSettingsData ZCalibrationRepository::zp_normaSettingsData(int calibrationIndex) const
{
    if(calibrationIndex < 0 || calibrationIndex >= zv_caibrationList.count())
    {
        return ZNormaSettingsData();
    }

    return ZNormaSettingsData(zv_caibrationList.at(calibrationIndex));
}
//======================================================
bool ZCalibrationRepository::zp_setNormaSettings(int calibrationIndex, ZNormaSettingsData settings)
{
    if(calibrationIndex < 0 || calibrationIndex >= zv_caibrationList.count())
    {
        return false;
    }

    ZCalibration* calibration = zv_caibrationList.value(calibrationIndex);
    calibration->zp_setNormaType(settings.normaType);
    calibration->zp_setNormaCustomString(settings.customNormaString);
    return true;
}
//======================================================
ZEquationSettingsData ZCalibrationRepository::zp_equationDataForSettings(int calibrationIndex) const
{
    if(calibrationIndex < 0 || calibrationIndex >= zv_caibrationList.count())
    {
        return ZEquationSettingsData();
    }

    return ZEquationSettingsData(zv_caibrationList.at(calibrationIndex));
}
//======================================================
bool ZCalibrationRepository::zp_setEquationDataFromSettings(int calibrationIndex, ZEquationSettingsData settings)
{
    if(calibrationIndex < 0 || calibrationIndex >= zv_caibrationList.count())
    {
        return false;
    }

    ZCalibration* calibration = zv_caibrationList.value(calibrationIndex);
    if(calibration->zp_setEquationType(settings.equationType))
    {
        emit zg_calibrationOperation(COT_CALIBRATION_EQUATION_TYPE_CHANGED, calibrationIndex, calibrationIndex);
    }

    bool res = calibration->zp_setBaseTermId(settings.baseTermId);
    res =  calibration->zp_setBaseTermNormalizerParameters(settings.fractionalBaseNormaSettingsData.normaType,
                                                           settings.fractionalBaseNormaSettingsData.customNormaString) || res;
    if(res)
    {
        emit zg_calibrationOperation(COT_CALIBRATION_EQUATION_BASE_TERM_CHANGED, calibrationIndex, calibrationIndex);
        emit zg_invokeCalibrationRecalc();
    }

    return true;
}
//======================================================
qreal ZCalibrationRepository::zp_equationIntercept(int calibrationIndex) const
{
    if(calibrationIndex < 0 || calibrationIndex >= zv_caibrationList.count())
    {
        return 0.0;
    }

    return zv_caibrationList.at(calibrationIndex)->zp_equationIntercept();
}
//======================================================
bool ZCalibrationRepository::zp_setEquationIntercept(int calibrationIndex, qreal value)
{
    if(calibrationIndex < 0 || calibrationIndex >= zv_caibrationList.count())
    {
        return false;
    }

    bool res = zv_caibrationList.at(calibrationIndex)->zp_setEquationIntercept(value);
    return res;
}
//======================================================
QString ZCalibrationRepository::zp_equationInterceptString(int calibrationIndex) const
{
    if(calibrationIndex < 0 || calibrationIndex >= zv_caibrationList.count())
    {
        return "0.0";
    }

    return zv_caibrationList.at(calibrationIndex)->zp_equationInterceptString();
}
//======================================================
bool ZCalibrationRepository::zp_setEquationInterceptString(int calibrationIndex, const QString& interceptString)
{
    if(calibrationIndex < 0 || calibrationIndex >= zv_caibrationList.count())
    {
        return false;
    }

    bool res = zv_caibrationList.at(calibrationIndex)->zp_setEquationInterceptString(interceptString);
    return res;
}
//======================================================
int ZCalibrationRepository::zp_baseTermIndex(int calibrationIndex)
{
    if(calibrationIndex < 0 || calibrationIndex >= zv_caibrationList.count())
    {
        return -1;
    }

    return zv_caibrationList.at(calibrationIndex)->zp_baseTermIndex();
}
//======================================================
qint64 ZCalibrationRepository::zp_baseTermId(int calibrationIndex)
{
    if(calibrationIndex < 0 || calibrationIndex >= zv_caibrationList.count())
    {
        return -1;
    }

    return zv_caibrationList.at(calibrationIndex)->zp_baseTermId();
}
//======================================================
int ZCalibrationRepository::zp_baseTermIndexForCalibrationId(qint64 calibrationId)
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return -1;
    }

    return calibration->zp_baseTermIndex();
}
//======================================================
qint64 ZCalibrationRepository::zp_baseTermIdForCalibrationId(qint64 calibrationId)
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return false;
    }

    return calibration->zp_baseTermId();
}
//======================================================
void ZCalibrationRepository::zh_notifyCalibrationRecalc(qint64 calibrationId) const
{
    ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return;
    }

    calibration->zh_notifyCalibrationRecalc();
}
//======================================================
bool ZCalibrationRepository::zp_termFactor(qint64 calibrationId, int termIndex, qreal& factor) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        factor = 0;
        return false;
    }

    return calibration->zp_termFactor(termIndex, factor);
}
//======================================================
bool ZCalibrationRepository::zp_termFactorString(qint64 calibrationId,
                                                 int termIndex, QString &factorString) const
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        factorString = "0.0";
        return false;
    }
    return calibration->zp_termFactorString(termIndex, factorString);
}
//======================================================
bool ZCalibrationRepository::zp_setTermFactorString(qint64 calibrationId,
                                                    int termIndex, const QString& factorString)
{
    ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return false;
    }
    return calibration->zp_setTermFactorString(termIndex, factorString);
}
//======================================================
bool ZCalibrationRepository::zp_termVariablePart(qint64 calibrationId,
                                                 int termIndex,
                                                 const ZAbstractSpectrum *spectrum,
                                                 qreal& value)
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return false;
    }
    return calibration->zp_termVariablePart(termIndex, spectrum, value);
}
//======================================================
bool ZCalibrationRepository::zp_calcBaseTermValue(qint64 calibrationId, const ZAbstractSpectrum *spectrum, qreal& value)
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return false;
    }

    return calibration->zp_calcBaseTermValue(spectrum, value);
}
//======================================================
void ZCalibrationRepository::zh_appendCalibrationsToArray(const QStringList& fileNameList)
{
    bool res = false;
    foreach(QString fileName, fileNameList)
    {
        res = zh_createCalibrationFromFile(fileName) || res;
    }

    if(res)
    {
        emit zg_setCurrentCalibrationIndex(zv_caibrationList.count() - 1);
    }

    zh_actionEnablingControl();
}
//======================================================
void ZCalibrationRepository::zp_onCurrentCalibrationChange(int current, int previous)
{
    qint64 calibrationId;
    zv_currentCalibrationIndex = current;
    if(current < 0 || current >= zv_caibrationList.count() )
    {
        calibrationId = -1;
        emit zg_currentCalibrationDirtyChanged(false);
    }
    else
    {
        calibrationId = zv_caibrationList.at(current)->zp_calibrationId();
        emit zg_currentCalibrationDirtyChanged(zv_caibrationList.at(current)->zp_isDirty());
    }

    emit zg_currentCalibrationChanged(calibrationId, current);
    zh_actionEnablingControl();
}
//======================================================
void ZCalibrationRepository::zp_onSelectedCalibrationChange(QList<int> selectedIndexList)
{
    zh_actionEnablingControl();
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
void ZCalibrationRepository::zp_onCurrentTermChange(int currentTermIndex, int previousTermIndex)
{
    zv_currentTermIndex = currentTermIndex;
    // emit zg_invokeCalibrationRecalc();
}
//======================================================
void ZCalibrationRepository::zp_calibrationQualityDataChanged(bool saveTocalibration,
                                                              qint64 calibrationId,
                                                              ZCalibrationQualityData qualityData)
{
    if(!saveTocalibration)
    {
        return;
    }

    ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return;
    }

    calibration->zp_setCalibrationQualityData(qualityData);
}
//======================================================
void ZCalibrationRepository::zh_onNewCalibrationAction()
{
    // new Calibration Name
    int maxCalibrationNumber = 1;
    for(int i = 0; i < zv_caibrationList.count(); i++)
    {
        QString name = zv_caibrationList.at(i)->zp_baseName();
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

    zh_actionEnablingControl();
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

    zh_actionEnablingControl();
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

    //int offset = qRound(currentVisibleSceneRect.width()/5);
    int windowWidth = qRound(currentVisibleSceneRect.width() / 20);
    int offset = qRound((currentVisibleSceneRect.width() - windowWidth) / 2);
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

    int newWindowIndex = zv_caibrationList.at(currentCalibrationIndex)->zp_createNewCalibrationWindow(firstChannel,
                                                                                                      lastChannel,
                                                                                                      zv_defaultCalibrationWindowType);

    emit zg_setCurrentWindowIndex(newWindowIndex);
    emit zg_startCurrentWindowEdition();

    zh_actionEnablingControl();
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

    zh_actionEnablingControl();
}
//======================================================
void ZCalibrationRepository::zh_onRecalcEquationFactorsAction()
{
    // zh_recalcEquationFactors(zv_currentCalibrationIndex);
    emit zg_invokeCalibrationRecalc();
}
//======================================================
void ZCalibrationRepository::zh_onCreateMixedTermsAction()
{
    if(zv_currentCalibrationIndex < 0 || zv_currentCalibrationIndex >= zv_caibrationList.count())
    {
        return;
    }

    ZCalibration* calibration = zv_caibrationList.at(zv_currentCalibrationIndex);
    calibration->zp_createMixedTerms(zv_currentTermIndex);
}
//======================================================
void ZCalibrationRepository::zh_onRemoveMixedTermsAction()
{
    if(zv_currentCalibrationIndex < 0 || zv_currentCalibrationIndex >= zv_caibrationList.count())
    {
        return;
    }

    ZCalibration* calibration = zv_caibrationList.at(zv_currentCalibrationIndex);
    calibration->zp_removeMixedTerms();
}
//======================================================
void ZCalibrationRepository::zh_onCreateCustomTermAction()
{

}
//======================================================
void ZCalibrationRepository::zh_onRemoveCustomTermAction()
{

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

    zv_caibrationList[currentCalibrationIndex]->zp_removeCalibrationWindow(spectrumWindowIndex);
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
            emit zg_invokeCalibrationRecalc();
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

        if(type == ZCalibration::TOT_BEGIN_INSERT_TERM)
        {
            emit zg_termOperation(TOT_BEGIN_INSERT_TERM, calibrationIndex, first, last);
        }
        else if(type == ZCalibration::TOT_END_INSERT_TERM)
        {
            emit zg_termOperation(TOT_END_INSERT_TERM, calibrationIndex, first, last);
        }
        else if(type == ZCalibration::TOT_BEGIN_REMOVE_TERM)
        {
            emit zg_termOperation(TOT_BEGIN_REMOVE_TERM, calibrationIndex, first, last);
        }
        else if(type == ZCalibration::TOT_END_REMOVE_TERM)
        {
            emit zg_termOperation(TOT_END_REMOVE_TERM, calibrationIndex, first, last);
            emit zg_invokeCalibrationRecalc();
        }
        else if(type == ZCalibration::TOT_TERM_NAME_CHANGED)
        {
            emit zg_termOperation(TOT_TERM_NAME_CHANGED, calibrationIndex, first, last);
        }
        else if(type == ZCalibration::TOT_TERM_STATE_CHANGED)
        {
            emit zg_termOperation(TOT_TERM_STATE_CHANGED, calibrationIndex, first, last);
            emit zg_invokeCalibrationRecalc();

            //zh_recalcEquationFactors(calibrationIndex);
        }
        else if(type == ZCalibration::TOT_TERM_WINDOW_MARGIN_CHANGED)
        {
            emit zg_termOperation(TOT_TERM_WINDOW_MARGIN_CHANGED, calibrationIndex, first, last);
        }
        else if(type == ZCalibration::TOT_TERM_FACTOR_CHANGED)
        {
            emit zg_termOperation(TOT_TERM_FACTOR_CHANGED, calibrationIndex, first, last);
        }

        break;
    }
}
//======================================================
void ZCalibrationRepository::zh_onNormalizerChange() const
{
    if(!sender())
    {
        return;
    }

    ZCalibration* calibration = static_cast<ZCalibration*>(sender());
    qint64 calibrationId = calibration->zp_calibrationId();
    emit zg_normalizerChanged(calibrationId);

    emit zg_invokeCalibrationRecalc();
}
//======================================================
void ZCalibrationRepository::zh_onCalibrationFreeTermChange() const
{
    if(!sender())
    {
        return;
    }

    ZCalibration* calibration = static_cast<ZCalibration*>(sender());
    qint64 calibrationId = calibration->zp_calibrationId();
    emit zg_calibrationOperation(COT_CALIBRATION_FREE_MEMBER_CHANGED, calibrationId, calibrationId);
}
//======================================================
void ZCalibrationRepository::zh_onCalibrationDirtyChange(bool dirty) const
{
    emit zg_currentCalibrationDirtyChanged(dirty);
    emit zg_calibrationOperation(COT_CALIBRATION_DIRTY_CHANGED, zv_currentCalibrationIndex, zv_currentArrayIndex);
}
//======================================================
void ZCalibrationRepository::zh_createCalibrationAndStartSaving(QString path, QString name) const
{
    if(zv_currentCalibrationIndex < 0 || zv_currentCalibrationIndex >= zv_caibrationList.count())
    {
        return;
    }

    if(path.isEmpty())
    {
        path = zv_caibrationList.at(zv_currentCalibrationIndex)->zp_path();
    }

    if(name.isEmpty())
    {
        name = zv_caibrationList.at(zv_currentCalibrationIndex)->zp_fileName();
    }

    emit zg_saveCalibration(zv_caibrationList.value(zv_currentCalibrationIndex), path, name);
}
//======================================================
void ZCalibrationRepository::zh_onCalibrationSaving(const ZCalibration* calibration, QString absFilePath)
{
    for(int i = 0; i < zv_caibrationList.count(); i++)
    {
        if(zv_caibrationList.at(i) == calibration)
        {
            zv_caibrationList.at(i)->zp_setPath(absFilePath);
            zv_caibrationList.at(i)->zp_setDirty(false);
            emit zg_calibrationOperation(COT_CALIBRATION_NAME_CHANGED, i, i);
            return;
        }
    }
}
//======================================================
void ZCalibrationRepository::zh_onSetCalibrationVisibleAction()
{
    for (int c= 0; c < zv_caibrationList.count(); c++)
    {
        zp_setCalibrationVisible(c, true);
    }
}
//======================================================
void ZCalibrationRepository::zh_onSetCalibrationInvisibleAction()
{
    for (int c= 0; c < zv_caibrationList.count(); c++)
    {
        zp_setCalibrationVisible(c, false);
    }
}
//======================================================
void ZCalibrationRepository::zh_onInvertCalibrationVisibilityAction()
{
    for (int c= 0; c < zv_caibrationList.count(); c++)
    {
        zp_setCalibrationVisible(c, !zp_isCalibrationVisible(c));
    }
}
//======================================================
void ZCalibrationRepository::zh_onSetWindowsVisibleAction()
{
    if(zv_currentCalibrationIndex < 0 || zv_currentCalibrationIndex >= zv_caibrationList.count())
    {
        return;
    }

    ZCalibration* calibration = zv_caibrationList.at(zv_currentCalibrationIndex);
    if(!calibration)
    {
        return;
    }

    for(int w = 0; w < calibration->zp_calibrationWindowCount(); w++)
    {
        calibration->zp_setCalibrationWindowVisible(w, true);
    }
}
//======================================================
void ZCalibrationRepository::zh_onSetWindowsInvisibleAction()
{
    if(zv_currentCalibrationIndex < 0 || zv_currentCalibrationIndex >= zv_caibrationList.count())
    {
        return;
    }

    ZCalibration* calibration = zv_caibrationList.at(zv_currentCalibrationIndex);
    if(!calibration)
    {
        return;
    }

    for(int w = 0; w < calibration->zp_calibrationWindowCount(); w++)
    {
        calibration->zp_setCalibrationWindowVisible(w, false);
    }
}
//======================================================
void ZCalibrationRepository::zh_onInvertWindowsVisibilityAction()
{
    if(zv_currentCalibrationIndex < 0 || zv_currentCalibrationIndex >= zv_caibrationList.count())
    {
        return;
    }

    ZCalibration* calibration = zv_caibrationList.at(zv_currentCalibrationIndex);
    if(!calibration)
    {
        return;
    }

    for(int w = 0; w < calibration->zp_calibrationWindowCount(); w++)
    {
        calibration->zp_setCalibrationWindowVisible(w, !calibration->zp_isCalibrationWindowVisible(w));
    }
}
//======================================================
void ZCalibrationRepository::zh_onCopySelectedCalibrationAction()
{
    // Get selected calibration index list
    QList<int> selectedCalibrationList;
    emit zg_requestSelectedCalibrationIndexList(selectedCalibrationList);

    if(selectedCalibrationList.isEmpty())
    {
        return;
    }

    qSort(selectedCalibrationList);

    for(int c = 0; c < selectedCalibrationList.count(); c++)
    {
        if(!zh_copyCalibration(selectedCalibrationList.at(c)))
        {
            // TODO error msg
        }
    }
}
//======================================================
bool ZCalibrationRepository::zh_copyCalibration(int calibrationIndex)
{
    if(calibrationIndex < 0 || calibrationIndex >= zv_caibrationList.count())
    {
        return false;
    }

    QString numberSeparator = "-";
    QRegExp digitrx("^\\d{1,}$");

    // Define New Name
    // name w/o number
    QString newName = zv_caibrationList.at(calibrationIndex)->zp_baseName();
    QStringList nameParts =  newName.split(numberSeparator);
    QString digitalPart;
    if(nameParts.isEmpty())
    {
        return false;
    }

    if(nameParts.count() > 1)
    {
        digitalPart = nameParts.last();
        if(digitrx.exactMatch(digitalPart.simplified()))
        {
            // remove digital part
            newName.chop(digitalPart.count());
            // remove whitespaces and minuses
            for(int i = newName.count() - 1; i > 0; i--)
            {
                if(newName.at(i) == ' ' || newName.at(i) == '-')
                {
                    newName.chop(1);
                    continue;
                }
                break;
            }
        }
    }

    // define aux number of calibration
    int maxCalibrationNumber = 1;
    QString name;
    bool ok;
    int number;
    for(int i = 0; i < zv_caibrationList.count(); i++)
    {
        name = zv_caibrationList.at(i)->zp_baseName();
        if(name.startsWith(newName))
        {
            nameParts = name.split(numberSeparator);
            if(nameParts.count() < 2)
            {
                continue;
            }

            digitalPart = nameParts.last();
            if(!digitrx.exactMatch(digitalPart.simplified()))
            {
                continue;
            }

            // get name w/o digital part
            name.chop(digitalPart.count());
            // remove whitespaces and minuses
            for(int i = name.count() - 1; i > 0; i--)
            {
                if(name.at(i) == ' ' || name.at(i) == '-')
                {
                    name.chop(1);
                    continue;
                }
                break;
            }

            // clone detected
            if(name == newName)
            {
                number = digitalPart.toInt(&ok);
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
    }

    // create new name
    newName = newName + numberSeparator +  QString::number(maxCalibrationNumber);

    // create a new clone calibration and put it in array
    // windows and terms are created in calibration
    ZCalibration* newCalibration = new ZCalibration(zv_caibrationList.at(calibrationIndex), newName, this);
    bool res = zh_appendCalibrationToList(newCalibration);
    if(!res)
    {
        delete newCalibration;
        return false;
    }

    newCalibration->zp_setVisible(true);
    res = zh_copyWindowsTermsAndEquation(calibrationIndex, newCalibration);
    if(!res)
    {
        // TODO Error report
    }

    return res;
}
//======================================================
bool ZCalibrationRepository::zh_copyWindowsTermsAndEquation(int srcCalibrationIndex, ZCalibration* trgCalibration)
{
    if(srcCalibrationIndex < 0 || srcCalibrationIndex >= zv_caibrationList.count())
    {
        return false;
    }

    const ZCalibration* srcCalibration = zv_caibrationList.at(srcCalibrationIndex);
    int firstChannel;
    int lastChannel;
    ZCalibrationWindow::WindowType windowType;

    // copy windows
    for(int w = 0; w < srcCalibration->zp_calibrationWindowCount(); w++)
    {
        // get window parameters
        firstChannel = srcCalibration->zp_calibrationWindowFirstChannel(w);
        lastChannel = srcCalibration->zp_calibrationWindowLastChannel(w);
        windowType = srcCalibration->zp_calibrationWindowType(w);

        // create new window in target calibration
        trgCalibration->zp_createNewCalibrationWindow(firstChannel, lastChannel, windowType);
    }

    // copy terms
    QList<int> windowIndexList;
    ZAbstractTerm::TermType termType;

    int trgTermIndex;
    for(int t = 0; t < srcCalibration->zp_termCount(); t++)
    {
        trgTermIndex = zh_findCorrespondingTermIndex(t, srcCalibration, trgCalibration);
        if(trgTermIndex < 0 || trgTermIndex >= trgCalibration->zp_termCount())
        {
            // get scrTerm data
            windowIndexList = srcCalibration->zp_termWindowIndexList(t);
            termType = srcCalibration->zp_termType(t);

            // create new term
            trgCalibration->zp_createTerm(windowIndexList,
                                          termType, ZAbstractTerm::TS_CONST_EXCLUDED, "0.0");
        }
    }

    // normalizer
    trgCalibration->zp_setNormaType(srcCalibration->zp_normaType());
    trgCalibration->zp_setNormaCustomString(srcCalibration->zp_normaCustomString());

    // Equation
    ZEquationSettingsData equationSettingsData(srcCalibration);
    // change base term id for target calibration
    equationSettingsData.baseTermId = trgCalibration->zp_termId(srcCalibration->zp_baseTermIndex());
    int trgCalibrationIndex = -1;
    for(int c = 0; c < zv_caibrationList.count(); c++)
    {
        if(zv_caibrationList.at(c) == trgCalibration)
        {
            trgCalibrationIndex = c;
            break;
        }
    }

    zp_setEquationDataFromSettings(trgCalibrationIndex, equationSettingsData);

    // copy intercept
    trgCalibration->zp_setEquationIntercept(srcCalibration->zp_equationIntercept());

    // copy term factors
    QString termFactorString;
    ZAbstractTerm::TermState termState;
    for(int t = 0; t < srcCalibration->zp_termCount(); t++)
    {
        trgTermIndex = zh_findCorrespondingTermIndex(t, srcCalibration, trgCalibration);

        // set term parameters
        // state
        termState = srcCalibration->zp_termState(t);
        trgCalibration->zp_setTermState(trgTermIndex, termState);

        // term factor
        if(!srcCalibration->zp_termFactorString(t, termFactorString))
        {
            termFactorString = "0.0";
        }

        trgCalibration->zp_setTermFactorString(trgTermIndex, termFactorString);
    }

    return true;
}
//======================================================
int ZCalibrationRepository::zh_findCorrespondingTermIndex(int srcTermIndex,
                                                          const ZCalibration *srcCalibration, const ZCalibration *trgCalibration)
{
    if(srcCalibration == 0 || trgCalibration == 0 || srcTermIndex < 0 || srcTermIndex >= srcCalibration->zp_termCount())
    {
        return -1;
    }

    QString termName = srcCalibration->zp_termName(srcTermIndex);
    ZAbstractTerm::TermType termType = srcCalibration->zp_termType(srcTermIndex);
    QList<int> indexList = srcCalibration->zp_termWindowIndexList(srcTermIndex);

    for(int t = 0; t < trgCalibration->zp_termCount(); t++)
    {
        if(trgCalibration->zp_termName(t) == termName
                && trgCalibration->zp_termType(t) == termType
                && trgCalibration->zp_termWindowIndexList(t) == indexList)
        {
            return t;
        }
    }

    return -1;
}
//======================================================
void ZCalibrationRepository::zh_onCopySelectedWindowsAction()
{

}
//======================================================
void ZCalibrationRepository::zh_onPasteWindowsAction()
{

}
//======================================================
void ZCalibrationRepository::zh_onResetTermStateAction()
{
    if(zv_currentCalibrationIndex < 0 || zv_currentCalibrationIndex >= zv_caibrationList.count())
    {
        return;
    }

    ZCalibration* calibration = zv_caibrationList.at(zv_currentCalibrationIndex);
    if(!calibration)
    {
        return;
    }

    for(int t = 0; t < calibration->zp_termCount(); t++)
    {
        calibration->zp_setTermState(t, ZAbstractTerm::TS_CONST_EXCLUDED);
        calibration->zp_setTermFactorString(t, "0.0");
    }

    calibration->zp_setEquationIntercept(0.0);
    emit zg_invokeCalibrationRecalc();
}
//======================================================
void ZCalibrationRepository::zh_removeCalibration(int index)
{
    if(index < 0 || index >= zv_caibrationList.count())
    {
        return;
    }

    zv_caibrationList.at(index)->zp_removeCalibrationWindows();

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

    // open file
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

    ZXMLCalibrationIOHandler* ioHandler = new ZXMLCalibrationIOHandler(this, this);

    ZCalibration* calibration = new ZCalibration(fileName, this);
    calibration->zp_setVisible(true);
    bool res = zh_appendCalibrationToList(calibration);
    if(res)
    {
        res = ioHandler->zp_getCalibrationFromFile(file, calibration);

    }

    if(!res)
    {
        for(int c = 0; c < zv_caibrationList.count(); c++)
        {
            if(zv_caibrationList.at(c) == calibration)
            {
                zh_removeCalibration(c);
            }
        }
    }

    if(file.isOpen())
    {
        file.close();
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
    connect(calibration, &ZCalibration::zg_normalizerChanged,
            this, &ZCalibrationRepository::zh_onNormalizerChange);
    connect(calibration, &ZCalibration::zg_interceptChanged,
            this, &ZCalibrationRepository::zh_onCalibrationFreeTermChange);

    connect(calibration, &ZCalibration::zg_dirtyChanged,
            this, &ZCalibrationRepository::zh_onCalibrationDirtyChange);

    int insertIndex = zv_caibrationList.count();
    emit zg_calibrationOperation(COT_INSERT_CALIBRATIONS, insertIndex, insertIndex);
    zv_caibrationList << calibration;
    emit zg_calibrationOperation(COT_END_INSERT_CALIBRATIONS, -1, -1);
    return true;
}
//======================================================
void ZCalibrationRepository::zh_actionEnablingControl()
{
    bool calibrationDisabilityFlag = zv_currentCalibrationIndex < 0 || zv_currentCalibrationIndex >= zv_caibrationList.count();

    zv_removeCalibrationAction->setDisabled(calibrationDisabilityFlag);
    zv_recalcEquationFactorsAction->setDisabled(calibrationDisabilityFlag);

    zv_setCalibrationVisibleAction->setDisabled(calibrationDisabilityFlag);
    zv_setCalibrationInvisibleAction->setDisabled(calibrationDisabilityFlag);
    zv_invertCalibrationVisibilityAction->setDisabled(calibrationDisabilityFlag);

    QList<int> selectedCalibrationList;
    emit zg_requestSelectedCalibrationIndexList(selectedCalibrationList);
    zv_copySelectedCalibrationAction->setDisabled(selectedCalibrationList.isEmpty() || calibrationDisabilityFlag);
}
//======================================================
void ZCalibrationRepository::zh_createActions()
{
    // calibration actions
    zv_newCalibrationAction = new QAction(this);
    zv_newCalibrationAction->setIcon(QIcon(glAddCalibrationIconString));
    zv_newCalibrationAction->setText(tr("Append a new calibration"));
    zv_newCalibrationAction->setToolTip(tr("Append a new calibration to the list"));

    zv_openCalibrationsAction = new QAction(this);
    zv_openCalibrationsAction->setIcon(QIcon(glOpenIconString));
    zv_openCalibrationsAction->setText(tr("Open calibrations"));
    zv_openCalibrationsAction->setToolTip(tr("Append a new calibration to the list"));

    zv_removeCalibrationAction = new QAction(this);
    zv_removeCalibrationAction->setIcon(QIcon(glRemoveIconString));
    zv_removeCalibrationAction->setText(tr("Remove selected calibrations"));
    zv_removeCalibrationAction->setToolTip(tr("Remove selected calibrations from the list"));

    // window actions
    zv_newWindowAction = new QAction(this);
    zv_newWindowAction->setIcon(QIcon(glAddWindowIconString));
    zv_newWindowAction->setText(tr("Append a new calibration window"));
    zv_newWindowAction->setToolTip(tr("Append a new calibration window to the list"));

    zv_removeWindowAction = new QAction(this);
    zv_removeWindowAction->setIcon(QIcon(glRemoveWindowIconString));
    zv_removeWindowAction->setText(tr("Remove selected calibration windows"));
    zv_removeWindowAction->setToolTip(tr("Remove selected calibration windows from the list"));

    // action actions
    zv_recalcEquationFactorsAction = new QAction(this);
    zv_recalcEquationFactorsAction->setIcon(QIcon(glCalcIconString));
    zv_recalcEquationFactorsAction->setText(tr("Recalculate calibration"));
    zv_recalcEquationFactorsAction->setToolTip(tr("Recalculate equation factors for current calibration"));

    // terms actions
    zv_createMixedTermsAction = new QAction(this);
    zv_createMixedTermsAction->setIcon(QIcon(glAddTermIconString));
    zv_createMixedTermsAction->setText(tr("Create mixed terms"));
    zv_createMixedTermsAction->setToolTip(tr("Create mixed terms"));

    zv_removeMixedTermsAction = new QAction(this);
    zv_removeMixedTermsAction->setIcon(QIcon(glRemoveIconString));
    zv_removeMixedTermsAction->setText(tr("Remove mixed terms"));
    zv_removeMixedTermsAction->setToolTip(tr("Remove mixed terms from the list"));

    zv_createCustomTermAction = new QAction(this);
    zv_createCustomTermAction->setIcon(QIcon(glAddTermIconString));
    zv_createCustomTermAction->setText(tr("Create custom term"));
    zv_createCustomTermAction->setToolTip(tr("Create custom term"));

    zv_removeCustomTermAction = new QAction(this);
    zv_removeCustomTermAction->setIcon(QIcon(glRemoveIconString));
    zv_removeCustomTermAction->setText(tr("Remove selected custom term"));
    zv_removeCustomTermAction->setToolTip(tr("Remove selected custom term from the list"));

    // new actions
    zv_setCalibrationVisibleAction = new QAction(this);
    zv_setCalibrationVisibleAction->setIcon(QIcon(glRemoveIconString));
    zv_setCalibrationVisibleAction->setText(tr("Set calibration visible"));
    zv_setCalibrationVisibleAction->setToolTip(tr("Set visible calibration column"));

    zv_setCalibrationInvisibleAction = new QAction(this);
    zv_setCalibrationInvisibleAction->setIcon(QIcon(glRemoveIconString));
    zv_setCalibrationInvisibleAction->setText(tr("Set calibration invisible"));
    zv_setCalibrationInvisibleAction->setToolTip(tr("Set invisible calibration column"));

    zv_invertCalibrationVisibilityAction = new QAction(this);
    zv_invertCalibrationVisibilityAction->setIcon(QIcon(glRemoveIconString));
    zv_invertCalibrationVisibilityAction->setText(tr("Invert calibration visibility"));
    zv_invertCalibrationVisibilityAction->setToolTip(tr("Invert calibration visibility"));


    zv_setWindowsVisibleAction = new QAction(this);
    zv_setWindowsVisibleAction->setIcon(QIcon(glRemoveIconString));
    zv_setWindowsVisibleAction->setText(tr("Set calibration window visible"));
    zv_setWindowsVisibleAction->setToolTip(tr("Set calibration window visible"));

    zv_setWindowsInvisibleAction = new QAction(this);
    zv_setWindowsInvisibleAction->setIcon(QIcon(glRemoveIconString));
    zv_setWindowsInvisibleAction->setText(tr("Set calibration window invisible"));
    zv_setWindowsInvisibleAction->setToolTip(tr("Set calibration window invisible"));

    zv_invertWindowsVisibilityAction = new QAction(this);
    zv_invertWindowsVisibilityAction->setIcon(QIcon(glRemoveIconString));
    zv_invertWindowsVisibilityAction->setText(tr("Invert calibration window visibility"));
    zv_invertWindowsVisibilityAction->setToolTip(tr("Invert calibration window visibility"));


    zv_copySelectedCalibrationAction = new QAction(this);
    zv_copySelectedCalibrationAction->setIcon(QIcon(glRemoveIconString));
    zv_copySelectedCalibrationAction->setText(tr("Copy selected calibrations"));
    zv_copySelectedCalibrationAction->setToolTip(tr("Copy selected calibrations"));

    zv_copySelectedWindowsAction = new QAction(this);
    zv_copySelectedWindowsAction->setIcon(QIcon(glRemoveIconString));
    zv_copySelectedWindowsAction->setText(tr("Copy selected windows"));
    zv_copySelectedWindowsAction->setToolTip(tr("Copy selected windows"));

    zv_pasteWindowsAction = new QAction(this);
    zv_pasteWindowsAction->setIcon(QIcon(glRemoveIconString));
    zv_pasteWindowsAction->setText(tr("Paste windows"));
    zv_pasteWindowsAction->setToolTip(tr("Paste windows"));

    zv_resetTermStateAction = new QAction(this);
    zv_resetTermStateAction->setIcon(QIcon(glRemoveIconString));
    zv_resetTermStateAction->setText(tr("Reset terms"));
    zv_resetTermStateAction->setToolTip(tr("Reset equation terms"));

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
    connect(zv_recalcEquationFactorsAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onRecalcEquationFactorsAction);

    connect(zv_createMixedTermsAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onCreateMixedTermsAction);
    connect(zv_removeMixedTermsAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onRemoveMixedTermsAction);
    connect(zv_createCustomTermAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onCreateCustomTermAction);
    connect(zv_removeCustomTermAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onRemoveCustomTermAction);

    connect(zv_setCalibrationVisibleAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onSetCalibrationVisibleAction);
    connect(zv_setCalibrationInvisibleAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onSetCalibrationInvisibleAction);
    connect(zv_invertCalibrationVisibilityAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onInvertCalibrationVisibilityAction);

    connect(zv_setWindowsVisibleAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onSetWindowsVisibleAction);
    connect(zv_setWindowsInvisibleAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onSetWindowsInvisibleAction);
    connect(zv_invertWindowsVisibilityAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onInvertWindowsVisibilityAction);

    connect(zv_copySelectedCalibrationAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onCopySelectedCalibrationAction);
    connect(zv_copySelectedWindowsAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onCopySelectedWindowsAction);
    connect(zv_pasteWindowsAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onPasteWindowsAction);

    connect(zv_resetTermStateAction, &QAction::triggered,
            this, &ZCalibrationRepository::zh_onResetTermStateAction);

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
int ZCalibrationRepository::zh_calibrationIndexForId(qint64 calibrationId) const
{
    if(calibrationId < 0)
    {
        return -1;
    }

    for(int c = 0; c < zv_caibrationList.count(); c++)
    {
        if(zv_caibrationList.at(c)->zp_calibrationId() != calibrationId)
        {
            continue;
        }

        return c;
    }

    return -1;
}
//======================================================

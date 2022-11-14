//======================================================
#include "X_CalibrationRepository.h"
#include "X_Calibration.h"
#include "X_CustomTermDialog.h"
#include "X_FileActionManager.h"
#include "X_General.h"
#include "X_SetGainFactorToCalibrationDialog.h"
#include "X_XMLCalibrationIOHandler.h"

#include <algorithm>
#include <QClipboard>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QMimeData>
#include <QModelIndex>
#include <QRegularExpression>
#include <QtAlgorithms>
//======================================================
X_CalibrationRepository::X_CalibrationRepository(QObject* parent)
    : QObject(parent)
{
    xv_spectrumArrayRepository = nullptr;
    xv_newCalibrationAction = nullptr;
    xv_openCalibrationsAction = nullptr;
    xv_saveCalibrationsAction = nullptr;
    xv_saveAsCalibrationsAction = nullptr;

    xv_removeCalibrationAction = nullptr;

    xv_newWindowAction = nullptr;
    xv_removeWindowAction = nullptr;

    xv_recalcEquationFactorsAction = nullptr;

    xv_defaultCalibrationName = tr("Calibration #");
    xv_defaultCalibrationWindowType = X_CalibrationWindow::WT_PEAK;
    xv_currentArrayId = -1;
    xv_currentArrayIndex = 0;
    xv_currentCalibrationIndex = -1;
    xv_currentTermIndex = -1;
    xv_currentWindowIndex = -1;
    xh_createActions();
    xh_createConnections();
    xh_actionEnablingControl();
    xv_blockCalibrationRecalc = false;
}
//==================================================================
void X_CalibrationRepository::xp_appendActionsToMenu(QMenu* menu) const
{
    if (menu->objectName() == "Edit")
    {
        //        QMenu* calibrationMenu = new QMenu(tr("Calibrations"));
        //        calibrationMenu->setIcon(QIcon(glCalibrationIconString));
        //        calibrationMenu->addAction(xv_newCalibrationAction);
        //        calibrationMenu->addAction(xv_removeCalibrationAction);
        //        menu->addMenu(calibrationMenu);

        //        QMenu* windowsMenu = new QMenu(tr("Calibration windows"));
        //        windowsMenu->setIcon(QIcon(glWindowIconString));
        //        windowsMenu->addAction(xv_newWindowAction);
        //        windowsMenu->addAction(xv_removeWindowAction);
        //        menu->addMenu(windowsMenu);

        //        QMenu* termMenu = new QMenu(tr("Equation terms"));
        //        termMenu->setIcon(QIcon());
        //        termMenu->addAction(xv_createMixedTermsAction);
        //        termMenu->addAction(xv_removeMixedTermsAction);
        //        termMenu->addSeparator();
        //        termMenu->addAction(xv_createCustomTermAction);
        //        termMenu->addAction(xv_removeCustomTermAction);

        //        menu->addMenu(termMenu);

        menu->addAction(xv_newCalibrationAction);
        menu->addAction(xv_removeCalibrationAction);
        menu->addSeparator();
        menu->addAction(xv_newWindowAction);
        menu->addAction(xv_removeWindowAction);
        menu->addSeparator();
        menu->addAction(xv_resetTermStateAction);
        menu->addAction(xv_createMixedTermsAction);
        menu->addAction(xv_removeMixedTermsAction);
        menu->addAction(xv_createCustomTermAction);
        menu->addAction(xv_removeCustomTermAction);
    }
    else if (menu->objectName() == "Actions")
    {
        menu->addAction(xv_recalcEquationFactorsAction);
        menu->addSeparator();
    }
}
//==================================================================
void X_CalibrationRepository::xp_connectToFileActionManager(
    X_FileActionManager* manager)
{
    manager->xp_initOpenCalibrationFromFileAction(xv_openCalibrationsAction);
    manager->xp_initSaveCalibrationFromFileAction(xv_saveCalibrationsAction);
    manager->xp_initSaveAsCalibrationFromFileAction(xv_saveAsCalibrationsAction);

    connect(manager,
            &X_FileActionManager::xg_calibrationFileListToOpen,
            this,
            &X_CalibrationRepository::xh_appendCalibrationsToArray);
    connect(manager,
            &X_FileActionManager::xg_requestCalibrationDataAndInitSaving,
            this,
            &X_CalibrationRepository::xh_createCalibrationAndStartSaving);

    connect(this,
            &X_CalibrationRepository::xg_currentCalibrationDirtyChanged,
            manager,
            &X_FileActionManager::xp_onCurrentCalibrationDirtyChange);
    connect(this,
            &X_CalibrationRepository::xg_saveCalibration,
            manager,
            &X_FileActionManager::xp_initSavingCalibrationToFile);

    connect(manager,
            &X_FileActionManager::xg_calibrationSaved,
            this,
            &X_CalibrationRepository::xh_onCalibrationSaving);
}
//==================================================================
QList<QAction*> X_CalibrationRepository::xp_calibrationActions() const
{
    QList<QAction*> actionList;
    actionList << xv_newCalibrationAction;
    // actionList << xv_openCalibrationsAction;
    actionList << xv_removeCalibrationAction;
    return actionList;
}
//==================================================================
QList<QAction*> X_CalibrationRepository::xp_calibrationContextMenuActions() const
{
    QList<QAction*> actionList;
    actionList << xv_newCalibrationAction;
    actionList << xv_openCalibrationsAction;
    actionList << xv_removeCalibrationAction;
    actionList << nullptr;
    actionList << xv_saveCalibrationsAction;
    actionList << xv_saveAsCalibrationsAction;
    actionList << nullptr;
    actionList << xv_copySelectedCalibrationAction;
    actionList << nullptr;
    actionList << xv_setCalibrationVisibleAction;
    actionList << xv_setCalibrationInvisibleAction;
    actionList << xv_invertCalibrationVisibilityAction;

    return actionList;
}
//==================================================================
QList<QAction*> X_CalibrationRepository::xp_calibrationVisibilityActions() const
{
    QList<QAction*> actionList;
    actionList << xv_setCalibrationVisibleAction;
    actionList << xv_setCalibrationInvisibleAction;
    actionList << xv_invertCalibrationVisibilityAction;
    return actionList;
}
//==================================================================
QList<QAction*> X_CalibrationRepository::xp_windowActions() const
{
    QList<QAction*> actionList;
    actionList << xv_newWindowAction;
    actionList << xv_removeWindowAction;
    return actionList;
}
//==================================================================
QList<QAction*> X_CalibrationRepository::xp_windowContextMenuActions() const
{
    QList<QAction*> actionList;
    actionList << xv_newWindowAction;
    actionList << xv_removeWindowAction;
    //    actionList << 0;
    //    actionList << xv_copySelectedWindowsAction;
    //    actionList << xv_pasteWindowsAction;
    actionList << nullptr;
    actionList << xv_setWindowsVisibleAction;
    actionList << xv_setWindowsInvisibleAction;
    actionList << xv_invertWindowsVisibilityAction;

    return actionList;
}
//==================================================================
QList<QAction*> X_CalibrationRepository::xp_termActions() const
{
    QList<QAction*> actionList;
    actionList << xv_createMixedTermsAction;
    actionList << xv_removeMixedTermsAction;
    //    actionList << 0;
    //    actionList << xv_createCustomTermAction;
    //    actionList << xv_removeCustomTermAction;
    return actionList;
}
//==================================================================
QList<QAction*> X_CalibrationRepository::xp_termContextMenuActions() const
{
    QList<QAction*> actionList;
    actionList << xv_resetTermStateAction;
    actionList << nullptr;
    actionList << xv_createMixedTermsAction;
    actionList << xv_removeMixedTermsAction;
    actionList << nullptr;
    actionList << xv_createCustomTermAction;
    actionList << xv_removeCustomTermAction;
    return actionList;
}
//==================================================================
bool X_CalibrationRepository::xp_isEmpty() const
{
    return xv_caibrationList.isEmpty();
}
//==================================================================
void X_CalibrationRepository::xp_clear()
{
    if (!xv_caibrationList.isEmpty())
    {
        emit xg_calibrationOperation(COT_REMOVE_CALIBRATIONS,
                                     0,
                                     xv_caibrationList.count() - 1);
        for (int i = xv_caibrationList.count() - 1; i >= 0; i--)
        {
            xv_caibrationList.takeAt(i)->deleteLater();
        }
        emit xg_calibrationOperation(COT_END_REMOVE_CALIBRATIONS, 0, 0);
    }
}
//======================================================
int X_CalibrationRepository::xp_calibrationCount() const
{
    return xv_caibrationList.count();
}
//======================================================
int X_CalibrationRepository::xp_visibleCalibrationCount() const
{
    int visible = 0;
    for (int i = 0; i < xv_caibrationList.count(); i++)
    {
        if (xv_caibrationList.at(i)->xp_isVisible())
        {
            visible++;
        }
    }

    return visible;
}
//======================================================
QString X_CalibrationRepository::xp_calibrationName(int calibrationIndex) const
{
    if (calibrationIndex < 0 || calibrationIndex >= xv_caibrationList.count())
    {
        return QString();
    }

    return xv_caibrationList.value(calibrationIndex)->xp_baseName();
}
//======================================================
qint64 X_CalibrationRepository::xp_calibrationIdForCalibrationIndex(
    int calibrationIndex) const
{
    if (calibrationIndex < 0 || calibrationIndex >= xv_caibrationList.count())
    {
        return -1;
    }

    return xv_caibrationList.at(calibrationIndex)->xp_calibrationId();
}
//======================================================
bool X_CalibrationRepository::xp_setCalibrationName(int row, const QString& name)
{
    if (row < 0 || row >= xv_caibrationList.count())
    {
        return false;
    }

    if (name.isEmpty() || name == xv_caibrationList.value(row)->xp_baseName())
    {
        return false;
    }

    xv_caibrationList[row]->xp_setBaseName(name);
    emit xg_calibrationOperation(COT_CALIBRATION_NAME_CHANGED, row, row);
    return true;
}
//======================================================
QString X_CalibrationRepository::xp_visibleCalibrationName(
    int visibleCalibrationIndex) const
{
    if (visibleCalibrationIndex < 0
        || visibleCalibrationIndex >= xp_visibleCalibrationCount())
    {
        return QString();
    }

    int visible = -1;
    for (int i = 0; i < xv_caibrationList.count(); i++)
    {
        if (!xv_caibrationList.at(i)->xp_isVisible())
        {
            continue;
        }

        visible++;
        if (visible == visibleCalibrationIndex)
        {
            return xv_caibrationList.at(i)->xp_baseName();
        }
    }

    return QString();
}
//======================================================
qint64 X_CalibrationRepository::xp_visibleCalibrationId(
    int visibleCalibrationIndex) const
{
    if (visibleCalibrationIndex < 0
        || visibleCalibrationIndex >= xp_visibleCalibrationCount())
    {
        return -1;
    }

    int visible = -1;
    for (int i = 0; i < xv_caibrationList.count(); i++)
    {
        if (!xv_caibrationList.at(i)->xp_isVisible())
        {
            continue;
        }

        visible++;
        if (visible == visibleCalibrationIndex)
        {
            return xv_caibrationList.at(i)->xp_calibrationId();
        }
    }

    return -1;
}
//======================================================
int X_CalibrationRepository::xp_calibrationIndexForVisibleIndex(
    int visibleCalibrationIndex) const
{
    if (visibleCalibrationIndex < 0
        || visibleCalibrationIndex >= xp_visibleCalibrationCount())
    {
        return -1;
    }

    int visible = -1;
    for (int i = 0; i < xv_caibrationList.count(); i++)
    {
        if (!xv_caibrationList.at(i)->xp_isVisible())
        {
            continue;
        }

        visible++;
        if (visible == visibleCalibrationIndex)
        {
            return i;
        }
    }

    return -1;
}
//======================================================
QString X_CalibrationRepository::xp_chemElement(int row) const
{
    if (row < 0 || row >= xv_caibrationList.count())
    {
        return QString();
    }

    return xv_caibrationList.at(row)->xp_chemElement();
}
//======================================================
QString X_CalibrationRepository::xp_chemElementForCalibrationId(qint64 id) const
{
    if (id < 0)
    {
        return QString();
    }

    for (int i = 0; i < xv_caibrationList.count(); i++)
    {
        if (xv_caibrationList.at(i)->xp_calibrationId() == id)
        {
            return xv_caibrationList.at(i)->xp_chemElement();
        }
    }

    return QString();
}
//======================================================
bool X_CalibrationRepository::xp_setChemElement(int row,
                                               const QString& chemElement)
{
    if (row < 0 || row >= xv_caibrationList.count())
    {
        return false;
    }

    if (chemElement.isEmpty()
        || chemElement == xv_caibrationList.at(row)->xp_chemElement())
    {
        return false;
    }

    xv_caibrationList[row]->xp_setChemElement(chemElement);
    emit xg_calibrationOperation(COT_CALIBRATION_CHEM_ELEMENT_CHANGED, row, row);
    return true;
}
//======================================================
QString X_CalibrationRepository::xp_baseTermString(int row) const
{
    if (row < 0 || row >= xv_caibrationList.count())
    {
        return QString();
    }

    return xv_caibrationList.at(row)->xp_baseTermDisplayString();
}
//======================================================
bool X_CalibrationRepository::xp_isCalibrationVisible(int row)
{
    if (row < 0 || row >= xv_caibrationList.count())
    {
        return false;
    }

    return xv_caibrationList.value(row)->xp_isVisible();
}
//======================================================
QColor X_CalibrationRepository::xp_calibrationColor(int row)
{
    if (row < 0 || row >= xv_caibrationList.count())
    {
        return QColor();
    }

    return xv_caibrationList.value(row)->xp_color();
}
//======================================================
bool X_CalibrationRepository::xp_setCalibrationVisible(int row, bool visibility)
{
    if (row < 0 || row >= xv_caibrationList.count())
    {
        return false;
    }

    if (visibility == xv_caibrationList.at(row)->xp_isVisible())
    {
        return false;
    }

    emit xg_calibrationOperation(COT_CALIBRATION_VISIBILITY_CHANGE, row, row);
    xv_caibrationList[row]->xp_setVisible(visibility);
    emit xg_calibrationOperation(COT_END_CALIBRATION_VISIBILITY_CHANGE,
                                 row,
                                 row);
    return true;
}
//======================================================
bool X_CalibrationRepository::xp_isDirty(int row)
{
    if (row < 0 || row >= xv_caibrationList.count())
    {
        return false;
    }

    return xv_caibrationList.value(row)->xp_isDirty();
}
//======================================================
void X_CalibrationRepository::xp_hasDirtyCalibrations(
    QList<int>& dirtyRowList) const
{
    dirtyRowList.clear();
    for (int row = 0; row < xv_caibrationList.count(); row++)
    {
        if (xv_caibrationList.at(row)->xp_isDirty())
        {
            dirtyRowList.append(row);
        }
    }
}
//======================================================
bool X_CalibrationRepository::xp_isCalibrationWindowVisible(qint64 calibrationId,
                                                           int windowIndex) const
{
    if (windowIndex < 0 || calibrationId < 0)
    {
        return false;
    }

    for (int c = 0; c < xv_caibrationList.count(); c++)
    {
        if (xv_caibrationList.at(c)->xp_calibrationId() != calibrationId)
        {
            continue;
        }

        return xv_caibrationList.at(c)->xp_isCalibrationWindowVisible(
            windowIndex);
    }

    return false;
}
//======================================================
bool X_CalibrationRepository::xp_isCalibrationWindowVisible(qint64 windowId) const
{
    for (int c = 0; c < xv_caibrationList.count(); c++)
    {
        int windowCount = xv_caibrationList.at(c)->xp_calibrationWindowCount();
        for (int w = 0; w < windowCount; w++)
        {
            if (windowId == xv_caibrationList.at(c)->xp_calibrationWindowId(w))
            {
                return xv_caibrationList.at(c)->xp_isCalibrationWindowVisible(w);
            }
        }
    }
    return false;
}
//======================================================
bool X_CalibrationRepository::xp_setCalibrationWindowVisible(qint64 calibrationId,
                                                            int windowIndex,
                                                            bool visibility)
{
    if (windowIndex < 0 || calibrationId < 0)
    {
        return false;
    }

    for (int c = 0; c < xv_caibrationList.count(); c++)
    {
        if (xv_caibrationList.at(c)->xp_calibrationId() != calibrationId)
        {
            continue;
        }

        return xv_caibrationList.at(c)
            ->xp_setCalibrationWindowVisible(windowIndex, visibility);
    }

    return false;
}
//======================================================
int X_CalibrationRepository::xp_calibrationWindowCount(qint64 calibrationId) const
{
    const X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return 0;
    }

    return calibration->xp_calibrationWindowCount();
}
//======================================================
QString X_CalibrationRepository::xp_calibrationWindowName(qint64 calibrationId,
                                                         int windowIndex) const
{
    const X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return QString();
    }

    return calibration->xp_calibrationWindowName(windowIndex);
}
//======================================================
bool X_CalibrationRepository::xp_setCalibrationWindowName(qint64 calibrationId,
                                                         int windowIndex,
                                                         const QString& name)
{
    X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return false;
    }

    return calibration->xp_setCalibrationWindowName(windowIndex, name);
}
//======================================================
X_CalibrationWindow::WindowType X_CalibrationRepository::xp_calibrationWindowType(
    qint64 calibrationId, int windowIndex) const
{
    const X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return X_CalibrationWindow::WT_NOT_DEFINED;
    }

    return calibration->xp_calibrationWindowType(windowIndex);
}
//======================================================
bool X_CalibrationRepository::xp_setCalibrationWindowType(
    qint64 calibrationId, int windowIndex, X_CalibrationWindow::WindowType type)
{
    X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return false;
    }

    return calibration->xp_setCalibrationWindowType(windowIndex, type);
}
//======================================================
int X_CalibrationRepository::xp_calibrationWindowFirstChannel(
    qint64 calibrationId, int windowIndex) const
{
    const X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return 0;
    }

    return calibration->xp_calibrationWindowFirstChannel(windowIndex);
}
//======================================================
int X_CalibrationRepository::xp_calibrationWindowLastChannel(
    qint64 calibrationId, int windowIndex) const
{
    const X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return 0;
    }

    return calibration->xp_calibrationWindowLastChannel(windowIndex);
}
//======================================================
bool X_CalibrationRepository::xp_setCalibrationWindowFirstChannel(
    qint64 calibrationId, int windowIndex, int channel)
{
    X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return false;
    }

    return calibration->xp_setCalibrationWindowFirstChannel(windowIndex,
                                                            channel);
}
//======================================================
bool X_CalibrationRepository::xp_setCalibrationWindowLastChannel(
    qint64 calibrationId, int windowIndex, int channel)
{
    X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return false;
    }

    return calibration->xp_setCalibrationWindowLastChannel(windowIndex, channel);
}
//======================================================
QColor X_CalibrationRepository::xp_calibrationWindowColor(qint64 calibrationId,
                                                         int windowIndex) const
{
    const X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return QColor();
    }

    return calibration->xp_color();
}
//======================================================
qint64 X_CalibrationRepository::xp_calibrationWindowId(qint64 calibrationId,
                                                      int windowIndex) const
{
    const X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return -1;
    }

    return calibration->xp_calibrationWindowId(windowIndex);
}
//======================================================
const X_CalibrationWindow* X_CalibrationRepository::xp_calibrationWindow(
    qint64 calibrationId, int windowIndex) const
{
    const X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return nullptr;
    }

    return calibration->xp_calibrationWindow(windowIndex);
}
//======================================================
bool X_CalibrationRepository::xp_calculateConcentration(
    int row, const X_AbstractSpectrum* spectrum, qreal& concentration) const
{
    if (row < 0 || row >= xv_caibrationList.count() || spectrum == nullptr)
    {
        concentration = 0;
        return false;
    }

    return xv_caibrationList.at(row)->xp_calcConcentration(spectrum,
                                                           concentration);
}
//======================================================
bool X_CalibrationRepository::xp_calculateConcentrationForId(
    qint64 calibrationId,
    const X_AbstractSpectrum* spectrum,
    qreal& concentration) const
{
    if (calibrationId < 0)
    {
        return false;
    }

    X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return false;
    }

    return calibration->xp_calcConcentration(spectrum, concentration);
}
//======================================================
bool X_CalibrationRepository::xp_calculateActiveTermValueSumForId(qint64 calibrationId,
                                         const X_AbstractSpectrum* spectrum,
                                         qreal& value) const
{
    if (calibrationId < 0)
    {
        return false;
    }

    X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return false;
    }

    return calibration->xp_calcActiveTermvalueSum(spectrum, value);
}
//======================================================
int X_CalibrationRepository::xp_termCount(qint64 calibrationId) const
{
    const X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return 0;
    }

    return calibration->xp_termCount();
}
//======================================================
QString X_CalibrationRepository::xp_termName(qint64 calibrationId,
                                            int termIndex) const
{
    const X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return QString();
    }
    return calibration->xp_termDisplayName(termIndex);
}
//======================================================
X_AbstractTerm::TermState X_CalibrationRepository::xp_termState(
    qint64 calibrationId, int termIndex)
{
    const X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return X_AbstractTerm::TS_NOT_DEFINED;
    }
    return calibration->xp_termStateForDisplay(termIndex);
}
//======================================================
void X_CalibrationRepository::xp_setNextUsersTermState(qint64 calibrationId,
                                                      int termLogIndex)
{
    X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return;
    }

    calibration->xp_setNextUsersTermState(termLogIndex);
}
//======================================================
X_TermNormalizer::NormaType X_CalibrationRepository::xp_normaTypeForCalibrationId(
    qint64 calibrationId) const
{
    const X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return X_TermNormalizer::NT_NONE;
    }

    return calibration->xp_normaType();
}
//======================================================
bool X_CalibrationRepository::xp_setNormaTypeForCalibrationId(
    qint64 calibrationId, X_TermNormalizer::NormaType type)
{
    X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return false;
    }

    return calibration->xp_setNormaType(type);
}
//======================================================
X_TermNormalizer::NormaType X_CalibrationRepository::xp_normaType(
    int calibrationIndex) const
{
    if (calibrationIndex < 0 || calibrationIndex >= xv_caibrationList.count())
    {
        return X_TermNormalizer::NT_NONE;
    }

    return xv_caibrationList.at(calibrationIndex)->xp_normaType();
}
//======================================================
bool X_CalibrationRepository::xp_setNormaType(int calibrationIndex,
                                             X_TermNormalizer::NormaType type)
{
    if (calibrationIndex < 0 || calibrationIndex >= xv_caibrationList.count())
    {
        return false;
    }

    return xv_caibrationList.at(calibrationIndex)->xp_setNormaType(type);
}
//======================================================
bool X_CalibrationRepository::xp_equationTypeForCalibrationId(
    qint64 calibrationId, X_Calibration::EquationType& type) const
{
    X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return false;
    }

    type = calibration->xp_equationType();
    return true;
}
//======================================================
bool X_CalibrationRepository::xp_setEquationTypeForCalibrationId(
    qint64 calibrationId, X_Calibration::EquationType type)
{
    X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return false;
    }

    return calibration->xp_setEquationType(type);
}
//======================================================
X_Calibration::EquationType X_CalibrationRepository::xp_equationType(
    int calibrationIndex) const
{
    if (calibrationIndex < 0 || calibrationIndex >= xv_caibrationList.count())
    {
        return X_Calibration::ET_NOT_DEFINED;
    }

    return xv_caibrationList.at(calibrationIndex)->xp_equationType();
}
//======================================================
bool X_CalibrationRepository::xp_setEquationType(int calibrationIndex,
                                                X_Calibration::EquationType type)
{
    if (calibrationIndex < 0 || calibrationIndex >= xv_caibrationList.count())
    {
        return false;
    }

    return xv_caibrationList.at(calibrationIndex)->xp_setEquationType(type);
}
//======================================================
X_NormaSettingsData X_CalibrationRepository::xp_normaSettingsData(
    int calibrationIndex) const
{
    if (calibrationIndex < 0 || calibrationIndex >= xv_caibrationList.count())
    {
        return X_NormaSettingsData();
    }

    return X_NormaSettingsData(xv_caibrationList.at(calibrationIndex));
}
//======================================================
bool X_CalibrationRepository::xp_setNormaSettings(int calibrationIndex,
                                                 X_NormaSettingsData settings)
{
    if (calibrationIndex < 0 || calibrationIndex >= xv_caibrationList.count())
    {
        return false;
    }

    X_Calibration* calibration = xv_caibrationList.value(calibrationIndex);
    calibration->xp_setNormaType(settings.normaType);
    calibration->xp_setNormaCustomString(settings.customNormaString);
    return true;
}
//======================================================
X_EquationSettingsData X_CalibrationRepository::xp_equationDataForSettings(
    int calibrationIndex) const
{
    if (calibrationIndex < 0 || calibrationIndex >= xv_caibrationList.count())
    {
        return X_EquationSettingsData();
    }

    return X_EquationSettingsData(xv_caibrationList.at(calibrationIndex));
}
//======================================================
bool X_CalibrationRepository::xp_setEquationDataFromSettings(
    int calibrationIndex, X_EquationSettingsData settings)
{
    if (calibrationIndex < 0 || calibrationIndex >= xv_caibrationList.count())
    {
        return false;
    }

    X_Calibration* calibration = xv_caibrationList.value(calibrationIndex);
    if (calibration->xp_setEquationType(settings.equationType))
    {
        emit xg_calibrationOperation(COT_CALIBRATION_EQUATION_TYPE_CHANGED,
                                     calibrationIndex,
                                     calibrationIndex);
    }

    bool res = calibration->xp_setBaseTermId(settings.baseTermId);
    res = calibration->xp_setBaseTermNormalizerParameters(
              settings.fractionalBaseNormaSettingsData.normaType,
              settings.fractionalBaseNormaSettingsData.customNormaString)
          || res;
    if (res)
    {
        emit xg_calibrationOperation(COT_CALIBRATION_EQUATION_BASE_TERM_CHANGED,
                                     calibrationIndex,
                                     calibrationIndex);
        if (!xv_blockCalibrationRecalc)
        {
            emit xg_invokeCalibrationRecalc();
        }
    }

    return true;
}
//======================================================
qreal X_CalibrationRepository::xp_equationIntercept(int calibrationIndex) const
{
    if (calibrationIndex < 0 || calibrationIndex >= xv_caibrationList.count())
    {
        return 0.0;
    }

    return xv_caibrationList.at(calibrationIndex)->xp_equationIntercept();
}
//======================================================
bool X_CalibrationRepository::xp_setEquationIntercept(int calibrationIndex,
                                                     qreal value)
{
    if (calibrationIndex < 0 || calibrationIndex >= xv_caibrationList.count())
    {
        return false;
    }

    bool res = xv_caibrationList.at(calibrationIndex)
                   ->xp_setEquationIntercept(value);
    return res;
}
//======================================================
QString X_CalibrationRepository::xp_equationInterceptString(
    int calibrationIndex) const
{
    if (calibrationIndex < 0 || calibrationIndex >= xv_caibrationList.count())
    {
        return "0.0";
    }

    return xv_caibrationList.at(calibrationIndex)->xp_equationInterceptString();
}
//======================================================
bool X_CalibrationRepository::xp_setEquationInterceptString(
    int calibrationIndex, const QString& interceptString)
{
    if (calibrationIndex < 0 || calibrationIndex >= xv_caibrationList.count())
    {
        return false;
    }

    bool res = xv_caibrationList.at(calibrationIndex)
                   ->xp_setEquationInterceptString(interceptString);
    return res;
}
//======================================================
int X_CalibrationRepository::xp_baseTermIndex(int calibrationIndex)
{
    if (calibrationIndex < 0 || calibrationIndex >= xv_caibrationList.count())
    {
        return -1;
    }

    return xv_caibrationList.at(calibrationIndex)->xp_baseTermIndex();
}
//======================================================
qint64 X_CalibrationRepository::xp_baseTermId(int calibrationIndex)
{
    if (calibrationIndex < 0 || calibrationIndex >= xv_caibrationList.count())
    {
        return -1;
    }

    return xv_caibrationList.at(calibrationIndex)->xp_baseTermId();
}
//======================================================
int X_CalibrationRepository::xp_baseTermIndexForCalibrationId(qint64 calibrationId)
{
    const X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return -1;
    }

    return calibration->xp_baseTermIndex();
}
//======================================================
qint64 X_CalibrationRepository::xp_baseTermIdForCalibrationId(qint64 calibrationId)
{
    const X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return false;
    }

    return calibration->xp_baseTermId();
}
//======================================================
void X_CalibrationRepository::xh_notifyCalibrationRecalc(qint64 calibrationId) const
{
    X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return;
    }

    calibration->xh_notifyCalibrationRecalc();
}
//======================================================
X_CalibrationQualityData X_CalibrationRepository::xp_calibrationQualityData(
    qint64 calibrationId) const
{
    X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return X_CalibrationQualityData();
    }

    return calibration->xp_calibrationQualityData();
}
//======================================================
bool X_CalibrationRepository::xp_termFactor(qint64 calibrationId,
                                           int termIndex,
                                           qreal& factor) const
{
    const X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        factor = 0;
        return false;
    }

    return calibration->xp_termFactor(termIndex, factor);
}
//======================================================
bool X_CalibrationRepository::xp_termFactorString(qint64 calibrationId,
                                                 int termIndex,
                                                 QString& factorString) const
{
    const X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        factorString = "0.0";
        return false;
    }
    return calibration->xp_termFactorString(termIndex, factorString);
}
//======================================================
bool X_CalibrationRepository::xp_setTermFactorString(qint64 calibrationId,
                                                    int termIndex,
                                                    const QString& factorString)
{
    X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return false;
    }
    return calibration->xp_setTermFactorString(termIndex, factorString);
}
//======================================================
bool X_CalibrationRepository::xp_termVariablePart(
    qint64 calibrationId,
    int termIndex,
    const X_AbstractSpectrum* spectrum,
    qreal& value)
{
    X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return false;
    }
    return calibration->xp_termVariablePart(termIndex, spectrum, value);
}
//======================================================
bool X_CalibrationRepository::xp_calcBaseTermValue(
    qint64 calibrationId, const X_AbstractSpectrum* spectrum, qreal& value)
{
    const X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return false;
    }

    return calibration->xp_calcBaseTermValue(spectrum, value);
}
//======================================================
void X_CalibrationRepository::xh_appendCalibrationsToArray(
    const QStringList& fileNameList)
{
    bool res = false;

    if(fileNameList.isEmpty())
    {
        return;
    }

    if(!xv_caibrationList.isEmpty())
    {
        QString question = tr("Clear current calibration list?");
        if(QMessageBox::question(nullptr, tr("Calibration appending"), question, QMessageBox::Yes, QMessageBox::No)
            == QMessageBox::Yes)
        {
            xp_clear();
        }
    }

    foreach (QString fileName, fileNameList)
    {
        res = xh_createCalibrationFromFile(fileName) || res;
    }

    if (res)
    {
        emit xg_setCurrentCalibrationIndex(xv_caibrationList.count() - 1);
    }

    xh_actionEnablingControl();
}
//======================================================
void X_CalibrationRepository::xp_onCurrentCalibrationChange(int current,
                                                           int previous)
{
    qint64 calibrationId;
    xv_currentCalibrationIndex = current;
    if (current < 0 || current >= xv_caibrationList.count())
    {
        calibrationId = -1;
        emit xg_currentCalibrationDirtyChanged(false, false);
    }
    else
    {
        calibrationId = xv_caibrationList.at(current)->xp_calibrationId();
        emit xg_currentCalibrationDirtyChanged(xv_caibrationList.at(current)
                                                   ->xp_isDirty(),
                                               true);
    }

    emit xg_currentCalibrationChanged(calibrationId, current);
    xh_actionEnablingControl();
}
//======================================================
void X_CalibrationRepository::xp_onSelectedCalibrationChange(
    QList<int> selectedIndexList)
{
    xh_actionEnablingControl();
}
//======================================================
void X_CalibrationRepository::xp_onCurrentCalibrationWindowChange(
    int currentIndex, int previousIndex)
{
    xv_currentWindowIndex = currentIndex;
    // current calibration
    int currentCalibrationIndex;

    emit xg_requestCurrentCalibrationIndex(currentCalibrationIndex);
    qint64 currentCalibrationWindowId;
    qint64 previousCalibrationWindowId;

    if (currentCalibrationIndex < 0
        || currentCalibrationIndex >= xv_caibrationList.count())
    {
        currentCalibrationWindowId = -1;
        previousCalibrationWindowId = -1;
    }
    else
    {
        if (currentIndex < 0
            || currentIndex >= xv_caibrationList.at(currentCalibrationIndex)
                                   ->xp_calibrationWindowCount())
        {
            currentCalibrationWindowId = -1;
        }
        else
        {
            currentCalibrationWindowId = xv_caibrationList
                                             .at(currentCalibrationIndex)
                                             ->xp_calibrationWindowId(
                                                 currentIndex);
        }

        if (previousIndex < 0
            || previousIndex >= xv_caibrationList.at(currentCalibrationIndex)
                                    ->xp_calibrationWindowCount())
        {
            previousCalibrationWindowId = -1;
        }
        else
        {
            previousCalibrationWindowId = xv_caibrationList
                                              .at(currentCalibrationIndex)
                                              ->xp_calibrationWindowId(
                                                  previousIndex);
        }
    }

    emit xg_currentCalibrationWindowChanged(currentCalibrationWindowId,
                                            currentIndex,
                                            previousCalibrationWindowId,
                                            previousIndex);
    xh_actionEnablingControl();
}
//======================================================
void X_CalibrationRepository::xp_onCalibrationWindowClick(const QModelIndex& index)
{
    if (!index.isValid())
    {
        return;
    }

    QString currentWindowName;
    int currentCalibrationIndex;

    emit xg_requestCurrentCalibrationIndex(currentCalibrationIndex);

    if (currentCalibrationIndex < 0
        || currentCalibrationIndex >= xv_caibrationList.count())
    {
        return;
    }

    currentWindowName = xv_caibrationList.at(currentCalibrationIndex)
                            ->xp_calibrationWindowName(index.row());
    if (!currentWindowName.isEmpty())
    {
        emit xg_currentCalibrationWindowName(currentWindowName);
    }
}
//======================================================
void X_CalibrationRepository::xp_onCalibrationWindowSelectionChange()
{
    xh_actionEnablingControl();
}
//======================================================
void X_CalibrationRepository::xp_onCurrentTermChange(int currentTermIndex,
                                                    int previousTermIndex)
{
    xv_currentTermIndex = currentTermIndex;
    // emit xg_invokeCalibrationRecalc();
    xh_actionEnablingControl();
}
//======================================================
void X_CalibrationRepository::xp_calibrationQualityDataChanged(
    bool saveTocalibration,
    qint64 calibrationId,
    X_CalibrationQualityData qualityData)
{
    if (!saveTocalibration)
    {
        return;
    }

    X_Calibration* calibration = xh_calibrationForId(calibrationId);
    if (!calibration)
    {
        return;
    }

    calibration->xp_setCalibrationQualityData(qualityData);
}
//======================================================
void X_CalibrationRepository::xp_onTermDoubleClocked(int row)
{
    X_Calibration* calibration = xv_caibrationList
                                    .value(xv_currentCalibrationIndex, nullptr);
    if (!calibration)
    {
        return;
    }

    if (calibration->xp_termType(row) == X_AbstractTerm::TT_CUSTOM)
    {
        X_CustomTermDialog* dialog = new X_CustomTermDialog(calibration, row);

        connect(this,
                &X_CalibrationRepository::xg_currentCalibrationWindowName,
                dialog,
                &X_CustomTermDialog::xp_insertVariable);
        dialog->show();
    }
}
//======================================================
void X_CalibrationRepository::xh_onNewCalibrationAction()
{
    // new Calibration Name
    int maxCalibrationNumber = 1;
    for (int i = 0; i < xv_caibrationList.count(); i++)
    {
        QString name = xv_caibrationList.at(i)->xp_baseName();
        if (name.startsWith(xv_defaultCalibrationName))
        {
            QString digitalPart = name.mid(xv_defaultCalibrationName.length());
            bool ok;
            int number = digitalPart.toInt(&ok);
            if (!ok)
            {
                continue;
            }
            if (number >= maxCalibrationNumber)
            {
                maxCalibrationNumber = ++number;
            }
        }
    }

    QString calibrationName = xv_defaultCalibrationName
                              + QString::number(maxCalibrationNumber);

    if (xh_createNewCalibration(calibrationName))
    {
        emit xg_setCurrentCalibrationIndex(xv_caibrationList.count() - 1);
        emit xg_startCurrentCalibrationEdition();
    }

    xh_actionEnablingControl();
}
//======================================================
void X_CalibrationRepository::xh_onRemoveCalibrationsAction()
{
    QList<int> selectedCalibrationIndexList;
    emit xg_requestSelectedCalibrationIndexList(selectedCalibrationIndexList);

    QString calibrationString;

    if (selectedCalibrationIndexList.isEmpty())
    {
        return;
    }
    else if (selectedCalibrationIndexList.count() == 1)
    {
        calibrationString = tr("selected calibration");
    }
    else
    {
        calibrationString = tr("selected calibrations");
    }

    QString question = tr("Do you want to remove %1 from the list?")
                           .arg(calibrationString);
    if (QMessageBox::question(nullptr,
                              tr("Calibration removing"),
                              question,
                              QMessageBox::Yes,
                              QMessageBox::No)
        == QMessageBox::No)
    {
        return;
    }

    //qSort(selectedCalibrationIndexList);
    std::sort(selectedCalibrationIndexList.begin(),
              selectedCalibrationIndexList.end());

    for (int i = selectedCalibrationIndexList.count() - 1; i >= 0; i--)
    {
        xh_removeCalibration(selectedCalibrationIndexList.value(i));
    }

    xh_actionEnablingControl();
}
//======================================================
void X_CalibrationRepository::xh_onNewWindowAction()
{
    int currentCalibrationIndex;
    emit xg_requestCurrentCalibrationIndex(currentCalibrationIndex);

    QRectF currentVisibleSceneRect;
    emit xg_requestCurrentVisibleSceneRect(currentVisibleSceneRect);

    if (currentCalibrationIndex < 0
        || currentCalibrationIndex >= xv_caibrationList.count())
    {
        return;
    }

    //int offset = qRound(currentVisibleSceneRect.width()/5);
    int windowWidth = qRound(currentVisibleSceneRect.width() / 20);
    int offset = qRound((currentVisibleSceneRect.width() - windowWidth) / 2);
    int firstChannel;
    int lastChannel;
    if (currentVisibleSceneRect.width() > 0)
    {
        firstChannel = qRound(currentVisibleSceneRect.left()) + offset;
        lastChannel = qRound(currentVisibleSceneRect.right()) - offset;
    }
    else
    {
        firstChannel = 0;
        lastChannel = 1;
    }

    int newWindowIndex
        = xv_caibrationList.at(currentCalibrationIndex)
              ->xp_createNewCalibrationWindow(firstChannel,
                                              lastChannel,
                                              xv_defaultCalibrationWindowType);

    emit xg_setCurrentWindowIndex(newWindowIndex);
    emit xg_startCurrentWindowEdition();

    xh_actionEnablingControl();
}
//======================================================
void X_CalibrationRepository::xh_onRemoveWindowAction()
{
    int currentCalibrationIndex;
    emit xg_requestCurrentCalibrationIndex(currentCalibrationIndex);

    if (currentCalibrationIndex < 0
        || currentCalibrationIndex >= xv_caibrationList.count())
    {
        return;
    }

    QList<int> selectedWindowList;
    emit xg_requestSelectedWindowIndexList(selectedWindowList);
    QString windowString;
    if (selectedWindowList.isEmpty())
    {
        //        QString string = tr("There is no window to remove!");
        //        QMessageBox::critical(0, tr("Window removing"), string, QMessageBox::Ok);
        return;
    }
    else if (selectedWindowList.count() == 1)
    {
        windowString = tr("selected window");
    }
    else
    {
        windowString = tr("selected windows");
    }

    QString question = tr("Do you want to remove %1 from the list?")
                           .arg(windowString);
    if (QMessageBox::question(nullptr,
                              tr("Window removing"),
                              question,
                              QMessageBox::Yes,
                              QMessageBox::No)
        == QMessageBox::No)
    {
        return;
    }

    //qSort(selectedWindowList);
    std::sort(selectedWindowList.begin(), selectedWindowList.end());

    for (int i = selectedWindowList.count() - 1; i >= 0; i--)
    {
        xh_removeCalibrationWindow(currentCalibrationIndex,
                                   selectedWindowList.value(i));
    }

    xh_actionEnablingControl();
}
//======================================================
void X_CalibrationRepository::xh_onRecalcEquationFactorsAction()
{
    // xh_recalcEquationFactors(xv_currentCalibrationIndex);
    // reset recalc block
    xv_blockCalibrationRecalc = false;
    emit xg_invokeCalibrationRecalc();
}
//======================================================
void X_CalibrationRepository::xh_onCreateMixedTermsAction()
{
    if (xv_currentCalibrationIndex < 0
        || xv_currentCalibrationIndex >= xv_caibrationList.count())
    {
        return;
    }

    X_Calibration* calibration = xv_caibrationList.at(xv_currentCalibrationIndex);
    calibration->xp_createMixedTerms(xv_currentTermIndex);
}
//======================================================
void X_CalibrationRepository::xh_onRemoveMixedTermsAction()
{
    if (xv_currentCalibrationIndex < 0
        || xv_currentCalibrationIndex >= xv_caibrationList.count())
    {
        return;
    }

    X_Calibration* calibration = xv_caibrationList.at(xv_currentCalibrationIndex);
    calibration->xp_removeMixedTerms();
}
//======================================================
void X_CalibrationRepository::xh_onCreateCustomTermAction()
{
    if (xv_currentCalibrationIndex < 0
        || xv_currentCalibrationIndex >= xv_caibrationList.count())
    {
        return;
    }
    X_Calibration* calibration = xv_caibrationList[xv_currentCalibrationIndex];

    X_CustomTermDialog* dialog = new X_CustomTermDialog(calibration);

    //    connect(dialog,
    //            &X_CustomTermDialog::xg_requestTermUpdate,
    //            calibration,
    //            &X_Calibration::xp_updateCustomTerm);
    //    connect(calibration,
    //            &X_Calibration::xg_message,
    //            dialog,
    //            &X_CustomTermDialog::xp_showMsg);
    connect(this,
            &X_CalibrationRepository::xg_currentCalibrationWindowName,
            dialog,
            &X_CustomTermDialog::xp_insertVariable);
    dialog->show();

    //    QList<int> windowIndexList;
    //    calibration->xp_createTerm(windowIndexList, X_AbstractTerm::TT_CUSTOM,
    //                               X_AbstractTerm::TS_CONST_EXCLUDED, "0.0");
}
//======================================================
void X_CalibrationRepository::xh_onRemoveCustomTermAction()
{
    // NOTE Remove custom term
    if (xv_currentCalibrationIndex < 0
        || xv_currentCalibrationIndex >= xv_caibrationList.count())
    {
        return;
    }

    X_Calibration* calibration = xv_caibrationList
                                    .value(xv_currentCalibrationIndex, nullptr);
    if (!calibration)
    {
        return;
    }

    // get current custom term xv_currentTermIndex
    calibration->xp_removeCustomTerm(xv_currentTermIndex);
}
//======================================================
void X_CalibrationRepository::xh_removeCalibrationWindow(
    int currentCalibrationIndex, int spectrumWindowIndex)
{
    if (currentCalibrationIndex < 0
        || currentCalibrationIndex >= xv_caibrationList.count())
    {
        return;
    }

    if (spectrumWindowIndex < 0
        || spectrumWindowIndex >= xv_caibrationList.at(currentCalibrationIndex)
                                      ->xp_calibrationWindowCount())
    {
        return;
    }

    xv_caibrationList[currentCalibrationIndex]->xp_removeCalibrationWindow(
        spectrumWindowIndex);
}
//======================================================
void X_CalibrationRepository::xh_onWindowOperation(
    X_Calibration::WindowOperationType type, int first, int last)
{
    if (!sender())
    {
        return;
    }

    X_Calibration* calibration = static_cast<X_Calibration*>(sender());
    for (int calibrationIndex = 0; calibrationIndex < xv_caibrationList.count();
         calibrationIndex++)
    {
        if (xv_caibrationList.at(calibrationIndex) != calibration)
        {
            continue;
        }

        if (type == X_Calibration::WOT_BRGIN_INSERT_WINDOWS)
        {
            emit xg_calibrationWindowOperation(WOT_INSERT_WINDOWS,
                                               calibrationIndex,
                                               first,
                                               last);
        }
        else if (type == X_Calibration::WOT_END_INSERT_WINDOWS)
        {
            emit xg_calibrationWindowOperation(WOT_END_INSERT_WINDOWS,
                                               calibrationIndex,
                                               first,
                                               last);
        }
        else if (type == X_Calibration::WOT_BEGIN_REMOVE_WINDOWS)
        {
            emit xg_calibrationWindowOperation(WOT_REMOVE_WINDOWS,
                                               calibrationIndex,
                                               first,
                                               last);
        }
        else if (type == X_Calibration::WOT_END_REMOVE_WINDOWS)
        {
            emit xg_calibrationWindowOperation(WOT_END_REMOVE_WINDOWS,
                                               calibrationIndex,
                                               first,
                                               last);
        }
        else if (type == X_Calibration::WOT_WINDOW_CHANGED)
        {
            emit xg_calibrationWindowOperation(WOT_WINDOW_CHANGED,
                                               calibrationIndex,
                                               first,
                                               last);
            if (!xv_blockCalibrationRecalc)
            {
                emit xg_invokeCalibrationRecalc();
            }
        }
        else if (type == X_Calibration::WOT_WINDOW_VISIBLE_CHANGED)
        {
            emit xg_calibrationWindowOperation(WOT_WINDOW_VISIBLE_CHANGED,
                                               calibrationIndex,
                                               first,
                                               last);
        }

        break;
    }
}
//======================================================
void X_CalibrationRepository::xh_onTermOperation(
    X_Calibration::TremOperationType type, int first, int last)
{
    if (!sender())
    {
        return;
    }

    X_Calibration* calibration = static_cast<X_Calibration*>(sender());
    for (int calibrationIndex = 0; calibrationIndex < xv_caibrationList.count();
         calibrationIndex++)
    {
        if (xv_caibrationList.at(calibrationIndex) != calibration)
        {
            continue;
        }

        if (type == X_Calibration::TOT_BEGIN_INSERT_TERM)
        {
            emit xg_termOperation(TOT_BEGIN_INSERT_TERM,
                                  calibrationIndex,
                                  first,
                                  last);
        }
        else if (type == X_Calibration::TOT_END_INSERT_TERM)
        {
            emit xg_termOperation(TOT_END_INSERT_TERM,
                                  calibrationIndex,
                                  first,
                                  last);
            xh_actionEnablingControl();
        }
        else if (type == X_Calibration::TOT_BEGIN_REMOVE_TERM)
        {
            emit xg_termOperation(TOT_BEGIN_REMOVE_TERM,
                                  calibrationIndex,
                                  first,
                                  last);
        }
        else if (type == X_Calibration::TOT_END_REMOVE_TERM)
        {
            emit xg_termOperation(TOT_END_REMOVE_TERM,
                                  calibrationIndex,
                                  first,
                                  last);

            if (!xv_blockCalibrationRecalc)
            {
                emit xg_invokeCalibrationRecalc();
            }

            xh_actionEnablingControl();
        }
        else if (type == X_Calibration::TOT_TERM_NAME_CHANGED)
        {
            emit xg_termOperation(TOT_TERM_NAME_CHANGED,
                                  calibrationIndex,
                                  first,
                                  last);
        }
        else if (type == X_Calibration::TOT_TERM_STATE_CHANGED)
        {
            emit xg_termOperation(TOT_TERM_STATE_CHANGED,
                                  calibrationIndex,
                                  first,
                                  last);

            if (!xv_blockCalibrationRecalc)
            {
                emit xg_invokeCalibrationRecalc();
            }

            //xh_recalcEquationFactors(calibrationIndex);
        }
        else if (type == X_Calibration::TOT_TERM_WINDOW_MARGIN_CHANGED)
        {
            emit xg_termOperation(TOT_TERM_WINDOW_MARGIN_CHANGED,
                                  calibrationIndex,
                                  first,
                                  last);
        }
        else if (type == X_Calibration::TOT_TERM_FACTOR_CHANGED)
        {
            emit xg_termOperation(TOT_TERM_FACTOR_CHANGED,
                                  calibrationIndex,
                                  first,
                                  last);
        }

        break;
    }
}
//======================================================
void X_CalibrationRepository::xh_onNormalizerChange() const
{
    if (!sender())
    {
        return;
    }

    X_Calibration* calibration = static_cast<X_Calibration*>(sender());
    qint64 calibrationId = calibration->xp_calibrationId();
    emit xg_normalizerChanged(calibrationId);

    if (!xv_blockCalibrationRecalc)
    {
        emit xg_invokeCalibrationRecalc();
    }
}
//======================================================
void X_CalibrationRepository::xh_onCalibrationFreeTermChange() const
{
    if (!sender())
    {
        return;
    }

    X_Calibration* calibration = static_cast<X_Calibration*>(sender());
    qint64 calibrationId = calibration->xp_calibrationId();

    int calibrationIndex = xh_calibrationIndexForId(calibrationId);
    emit xg_calibrationOperation(COT_CALIBRATION_FREE_MEMBER_CHANGED,
                                 calibrationIndex,
                                 calibrationIndex);
}
//======================================================
void X_CalibrationRepository::xh_onCalibrationDirtyChange(bool dirty) const
{
    // find calibrationIndex
    X_Calibration* calibration = qobject_cast<X_Calibration*>(sender());
    if (calibration == nullptr)
    {
        return;
    }

    int calibrationIndex = xh_calibrationIndexForId(
        calibration->xp_calibrationId());

    emit xg_currentCalibrationDirtyChanged(dirty, calibrationIndex >= 0);
    emit xg_calibrationOperation(COT_CALIBRATION_DIRTY_CHANGED,
                                 calibrationIndex,
                                 xv_currentArrayIndex);

    //    emit xg_currentCalibrationDirtyChanged(dirty,
    //                                           xv_currentCalibrationIndex >= 0 && xv_currentCalibrationIndex < xv_caibrationList.count());
    //    emit xg_calibrationOperation(COT_CALIBRATION_DIRTY_CHANGED, xv_currentCalibrationIndex, xv_currentArrayIndex);
}
//======================================================
void X_CalibrationRepository::xh_createCalibrationAndStartSaving(
    QString path, QString name) const
{
    if (xv_currentCalibrationIndex < 0
        || xv_currentCalibrationIndex >= xv_caibrationList.count())
    {
        qCritical() << "Invalid current calibration index. Cannot get "
                       "calibration from repository.";
        return;
    }

    X_Calibration* calibration = xv_caibrationList.value(
        xv_currentCalibrationIndex);
    if (!calibration)
    {
        return;
    }

    if (path.isEmpty())
    {
        path = calibration->xp_path();
    }

    if (name.isEmpty())
    {
        name = calibration->xp_fileName();
    }

    // check gain factor
    int gainFactor = calibration->xp_gainFactor();
    if (gainFactor < 1)
    {
        X_SetGainFactorToCalibrationDialog dialog(name);
        if (dialog.exec())
        {
            gainFactor = dialog.xp_gainFactor();
        }
        else
        {
            gainFactor = 0;
        }

        calibration->xp_setGainFactor(gainFactor);
    }

    emit xg_saveCalibration(xv_caibrationList.value(xv_currentCalibrationIndex),
                            path,
                            name);
}
//======================================================
void X_CalibrationRepository::xh_onCalibrationSaving(
    const X_Calibration* calibration, QString absFilePath)
{
    for (int i = 0; i < xv_caibrationList.count(); i++)
    {
        if (xv_caibrationList.at(i) == calibration)
        {
            xv_caibrationList.at(i)->xp_setPath(absFilePath);
            xv_caibrationList.at(i)->xp_setDirty(false);
            emit xg_calibrationOperation(COT_CALIBRATION_NAME_CHANGED, i, i);
            break;
        }
    }
}
//======================================================
void X_CalibrationRepository::xh_onSetCalibrationVisibleAction()
{
    for (int c = 0; c < xv_caibrationList.count(); c++)
    {
        xp_setCalibrationVisible(c, true);
    }
}
//======================================================
void X_CalibrationRepository::xh_onSetCalibrationInvisibleAction()
{
    for (int c = 0; c < xv_caibrationList.count(); c++)
    {
        xp_setCalibrationVisible(c, false);
    }
}
//======================================================
void X_CalibrationRepository::xh_onInvertCalibrationVisibilityAction()
{
    for (int c = 0; c < xv_caibrationList.count(); c++)
    {
        xp_setCalibrationVisible(c, !xp_isCalibrationVisible(c));
    }
}
//======================================================
void X_CalibrationRepository::xh_onSetWindowsVisibleAction()
{
    if (xv_currentCalibrationIndex < 0
        || xv_currentCalibrationIndex >= xv_caibrationList.count())
    {
        return;
    }

    X_Calibration* calibration = xv_caibrationList.at(xv_currentCalibrationIndex);
    if (!calibration)
    {
        return;
    }

    for (int w = 0; w < calibration->xp_calibrationWindowCount(); w++)
    {
        calibration->xp_setCalibrationWindowVisible(w, true);
    }
}
//======================================================
void X_CalibrationRepository::xh_onSetWindowsInvisibleAction()
{
    if (xv_currentCalibrationIndex < 0
        || xv_currentCalibrationIndex >= xv_caibrationList.count())
    {
        return;
    }

    X_Calibration* calibration = xv_caibrationList.at(xv_currentCalibrationIndex);
    if (!calibration)
    {
        return;
    }

    for (int w = 0; w < calibration->xp_calibrationWindowCount(); w++)
    {
        calibration->xp_setCalibrationWindowVisible(w, false);
    }
}
//======================================================
void X_CalibrationRepository::xh_onInvertWindowsVisibilityAction()
{
    if (xv_currentCalibrationIndex < 0
        || xv_currentCalibrationIndex >= xv_caibrationList.count())
    {
        return;
    }

    X_Calibration* calibration = xv_caibrationList.at(xv_currentCalibrationIndex);
    if (!calibration)
    {
        return;
    }

    for (int w = 0; w < calibration->xp_calibrationWindowCount(); w++)
    {
        calibration->xp_setCalibrationWindowVisible(
            w, !calibration->xp_isCalibrationWindowVisible(w));
    }
}
//======================================================
void X_CalibrationRepository::xh_onCopySelectedCalibrationAction()
{
    // Get selected calibration index list
    QList<int> selectedCalibrationList;
    emit xg_requestSelectedCalibrationIndexList(selectedCalibrationList);

    if (selectedCalibrationList.isEmpty())
    {
        return;
    }

    //qSort(selectedCalibrationList);
    std::sort(selectedCalibrationList.begin(), selectedCalibrationList.end());

    xv_blockCalibrationRecalc = true;
    for (int c = 0; c < selectedCalibrationList.count(); c++)
    {
        if (!xh_copyCalibration(selectedCalibrationList.at(c)))
        {
            // TODO error msg
        }
    }
    xv_blockCalibrationRecalc = false;
}
//======================================================
bool X_CalibrationRepository::xh_copyCalibration(int calibrationIndex)
{
    if (calibrationIndex < 0 || calibrationIndex >= xv_caibrationList.count())
    {
        return false;
    }

    QString numberSeparator = "-";
    QRegularExpression digitrx("^\\d{1,}$");

    // Define New Name
    // name w/o number
    QString newName = xv_caibrationList.at(calibrationIndex)->xp_baseName();
    QStringList nameParts = newName.split(numberSeparator);
    QString digitalPart;
    if (nameParts.isEmpty())
    {
        return false;
    }

    if (nameParts.count() > 1)
    {
        digitalPart = nameParts.last();
        if (digitrx.match(digitalPart.simplified()).hasPartialMatch())
        {
            // remove digital part
            newName.chop(digitalPart.length());
            // remove whitespaces and minuses
            for (int i = newName.length() - 1; i > 0; i--)
            {
                if (newName.at(i) == ' ' || newName.at(i) == '-')
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
    for (int i = 0; i < xv_caibrationList.count(); i++)
    {
        name = xv_caibrationList.at(i)->xp_baseName();
        if (name.startsWith(newName))
        {
            nameParts = name.split(numberSeparator);
            if (nameParts.count() < 2)
            {
                continue;
            }

            digitalPart = nameParts.last();
            if (!digitrx.match(digitalPart.simplified()).hasPartialMatch())
            {
                continue;
            }

            // get name w/o digital part
            name.chop(digitalPart.length());
            // remove whitespaces and minuses
            for (int i = name.length() - 1; i > 0; i--)
            {
                if (name.at(i) == ' ' || name.at(i) == '-')
                {
                    name.chop(1);
                    continue;
                }
                break;
            }

            // clone detected
            if (name == newName)
            {
                number = digitalPart.toInt(&ok);
                if (!ok)
                {
                    continue;
                }

                if (number >= maxCalibrationNumber)
                {
                    maxCalibrationNumber = ++number;
                }
            }
        }
    }

    // create new name
    newName = newName + numberSeparator + QString::number(maxCalibrationNumber);

    // create a new clone calibration and put it in array
    // windows and terms are created in calibration
    X_Calibration* newCalibration = new X_Calibration(xv_caibrationList.at(
                                                        calibrationIndex),
                                                    newName,
                                                    this);
    bool res = xh_appendCalibrationToList(newCalibration);
    if (!res)
    {
        delete newCalibration;
        return false;
    }

    newCalibration->xp_setVisible(true);
    res = xh_copyWindowsTermsAndEquation(calibrationIndex, newCalibration);
    if (!res)
    {
        // TODO Error report
    }

    return res;
}
//======================================================
bool X_CalibrationRepository::xh_copyWindowsTermsAndEquation(
    int srcCalibrationIndex, X_Calibration* trgCalibration)
{
    if (srcCalibrationIndex < 0
        || srcCalibrationIndex >= xv_caibrationList.count())
    {
        return false;
    }

    const X_Calibration* srcCalibration = xv_caibrationList.at(
        srcCalibrationIndex);
    int firstChannel;
    int lastChannel;
    X_CalibrationWindow::WindowType windowType;
    QString windowName;

    // copy windows
    for (int w = 0; w < srcCalibration->xp_calibrationWindowCount(); w++)
    {
        // get window parameters
        firstChannel = srcCalibration->xp_calibrationWindowFirstChannel(w);
        lastChannel = srcCalibration->xp_calibrationWindowLastChannel(w);
        windowType = srcCalibration->xp_calibrationWindowType(w);
        windowName = srcCalibration->xp_calibrationWindowName(w);

        // create new window in target calibration
        trgCalibration->xp_createNewCalibrationWindow(firstChannel,
                                                      lastChannel,
                                                      windowType,
                                                      windowName);
    }

    // copy terms
    QList<int> windowIndexList;
    X_AbstractTerm::TermType termType;

    int trgTermIndex;
    for (int t = 0; t < srcCalibration->xp_termCount(); t++)
    {
        trgTermIndex = xh_findCorrespondingTermIndex(t,
                                                     srcCalibration,
                                                     trgCalibration);
        if (trgTermIndex < 0 || trgTermIndex >= trgCalibration->xp_termCount())
        {
            // get scrTerm data

            termType = srcCalibration->xp_termType(t);

            // create new term
            if (termType == X_AbstractTerm::TT_CUSTOM)
            {
                trgCalibration
                    ->xp_createCustomTerm(X_AbstractTerm::TS_CONST_EXCLUDED,
                                          srcCalibration->xp_termName(t),
                                          srcCalibration->xp_termExpression(t),
                                          srcCalibration->xp_termDescription(t),
                                          "0.0");
            }
            else
            {
                windowIndexList = srcCalibration->xp_termWindowIndexList(t);
                trgCalibration->xp_createTerm(windowIndexList,
                                              termType,
                                              X_AbstractTerm::TS_CONST_EXCLUDED,
                                              "0.0");
            }
        }
    }

    // normalizer
    trgCalibration->xp_setNormaType(srcCalibration->xp_normaType());
    trgCalibration->xp_setNormaCustomString(
        srcCalibration->xp_normaCustomString());

    // Equation
    X_EquationSettingsData equationSettingsData(srcCalibration);
    // change base term id for target calibration
    equationSettingsData.baseTermId = trgCalibration->xp_termId(
        srcCalibration->xp_baseTermIndex());
    int trgCalibrationIndex = -1;
    for (int c = 0; c < xv_caibrationList.count(); c++)
    {
        if (xv_caibrationList.at(c) == trgCalibration)
        {
            trgCalibrationIndex = c;
            break;
        }
    }

    xp_setEquationDataFromSettings(trgCalibrationIndex, equationSettingsData);

    // copy intercept
    trgCalibration->xp_setEquationIntercept(
        srcCalibration->xp_equationIntercept());

    // copy term factors
    QString termFactorString;
    X_AbstractTerm::TermState termState;
    for (int t = 0; t < srcCalibration->xp_termCount(); t++)
    {
        trgTermIndex = xh_findCorrespondingTermIndex(t,
                                                     srcCalibration,
                                                     trgCalibration);

        // set term parameters
        // state
        termState = srcCalibration->xp_termState(t);
        trgCalibration->xp_setTermState(trgTermIndex, termState);

        // term factor
        if (!srcCalibration->xp_termFactorString(t, termFactorString))
        {
            termFactorString = "0.0";
        }

        trgCalibration->xp_setTermFactorString(trgTermIndex, termFactorString);
    }

    // equation quality data
    trgCalibration->xp_setCalibrationQualityData(
        srcCalibration->xp_calibrationQualityData());
    return true;
}
//======================================================
int X_CalibrationRepository::xh_findCorrespondingTermIndex(
    int srcTermIndex,
    const X_Calibration* srcCalibration,
    const X_Calibration* trgCalibration)
{
    if (srcCalibration == nullptr || trgCalibration == nullptr
        || srcTermIndex < 0 || srcTermIndex >= srcCalibration->xp_termCount())
    {
        return -1;
    }

    QString termName = srcCalibration->xp_termName(srcTermIndex);
    X_AbstractTerm::TermType termType = srcCalibration->xp_termType(srcTermIndex);
    QList<int> indexList = srcCalibration->xp_termWindowIndexList(srcTermIndex);

    for (int t = 0; t < trgCalibration->xp_termCount(); t++)
    {
        if (trgCalibration->xp_termName(t) == termName
            && trgCalibration->xp_termType(t) == termType
            && trgCalibration->xp_termWindowIndexList(t) == indexList)
        {
            return t;
        }
    }

    return -1;
}
//======================================================
void X_CalibrationRepository::xh_onCopySelectedWindowsAction() {}
//======================================================
void X_CalibrationRepository::xh_onPasteWindowsAction() {}
//======================================================
void X_CalibrationRepository::xh_onResetTermStateAction()
{
    if (xv_currentCalibrationIndex < 0
        || xv_currentCalibrationIndex >= xv_caibrationList.count())
    {
        return;
    }

    X_Calibration* calibration = xv_caibrationList.at(xv_currentCalibrationIndex);
    if (!calibration)
    {
        return;
    }

    for (int t = 0; t < calibration->xp_termCount(); t++)
    {
        calibration->xp_setTermState(t, X_AbstractTerm::TS_CONST_EXCLUDED);
        calibration->xp_setTermFactorString(t, "0.0");
    }

    calibration->xp_setEquationIntercept(0.0);
    if (!xv_blockCalibrationRecalc)
    {
        emit xg_invokeCalibrationRecalc();
    }
}
//======================================================
void X_CalibrationRepository::xh_removeCalibration(int index)
{
    if (index < 0 || index >= xv_caibrationList.count())
    {
        return;
    }

    xv_caibrationList.at(index)->xp_removeCalibrationWindows();

    emit xg_calibrationOperation(COT_REMOVE_CALIBRATIONS, index, index);
    xv_caibrationList.takeAt(index)->deleteLater();
    emit xg_calibrationOperation(COT_END_REMOVE_CALIBRATIONS, index, index);
}
//======================================================
bool X_CalibrationRepository::xh_createCalibrationFromFile(const QString& fileName)
{
    QFileInfo fileInfo(fileName);
    QString suffix = fileInfo.suffix();

    if (!fileInfo.exists() || !fileInfo.isFile())
    {
        QString error = tr("Error: \"%1\" is not a file!").arg(fileName);
        emit xg_message(error);
        qCritical().noquote() << error;
        return false;
    }
    else if (suffix != "clbx" && suffix != "xml")
    {
        QString error = QObject::tr("Cannot handle file of type \"%1\"!")
                            .arg(suffix);
        emit xg_message(error);
        qCritical().noquote() << error;
        return false;
    }

    // open file
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QString errorMsg;
        if (file.error() != QFile::NoError)
        {
            errorMsg = tr("Cannot read file \"%1\"! %2")
                           .arg(file.fileName(), file.errorString());
        }
        else
        {
            errorMsg = tr("Cannot read file \"%1\"! %2")
                           .arg(file.fileName(), tr("Unknown error"));
        }
        emit xg_message(errorMsg);
        qCritical().noquote() << errorMsg;
        return false;
    }

    //X_XMLCalibrationIOHandler* ioHandler = new X_XMLCalibrationIOHandler(this);
    X_XMLCalibrationIOHandler ioHandler;

    X_Calibration* calibration = new X_Calibration(fileName, this);
    calibration->xp_setVisible(true);
    xv_blockCalibrationRecalc = true;

    bool res = xh_appendCalibrationToList(calibration);

    if (res)
    {
        // res = ioHandler->xp_getCalibrationFromFile(file, calibration);
        res = ioHandler.xp_getCalibrationFromFile(file, calibration);
    }

    if (!res)
    {
        for (int c = 0; c < xv_caibrationList.count(); c++)
        {
            if (xv_caibrationList.at(c) == calibration)
            {
                xh_removeCalibration(c);
            }
        }
    }

    if (file.isOpen())
    {
        file.close();
    }

    xv_blockCalibrationRecalc = false;
    // delete ioHandler;
    return res;
}
//======================================================
bool X_CalibrationRepository::xh_createNewCalibration(const QString& name)
{
    if (name.isEmpty())
    {
        return false;
    }

    X_Calibration* calibration = new X_Calibration(name, this);
    calibration->xp_setVisible(true);
    return xh_appendCalibrationToList(calibration);
}
//======================================================
bool X_CalibrationRepository::xh_appendCalibrationToList(X_Calibration* calibration)
{
    if (!calibration)
    {
        return false;
    }

    connect(calibration,
            &X_Calibration::xg_message,
            this,
            &X_CalibrationRepository::xg_message);
    connect(calibration,
            &X_Calibration::xg_windowOperation,
            this,
            &X_CalibrationRepository::xh_onWindowOperation);
    connect(calibration,
            &X_Calibration::xg_termOperation,
            this,
            &X_CalibrationRepository::xh_onTermOperation);
    connect(calibration,
            &X_Calibration::xg_normalizerChanged,
            this,
            &X_CalibrationRepository::xh_onNormalizerChange);
    connect(calibration,
            &X_Calibration::xg_interceptChanged,
            this,
            &X_CalibrationRepository::xh_onCalibrationFreeTermChange);

    connect(calibration,
            &X_Calibration::xg_dirtyChanged,
            this,
            &X_CalibrationRepository::xh_onCalibrationDirtyChange);

    int insertIndex = xv_caibrationList.count();
    emit xg_calibrationOperation(COT_INSERT_CALIBRATIONS,
                                 insertIndex,
                                 insertIndex);
    xv_caibrationList << calibration;
    emit xg_calibrationOperation(COT_END_INSERT_CALIBRATIONS, -1, -1);

    // set new calibration current
    emit xg_setCurrentCalibrationIndex(insertIndex);

    return true;
}
//======================================================
void X_CalibrationRepository::xh_actionEnablingControl()
{
    bool calibrationDisabilityFlag = xv_currentCalibrationIndex < 0
                                     || xv_currentCalibrationIndex
                                            >= xv_caibrationList.count();
    QList<int> selectedCalibrationList;
    emit xg_requestSelectedCalibrationIndexList(selectedCalibrationList);

    xv_removeCalibrationAction->setDisabled(
        calibrationDisabilityFlag || selectedCalibrationList.isEmpty());
    xv_copySelectedCalibrationAction->setDisabled(
        calibrationDisabilityFlag || selectedCalibrationList.isEmpty());

    xv_saveCalibrationsAction->setDisabled(
        calibrationDisabilityFlag
        || !xv_caibrationList.at(xv_currentCalibrationIndex)->xp_isDirty());
    xv_saveAsCalibrationsAction->setDisabled(calibrationDisabilityFlag);
    xv_recalcEquationFactorsAction->setDisabled(calibrationDisabilityFlag);

    xv_setCalibrationVisibleAction->setDisabled(calibrationDisabilityFlag);
    xv_setCalibrationInvisibleAction->setDisabled(calibrationDisabilityFlag);
    xv_invertCalibrationVisibilityAction->setDisabled(calibrationDisabilityFlag);

    xv_newWindowAction->setDisabled(calibrationDisabilityFlag);

    QList<int> selectedWindowList;
    emit xg_requestSelectedWindowIndexList(selectedWindowList);
    xv_removeWindowAction->setDisabled(selectedWindowList.isEmpty());

    bool visibilityActionDisabled = calibrationDisabilityFlag
                                    || xv_caibrationList
                                               .at(xv_currentCalibrationIndex)
                                               ->xp_calibrationWindowCount()
                                           <= 0;
    xv_setWindowsVisibleAction->setDisabled(visibilityActionDisabled);
    xv_setWindowsInvisibleAction->setDisabled(visibilityActionDisabled);
    xv_invertWindowsVisibilityAction->setDisabled(visibilityActionDisabled);

    bool termResetActionDisabled
        = calibrationDisabilityFlag
          || xv_caibrationList.at(xv_currentCalibrationIndex)->xp_termCount()
                 <= 0;
    xv_resetTermStateAction->setDisabled(termResetActionDisabled);

    bool mixedTermCreationDisabled = calibrationDisabilityFlag
                                     || xv_caibrationList
                                                .at(xv_currentCalibrationIndex)
                                                ->xp_termType(
                                                    xv_currentTermIndex)
                                            != X_AbstractTerm::TT_SIMPLE;

    xv_createMixedTermsAction->setDisabled(mixedTermCreationDisabled);

    bool mixedTermRemoveDisabled = calibrationDisabilityFlag
                                   || !xv_caibrationList
                                           .at(xv_currentCalibrationIndex)
                                           ->xp_mixedTermExists();

    xv_removeMixedTermsAction->setDisabled(mixedTermRemoveDisabled);

    bool customTermCreationDisabled = calibrationDisabilityFlag
                                      || xv_caibrationList
                                                 .at(xv_currentCalibrationIndex)
                                                 ->xp_calibrationWindowCount()
                                             <= 0;
    xv_createCustomTermAction->setDisabled(customTermCreationDisabled);

    bool customTermRemoveDisabled = calibrationDisabilityFlag
                                    || xv_caibrationList
                                               .at(xv_currentCalibrationIndex)
                                               ->xp_termType(xv_currentTermIndex)
                                           != X_AbstractTerm::TT_CUSTOM;

    xv_removeCustomTermAction->setDisabled(customTermRemoveDisabled);
}
//======================================================
void X_CalibrationRepository::xh_createActions()
{
    // calibration actions
    xv_newCalibrationAction = new QAction(this);
    xv_newCalibrationAction->setIcon(QIcon(NS_Icons::glIconAddCalibration));
    xv_newCalibrationAction->setText(tr("New calibration"));
    xv_newCalibrationAction->setToolTip(
        tr("Append a new calibration to the list"));

    xv_openCalibrationsAction = new QAction(this);
    //    xv_openCalibrationsAction->setIcon(QIcon(glOpenCalibrationIconString));
    //    xv_openCalibrationsAction->setText(tr("Open calibrations"));
    //    xv_openCalibrationsAction->setToolTip(tr("Append a new calibration to the list"));

    xv_saveCalibrationsAction = new QAction(this);
    //    xv_openCalibrationsAction->setIcon(QIcon(glOpenCalibrationIconString));
    //    xv_openCalibrationsAction->setText(tr("Open calibrations"));
    //    xv_openCalibrationsAction->setToolTip(tr("Append a new calibration to the list"));

    xv_saveAsCalibrationsAction = new QAction(this);
    //    xv_openCalibrationsAction->setIcon(QIcon(glOpenCalibrationIconString));
    //    xv_openCalibrationsAction->setText(tr("Open calibrations"));
    //    xv_openCalibrationsAction->setToolTip(tr("Append a new calibration to the list"));

    xv_removeCalibrationAction = new QAction(this);
    xv_removeCalibrationAction->setDisabled(true);
    xv_removeCalibrationAction->setIcon(
        QIcon(NS_Icons::glIconRemoveCalibration));
    xv_removeCalibrationAction->setText(tr("Remove selected calibrations"));
    xv_removeCalibrationAction->setToolTip(
        tr("Remove selected calibrations from the list"));

    // window actions
    xv_newWindowAction = new QAction(this);
    xv_newWindowAction->setDisabled(true);
    xv_newWindowAction->setIcon(QIcon(NS_Icons::glIconAddWindow));
    xv_newWindowAction->setText(tr("New window"));
    xv_newWindowAction->setToolTip(tr("Append a new window to the list"));

    xv_removeWindowAction = new QAction(this);
    xv_removeWindowAction->setDisabled(true);
    xv_removeWindowAction->setIcon(QIcon(NS_Icons::glIconRemoveWindow));
    xv_removeWindowAction->setText(tr("Remove selected windows"));
    xv_removeWindowAction->setToolTip(
        tr("Remove selected windows from the list"));

    // action actions
    xv_recalcEquationFactorsAction = new QAction(this);
    xv_recalcEquationFactorsAction->setDisabled(true);
    xv_recalcEquationFactorsAction->setIcon(QIcon(NS_Icons::glIconCalc));
    xv_recalcEquationFactorsAction->setText(tr("Recalculate calibration"));
    xv_recalcEquationFactorsAction->setToolTip(
        tr("Recalculate equation factors for current calibration"));

    // terms actions
    xv_createMixedTermsAction = new QAction(this);
    xv_createMixedTermsAction->setDisabled(true);
    xv_createMixedTermsAction->setIcon(QIcon(NS_Icons::glIconAddMixedTerms));
    xv_createMixedTermsAction->setText(tr("Create mixed terms"));
    xv_createMixedTermsAction->setToolTip(tr("Create mixed terms"));

    xv_removeMixedTermsAction = new QAction(this);
    xv_removeMixedTermsAction->setDisabled(true);
    xv_removeMixedTermsAction->setIcon(QIcon(NS_Icons::glIconRemoveMixedTerms));
    xv_removeMixedTermsAction->setText(tr("Remove mixed terms"));
    xv_removeMixedTermsAction->setToolTip(
        tr("Remove mixed terms from the list"));

    xv_createCustomTermAction = new QAction(this);
    xv_createCustomTermAction->setDisabled(true);
    xv_createCustomTermAction->setIcon(QIcon(NS_Icons::glIconAddCustomTerm));
    xv_createCustomTermAction->setText(tr("Create custom term"));
    xv_createCustomTermAction->setToolTip(tr("Create custom term"));

    xv_removeCustomTermAction = new QAction(this);
    xv_removeCustomTermAction->setDisabled(true);
    xv_removeCustomTermAction->setIcon(QIcon(NS_Icons::glIconRemoveCustomTerm));
    xv_removeCustomTermAction->setText(tr("Remove custom term"));
    xv_removeCustomTermAction->setToolTip(
        tr("Remove custom term from the list"));

    // new actions
    xv_setCalibrationVisibleAction = new QAction(this);
    xv_setCalibrationVisibleAction->setDisabled(true);
    xv_setCalibrationVisibleAction->setIcon(QIcon(NS_Icons::glIconVisible));
    xv_setCalibrationVisibleAction->setText(tr("Set calibration visible"));
    xv_setCalibrationVisibleAction->setToolTip(
        tr("Set visible calibration column"));

    xv_setCalibrationInvisibleAction = new QAction(this);
    xv_setCalibrationInvisibleAction->setDisabled(true);
    xv_setCalibrationInvisibleAction->setIcon(QIcon(NS_Icons::glIconInvisible));
    xv_setCalibrationInvisibleAction->setText(tr("Set calibration invisible"));
    xv_setCalibrationInvisibleAction->setToolTip(
        tr("Set invisible calibration column"));

    xv_invertCalibrationVisibilityAction = new QAction(this);
    xv_invertCalibrationVisibilityAction->setDisabled(true);
    xv_invertCalibrationVisibilityAction->setIcon(
        QIcon(NS_Icons::glIconInverseVisible));
    xv_invertCalibrationVisibilityAction->setText(
        tr("Invert calibration visibility"));
    xv_invertCalibrationVisibilityAction->setToolTip(
        tr("Invert calibration visibility"));

    xv_setWindowsVisibleAction = new QAction(this);
    xv_setWindowsVisibleAction->setDisabled(true);
    xv_setWindowsVisibleAction->setIcon(QIcon(NS_Icons::glIconVisible));
    xv_setWindowsVisibleAction->setText(tr("Set window visible"));
    xv_setWindowsVisibleAction->setToolTip(tr("Set window visible"));

    xv_setWindowsInvisibleAction = new QAction(this);
    xv_setWindowsInvisibleAction->setDisabled(true);
    xv_setWindowsInvisibleAction->setIcon(QIcon(NS_Icons::glIconInvisible));
    xv_setWindowsInvisibleAction->setText(tr("Set window invisible"));
    xv_setWindowsInvisibleAction->setToolTip(tr("Set window invisible"));

    xv_invertWindowsVisibilityAction = new QAction(this);
    xv_invertWindowsVisibilityAction->setDisabled(true);
    xv_invertWindowsVisibilityAction->setIcon(
        QIcon(NS_Icons::glIconInverseVisible));
    xv_invertWindowsVisibilityAction->setText(tr("Invert window visibility"));
    xv_invertWindowsVisibilityAction->setToolTip(
        tr("Invert window visibility"));

    xv_copySelectedCalibrationAction = new QAction(this);
    xv_copySelectedCalibrationAction->setDisabled(true);
    xv_copySelectedCalibrationAction->setIcon(
        QIcon(NS_Icons::glIconDuplicateCalibration));
    xv_copySelectedCalibrationAction->setText(
        tr("Duplicate selected calibrations"));
    xv_copySelectedCalibrationAction->setToolTip(
        tr("Duplicate selected calibrations"));

    xv_copySelectedWindowsAction = new QAction(this);
    xv_copySelectedWindowsAction->setDisabled(true);
    xv_copySelectedWindowsAction->setIcon(QIcon(NS_Icons::glIconRemove));
    xv_copySelectedWindowsAction->setText(tr("Copy selected windows"));
    xv_copySelectedWindowsAction->setToolTip(tr("Copy selected windows"));

    xv_pasteWindowsAction = new QAction(this);
    xv_pasteWindowsAction->setDisabled(true);
    xv_pasteWindowsAction->setIcon(QIcon(NS_Icons::glIconRemove));
    xv_pasteWindowsAction->setText(tr("Paste windows"));
    xv_pasteWindowsAction->setToolTip(tr("Paste windows"));

    xv_resetTermStateAction = new QAction(this);
    xv_resetTermStateAction->setDisabled(true);
    xv_resetTermStateAction->setIcon(QIcon(NS_Icons::glIconReset));
    xv_resetTermStateAction->setText(tr("Reset terms"));
    xv_resetTermStateAction->setToolTip(tr("Reset equation terms"));
}
//======================================================
void X_CalibrationRepository::xh_createConnections()
{
    connect(xv_newCalibrationAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onNewCalibrationAction);
    //    connect(xv_openCalibrationsAction, &QAction::triggered,
    //            this, &X_CalibrationRepository::xg_openCalibrationsActionTriggered);
    //    connect(xv_saveCalibrationsAction, &QAction::triggered,
    //            this, &X_CalibrationRepository::xg_saveCalibrationsActionTriggered);
    //    connect(xv_saveAsCalibrationsAction, &QAction::triggered,
    //            this, &X_CalibrationRepository::xg_saveAsCalibrationsActionTriggered);

    connect(xv_removeCalibrationAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onRemoveCalibrationsAction);
    connect(xv_newWindowAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onNewWindowAction);
    connect(xv_removeWindowAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onRemoveWindowAction);
    connect(xv_recalcEquationFactorsAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onRecalcEquationFactorsAction);

    connect(xv_createMixedTermsAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onCreateMixedTermsAction);
    connect(xv_removeMixedTermsAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onRemoveMixedTermsAction);
    connect(xv_createCustomTermAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onCreateCustomTermAction);
    connect(xv_removeCustomTermAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onRemoveCustomTermAction);

    connect(xv_setCalibrationVisibleAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onSetCalibrationVisibleAction);
    connect(xv_setCalibrationInvisibleAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onSetCalibrationInvisibleAction);
    connect(xv_invertCalibrationVisibilityAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onInvertCalibrationVisibilityAction);

    connect(xv_setWindowsVisibleAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onSetWindowsVisibleAction);
    connect(xv_setWindowsInvisibleAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onSetWindowsInvisibleAction);
    connect(xv_invertWindowsVisibilityAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onInvertWindowsVisibilityAction);

    connect(xv_copySelectedCalibrationAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onCopySelectedCalibrationAction);
    connect(xv_copySelectedWindowsAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onCopySelectedWindowsAction);
    connect(xv_pasteWindowsAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onPasteWindowsAction);

    connect(xv_resetTermStateAction,
            &QAction::triggered,
            this,
            &X_CalibrationRepository::xh_onResetTermStateAction);
}
//======================================================
X_Calibration* X_CalibrationRepository::xh_calibrationForId(
    qint64 calibrationId) const
{
    if (calibrationId < 0)
    {
        return nullptr;
    }

    for (int c = 0; c < xv_caibrationList.count(); c++)
    {
        if (xv_caibrationList.at(c)->xp_calibrationId() != calibrationId)
        {
            continue;
        }

        return xv_caibrationList.value(c);
    }

    return nullptr;
}
//======================================================
int X_CalibrationRepository::xh_calibrationIndexForId(qint64 calibrationId) const
{
    if (calibrationId < 0)
    {
        return -1;
    }

    for (int c = 0; c < xv_caibrationList.count(); c++)
    {
        if (xv_caibrationList.at(c)->xp_calibrationId() != calibrationId)
        {
            continue;
        }

        return c;
    }

    return -1;
}
//======================================================

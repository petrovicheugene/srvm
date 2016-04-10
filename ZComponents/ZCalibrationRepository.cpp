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
            return zv_caibrationList.at(i)->zp_name();
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
QString ZCalibrationRepository::zp_baseMemberString(int row) const
{
    if(row < 0 || row >= zv_caibrationList.count())
    {
        return QString();
    }

    return zv_caibrationList.at(row)->zp_baseTermString();
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
    return calibration->zp_termName(termIndex);
}
//======================================================
ZAbstractTerm::TermState ZCalibrationRepository::zp_termState(qint64 calibrationId, int termIndex)
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return ZAbstractTerm::TS_CONST_EXCLUDED;
    }
    return calibration->zp_termState(termIndex);
}
//======================================================
void ZCalibrationRepository::zp_setNextUsersTermState(qint64 calibrationId, int termLogIndex)
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
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
    if(calibrationIndex < 0 || calibrationIndex > zv_caibrationList.count())
    {
        return ZTermNormalizer::NT_NONE;
    }

    return zv_caibrationList.at(calibrationIndex)->zp_normaType();
}
//======================================================
bool ZCalibrationRepository::zp_setNormaType(int calibrationIndex, ZTermNormalizer::NormaType type)
{
    if(calibrationIndex < 0 || calibrationIndex > zv_caibrationList.count())
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
    if(calibrationIndex < 0 || calibrationIndex > zv_caibrationList.count())
    {
        return ZCalibration::ET_NOT_DEFINED;
    }

    return zv_caibrationList.at(calibrationIndex)->zp_equationType();
}
//======================================================
bool ZCalibrationRepository::zp_setEquationType(int calibrationIndex, ZCalibration::EquationType type)
{
    if(calibrationIndex < 0 || calibrationIndex > zv_caibrationList.count())
    {
        return false;
    }

    return zv_caibrationList.at(calibrationIndex)->zp_setEquationType(type);
}
//======================================================
ZNormaSettingsData ZCalibrationRepository::zp_normaSettingsData(int calibrationIndex) const
{
    if(calibrationIndex < 0 || calibrationIndex > zv_caibrationList.count())
    {
        return ZNormaSettingsData();
    }

    return ZNormaSettingsData(zv_caibrationList.at(calibrationIndex));
}
//======================================================
bool ZCalibrationRepository::zp_setNormaSettings(int calibrationIndex, ZNormaSettingsData settings)
{
    if(calibrationIndex < 0 || calibrationIndex > zv_caibrationList.count())
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
    if(calibrationIndex < 0 || calibrationIndex > zv_caibrationList.count())
    {
        return ZEquationSettingsData();
    }

    return ZEquationSettingsData(zv_caibrationList.at(calibrationIndex));
}
//======================================================
bool ZCalibrationRepository::zp_setEquationDataFromSettings(int calibrationIndex, ZEquationSettingsData settings)
{
    if(calibrationIndex < 0 || calibrationIndex > zv_caibrationList.count())
    {
        return false;
    }

    ZCalibration* calibration = zv_caibrationList.value(calibrationIndex);
    if(calibration->zp_setEquationType(settings.equationType))
    {
        emit zg_calibrationOperation(COT_CALIBRATION_EQUATION_TYPE_CHANGED, calibrationIndex, calibrationIndex);
    }
    if(calibration->zp_setBaseTermId(settings.baseTermId))
    {
        emit zg_calibrationOperation(COT_CALIBRATION_EQUATION_BASE_TERM_CHANGED, calibrationIndex, calibrationIndex);
    }
    return true;
}
//======================================================
qreal ZCalibrationRepository::zp_equationFreeMemeber(int calibrationIndex) const
{
    if(calibrationIndex < 0 || calibrationIndex > zv_caibrationList.count())
    {
        return 0.0;
    }

    return zv_caibrationList.at(calibrationIndex)->zp_equationFreeMember();
}
//======================================================
bool ZCalibrationRepository::zp_setEquationFreeMember(int calibrationIndex, qreal value)
{
    if(calibrationIndex < 0 || calibrationIndex > zv_caibrationList.count())
    {
        return false;
    }

    bool res = zv_caibrationList.at(calibrationIndex)->zp_setEquationFreeMember(value);
    return res;
}
//======================================================
QString ZCalibrationRepository::zp_equationFreeMemeberString(int calibrationIndex) const
{
    if(calibrationIndex < 0 || calibrationIndex > zv_caibrationList.count())
    {
        return "0.0";
    }

    return zv_caibrationList.at(calibrationIndex)->zp_equationFreeMemberString();
}
//======================================================
bool ZCalibrationRepository::zp_setEquationFreeMemberString(int calibrationIndex, const QString& freeMemberString)
{
    if(calibrationIndex < 0 || calibrationIndex > zv_caibrationList.count())
    {
        return false;
    }

    bool res = zv_caibrationList.at(calibrationIndex)->zp_setEquationFreeMemberString(freeMemberString);
    return res;
}
//======================================================
int ZCalibrationRepository::zp_baseTermIndex(int calibrationIndex)
{
    if(calibrationIndex < 0 || calibrationIndex > zv_caibrationList.count())
    {
        return -1;
    }

    return zv_caibrationList.at(calibrationIndex)->zp_baseTermIndex();
}
//======================================================
qint64 ZCalibrationRepository::zp_baseTermId(int calibrationIndex)
{
    if(calibrationIndex < 0 || calibrationIndex > zv_caibrationList.count())
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
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
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
bool ZCalibrationRepository::zp_setTermFactor(qint64 calibrationId, int termIndex, qreal factor)
{
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
    if(!calibration)
    {
        return false;
    }

    return calibration->zp_setTermFactor(termIndex, factor);
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
    const ZCalibration* calibration = zh_calibrationForId(calibrationId);
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
    zv_currentCalibrationIndex = current;
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

    int newWindowIndex = -1;
    zv_caibrationList.at(currentCalibrationIndex)->zp_createNewCalibrationWindow(newWindowIndex,
                                                                                 firstChannel,
                                                                                 lastChannel,
                                                                                 zv_defaultCalibrationWindowType);

    emit zg_setCurrentWindowIndex(newWindowIndex);
    emit zg_startCurrentWindowEdition();
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
void ZCalibrationRepository::zh_onRecalcEquationFactorsAction()
{
    // zh_recalcEquationFactors(zv_currentCalibrationIndex);
    emit zg_invokeCalibrationRecalc();
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
        }
        else if(type == ZCalibration::TOT_TERM_NAME_CHANGED)
        {
            emit zg_termOperation(TOT_TERM_NAME_CHANGED, calibrationIndex, first, last);
        }
        else if(type == ZCalibration::TOT_TERM_STATE_CHANGED)
        {
            emit zg_termOperation(TOT_TERM_STATE_CHANGED, calibrationIndex, first, last);
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
    connect(calibration, &ZCalibration::zg_normalizerChanged,
            this, &ZCalibrationRepository::zh_onNormalizerChange);
    connect(calibration, &ZCalibration::zg_freeTermChanged,
            this, &ZCalibrationRepository::zh_onCalibrationFreeTermChange);

    int insertIndex = zv_caibrationList.count();
    emit zg_calibrationOperation(COT_INSERT_CALIBRATIONS, insertIndex, insertIndex);
    zv_caibrationList << calibration;
    emit zg_calibrationOperation(COT_END_INSERT_CALIBRATIONS, -1, -1);
    return true;
}
//======================================================
void ZCalibrationRepository::zh_actionAvailabilityControl(int current)
{
    bool disabilityFlag = current < 0 || current >= zv_caibrationList.count();
    if(zv_removeCalibrationAction)
    {
        zv_removeCalibrationAction->setDisabled(disabilityFlag);
    }
    if(zv_recalcEquationFactorsAction)
    {
        zv_recalcEquationFactorsAction->setDisabled(disabilityFlag);
    }
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

    // action actions
    zv_recalcEquationFactorsAction = new QAction(this);
    zv_recalcEquationFactorsAction->setIcon(QIcon(":/images/calc.png"));
    zv_recalcEquationFactorsAction->setText(tr("Recalculate calibration"));
    zv_recalcEquationFactorsAction->setToolTip(tr("Recalculate equation factors for current calibration"));

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

//=========================================================
#include "ZCalibration.h"
#include "ZAbstractSpectrum.h"
#include "ZSimpleTerm.h"
#include "ZQuadraticTerm.h"
#include "ZCrossProductTerm.h"
#include "ZCustomTerm.h"
#include "ZTermNormalizer.h"
#include "globalVariables.h"

#include <QFileInfo>
#include <QPointer>
//=========================================================
QList<QColor> ZCalibration::zv_colorList = ZCalibration::zp_createColorList();
qint64 ZCalibration::zv_lastCalibrationId = 0;
int ZCalibration::zv_lastColorIndex = 0;
//=========================================================
ZCalibration::ZCalibration(const QString& name, QObject *parent) : QObject(parent)
{
    zv_visibility = false;
    zv_defaultWindowName = tr("Peak #");
    QFileInfo fileInfo(name);
    if(!fileInfo.absoluteFilePath().isEmpty())
    {
        zv_baseName = fileInfo.baseName();
        zv_path = fileInfo.absolutePath();
        zv_dirty = false;
    }
    else
    {
        zv_path = QString();
        zv_baseName = name;
        zv_dirty = true;
    }

    zv_color = zv_colorList.value(zv_lastColorIndex++);
    if(zv_lastColorIndex >= zv_colorList.count())
    {
        zv_lastColorIndex = 0;
    }

    zv_calibrationId = zv_lastCalibrationId++;
    zv_termNormalizer  = new ZTermNormalizer(this);

}
//=========================================================
ZCalibration::~ZCalibration()
{

}
//=========================================================
QString ZCalibration::zp_name() const
{
    return zv_baseName;
}
//=========================================================
void ZCalibration::zp_setBaseName(const QString& name)
{
    if(name == zv_baseName || name.isEmpty())
    {
        return;
    }

    zv_baseName = name;
    zv_dirty = false;
}
//=========================================================
QString ZCalibration::zp_path() const
{
    return zv_path;
}
//=========================================================
void ZCalibration::zp_setPath(const QString& path)
{
    if(path == zv_path || path.isEmpty())
    {
        return;
    }
    QFileInfo fileInfo(path);
    if(fileInfo.absoluteFilePath().isEmpty())
    {
        return;
    }

    zv_path = fileInfo.absolutePath();
    zv_dirty = true;

    emit zg_dirtyChanged(zv_dirty);
}
//=========================================================
QString ZCalibration::zp_chemElement() const
{
    return zv_chemElement;
}
//=========================================================
bool ZCalibration::zp_setChemElement(const QString& chemElement)
{
    if(chemElement.isEmpty())
    {
        return false;
    }

    zv_chemElement = chemElement;
    return true;
}
//=========================================================
bool ZCalibration::zp_isVisible() const
{
    return zv_visibility;
}
//=========================================================
void ZCalibration::zp_setVisible(bool visibility)
{
    if(visibility != zv_visibility)
    {
        zv_visibility = visibility;
        emit zg_visibilityChanged(zv_visibility);
    }
}
//=========================================================
bool ZCalibration::zp_isDirty() const
{
    return zv_dirty;
}
//=========================================================
QColor ZCalibration::zp_color() const
{
    return zv_color;
}
//=========================================================
qint64 ZCalibration::zp_calibrationId() const
{
    return zv_calibrationId;
}
//=========================================================
bool ZCalibration::zp_calcConcentration(const ZAbstractSpectrum* const spectrum, qreal& concentration)
{
    if(spectrum == 0)
    {
        concentration = 0;
        return false;
    }

    concentration = 0;
    return true;
}
//=========================================================
void ZCalibration::zp_createNewCalibrationWindow(int& windowNewIndex, int firstChannel, int lastChannel)
{
    int nextWindowIndex = zv_windowList.count();
    QString nextName = zv_defaultWindowName + QString::number(++nextWindowIndex);

    while(zh_isWindowExist(nextName))
    {
        nextName = zv_defaultWindowName + QString::number(++nextWindowIndex);
    }

    windowNewIndex = zv_windowList.count();
    ZCalibrationWindow* calibrationWindow = new ZCalibrationWindow(nextName,
                                                                   ZCalibrationWindow::WT_NOT_DEFINED,
                                                                   firstChannel,
                                                                   lastChannel,
                                                                   this);

    emit zg_windowOperation(WOT_BRGIN_INSERT_WINDOWS, windowNewIndex, windowNewIndex);
    zv_windowList.append(calibrationWindow);
    emit zg_windowOperation(WOT_END_INSERT_WINDOWS, windowNewIndex, windowNewIndex);

    // zh_createTermsForWindow(calibrationWindow);
}
//=========================================================
void ZCalibration::zh_createTermsForWindow(const ZCalibrationWindow* window)
{
    // creation simple
    int windowIndex = zv_windowList.indexOf(const_cast<ZCalibrationWindow*>( window));

    if(!zh_windowHasTerms(window, ZAbstractTerm::TT_SIMPLE))
    {
        // define newTermIndex;
        int newTermIndex = -1;
        const ZCalibrationWindow* currentWindow;

        for(int t = 0; t < zv_termList.count(); t++)
        {
            if(zv_termList.at(t)->zp_termType() != ZAbstractTerm::TT_SIMPLE)
            {
                continue;
            }

            const ZSimpleTerm* simpleTerm = qobject_cast<const ZSimpleTerm*>(zv_termList.at(t));
            if(!simpleTerm)
            {
                continue;
            }

            currentWindow = simpleTerm->zp_window();
            if(zv_windowList.indexOf(const_cast<ZCalibrationWindow*>(currentWindow)) > windowIndex)
            {
                break;
            }
            newTermIndex = t;
        }

        newTermIndex++;

        // creation and insertion
        ZSimpleTerm* simpleTerm = new ZSimpleTerm(window, this);
        emit zg_termOperation(TOT_BEGIN_INSERT_TERM, newTermIndex, newTermIndex);
        zv_termList.insert(newTermIndex, simpleTerm);
        emit zg_termOperation(TOT_END_INSERT_TERM, newTermIndex, newTermIndex);

    }

    // quadratic
    if(!zh_windowHasTerms(window, ZAbstractTerm::TT_QUADRATIC))
    {
        // define newTermIndex;
        int newTermIndex = -1;
        int lastSimpleTermIndex = -1;
        const ZCalibrationWindow* currentWindow;

        for(int t = 0; t < zv_termList.count(); t++)
        {
            if(zv_termList.at(t)->zp_termType() == ZAbstractTerm::TT_SIMPLE)
            {
                lastSimpleTermIndex = t;
                continue;
            }

            if(zv_termList.at(t)->zp_termType() != ZAbstractTerm::TT_QUADRATIC)
            {
                continue;
            }

            const ZQuadraticTerm* simpleTerm = qobject_cast<const ZQuadraticTerm*>(zv_termList.at(t));
            if(!simpleTerm)
            {
                continue;
            }

            currentWindow = simpleTerm->zp_window();
            if(zv_windowList.indexOf(const_cast<ZCalibrationWindow*>(currentWindow)) > windowIndex)
            {
                break;
            }

            newTermIndex = t;
        }

        if(newTermIndex < 0)
        {
            newTermIndex = lastSimpleTermIndex + 1;
        }
        else
        {
            newTermIndex++;
        }

        // creation and insertion
        ZQuadraticTerm* quadraticTerm = new ZQuadraticTerm(window, this);
        // insertion
        emit zg_termOperation(TOT_BEGIN_INSERT_TERM, newTermIndex, newTermIndex);
        zv_termList.insert(newTermIndex, quadraticTerm );
        emit zg_termOperation(TOT_END_INSERT_TERM, newTermIndex, newTermIndex);
    }
}
//=========================================================
bool ZCalibration::zh_windowHasTerms(const ZCalibrationWindow* window, ZAbstractTerm::TermType type) const
{
    for(int t = 0; t < zv_termList.count(); t++)
    {
        if(zv_termList.at(t)->zp_termType() != type)
        {
            continue;
        }

        if(zv_termList.at(t)->zp_termBelongsToWindow(window))
        {
            return true;
        }
    }

    return false;
}
//=========================================================
void ZCalibration::zh_normalizerValue(qreal& value) const
{
    zv_termNormalizer->zp_value(value);
}
//=========================================================
int ZCalibration::zp_termIndex(const ZAbstractTerm* term) const
{
    if(!term)
    {
        return -1;
    }

    for(int i = 0; i < zv_termList.count(); i++)
    {
        if(zv_termList.at(i) == term)
        {
            return i;
        }
    }

    return -1;
}
//=========================================================
bool ZCalibration::zp_termFactor(int termIndex, qreal& factor) const
{
    if(termIndex < 0 || termIndex >= zv_termList.count() )
    {
        factor = 0;
        return false;
    }

    factor =  zv_termList.at(termIndex)->zp_termFactor();
    return true;
}
//=========================================================
bool ZCalibration::zp_setTermFactor(int termIndex, qreal factor) const
{
    if(termIndex < 0 || termIndex >= zv_termList.count() )
    {
        factor = 0;
        return false;
    }

    zv_termList.at(termIndex)->zp_setTermFactor(factor);
    return true;
}
//=========================================================
bool ZCalibration::zp_termVariablePart(int termIndex, const ZAbstractSpectrum* spectrum,  qreal& value) const
{
    if(termIndex < 0 || termIndex >= zv_termList.count() )
    {
        return false;
    }

    qint64 termValue;
    value = 0.0;
    bool res = zv_termList.at(termIndex)->zp_calcTermVariablePart(spectrum, termValue);
    if(res)
    {
        value = static_cast<qreal>(termValue);
        res = zv_termNormalizer->zp_calcNormalizedValue(spectrum, value);
    }

    return res;
}
//=========================================================
void ZCalibration::zh_onTermNameChange() const
{
    ZAbstractTerm* term = qobject_cast<ZAbstractTerm*>(sender());
    if(term == 0)
    {
        return;
    }

    int termIndex = zp_termIndex(term);

    emit zg_termOperation(TOT_TERM_NAME_CHANGED, termIndex, termIndex);
}
//=========================================================
void ZCalibration::zh_onTermWindowMarginChange()
{
    ZAbstractTerm* term = qobject_cast<ZAbstractTerm*>(sender());
    if(!term)
    {
        return;
    }
    int termIndex = zp_termIndex(term);
    emit zg_termOperation(TOT_TERM_VALUE_CHANGED, termIndex, termIndex);
}
//=========================================================
void ZCalibration::zh_onTermAverageValueChange() const
{

}
//=========================================================
void ZCalibration::zh_removeTerm(ZAbstractTerm* term)
{
    for(int i = 0; i < zv_termList.count(); i ++)
    {
        if(zv_termList.at(i) == term)
        {
            emit zg_termOperation(TOT_BEGIN_REMOVE_TERM, i, i);
            delete zv_termList.takeAt(i);
            emit zg_termOperation(TOT_END_REMOVE_TERM, i, i);

            return;
        }
    }
}
//=========================================================
bool ZCalibration::zh_isWindowExist(const QString& windowName)
{
    for(int i = 0; i < zv_windowList.count(); i++)
    {
        if(zv_windowList.at(i)->zp_windowName() == windowName)
        {
            return true;
        }
    }

    return false;
}
//=========================================================
bool ZCalibration::zp_isCalibrationWindowVisible(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return false;
    }

    return zv_windowList.at(windowIndex)->zp_isWindowVisible();
}
//=========================================================
bool ZCalibration::zp_setCalibrationWindowVisible(int windowIndex, bool visibility)
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return false;
    }

    bool res = zv_windowList[windowIndex]->zp_setWindowVisible(visibility);
    if(res)
    {
        emit zg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
    }
    return res;
}
//=========================================================
int ZCalibration::zp_calibrationWindowCount() const
{
    return zv_windowList.count();
}
//=========================================================
QString ZCalibration::zp_calibrationWindowName(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return QString();
    }

    return zv_windowList.at(windowIndex)->zp_windowName();
}
//=========================================================
const ZCalibrationWindow* ZCalibration::zp_calibrationWindow(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return 0;
    }

    return zv_windowList.at(windowIndex);
}
//=========================================================
bool ZCalibration::zp_setCalibrationWindowName(int windowIndex, const QString& name)
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return false;
    }

    bool res = zv_windowList[windowIndex]->zp_setWindowName(name);
    if(res)
    {
        emit zg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
    }
    return res;
}
//=========================================================
ZCalibrationWindow::WindowType ZCalibration::zp_calibrationWindowType(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return ZCalibrationWindow::WT_NOT_DEFINED;
    }

    return zv_windowList.at(windowIndex)->zp_type();
}
//=========================================================
bool ZCalibration::zp_setCalibrationWindowType(int windowIndex, ZCalibrationWindow::WindowType type)
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return false;
    }

    bool res = zv_windowList[windowIndex]->zp_setWindowType(type);
    if(res)
    {
        emit zg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
        if(type == ZCalibrationWindow::WT_BASE_PEAK || type == ZCalibrationWindow::WT_PEAK)
        {
            zh_createTermsForWindow(zv_windowList.at(windowIndex));
        }
    }
    return res;
}
//=========================================================
int ZCalibration::zp_calibrationWindowFirstChannel(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return 0;
    }

    return zv_windowList.at(windowIndex)->zp_firstChannel();
}
//=========================================================
bool ZCalibration::zp_setCalibrationWindowFirstChannel(int windowIndex, int channel)
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return 0;
    }

    bool res = zv_windowList[windowIndex]->zp_setWindowFirstChannel(channel);
    if(res)
    {
        emit zg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
    }
    return res;
}
//=========================================================
int ZCalibration::zp_calibrationWindowLastChannel(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return 0;
    }

    return zv_windowList.at(windowIndex)->zp_lastChannel();
}
//=========================================================
bool ZCalibration::zp_setCalibrationWindowLastChannel(int windowIndex, int channel)
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return 0;
    }

    bool res = zv_windowList[windowIndex]->zp_setWindowLastChannel(channel);
    if(res)
    {
        emit zg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
    }
    return res;
}
//=========================================================
qint64 ZCalibration::zp_calibrationWindowId(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return 0.0;
    }

    return zv_windowList.at(windowIndex)->zp_windowId();
}
//=========================================================
bool ZCalibration::zp_removeCalibrationWindow(int windowIndex)
{
    if(windowIndex < 0 || windowIndex >= zv_windowList.count())
    {
        return false;
    }

    emit zg_windowOperation(WOT_BEGIN_REMOVE_WINDOWS, windowIndex, windowIndex);
    delete zv_windowList.takeAt(windowIndex);
    emit zg_windowOperation(WOT_END_REMOVE_WINDOWS, windowIndex, windowIndex);

    return true;
}
//=========================================================
int ZCalibration::zp_termCount() const
{
    return zv_termList.count();
}
//=========================================================
QString ZCalibration::zp_termName(int termIndex) const
{
    if(termIndex < 0 || termIndex >= zv_termList.count() )
    {
        return QString();
    }

    return zv_termList.at(termIndex)->zp_termName();
}
//=========================================================
ZAbstractTerm::TermState ZCalibration::zp_termState(int termIndex) const
{
    if(termIndex < 0 || termIndex >= zv_termList.count() )
    {
        return ZAbstractTerm::TS_CONST_EXCLUDED;
    }

    return zv_termList.at(termIndex)->zp_termState();
}
//=========================================================
void ZCalibration::zp_setNextUsersTermState(int termIndex) const
{
    if(termIndex < 0 || termIndex >= zv_termList.count() )
    {
        return;
    }

    ZAbstractTerm::TermState state = zv_termList.at(termIndex)->zp_termState();
    bool res;
    switch(state)
    {
    case ZAbstractTerm::TS_CONST_INCLUDED:
        res = zv_termList.at(termIndex)->zp_setTermState(ZAbstractTerm::TS_EXAM_WAITING);
        break;
    case ZAbstractTerm::TS_EXAM_WAITING:
        res = zv_termList.at(termIndex)->zp_setTermState(ZAbstractTerm::TS_CONST_EXCLUDED);
        break;
    case ZAbstractTerm::TS_INCLUDED:
        res = zv_termList.at(termIndex)->zp_setTermState(ZAbstractTerm::TS_CONST_EXCLUDED);
        break;
    case ZAbstractTerm::TS_EXCEPTED:
        res = zv_termList.at(termIndex)->zp_setTermState(ZAbstractTerm::TS_CONST_EXCLUDED);
        break;
    case ZAbstractTerm::TS_CONST_EXCLUDED:
        res =zv_termList.at(termIndex)->zp_setTermState(ZAbstractTerm::TS_CONST_INCLUDED);
        break;
    }

    if(res)
    {
        emit zg_termOperation(TOT_TERM_STATE_CHANGED, termIndex, termIndex);
    }
}
//=========================================================
bool ZCalibration::checkColor(QColor color)
{
    if(!color.isValid() || zv_colorList.contains(color))
    {
        return false;
    }

    int colorSumm = color.red() + color.green() + color.blue();
    int averageVol = colorSumm / 3;

    if(colorSumm > 450)
    {
        return false;
    }

    if(abs(averageVol - color.red()) + abs(averageVol - color.green()) + abs(averageVol - color.blue()) < 90)
    {
        return false;
    }

    return true;
}
//======================================================
QList<QColor> ZCalibration::zp_createColorList()
{
    int red;
    int green;
    int blue;
    QColor color;

    QList<QColor> colorList;

    while(colorList.count() < 1500)
    {
        qsrand(QDateTime::currentMSecsSinceEpoch() + 500);
        red = ((qrand() % 24) * 10) + 15;
        green = ((qrand() % 24) * 10) + 15;
        blue = ((qrand() % 24) * 10) +15;

        color = QColor(red, green, blue);
        if(ZCalibration::checkColor(color))
        {
            colorList << color;
        }
    }
    return colorList;
}
//======================================================

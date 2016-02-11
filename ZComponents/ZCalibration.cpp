//=========================================================
#include "ZCalibration.h"
#include "ZAbstractSpectrum.h"
#include "ZSimpleTerm.h"
#include "ZQuadraticTerm.h"
#include "ZCrossProductTerm.h"
#include "ZCustomTerm.h"
#include "ZTermNormalizer.h"

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
double ZCalibration::zp_calcConcentration(const ZAbstractSpectrum* const spectrum, bool *ok)
{
    if(spectrum == 0)
    {
        if(ok != 0)
        {
            *ok = false;
            return 0;
        }
    }

    * ok = true;
    return 0;
}
//=========================================================
void ZCalibration::zp_createNewCalibrationWindow(int& windowNewIndex, int firstChannel, int lastChannel)
{
    int nextWindowIndex = zv_spectrumWindowList.count();
    QString nextName = zv_defaultWindowName + QString::number(++nextWindowIndex);

    while(zh_isWindowExist(nextName))
    {
        nextName = zv_defaultWindowName + QString::number(++nextWindowIndex);
    }

    windowNewIndex = zv_spectrumWindowList.count();
    ZCalibrationWindow* calibrationWindow = new ZCalibrationWindow(nextName,
                                                                   ZCalibrationWindow::WT_NOT_DEFINED,
                                                                   firstChannel,
                                                                   lastChannel,
                                                                   this);

    emit zg_windowOperation(WOT_BRGIN_INSERT_WINDOWS, windowNewIndex, windowNewIndex);
    zv_spectrumWindowList.append(calibrationWindow);
    emit zg_windowOperation(WOT_END_INSERT_WINDOWS, windowNewIndex, windowNewIndex);

    zh_createTermsForWindow(calibrationWindow);
}
//=========================================================
void ZCalibration::zh_createTermsForWindow(ZCalibrationWindow* window)
{
    // creation
    ZSimpleTerm* simpleTerm = new ZSimpleTerm(window, this);

    ZQuadraticTerm* quadraticTerm = new ZQuadraticTerm(window, this);
    // indexes by default (if list of terms is empty)
    int quadraticIndex = 1;
    int simpleIndex = 0;

    // indexes to insertion before
    for(int i = 0; i < zv_termList.count(); i++)
    {
        if(zv_termList.at(i)->zp_termType() == ZAbstractTerm::PT_SIMPLE)
        {
            simpleIndex = i + 1;
        }

        if(zv_termList.at(i)->zp_termType() == ZAbstractTerm::PT_QUADRATIC)
        {
            quadraticIndex = i + 1;
        }
    }

    // insertion
    emit zg_termOperation(TOT_BEGIN_INSERT_TERM, quadraticIndex, quadraticIndex);
    zv_termList.insert(quadraticIndex, quadraticTerm );
    emit zg_termOperation(TOT_END_INSERT_TERM, quadraticIndex, quadraticIndex);

    emit zg_termOperation(TOT_BEGIN_INSERT_TERM, simpleIndex, simpleIndex);
    zv_termList.insert(simpleIndex, simpleTerm);
    emit zg_termOperation(TOT_END_INSERT_TERM, simpleIndex, simpleIndex);

}
//=========================================================
void ZCalibration::zp_isNormalizerValid(bool& validFlag) const
{
   zv_termNormalizator->zp_isValid(validFlag);
}
//=========================================================
void ZCalibration::zp_normalizerValue(qreal& value) const
{
   zv_termNormalizator->zp_value(value);
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
qreal ZCalibration::zp_termFactor(int termIndex, bool* ok) const
{
    if(termIndex < 0 || termIndex >= zv_termList.count() )
    {
        if(ok)
        {
            *ok = false;
        }

        return 0;
    }

    if(ok)
    {
        *ok = true;
    }

    return zv_termList.at(termIndex)->zp_termFactor();
}
//=========================================================
void ZCalibration::zp_onTermNameChange() const
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
    for(int i = 0; i < zv_spectrumWindowList.count(); i++)
    {
        if(zv_spectrumWindowList.at(i)->zp_windowName() == windowName)
        {
            return true;
        }
    }

    return false;
}
//=========================================================
bool ZCalibration::zp_isCalibrationWindowVisible(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return false;
    }

    return zv_spectrumWindowList.at(windowIndex)->zp_isWindowVisible();
}
//=========================================================
bool ZCalibration::zp_setCalibrationWindowVisible(int windowIndex, bool visibility)
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return false;
    }

    bool res = zv_spectrumWindowList[windowIndex]->zp_setWindowVisible(visibility);
    if(res)
    {
        emit zg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
    }
    return res;
}
//=========================================================
int ZCalibration::zp_calibrationWindowCount() const
{
    return zv_spectrumWindowList.count();
}
//=========================================================
QString ZCalibration::zp_calibrationWindowName(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return QString();
    }

    return zv_spectrumWindowList.at(windowIndex)->zp_windowName();
}
//=========================================================
const ZCalibrationWindow* ZCalibration::zp_calibrationWindow(int windowIndex) const
{
   if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
   {
       return 0;
   }

   return zv_spectrumWindowList.at(windowIndex);
}
//=========================================================
bool ZCalibration::zp_setCalibrationWindowName(int windowIndex, const QString& name)
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return false;
    }

    bool res = zv_spectrumWindowList[windowIndex]->zp_setWindowName(name);
    if(res)
    {
        emit zg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
    }
    return res;
}
//=========================================================
ZCalibrationWindow::WindowType ZCalibration::zp_calibrationWindowType(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return ZCalibrationWindow::WT_NOT_DEFINED;
    }

    return zv_spectrumWindowList.at(windowIndex)->zp_type();
}
//=========================================================
bool ZCalibration::zp_setCalibrationWindowType(int windowIndex, ZCalibrationWindow::WindowType type)
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return false;
    }

    bool res = zv_spectrumWindowList[windowIndex]->zp_setWindowType(type);
    if(res)
    {
        emit zg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
    }
    return res;
}
//=========================================================
int ZCalibration::zp_calibrationWindowFirstChannel(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return 0;
    }

    return zv_spectrumWindowList.at(windowIndex)->zp_firstChannel();
}
//=========================================================
bool ZCalibration::zp_setCalibrationWindowFirstChannel(int windowIndex, int channel)
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return 0;
    }

    bool res = zv_spectrumWindowList[windowIndex]->zp_setWindowFirstChannel(channel);
    if(res)
    {
        emit zg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
    }
    return res;
}
//=========================================================
int ZCalibration::zp_calibrationWindowLastChannel(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return 0;
    }

    return zv_spectrumWindowList.at(windowIndex)->zp_lastChannel();
}
//=========================================================
bool ZCalibration::zp_setCalibrationWindowLastChannel(int windowIndex, int channel)
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return 0;
    }

    bool res = zv_spectrumWindowList[windowIndex]->zp_setWindowLastChannel(channel);
    if(res)
    {
        emit zg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
    }
    return res;
}
//=========================================================
qint64 ZCalibration::zp_calibrationWindowId(int windowIndex) const
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return 0.0;
    }

    return zv_spectrumWindowList.at(windowIndex)->zp_windowId();
}
//=========================================================
bool ZCalibration::zp_removeSpectrumWindow(int windowIndex)
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return false;
    }

    emit zg_windowOperation(WOT_BEGIN_REMOVE_WINDOWS, windowIndex, windowIndex);
    delete zv_spectrumWindowList.takeAt(windowIndex);
    emit zg_windowOperation(WOT_END_REMOVE_WINDOWS, windowIndex, windowIndex);

    return true;
}
//=========================================================
void ZCalibration::zp_connectTermToCalibration(const ZAbstractTerm* term)
{
   if(term == 0)
   {
      return;
   }
   connect(term, &ZAbstractTerm::zg_requestForDelete,
           this, &ZCalibration::zh_removeTerm);
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
bool ZCalibration::checkColor(QColor color)
{
    if(!color.isValid() || zv_colorList.contains(color))
    {
        return false;
    }

    //    if(color.red() == 255 &&  color.green() < 10 && color.blue() < 10  ||
    //            color.red() < 10 &&  color.green() == 255 && color.blue() < 10  ||
    //            color.red() < 10 &&  color.green() < 10 && color.blue() == 255  )
    //    {
    //        return false;
    //    }

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

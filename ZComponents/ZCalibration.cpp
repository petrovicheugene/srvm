//=========================================================
#include "ZCalibration.h"
#include "ZAbstractSpectrum.h"
#include <QFileInfo>
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
void ZCalibration::zp_createNewSpectrumWindow()
{
    int nextWindowIndex = zv_spectrumWindowList.count();
    QString nextName = zv_defaultWindowName + QString::number(++nextWindowIndex);

    while(zh_isWindowExist(nextName))
    {
        nextName = zv_defaultWindowName + QString::number(++nextWindowIndex);
    }

    int windowNewIndex = zv_spectrumWindowList.count();
    emit zg_windowOperation(WOT_INSERT_WINDOWS, windowNewIndex, windowNewIndex);
    zv_spectrumWindowList.append(ZCalibrationWindow(nextName));
    emit zg_windowOperation(WOT_END_INSERT_WINDOWS, windowNewIndex, windowNewIndex);
}
//=========================================================
bool ZCalibration::zh_isWindowExist(const QString& windowName)
{
    for(int i = 0; i < zv_spectrumWindowList.count(); i++)
    {
        if(zv_spectrumWindowList.at(i).zp_name() == windowName)
        {
            return true;
        }
    }

    return false;
}
//=========================================================
bool ZCalibration::zp_isSpectrumWindowVisible(int windowIndex)
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return false;
    }

    return zv_spectrumWindowList.at(windowIndex).zp_isWindowVisible();
}
//=========================================================
int ZCalibration::zp_spectrumWindowCount()
{
    return zv_spectrumWindowList.count();
}
//=========================================================
QString ZCalibration::zp_spectrumWindowName(int windowIndex)
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return QString();
    }

    return zv_spectrumWindowList.at(windowIndex).zp_name();
}
//=========================================================
ZCalibrationWindow::WindowType ZCalibration::zp_spectrumWindowType(int windowIndex)
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return ZCalibrationWindow::WT_NOT_DEFINED;
    }

    return zv_spectrumWindowList.at(windowIndex).zp_type();
}
//=========================================================
int ZCalibration::zp_spectrumWindowFirstChannel(int windowIndex)
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return 0;
    }

    return zv_spectrumWindowList.at(windowIndex).zp_firstChannel();
}
//=========================================================
int ZCalibration::zp_spectrumWindowLastChannel(int windowIndex)
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return 0;
    }

    return zv_spectrumWindowList.at(windowIndex).zp_lastChannel();
}
//=========================================================
qint64 ZCalibration::zp_spectrumWindowId(int windowIndex)
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return 0.0;
    }

    return zv_spectrumWindowList.at(windowIndex).zp_windowId();
}
//=========================================================
bool ZCalibration::zp_removeSpectrumWindow(int windowIndex)
{
    if(windowIndex < 0 || windowIndex >= zv_spectrumWindowList.count())
    {
        return false;
    }

    emit zg_windowOperation(WOT_REMOVE_WINDOWS, windowIndex, windowIndex);
    zv_spectrumWindowList.removeAt(windowIndex);
    emit zg_windowOperation(WOT_END_REMOVE_WINDOWS, windowIndex, windowIndex);
    return true;
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

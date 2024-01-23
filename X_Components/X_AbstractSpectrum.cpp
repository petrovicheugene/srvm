//==========================================================
#include "X_AbstractSpectrum.h"
#include <QFileInfo>
#include "X_CalibrationWindow.h"
#include "X_LocaleDoubleConverter.h"
//==========================================================
qint64 X_AbstractSpectrum::xv_lastSpectrumId = 0;
//==========================================================
X_AbstractSpectrum::X_AbstractSpectrum(const QList<quint32> &intensityList,
                                     const QString& path, QColor color, bool completed, QObject *parent)
    : QObject(parent)
{
    xv_visible = true;
    xv_path = path;
    xv_color = color;
    xv_spectrumId = xv_lastSpectrumId++;
    xv_checked = true;
    xv_completed = completed;
    xv_maxIntensity = 15;

    QFileInfo fileInfo(path);
    QString suffix = fileInfo.suffix();
    if(suffix == "spe")
    {
        xv_type = ST_SPE;
    }
    else
    {
        xv_type = ST_NA;
    }

    xv_name = fileInfo.fileName();
    xv_spectrumData = intensityList;
    xv_channelCount = intensityList.count();
    xv_maxIntensity = 0;
    for(int i = 0; i < xv_channelCount; i++)
    {
        if(xv_maxIntensity < intensityList.value(i))
        {
            xv_maxIntensity = intensityList.value(i);
        }
    }

    // xh_recalcSpectrumPainterPath();
}
//==========================================================
X_AbstractSpectrum::~X_AbstractSpectrum()
{

}
//==========================================================
X_AbstractSpectrum::SpectrumType X_AbstractSpectrum::xp_type() const
{
    return xv_type;
}
//==========================================================
bool X_AbstractSpectrum::xp_isSpectrumVisible() const
{
    return xv_visible;
}
//==========================================================
void X_AbstractSpectrum::xp_setSpectrumVisible(bool visible)
{
    xv_visible = visible;
}
//==========================================================
bool X_AbstractSpectrum::xp_isSpectrumChecked() const
{
    return xv_checked;
}
//==========================================================
bool X_AbstractSpectrum::xp_setSpectrumChecked(bool checked)
{
    if(xv_checked == checked)
    {
        return false;
    }

    xv_checked = checked;
    return true;
}
//==========================================================
void X_AbstractSpectrum::xp_insertConcentration(qint64 chemElementId, const QString& concentration)
{
    xv_concentrationMap.insert(chemElementId, concentration);
}
//==========================================================
bool X_AbstractSpectrum::xp_removeConcentration(qint64 chemElementId)
{
    if(xv_concentrationMap.remove(chemElementId) > 0)
    {
        return true;
    }

    return false;
}
//==========================================================
QString X_AbstractSpectrum::xp_concentrationString(qint64 chemElementId) const
{
    if(!xv_concentrationMap.keys().contains(chemElementId))
    {
        return QString();
    }

    return xv_concentrationMap.value(chemElementId);
}
//==========================================================
qreal X_AbstractSpectrum::xp_concentrationValue(qint64 chemElementId) const
{
    QString concentrationString =  xp_concentrationString(chemElementId);
    qreal concentration = 0.0;
    bool ok;
    if(concentrationString.isEmpty())
    {
        concentration = 0.0;
    }
    else
    {
        concentration = X_LocaleDoubleConverter::toDouble(concentrationString, &ok);

        if(!ok)
        {
            concentration = 0.0;
        }
    }
    return concentration;
}
//==========================================================
bool X_AbstractSpectrum::xp_setConcentration(qint64 chemElementId,
                                            const QString& concentration)
{
    if(chemElementId < 0)
    {
        return false;
    }

    if(xv_concentrationMap.contains(chemElementId)
            && ( xv_concentrationMap.value(chemElementId) == concentration))
    {
        return false;
    }

    xv_concentrationMap.insert(chemElementId, concentration);
    return true;
}
//==========================================================
QList<quint32> X_AbstractSpectrum::xp_spectrumData() const
{
    return xv_spectrumData;
}
//==========================================================
void X_AbstractSpectrum::xp_setSpectrumData(QList<quint32> data)
{
    xv_spectrumData = data;
    xv_channelCount = data.count();
    xv_maxIntensity = 0;
    for(int i = 0; i < xv_channelCount; i++)
    {
        if(xv_maxIntensity < data.value(i))
        {
            xv_maxIntensity = data.value(i);
        }
    }

    xh_recalcSpectrumPainterPath();
}
//==========================================================
QString X_AbstractSpectrum::xp_name() const
{
    return xv_name;
}
//==========================================================
void X_AbstractSpectrum::xp_setSpectrumName(const QString& name)
{
    xv_name = name;
}
//==========================================================
QString X_AbstractSpectrum::xp_path() const
{
    return xv_path;
}
//==========================================================
bool X_AbstractSpectrum::xp_intensityInWindow(int startChannel, int lastChannel, qreal& intensity) const
{
    if(startChannel < 0)
    {
        startChannel = 0;
    }
    else if(startChannel > xv_spectrumData.count())
    {
        startChannel = xv_spectrumData.count() - 1;
    }

    if(lastChannel < 0)
    {
        lastChannel = 0;
    }
    else if(lastChannel > xv_spectrumData.count())
    {
        lastChannel = xv_spectrumData.count() - 1;
    }

//    if(startChannel == lastChannel)
//    {
//        return false;
//    }

    if(startChannel > lastChannel)
    {
        qSwap(startChannel, lastChannel);
    }

    intensity = 0.0;
    for(int c = startChannel; c <= lastChannel; c++)
    {
        intensity += static_cast<qreal>(xv_spectrumData.value(c));
    }

    return true;
}
//==========================================================
bool X_AbstractSpectrum::xp_intensityInWindow(const X_CalibrationWindow* window, qreal &intensity) const
{
    if(!window)
    {
        return false;
    }

    return xp_intensityInWindow(window->xp_firstChannel(), window->xp_lastChannel(),  intensity);
}
//==========================================================
int X_AbstractSpectrum::xp_channelCount() const
{
    return xv_channelCount;
}
//==========================================================
int X_AbstractSpectrum::xp_maxIntensity() const
{
    return xv_maxIntensity;
}
//==========================================================
QColor X_AbstractSpectrum::xp_color() const
{
    return xv_color;
}
//==========================================================
void X_AbstractSpectrum::xp_setColor(QColor color)
{
    xv_color = color;
}
//==========================================================
qint64 X_AbstractSpectrum::xp_spectrumId() const
{
    return xv_spectrumId;
}
//==========================================================
QPainterPath X_AbstractSpectrum::xp_spectrumPainterPath() const
{
    return xv_spectrumPaintPath;
}
//==========================================================
void X_AbstractSpectrum::xp_setCompleted(bool completed)
{
    xv_completed = completed;
}
//==========================================================
bool X_AbstractSpectrum::xp_isCompleted() const
{
    return xv_completed;
}
//==========================================================
void X_AbstractSpectrum::xh_recalcSpectrumPainterPath()
{
    QPointF point(0.0, 0.0);
    QPainterPath newShape;
    newShape.moveTo(point);

    qreal scValue = 0.0;
    for(int i = 0; i < xv_spectrumData.count(); i++)
    {
        scValue = (qreal)(xv_spectrumData.value(i));
        point = QPointF((qreal)i,  scValue*-1);
        newShape.lineTo(point);
        point = QPointF((qreal)(i+1),  scValue*-1);
        newShape.lineTo(point);
    }

    // last point
    point = QPointF((qreal)(xv_spectrumData.count() - 1),  0);
    newShape.lineTo(point);
    xv_spectrumPaintPath = newShape;
}
//==========================================================

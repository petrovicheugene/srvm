//=========================================================
#include "ZAbstractCalibration.h"
#include <QFileInfo>
//=========================================================
const QMap<ZAbstractCalibration::CalibrationType, QString> ZAbstractCalibration::suffixMap(ZAbstractCalibration::zp_createSuffixMap());
//=========================================================
QMap<ZAbstractCalibration::CalibrationType, QString> ZAbstractCalibration::zp_createSuffixMap()
{
    QMap<ZAbstractCalibration::CalibrationType, QString> map;
    map.insert(ZAbstractCalibration::CT_NA, QString());
    map.insert(ZAbstractCalibration::CT_SD, QString("clbr"));
    map.insert(ZAbstractCalibration::CT_XML, QString("clbx"));
    return map;
}
//=========================================================
ZAbstractCalibration::ZAbstractCalibration(const QString& path, QObject *parent) : QObject(parent)
{
    zv_visibility = false;
    QFileInfo fileInfo(path);
    if(!fileInfo.absoluteFilePath().isEmpty())
    {
        QString suffix = fileInfo.suffix();

        if(!suffixMap.values().contains(suffix))
        {
            zv_type = CT_NA;
        }
        else
        {
            zv_type = suffixMap.key(suffix);
        }

        zv_baseName = fileInfo.baseName();
        zv_path = fileInfo.absolutePath();
        zv_dirty = false;
    }
    else
    {
        zv_type = CT_NA;
        zv_path = QString();
        zv_baseName = QString();
        zv_dirty = true;
    }
}
//=========================================================
ZAbstractCalibration::~ZAbstractCalibration()
{

}
//=========================================================
QString ZAbstractCalibration::zp_name() const
{
    QString name = zv_baseName+"."+suffixMap.value(zv_type);
    return name;
}
//=========================================================
void ZAbstractCalibration::zp_setBaseName(const QString& name)
{
    if(name == zv_baseName || name.isEmpty())
    {
        return;
    }

    zv_baseName = name;
    zv_dirty = false;
}
//=========================================================
QString ZAbstractCalibration::zp_path() const
{
    return zv_path;
}
//=========================================================
void ZAbstractCalibration::zp_setPath(const QString& path)
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
ZAbstractCalibration::CalibrationType ZAbstractCalibration::zp_type() const
{
    return zv_type;
}
//=========================================================
bool ZAbstractCalibration::zp_isVisible() const
{
    return zv_visibility;
}
//=========================================================
void ZAbstractCalibration::zp_setVisible(bool visibility)
{
    if(visibility != zv_visibility)
    {
        zv_visibility = visibility;
        emit zg_checkedChanged(zv_visibility);
    }
}
//=========================================================
bool ZAbstractCalibration::zp_isDirty() const
{
    return zv_dirty;
}
//=========================================================
QString ZAbstractCalibration::zp_suffix() const
{
    QString suffix = suffixMap.value(zv_type);
    return suffix;
}
//=========================================================

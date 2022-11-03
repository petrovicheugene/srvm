//======================================================
#include "X_EnergyLineSetItem.h"
#include <QApplication>
//======================================================
const QString X_EnergyLineSetItem::xv_settingsSectionName("EnergyLineVisibility");
//======================================================
X_EnergyLineProperties::X_EnergyLineProperties()
{
    xv_energyLineValue = QString();
    xv_visibility = false;
}
//======================================================
X_EnergyLineProperties::X_EnergyLineProperties(const QString& energyLineValue)
{
    xv_energyLineValue = energyLineValue;
    xv_visibility = false;
}
//======================================================
X_EnergyLineProperties::X_EnergyLineProperties(const X_EnergyLineProperties& src)
{
    xv_energyLineValue = src.xv_energyLineValue;
    xv_visibility = src.xv_visibility;
    xv_color = src.xv_color;
}
//======================================================
X_EnergyLineProperties::X_EnergyLineProperties(const QString& energyLineValue, QColor color)
{
    xv_energyLineValue = energyLineValue;
    xv_color = color;
    xv_visibility = false;
}
//======================================================
X_EnergyLineProperties::X_EnergyLineProperties(const QString& energyLineValue,
                      const QString& energyLineRelativeIntensity,
                      QColor color)
{
    xv_energyLineValue = energyLineValue;
    xv_energyLineRelativeIntensity = energyLineRelativeIntensity;
    xv_color = color;
    xv_visibility = false;
}
//======================================================
QString X_EnergyLineProperties::xp_energyLineValue() const
{
    return xv_energyLineValue;
}
//======================================================
QString X_EnergyLineProperties::xp_energyLineRelativeIntensity() const
{
    return xv_energyLineRelativeIntensity;
}
//======================================================
void X_EnergyLineProperties::xp_setEnergyLineRelativeIntensity(const QString relativeIntensity)
{
    xv_energyLineRelativeIntensity = relativeIntensity;
}
//======================================================
bool X_EnergyLineProperties::xp_isVisible() const
{
    return xv_visibility;
}
//======================================================
void X_EnergyLineProperties::xp_setVisible(bool visible)
{
    xv_visibility = visible;
}
//======================================================
QColor X_EnergyLineProperties::xp_color() const
{
    return xv_color;
}
//======================================================
void X_EnergyLineProperties::xp_setColor(QColor color)
{
    xv_color = color;
}
//======================================================
// END X_EnergyLineProperties
//======================================================
X_EnergyLineSetItem::X_EnergyLineSetItem(int X_Number,
                                       const QString &symbol,
                                       QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<PropertyList>("PropertyList");

    xv_X_Number = X_Number;
    xv_symbol = symbol;
}
//======================================================
X_EnergyLineSetItem::~X_EnergyLineSetItem()
{
    xh_saveVisibility();

    for(auto& key : xv_energyLinePropertyMap.keys())
    {
        emit xg_energyLineOperation(xv_symbol, key, EL_REMOVED);
    }
}
//======================================================
void X_EnergyLineSetItem::xp_createEnergyLines(const PropertyList &propertyList)
{
    QColor color;
    for(auto& property : propertyList)
    {
        if(property.first.contains(QChar('K'), Qt::CaseSensitive))
        {
            color = QColor(Qt::blue);
        }
        else if(property.first.contains(QChar('L'), Qt::CaseSensitive))
        {
            color = QColor(Qt::green);
        }
        else if(property.first.contains(QChar('M'), Qt::CaseSensitive))
        {
            color = QColor(Qt::red);
        }
        else
        {
            color = QColor(Qt::magenta);
        }

        xv_energyLinePropertyMap.insert(property.first, X_EnergyLineProperties(property.second, color));

        emit xg_energyLineOperation(xv_symbol, property.first, EL_INSERTED);
    }

    xh_restoreVisibility();
}
//======================================================
void X_EnergyLineSetItem::xp_loadRelativeIntensity(const PropertyList &propertyList)
{
    for(auto& property : propertyList)
    {
        if(!xv_energyLinePropertyMap.keys().contains(property.first))
        {
            continue;
        }
        xv_energyLinePropertyMap[property.first].xp_setEnergyLineRelativeIntensity(property.second);
        emit xg_energyLineOperation(xv_symbol, property.first, EL_PROPERTY_CHANGED);
    }
}
//======================================================
int X_EnergyLineSetItem::xp_X_Number() const
{
    return xv_X_Number;
}
//======================================================
QString X_EnergyLineSetItem::xp_symbol() const
{
    return xv_symbol;
}
//======================================================
bool X_EnergyLineSetItem::xp_energyLineValue(const QString& lineName,
                                            QString& value) const
{
    if(!xv_energyLinePropertyMap.keys().contains(lineName))
    {
        return false;
    }

    value = xv_energyLinePropertyMap[lineName].xp_energyLineValue();
    return true;
}
//======================================================
bool X_EnergyLineSetItem::xp_energyLineRelativeIntensity(const QString& lineName, QString &value) const
{
    if(!xv_energyLinePropertyMap.keys().contains(lineName))
    {
        return false;
    }

    value = xv_energyLinePropertyMap[lineName].xp_energyLineRelativeIntensity();
    return true;
}
//======================================================
bool X_EnergyLineSetItem::xp_isEnergyLineVisible(const QString& lineName, bool& visibility) const
{
    if(!xv_energyLinePropertyMap.keys().contains(lineName))
    {
        return false;
    }

    visibility = xv_energyLinePropertyMap[lineName].xp_isVisible();
    return true;
}
//======================================================
bool X_EnergyLineSetItem::xp_energyLineColor(const QString& lineName, QColor &color) const
{
    if(!xv_energyLinePropertyMap.keys().contains(lineName))
    {
        return false;
    }

    color = xv_energyLinePropertyMap[lineName].xp_color();
    return true;
}
//======================================================
bool X_EnergyLineSetItem::xp_setEnergyLineVisible(const QString& lineName, bool visibility)
{
    if(!xv_energyLinePropertyMap.keys().contains(lineName))
    {
        return false;
    }

    xv_energyLinePropertyMap[lineName].xp_setVisible(visibility);

    emit xg_energyLineOperation(xv_symbol, lineName, EL_VISIBILITY_CHANGED);

    return true;
}
//======================================================
int X_EnergyLineSetItem::xp_energyLineCount() const
{
    return xv_energyLinePropertyMap.count();
}
//======================================================
QStringList X_EnergyLineSetItem::xp_energyLineNameStringList() const
{
    QStringList energyLineNameStringList = xv_energyLinePropertyMap.keys();
    return energyLineNameStringList;
}
//======================================================
void X_EnergyLineSetItem::xh_restoreVisibility()
{
    QSettings settings;
    xh_openSettingsGroup(settings);
    QStringList keys = settings.childKeys();
    QVariant vData;
    xv_energyLinePropertyMap.keys();
    foreach(QString key, keys)
    {
        vData = settings.value(key);
        if(!xv_energyLinePropertyMap.keys().contains(key) || !vData.isValid() || !vData.canConvert<bool>())
        {
            continue;
        }

        xv_energyLinePropertyMap[key].xp_setVisible(vData.toBool());
        emit xg_energyLineOperation(xv_symbol, key, EL_VISIBILITY_CHANGED);

    }
    xh_closeSettingsGroup(settings);
}
//======================================================
void X_EnergyLineSetItem::xh_saveVisibility()
{
    QSettings settings;
    xh_openSettingsGroup(settings);

    QMap<QString, X_EnergyLineProperties>::const_iterator it;
    for(it = xv_energyLinePropertyMap.begin(); it != xv_energyLinePropertyMap.end(); it++)
    {
        settings.setValue(it.key(), QVariant(it.value().xp_isVisible()));
    }

    xh_closeSettingsGroup(settings);
}
//======================================================
void X_EnergyLineSetItem::xh_openSettingsGroup(QSettings& settings)
{
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup(xv_settingsSectionName);
    settings.beginGroup(QString::number(xv_X_Number));
}
//======================================================
void X_EnergyLineSetItem::xh_closeSettingsGroup(QSettings& settings)
{
    while(settings.group().isEmpty())
    {
        settings.endGroup();
    }
}
//======================================================

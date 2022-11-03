//======================================================
#ifndef X_ENERGYLINESETITEM_H
#define X_ENERGYLINESETITEM_H
//======================================================
#include <QObject>

#include <QColor>
#include <QList>
#include <QMap>
#include <QPair>
#include <QSettings>
#include <QString>
//======================================================

//======================================================
class X_EnergyLineProperties
{
public:

    X_EnergyLineProperties();
    X_EnergyLineProperties(const QString& energyLineValue);
    X_EnergyLineProperties(const QString& energyLineValue, QColor color);
    X_EnergyLineProperties(const QString& energyLineValue,
                          const QString& energyLineRelativeIntensity,
                          QColor color);

    X_EnergyLineProperties(const X_EnergyLineProperties &);

    QString xp_energyLineValue() const;
    QString xp_energyLineRelativeIntensity() const;
    void xp_setEnergyLineRelativeIntensity(const QString relativeIntensity);
    bool xp_isVisible() const;
    void xp_setVisible(bool visible);
    QColor xp_color() const;
    void xp_setColor(QColor color);

private:

    QString xv_energyLineValue;
    QString xv_energyLineRelativeIntensity;
    bool xv_visibility;
    QColor xv_color;

};
//======================================================
typedef QList<QPair<QString, QString> > PropertyList;
typedef int EnergyLineOperation;
//======================================================
class X_EnergyLineSetItem : public QObject
{
    Q_OBJECT
public:
    enum EnergyLineOperation{EL_REMOVED = 0,
                             EL_INSERTED = 1,
                             EL_VISIBILITY_CHANGED = 2,
                             EL_COLOR_CHANGED = 3,
                             EL_PROPERTY_CHANGED = 4};

    explicit X_EnergyLineSetItem(int X_Number, const QString& symbol,
                                QObject* parent = nullptr);
    ~X_EnergyLineSetItem();

    int xp_X_Number() const;
    QString xp_symbol() const;

    bool xp_energyLineValue(const QString& lineName, QString &value) const;
    bool xp_energyLineRelativeIntensity(const QString& lineName, QString &value) const;
    bool xp_isEnergyLineVisible(const QString& lineName, bool &visibility) const;
    bool xp_energyLineColor(const QString& lineName, QColor &color) const;
    bool xp_setEnergyLineVisible(const QString& lineName, bool visibility);
    int xp_energyLineCount() const;
    QStringList xp_energyLineNameStringList() const;

    void xp_createEnergyLines(const PropertyList& propertyList);
    void xp_loadRelativeIntensity(const PropertyList &propertyList);

signals:

    void xg_energyLineOperation(const QString& lineSymbol,
                                const QString &name,
                                EnergyLineOperation operationType);

private slots:

    void zh_restoreVisibility();
private:

    // VARS
    int xv_X_Number;
    QString xv_symbol;
    QMap<QString, X_EnergyLineProperties> xv_energyLinePropertyMap;
    static const QString xv_settingsSectionName;

    // FUNCS
    void zh_saveVisibility();
    void zh_openSettingsGroup(QSettings& settings);
    void zh_closeSettingsGroup(QSettings&  settings);

};
Q_DECLARE_METATYPE(PropertyList)
//======================================================
#endif // X_ENERGYLINESETITEM_H

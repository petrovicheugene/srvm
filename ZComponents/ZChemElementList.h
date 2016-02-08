//=====================================================
#ifndef ZCHEMELEMENTLIST_H
#define ZCHEMELEMENTLIST_H
//=====================================================
#include <QObject>
#include <QStringList>
#include <QList>

//=====================================================
class ZChemElementItem
{
public:
    ZChemElementItem()
    {
        element = QString();
        visibility = true;
        averageConcentrationValue = 0;
        zv_id = zv_lastId++;
    }

    ZChemElementItem(const QString& name)
    {
        element = name;
        visibility = true;
        averageConcentrationValue = 0;
        zv_id = zv_lastId++;
    }

    QString element;
    bool visibility;
    qreal averageConcentrationValue;

    qint64 zp_chemElementId()
    {
        return zv_id;
    }

private:

    // VARS
    qint64 zv_id;
    static qint64 zv_lastId;

};

//=====================================================
class ZChemElementList : public QObject
{
    Q_OBJECT
public:
    explicit ZChemElementList(QObject *parent = 0);

    // VARS
    enum OperationType {OT_INSERT_CHEM_ELEMENT,
                        OT_END_INSERT_CHEM_ELEMENT,
                        OT_REMOVE_CHEM_ELEMENT,
                        OT_END_REMOVE_CHEM_ELEMENT,
                        OT_CHANGED,
                        OT_VISIBLE_CHANGE,
                        OT_END_VISIBLE_CHANGE,
                        OT_AVERAGE_VALUE_CHANGED};

    bool zp_appendElement(const QString&, qint64 &chemElementId);
    bool zp_appendElement(const QString& element);

    bool zp_removeElement(const QString&);
    void zp_clearList();

    QStringList zp_chemElementList() const;
    bool zp_containsElement(const QString&) const;
    bool zp_containsElementId(qint64) const;

    bool zp_isElementVisible(const QString&) const;
    int zp_findElement(const QString&) const;

    int zp_chemElementCount() const;
    int zp_visibleElementCount() const;
    int zp_numberVisibleChemElementsBeforeIndex(int index) const;
    bool zp_isEmpty() const;

    QString zp_visibleChemElementName(int visibleIndex) const;
    qint64 zp_visibleChemElementId(int visibleIndex) const;
    qint64 zp_chemElementId(int index) const;

    QString zp_chemElementName(int index) const;
    bool zp_setChemElementName(int index, const QString& name);

    bool zp_chemElementIsVisible(int index) const;
    bool zp_setChemElementVisible(int row, bool visible);
    bool zp_setChemElementVisible(const QString&, bool);

    bool zp_setAverageValue(const QString&, qreal);
    qreal zp_averageValue(const QString&) const;
    bool zp_setAverageValue(int, qreal) ;
    qreal zp_averageValue(int) const;

signals:

    void zg_operationType(OperationType, int first, int last);

public slots:

private:

    // VARS
    QList<ZChemElementItem> zv_elementList;
};
//=====================================================
#endif // ZCHEMELEMENTLIST_H

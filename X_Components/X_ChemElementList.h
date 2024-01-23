//=====================================================
#ifndef X_CHEMELEMENTLIST_H
#define X_CHEMELEMENTLIST_H
//=====================================================
#include <QObject>
#include <QStringList>
#include <QList>

//=====================================================
class X_ChemElementItem
{
public:
    X_ChemElementItem()
    {
        element = QString();
        visibility = true;
        // averageConcentrationValue = 0;
        xv_id = xv_lastId++;
    }

    explicit X_ChemElementItem(const QString& name)
    {
        element = name;
        visibility = true;
        // averageConcentrationValue = 0;
        xv_id = xv_lastId++;
    }

    QString element;
    bool visibility;
    // qreal averageConcentrationValue;

    qint64 xp_id() const
    {
        return xv_id;
    }

private:

    // VARS
    qint64 xv_id;
    static qint64 xv_lastId;

};

//=====================================================
class X_ChemElementList : public QObject
{
    Q_OBJECT
public:
    explicit X_ChemElementList(QObject *parent = nullptr);

    // VARS
    enum OperationType {OT_INSERT_CHEM_ELEMENT,
                        OT_END_INSERT_CHEM_ELEMENT,
                        OT_REMOVE_CHEM_ELEMENT,
                        OT_END_REMOVE_CHEM_ELEMENT,
                        OT_CHANGED,
                        OT_VISIBLE_CHANGE,
                        OT_END_VISIBLE_CHANGE,
                        OT_AVERAGE_VALUE_CHANGED};

    bool xp_appendElement(const QString&, qint64 &chemElementId);
    bool xp_removeElement(const QString&, qint64& chemElementId);
    void xp_clearList();

    QStringList xp_chemElementList() const;
    bool xp_containsElement(const QString&) const;
    bool xp_containsElementId(qint64) const;

    bool xp_isElementVisible(const QString&) const;
    int xp_findElement(const QString&) const;

    int xp_chemElementCount() const;
    int xp_visibleElementCount() const;
    int xp_numberVisibleChemElementsBeforeIndex(int index) const;
    bool xp_isEmpty() const;

    QString xp_visibleChemElementName(int visibleIndex) const;
    qint64 xp_visibleChemElementId(int visibleIndex) const;
    qint64 xp_chemElementId(int index) const;

    QString xp_chemElementName(int index) const;
    bool xp_setChemElementName(int index, const QString& name);

    bool xp_chemElementIsVisible(int index) const;
    bool xp_setChemElementVisible(int row, bool visible);
    bool xp_setChemElementVisible(const QString&, bool);

//    bool xp_setAverageChemConcentration(const QString&, qreal);
//    bool xp_averageChemConcentration(const QString&, qreal &value) const;
//    bool xp_setAverageChemConcentration(int, qreal) ;
//    bool xp_averageChemConcentration(int, qreal &value) const;
//    bool xp_setAverageChemConcentrationForChemElementId(qint64, qreal) ;
//    bool xp_averageChemConcentrationForChemElementId(qint64, qreal &value) const;

signals:

    void xg_operationType(OperationType, int first, int last);

public slots:

private:

    // VARS
    QList<X_ChemElementItem> xv_elementList;
};
//=====================================================
#endif // X_CHEMELEMENTLIST_H

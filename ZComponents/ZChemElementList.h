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
    }

    ZChemElementItem(const QString& name)
    {
        element = name;
        visibility = true;
    }

    QString element;
    bool visibility;
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
                        OT_END_VISIBLE_CHANGE};

    bool zp_appendElement(const QString&);
    bool zp_removeElement(const QString&);
    void zp_clearList();

    QStringList zp_chemElementList() const;
    bool zp_containsElement(const QString&) const;
    bool zp_isElementVisible(const QString&) const;
    int zp_findElement(const QString&) const;

    int zp_chemElementCount() const;
    int zp_visibleElementCount() const;
    int zp_numberVisibleChemElementsBeforeIndex(int index) const;

    QString zp_visibleChemElementName(int visibleIndex) const;
    QString zp_chemElementName(int index) const;
    bool zp_setChemElementName(int index, const QString& name);

    bool zp_chemElementIsVisible(int index) const;
    bool zp_setChemElementVisible(int row, bool visible);
    bool zp_setChemElementVisible(const QString&, bool);

signals:

    void zg_operationType(OperationType, int first, int last);

public slots:

private:

    // VARS
    QList<ZChemElementItem> zv_elementList;
};
//=====================================================
#endif // ZCHEMELEMENTLIST_H

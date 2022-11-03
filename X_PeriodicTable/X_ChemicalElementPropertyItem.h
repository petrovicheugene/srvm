//======================================================
#ifndef X_CHEMICALELEMENTPROPERTYITEM_H
#define X_CHEMICALELEMENTPROPERTYITEM_H
//======================================================
#include <QObject>
#include <QList>
#include <QPair>
//======================================================
typedef QList<QStringList> PropertyNames;
//======================================================
class X_ChemicalElementPropertyItem : public QObject
{
    Q_OBJECT
public:
    explicit X_ChemicalElementPropertyItem(QObject* parent = nullptr);
    explicit X_ChemicalElementPropertyItem(const QString& name, QObject* parent = nullptr);

    QString xp_name() const;
    void xp_setName(const QString& name);
    bool xp_createName(const QString& nameTemplate,
                       const QString &separator = " ");

    void xp_addChild(X_ChemicalElementPropertyItem* child);
    QStringList xp_chemicalElementPropertySectionList() const;

    void xp_setChemicalElementProperty(const QString& key, const QString& value);
    QList<QPair<QString, QString> > xp_chemicalElementPropertyList() const;
    bool xp_setChemicalElementPropertyName(int index, const QString& name);
    bool xp_setChemicalElementPropertyValue(int index, const QString& value);
    QString xp_chemicalElementPropertyValue(const QString& key) const;
    QPair<QString, QString>xp_chemicalElementProperty(int index) const;
    void xp_setChemicalElementPropertyList(const QList<QPair<QString, QString> >& propertyList);

    X_ChemicalElementPropertyItem* xp_parent() const;
    X_ChemicalElementPropertyItem* xp_childAtRow(int row) const;
    X_ChemicalElementPropertyItem* xp_childNamed(const QStringList &nameBranch);
    X_ChemicalElementPropertyItem* xp_childNamed(const QString& name);
    void xp_chemicalElementPropertySectionBranch(QStringList& branch);

    int xp_childCount() const;
    bool xp_hasCildren() const;
    void xp_removeChildren();
    int xp_rowOfChildItem(const X_ChemicalElementPropertyItem *child) const;
    void xp_completeChemicalElementPropertyNameList(PropertyNames &propertyNames) const;
    int xp_chemicalElementPropertyCount() const;
    int xh_elementLevelNumber() const;
    int xp_zNumber(const X_ChemicalElementPropertyItem *callingItem) const;
    void xp_totalDescendantList(QList<X_ChemicalElementPropertyItem *> &descendantList) const;

    bool xp_initRemoving();
    void xp_removeChild(X_ChemicalElementPropertyItem* child);
    bool xp_removeChemicalElementProperty(int propertyIndex);
    void xp_removeChemicalElementProperties();
    void xp_clear();

signals:

    void xg_itemAboutToBeInserted(int row, const X_ChemicalElementPropertyItem* parentItem) const;
    void xg_itemInserted(const X_ChemicalElementPropertyItem* item) const;

    void xg_requestItemRemovingPermission(const X_ChemicalElementPropertyItem* item,
                                bool& permission);
    void xg_itemAboutToBeRemoved(const X_ChemicalElementPropertyItem* item);
    void xg_itemRemoved(const X_ChemicalElementPropertyItem* item);

    void xg_itemIsChanged(const X_ChemicalElementPropertyItem* item);
    void xg_itemPropertyValueIsChanged(const X_ChemicalElementPropertyItem* item);
    void xg_itemPropertyNameIsChanged(const X_ChemicalElementPropertyItem* item);

private:

    //VARS
    QString xv_name;
    X_ChemicalElementPropertyItem* xv_parentItem;
    QList<X_ChemicalElementPropertyItem*> xv_childItemList;
    QList<QPair<QString, QString> > xv_chemicalElementPropertyList;

    // FUNCS
    X_ChemicalElementPropertyItem* xp_createChild(const QString& name);

};
typedef const X_ChemicalElementPropertyItem* X_ChemicalElementPropertyItemCPtr;
Q_DECLARE_METATYPE(X_ChemicalElementPropertyItemCPtr)
//======================================================
#endif // X_CHEMICALELEMENTPROPERTYITEM_H

//======================================================
#ifndef X_ABSTRACTCHEMICALELEMENTPROPERTYREPOSITORY_H
#define X_ABSTRACTCHEMICALELEMENTPROPERTYREPOSITORY_H
//======================================================
#include <QList>
#include <QPair>
#include <QString>
//======================================================
typedef QPair<QString, QString> PropertyPair;
typedef QList<QPair<QString, QString> > PropertyList;
//======================================================
class X_AbstractChemicalElementPropertyRepository
{
public:
    // friend class X_ChemicalElementPropertyIOManager;
    X_AbstractChemicalElementPropertyRepository();
    virtual ~X_AbstractChemicalElementPropertyRepository();

    virtual bool xp_setChemicalElementProperty(int X_Number,
                                               const QString& key,
                                               const QString& value) = 0;

    virtual bool xp_setChemicalElementProperties(int X_Number,
                                                 const QStringList& propertySectionBranch,
                                                 const PropertyList& propertyList) = 0;


    virtual bool xp_addChemicalElementPropertyBranch(int X_Number,
                                                     const QStringList& propertySectionBranch) = 0;


    virtual bool xp_chemicalElementProperties(int X_Number,
                                              const QStringList& propertySectionBranch,
                                              PropertyList& propertyList) = 0;

    virtual bool xp_chemicalElementPropertySectionList(int X_Number,
                                                       const QStringList& propertySectionBranch,
                                                       QStringList& sectionList) = 0;

    int xp_chemicalElementCount() const;
    QString xp_lastError() const;

    virtual void xp_clearData() = 0;

protected:

    // VARS
    int xv_chemicalElementCount;
    QString xv_errorMsg;
};
//======================================================
#endif // X_ABSTRACTCHEMICALELEMENTPROPERTYREPOSITORY_H

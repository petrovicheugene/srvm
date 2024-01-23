//======================================================
#ifndef X_CHEMICALELEMENTPROPERTYTREEMODEL_H
#define X_CHEMICALELEMENTPROPERTYTREEMODEL_H
//======================================================
#include <QAbstractItemModel>
#include "X_AbstractChemicalElementPropertyRepository.h"
//======================================================
class X_ChemicalElementPropertyItem;
class QSettings;
typedef QList<QStringList> PropertyNames;
//======================================================
const int X_NumberRole = Qt::UserRole + 1;
const int SymbolRole = Qt::UserRole + 2;
const int NameRole = Qt::UserRole + 3;
const int PropertyCountRole = Qt::UserRole + 4;
const int PropertyFirstColumnRole = Qt::UserRole + 5;
const int PropertyNameRole = Qt::UserRole + 6;
const int PropertyValueRole = Qt::UserRole + 7;

//======================================================
class X_ChemicalElementPropertyTreeModel : public QAbstractItemModel,
        public X_AbstractChemicalElementPropertyRepository
{
    Q_OBJECT
public:
    explicit X_ChemicalElementPropertyTreeModel(QObject *parent = nullptr);
    ~X_ChemicalElementPropertyTreeModel();

    // reimplemented FUNCS
    Qt::ItemFlags	flags(const QModelIndex & index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const override;
    int	rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;


    bool xp_setChemicalElementProperty(int X_Number,
                                      const QString& key,
                                      const QString& value) override;

    bool xp_setChemicalElementProperties(int X_Number,
                                           const QStringList& propertySectionBranch,
                                           const PropertyList& propertyList) override;

    bool xp_addChemicalElementPropertyBranch(int X_Number,
                                            const QStringList& propertySectionBranch) override;

    bool xp_chemicalElementPropertySectionBranch(const QModelIndex &index, QStringList &branch);

    bool xp_removeChemicalElementProperty(int X_Number,
                                          const QStringList& propertySectionBranch,
                                          int propertyIndex);
    bool xp_chemicalElementProperties(int X_Number,
                                              const QStringList& propertySectionBranch,
                                              PropertyList& propertyList) override;

    bool xp_chemicalElementPropertySectionList(int X_Number,
                                                       const QStringList& propertySectionBranch,
                                                       QStringList& sectionList) override;
    void xp_clearData() override;
    QStringList xp_lastErrorStringList() const;


    bool xp_zNumberForIndex(const QModelIndex& index, int& zNumber) const;
    QString xp_zNumberPropertyName() const;
    void xp_setX_NumberPropertyName(const QString& name);
    QString xp_symbolPropertyName() const;
    void xp_setSymbolPropertyName(const QString& name);
    QString xp_namePropertyName() const;
    void xp_setNamePropertyName(const QString& name);

    int xp_chemicalElementCount() const;
    bool xp_isRemovable(const QModelIndex& index) const;
    void xp_removeItems(const QModelIndexList& indexList);

    bool xp_isDirty() const;
    bool xp_saveDataToXMLFile(const QString& filePath);

    QString xp_propertiesSourceFilePath() const;
    bool xp_setPropertiesSourceFileName(const QString& propertiesFileName);

    bool xp_loadDataFromFile(const QString& filePath);

    QModelIndex xp_indexForSection(int X_Number, const QStringList& sectionBranch) const;

signals:

    void xg_propertiesSourceFilePathChanged(const QString& fileName);

public slots:

protected:
    bool xh_setCemicalElementProperty(int X_Number,
                                      const QString& key,
                                      const QString& value);

    bool xh_setCemicalElementProperties(int X_Number,
                                           const QStringList& propertySectionBranch,
                                           const PropertyList& propertyList);

    bool xh_addCemicalElementPropertyBranch(int X_Number,
                                            const QStringList& propertySectionBranch);

private slots:

    void xh_onItemAboutToBeInserted(int row, const X_ChemicalElementPropertyItem* parentItem);
    void xh_onItemInsertion(const X_ChemicalElementPropertyItem* item);
    void xh_notifyItemAboutToBeRemoved(const X_ChemicalElementPropertyItem *item);
    void xh_notifyItemRemoved(const X_ChemicalElementPropertyItem* item);
    void xh_notifyItemIsChanged(const X_ChemicalElementPropertyItem *item);

    void xh_notifyItemPropertyNameIsChanged(const X_ChemicalElementPropertyItem *item);
    void xh_notifyItemPropertyValueIsChanged(const X_ChemicalElementPropertyItem *item);

    void xh_itemRemovingPermission(const X_ChemicalElementPropertyItem* item,
                                   bool& permissionToRemove);

private:

    // VARS
    QStringList xv_errorStringList;
    X_ChemicalElementPropertyItem* xv_root;
    const int xv_chemicalElementCount = 118;
    const QString xv_defaultPropertiesSourceFileName = ":/files/chemicalElementBasicProperties.xml";
    PropertyNames xv_propertyNames;

    QString xv_zNumberPropertyName;
    QString xv_symbolPropertyName;
    QString xv_namePropertyName;

    QString xv_settingsGroupName;
    const QString xv_propertyFilePathSettingsName = "propertyFilePath";
    bool xv_dirty;

    // FUNCS
    void xh_createComponents();
    void xh_createConnections();
    void xh_loadBasicChemicalElementProperties();

    X_ChemicalElementPropertyItem* xh_itemForIndex(const QModelIndex &index) const;
    QModelIndex xh_indexForItem(const X_ChemicalElementPropertyItem* item) const;
    X_ChemicalElementPropertyItem* xh_itemForX_Number(int X_Number) const;
    void xh_redefinePropertyNameList();
    bool xh_firstPropertyColumnForItem(X_ChemicalElementPropertyItem* item,
                                      int& columnNumber) const;

    void xh_defineChemicalElementItemNames(const QString &nameTemplate, QString separator = " ");
    int xh_propertyNameCount(const PropertyNames& propertyNames) const;

    bool xh_openDialogSettingsGroup(QSettings* settings) const;
    bool xh_closeDialogSettingsGroup(QSettings* settings) const;

};
//======================================================
#endif // X_CHEMICALELEMENTPROPERTYTREEMODEL_H

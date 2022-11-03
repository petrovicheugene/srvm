//======================================================
#ifndef X_CHEMICALELEMENTPROPRTYEDITOR_H
#define X_CHEMICALELEMENTPROPRTYEDITOR_H
//======================================================
#include "X_BaseDialog.h"
#include "X_ChemicalElementPropertyTreeModel.h"

#include <QItemSelection>
//======================================================
class QAbstractItemModel;
class QAction;
class QClipboard;
class QLabel;
class QPushButton;
class QSplitter;
class QTableView;
class QTreeView;

class X_ChemicalPropertyProxyTableModel;
//======================================================
class X_ChemicalElementProprtyEditor : public X_BaseDialog
{
    Q_OBJECT
public:
    explicit X_ChemicalElementProprtyEditor(QWidget *parent = nullptr);
    ~X_ChemicalElementProprtyEditor() override;

    void xp_setModel(QAbstractItemModel* model);

signals:

public slots:


protected:

    void zh_saveSettings() override;
    void zh_restoreSettings() override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void closeEvent(QCloseEvent* event) override;

private slots:

    void zh_addNewChemicalElementPropertySection();
    void zh_removeSelectedChemicalElementPropertySections();
    void zh_addNewChemicalElementProperty();
    void zh_removeSelectedChemicalElementProperties();
    void zh_copyPropertyToClipboard();
    void zh_pastePropertyFromClipboard();

    void zh_onTreeElementSelectionChange(const QItemSelection& selected,
                                         const QItemSelection& deselected);
    void zh_onPropertyTableElementSelectionChange(const QItemSelection& selected,
                                                  const QItemSelection& deselected);
    void zh_setPropertiesSourceFilePath(const QString& fileName);
    void zh_onClipboardDataChange();
    void zh_treeColumnVisibilityControl();
    void zh_onModelColumnsInserted(const QModelIndex& parent, int first, int last);

    void zh_onLoadButtonClick();
    void zh_onCloseButtonClick();
    void zh_onSaveButtonClick();

private:

    // VARS
    static QString xv_splitterStateName;

    const int xv_maxStretchValue = 99999999;
    QLabel* xv_sourceFileNameLabel;
    QClipboard* xv_clipboard;
    QAction* xv_addChemicalElementPropertySectionsAction;
    QAction* xv_removeChemicalElementPropertySectionsAction;
    QAction* xv_addChemicalElementPropertyAction;
    QAction* xv_removeChemicalElementPropertiesAction;
    QAction* xv_copyChemicalElementPropertiesAction;
    QAction* xv_pasteChemicalElementPropertiesAction;

    QAction* xv_expandAllAction;
    QAction* xv_collapseAllAction;

    QSplitter* xv_mainSplitter;
    QTreeView* xv_propertySectionTreeView;
    QTableView* xv_propertyTableView;
    X_ChemicalPropertyProxyTableModel* xv_propertyFilterProxyModel;

    QPushButton* xv_addChemicalElementPropertySectionButton;
    QPushButton* xv_removeChemicalElementPropertySectionButton;
    QPushButton* xv_addChemicalElementPropertyButton;
    QPushButton* xv_removeChemicalElementPropertiesButton;

    QPushButton* xv_loadButton;
    QPushButton* xv_saveButton;
    QPushButton* xv_closeButton;

    // FUNCS
    void zh_createActions();
    void zh_createComponents();
    void zh_createMenu();
    void zh_createConnections();

    QWidget* zh_createPropertySectionTreeWidget();
    QWidget* zh_createPropertyTableWidget();

    bool zh_addChemicalElementPropertySectionBranch(const QList<int>& zNumberList,
                                                    const QStringList& sectionBranch);
    bool zh_currentChemicalElementSectionBrunch(QStringList &branch) const;

    bool zh_insertChemicalElementPropertiesToCurrentBranch(const PropertyList& propertyList);

    bool zh_selectedX_NumberList(QList<int>& zNumberList) const;
    bool zh_currentX_Number(int& currentX_Number) const;
    void zh_adjustRemoveSectionAbility(const QItemSelection& selected);
    void zh_adjustRemoveAndCopyPropertyAbility(const QItemSelection& selected);
    void zh_adjustPastePropertyAbility();
    void zh_savePropertyDataToFile();
    QString zh_dataFileFolder() const;
    QString zh_standardLocation() const;

};
//======================================================
#endif // X_CHEMICALELEMENTPROPRTYEDITOR_H

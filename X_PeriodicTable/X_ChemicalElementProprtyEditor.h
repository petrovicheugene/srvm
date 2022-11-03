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

    void xh_saveSettings() override;
    void xh_restoreSettings() override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void closeEvent(QCloseEvent* event) override;

private slots:

    void xh_addNewChemicalElementPropertySection();
    void xh_removeSelectedChemicalElementPropertySections();
    void xh_addNewChemicalElementProperty();
    void xh_removeSelectedChemicalElementProperties();
    void xh_copyPropertyToClipboard();
    void xh_pastePropertyFromClipboard();

    void xh_onTreeElementSelectionChange(const QItemSelection& selected,
                                         const QItemSelection& deselected);
    void xh_onPropertyTableElementSelectionChange(const QItemSelection& selected,
                                                  const QItemSelection& deselected);
    void xh_setPropertiesSourceFilePath(const QString& fileName);
    void xh_onClipboardDataChange();
    void xh_treeColumnVisibilityControl();
    void xh_onModelColumnsInserted(const QModelIndex& parent, int first, int last);

    void xh_onLoadButtonClick();
    void xh_onCloseButtonClick();
    void xh_onSaveButtonClick();

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
    void xh_createActions();
    void xh_createComponents();
    void xh_createMenu();
    void xh_createConnections();

    QWidget* xh_createPropertySectionTreeWidget();
    QWidget* xh_createPropertyTableWidget();

    bool xh_addChemicalElementPropertySectionBranch(const QList<int>& zNumberList,
                                                    const QStringList& sectionBranch);
    bool xh_currentChemicalElementSectionBrunch(QStringList &branch) const;

    bool xh_insertChemicalElementPropertiesToCurrentBranch(const PropertyList& propertyList);

    bool xh_selectedX_NumberList(QList<int>& zNumberList) const;
    bool xh_currentX_Number(int& currentX_Number) const;
    void xh_adjustRemoveSectionAbility(const QItemSelection& selected);
    void xh_adjustRemoveAndCopyPropertyAbility(const QItemSelection& selected);
    void xh_adjustPastePropertyAbility();
    void xh_savePropertyDataToFile();
    QString xh_dataFileFolder() const;
    QString xh_standardLocation() const;

};
//======================================================
#endif // X_CHEMICALELEMENTPROPRTYEDITOR_H

//======================================================
#include "X_ChemicalElementProprtyEditor.h"
#include "X_CaptionFormatter.h"
#include "X_ChemicalPropertyProxyTableModel.h"
#include "X_PropertyEditDialog.h"
#include "X_PropertySectionEditDialog.h"

#include <QAbstractItemModel>
#include <QApplication>
#include <QAction>
#include <QClipboard>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>
#include <QMimeData>
#include <QPushButton>
#include <QRegularExpression>
#include <QSettings>
#include <QSplitter>
#include <QStandardPaths>
#include <QStyledItemDelegate>
#include <QTableView>
#include <QTreeView>
#include <QVBoxLayout>

//======================================================
// STATIC
QString X_ChemicalElementProprtyEditor::xv_splitterStateName = "splitterState";
// END STATIC
//======================================================
X_ChemicalElementProprtyEditor::X_ChemicalElementProprtyEditor(QWidget *parent)
    : X_BaseDialog(QString("X_ChemicalElementProprtyEditor"), parent)
{
    setWindowTitle(qApp->applicationDisplayName());

    xh_createActions();
    xh_createComponents();
    xh_createMenu();
    xh_createConnections();
    xh_restoreSettings();

    xh_adjustRemoveAndCopyPropertyAbility(QItemSelection());
    xh_adjustRemoveSectionAbility(QItemSelection());
    xh_adjustPastePropertyAbility();
}
//======================================================
X_ChemicalElementProprtyEditor::~X_ChemicalElementProprtyEditor()
{
    xh_saveSettings();
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_createActions()
{
    xv_addChemicalElementPropertySectionsAction = new QAction(this);
    xv_addChemicalElementPropertySectionsAction->setText(tr("Add"));

    xv_removeChemicalElementPropertySectionsAction = new QAction(this);
    xv_removeChemicalElementPropertySectionsAction->setText(tr("Remove"));

    xv_addChemicalElementPropertyAction = new QAction(this);
    xv_addChemicalElementPropertyAction->setText(tr("Add"));

    xv_removeChemicalElementPropertiesAction = new QAction(this);
    xv_removeChemicalElementPropertiesAction->setText(tr("Remove"));

    xv_copyChemicalElementPropertiesAction = new QAction(this);
    xv_copyChemicalElementPropertiesAction->setText(tr("Copy"));

    xv_pasteChemicalElementPropertiesAction = new QAction(this);
    xv_pasteChemicalElementPropertiesAction->setText(tr("Paste"));

    xv_expandAllAction = new QAction(this);
    xv_expandAllAction->setText(tr("Expand all"));

    xv_collapseAllAction = new QAction(this);
    xv_collapseAllAction->setText(tr("Collapse all"));


}
//======================================================
void X_ChemicalElementProprtyEditor::xh_createComponents()
{
    xv_clipboard = QGuiApplication::clipboard();
    xv_propertyFilterProxyModel = new X_ChemicalPropertyProxyTableModel(this);

    QHBoxLayout* fileNameLabelLayout = new QHBoxLayout;
    QLabel* label = new QLabel(this);
    QString caption = tr("Source file:");
    label->setText(X_CaptionFormatter::xp_format(caption));
    fileNameLabelLayout->addWidget(label);
    xv_sourceFileNameLabel = new QLabel(this);
    fileNameLabelLayout->addWidget(xv_sourceFileNameLabel, xv_maxStretchValue, Qt::AlignVCenter | Qt::AlignLeft);
    xh_addLayoutToMainLayout(fileNameLabelLayout, 0);

    xv_mainSplitter = new QSplitter(Qt::Horizontal, this);
    xh_addWidgetToMainLayout(xv_mainSplitter, xv_maxStretchValue);
    xv_mainSplitter->addWidget(xh_createPropertySectionTreeWidget());
    xv_mainSplitter->addWidget(xh_createPropertyTableWidget());

    xv_loadButton = xh_createBasementButton(tr("Load"),
                                            tr("Load chemical element properties from file"),
                                            QIcon());
    xv_saveButton = xh_createBasementButton(tr("Save"),
                                            tr("Save chemical element properties to file"),
                                            QIcon());
    xv_closeButton = xh_createBasementButton(tr("Close"),
                                             tr("Close editor"),
                                             QIcon());
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_createMenu()
{
    xv_propertySectionTreeView->addAction(xv_addChemicalElementPropertySectionsAction);
    xv_propertySectionTreeView->addAction(xv_removeChemicalElementPropertySectionsAction);
    xv_propertySectionTreeView->addAction(xv_expandAllAction);
    xv_propertySectionTreeView->addAction(xv_collapseAllAction);
    xv_propertySectionTreeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    xv_propertyTableView->addAction(xv_addChemicalElementPropertyAction);
    xv_propertyTableView->addAction(xv_removeChemicalElementPropertiesAction);
    xv_propertyTableView->addAction(xv_copyChemicalElementPropertiesAction);
    xv_propertyTableView->addAction(xv_pasteChemicalElementPropertiesAction);
    xv_propertyTableView->setContextMenuPolicy(Qt::ActionsContextMenu);

}
//======================================================
void X_ChemicalElementProprtyEditor::xh_createConnections()
{
    xv_propertyTableView->setModel(xv_propertyFilterProxyModel);

    connect(xv_addChemicalElementPropertySectionButton, &QPushButton::clicked,
            xv_addChemicalElementPropertySectionsAction, &QAction::trigger);
    connect(xv_addChemicalElementPropertySectionsAction, &QAction::triggered,
            this, &X_ChemicalElementProprtyEditor::xh_addNewChemicalElementPropertySection);

    connect(xv_removeChemicalElementPropertySectionButton, &QPushButton::clicked,
            xv_removeChemicalElementPropertySectionsAction, &QAction::trigger);
    connect(xv_removeChemicalElementPropertySectionsAction, &QAction::triggered,
            this, &X_ChemicalElementProprtyEditor::xh_removeSelectedChemicalElementPropertySections);

    connect(xv_addChemicalElementPropertyButton, &QPushButton::clicked,
            xv_addChemicalElementPropertyAction, &QAction::trigger);
    connect(xv_addChemicalElementPropertyAction, &QAction::triggered,
            this, &X_ChemicalElementProprtyEditor::xh_addNewChemicalElementProperty);

    connect(xv_removeChemicalElementPropertiesButton, &QPushButton::clicked,
            xv_removeChemicalElementPropertiesAction, &QAction::trigger);
    connect(xv_removeChemicalElementPropertiesAction, &QAction::triggered,
            this, &X_ChemicalElementProprtyEditor::xh_removeSelectedChemicalElementProperties);

    connect(xv_copyChemicalElementPropertiesAction, &QAction::triggered,
            this, &X_ChemicalElementProprtyEditor::xh_copyPropertyToClipboard);
    connect(xv_pasteChemicalElementPropertiesAction, &QAction::triggered,
            this, &X_ChemicalElementProprtyEditor::xh_pastePropertyFromClipboard);

    connect(xv_expandAllAction, &QAction::triggered,
            xv_propertySectionTreeView, &QTreeView::expandAll);
    connect(xv_collapseAllAction, &QAction::triggered,
            xv_propertySectionTreeView, &QTreeView::collapseAll);

    connect(xv_clipboard, &QClipboard::dataChanged,
            this, &X_ChemicalElementProprtyEditor::xh_onClipboardDataChange);

    connect(xv_loadButton, &QPushButton::clicked,
            this, &X_ChemicalElementProprtyEditor::xh_onLoadButtonClick);
    connect(xv_saveButton, &QPushButton::clicked,
            this, &X_ChemicalElementProprtyEditor::xh_onSaveButtonClick);
    connect(xv_closeButton, &QPushButton::clicked,
            this, &X_ChemicalElementProprtyEditor::xh_onCloseButtonClick);
}
//======================================================
QWidget* X_ChemicalElementProprtyEditor::xh_createPropertySectionTreeWidget()
{
    QWidget* sectionTreeWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    sectionTreeWidget->setLayout(mainLayout);

    QLabel* label = new QLabel(sectionTreeWidget);
    QString caption = tr("Property sections:");
    label->setText(X_CaptionFormatter::xp_format(caption));
    mainLayout->addWidget(label);

    xv_propertySectionTreeView = new QTreeView;
    xv_propertySectionTreeView->installEventFilter(this);
    mainLayout->addWidget(xv_propertySectionTreeView);

    // basement
    QDialogButtonBox* buttonBox = new QDialogButtonBox(sectionTreeWidget);
    mainLayout->addWidget(buttonBox);

    xv_addChemicalElementPropertySectionButton = new QPushButton(sectionTreeWidget);
    xv_addChemicalElementPropertySectionButton->setFocusPolicy(Qt::NoFocus);
    xv_addChemicalElementPropertySectionButton->setText(xv_addChemicalElementPropertySectionsAction->text());
    xv_addChemicalElementPropertySectionButton->setIcon(xv_addChemicalElementPropertySectionsAction->icon());
    xv_addChemicalElementPropertySectionButton->setToolTip(xv_addChemicalElementPropertySectionsAction->toolTip());

    buttonBox->addButton(xv_addChemicalElementPropertySectionButton, QDialogButtonBox::ActionRole);

    xv_removeChemicalElementPropertySectionButton = new QPushButton(sectionTreeWidget);
    xv_removeChemicalElementPropertySectionButton->setFocusPolicy(Qt::NoFocus);
    xv_removeChemicalElementPropertySectionButton->setText(xv_removeChemicalElementPropertySectionsAction->text());
    xv_removeChemicalElementPropertySectionButton->setIcon(xv_removeChemicalElementPropertySectionsAction->icon());
    xv_removeChemicalElementPropertySectionButton->setToolTip(xv_removeChemicalElementPropertySectionsAction->toolTip());
    buttonBox->addButton(xv_removeChemicalElementPropertySectionButton, QDialogButtonBox::ActionRole);

    return sectionTreeWidget;
}
//======================================================
QWidget* X_ChemicalElementProprtyEditor::xh_createPropertyTableWidget()
{
    QWidget* propertyTableWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    propertyTableWidget->setLayout(mainLayout);

    QLabel* label = new QLabel(propertyTableWidget);
    QString caption = tr("Property:");
    label->setText(X_CaptionFormatter::xp_format(caption));
    mainLayout->addWidget(label);

    xv_propertyTableView = new QTableView;
    xv_propertyTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    xv_propertyTableView->installEventFilter(this);
    mainLayout->addWidget(xv_propertyTableView);


    // basement
    QDialogButtonBox* buttonBox = new QDialogButtonBox(propertyTableWidget);
    mainLayout->addWidget(buttonBox);

    xv_addChemicalElementPropertyButton = new QPushButton(propertyTableWidget);
    xv_addChemicalElementPropertyButton->setFocusPolicy(Qt::NoFocus);
    xv_addChemicalElementPropertyButton->setText(xv_addChemicalElementPropertySectionsAction->text());
    xv_addChemicalElementPropertyButton->setIcon(xv_addChemicalElementPropertySectionsAction->icon());
    xv_addChemicalElementPropertyButton->setToolTip(xv_addChemicalElementPropertySectionsAction->toolTip());

    buttonBox->addButton(xv_addChemicalElementPropertyButton, QDialogButtonBox::ActionRole);

    xv_removeChemicalElementPropertiesButton = new QPushButton(propertyTableWidget);
    xv_removeChemicalElementPropertiesButton->setFocusPolicy(Qt::NoFocus);
    xv_removeChemicalElementPropertiesButton->setText(xv_removeChemicalElementPropertySectionsAction->text());
    xv_removeChemicalElementPropertiesButton->setIcon(xv_removeChemicalElementPropertySectionsAction->icon());
    xv_removeChemicalElementPropertiesButton->setToolTip(xv_removeChemicalElementPropertySectionsAction->toolTip());
    buttonBox->addButton(xv_removeChemicalElementPropertiesButton, QDialogButtonBox::ActionRole);

    return propertyTableWidget;
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_saveSettings()
{
    QSettings settings;
    if(!xh_openDialogSettingsGroup(&settings))
    {
        return;
    }

    settings.setValue(xv_splitterStateName, xv_mainSplitter->saveState());

    // close dialog group
    settings.endGroup();

    xh_closeDialogSettingsGroup(&settings);
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_restoreSettings()
{
    QSettings settings;
    if(!xh_openDialogSettingsGroup(&settings))
    {
        return;
    }

    if(settings.value(xv_splitterStateName).canConvert<QByteArray>())
    {
        xv_mainSplitter->restoreState(settings.value(xv_splitterStateName).toByteArray());
    }

    xh_closeDialogSettingsGroup(&settings);
}
//======================================================
bool X_ChemicalElementProprtyEditor::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::KeyRelease)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if(!keyEvent)
        {
            return false;
        }

        if(watched == xv_propertySectionTreeView)
        {
            if(keyEvent->key() == Qt::Key_Delete
                    && xv_removeChemicalElementPropertySectionsAction->isEnabled())
            {
                xv_removeChemicalElementPropertySectionsAction->trigger();
            }
            else if(keyEvent->key() == Qt::Key_Insert)
            {
                xv_addChemicalElementPropertySectionsAction->trigger();
            }
        }
        else if(watched == xv_propertyTableView)
        {
            if(keyEvent->key() == Qt::Key_Delete
                    && xv_removeChemicalElementPropertiesAction->isEnabled())
            {
                xv_removeChemicalElementPropertiesAction->trigger();
            }
            else if(keyEvent->key() == Qt::Key_Insert)
            {
                xv_addChemicalElementPropertyAction->trigger();
            }
        }
    }

    return false;
}
//======================================================
void X_ChemicalElementProprtyEditor::closeEvent(QCloseEvent* event)
{
    X_ChemicalElementPropertyTreeModel* model =
            qobject_cast<X_ChemicalElementPropertyTreeModel*>(xv_propertySectionTreeView->model());
    if(!model)
    {
        return;
    }

    if(model->xp_isDirty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Chemical element properties were changed."));
        msgBox.setInformativeText(tr("Do you want to save them to file?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int answer = msgBox.exec();

        if(answer == QMessageBox::Yes)
        {
            xh_savePropertyDataToFile();
        }
    }
}
//======================================================
void X_ChemicalElementProprtyEditor::xp_setModel(QAbstractItemModel* model)
{
    // set up tree view
    xv_propertySectionTreeView->setModel(model);
    // xv_propertySectionTreeView->header()->setVisible(false);
    xv_propertySectionTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    xv_propertySectionTreeView->setSelectionBehavior(QAbstractItemView::SelectItems);


    // connect signal current tree element changed
    // in order to show chemical properties approprietary to current element
    connect(xv_propertySectionTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
            xv_propertyFilterProxyModel, &X_ChemicalPropertyProxyTableModel::xp_onCurrentTreeElementChanged);
    connect(model, &QAbstractItemModel::dataChanged,
            xv_propertyFilterProxyModel, &X_ChemicalPropertyProxyTableModel::xp_onSourceModelDataChanged);

    connect(xv_propertySectionTreeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &X_ChemicalElementProprtyEditor::xh_onTreeElementSelectionChange);

    connect(xv_propertyTableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &X_ChemicalElementProprtyEditor::xh_onPropertyTableElementSelectionChange);

    connect(model, &QAbstractItemModel::columnsInserted,
            this, &X_ChemicalElementProprtyEditor::xh_onModelColumnsInserted);

    X_ChemicalElementPropertyTreeModel* treeModel = qobject_cast<X_ChemicalElementPropertyTreeModel*>(model);
    if(treeModel)
    {
        connect(treeModel, &X_ChemicalElementPropertyTreeModel::xg_propertiesSourceFilePathChanged,
                this, &X_ChemicalElementProprtyEditor::xh_setPropertiesSourceFilePath);
        xh_setPropertiesSourceFilePath(treeModel->xp_propertiesSourceFilePath());
    }

    xh_treeColumnVisibilityControl();
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_addNewChemicalElementPropertySection()
{
    QStringList sectionBranch;
    xh_currentChemicalElementSectionBrunch(sectionBranch);

    // set first section chemical element symbol
    if(!sectionBranch.isEmpty())
    {
        sectionBranch.removeFirst();
    }

    int currentX_Number;
    if(xh_currentX_Number(currentX_Number))
    {
        QAbstractItemModel* model =
                qobject_cast<QAbstractItemModel*>(xv_propertySectionTreeView->model());
        if(model)
        {
            QModelIndex index = model->index(currentX_Number - 1, 0, QModelIndex());
            if(index.isValid())
            {
                QVariant vData = index.data(Qt::DisplayRole);
                if(vData.isValid() && vData.canConvert<QString>())
                {
                    sectionBranch.push_front(vData.toString());
                }
            }
        }
    }
    //

    bool currentVisible = xv_propertySectionTreeView->currentIndex().isValid();
    bool selectedVisible = !xv_propertySectionTreeView->selectionModel()->selectedIndexes().isEmpty();

    X_PropertySectionEditDialog dialog(sectionBranch, currentVisible, selectedVisible);
    if(!dialog.exec())
    {
        return;
    }

    QList<int>zNumberList;
    if(dialog.xp_additionTarget() == X_PropertySectionEditDialog::AT_ALL)
    {
        for(int z = 1; z <= 118; z++)
        {
            zNumberList.append(z);
        }
    }
    else if(dialog.xp_additionTarget() == X_PropertySectionEditDialog::AT_SELECTED)
    {
        if(!xh_selectedX_NumberList(zNumberList))
        {
            return;
        }
    }
    else if(dialog.xp_additionTarget() == X_PropertySectionEditDialog::AT_CURRENT)
    {
        int currentX_Number;
        if(!xh_currentX_Number(currentX_Number))
        {
            return;
        }
        zNumberList.append(currentX_Number);
    }
    else
    {
        return;
    }

    sectionBranch = dialog.xp_newSectionBranch();
    // remove first chemical element section
    if(!sectionBranch.isEmpty())
    {
        sectionBranch.removeFirst();
    }
    xh_addChemicalElementPropertySectionBranch(zNumberList, sectionBranch);
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_removeSelectedChemicalElementPropertySections()
{
    X_ChemicalElementPropertyTreeModel* model =
            qobject_cast<X_ChemicalElementPropertyTreeModel*>(xv_propertySectionTreeView->model());

    if(!model)
    {
        return;
    }

    // get selected indexes
    QModelIndexList selectedIndexList = xv_propertySectionTreeView->selectionModel()->selectedIndexes();

    // let model remove remained indexes
    model->xp_removeItems(selectedIndexList);
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_addNewChemicalElementProperty()
{
    int currentX_Number;
    if(!xh_currentX_Number(currentX_Number))
    {
        QString msg = tr("Select chemical element or property section!");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return;
    }

    X_PropertyEditDialog dialog;
    if(!dialog.exec())
    {
        return;
    }

    PropertyList propertyList;
    propertyList.append(dialog.xp_property());
    xh_insertChemicalElementPropertiesToCurrentBranch(propertyList);
}
//======================================================
bool X_ChemicalElementProprtyEditor::xh_insertChemicalElementPropertiesToCurrentBranch(const PropertyList& propertyList)
{
    X_ChemicalElementPropertyTreeModel* model =
            qobject_cast<X_ChemicalElementPropertyTreeModel*>(xv_propertySectionTreeView->model());

    if(!model)
    {
        QString msg = tr("Cannot access to chemical element property model.");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return false;
    }

    int currentX_Number;
    if(!xh_currentX_Number(currentX_Number))
    {
        QString msg = tr("Select chemical element or property section!");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return false;
    }

    QStringList sectionBranch;
    xh_currentChemicalElementSectionBrunch(sectionBranch);
    // remove first chemical element section
    if(!sectionBranch.isEmpty())
    {
        sectionBranch.removeFirst();
    }

    QString errorMsg;
    if(!model->xp_setChemicalElementProperties(currentX_Number, sectionBranch, propertyList))
    {
        QMessageBox::critical(this, tr("Error"), model->xp_lastError(), QMessageBox::Ok);
        return false;
    }

    return true;
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_removeSelectedChemicalElementProperties()
{
    X_ChemicalElementPropertyTreeModel* model =
            qobject_cast<X_ChemicalElementPropertyTreeModel*>(xv_propertySectionTreeView->model());
    if(!model)
    {
        return;
    }

    // get selected properties
    QModelIndexList selectedIndexList = xv_propertyTableView->selectionModel()->selectedIndexes();
    if(selectedIndexList.isEmpty())
    {
        return;
    }

    QList<int> rowList;
    foreach(QModelIndex index, selectedIndexList)
    {
        if(!rowList.contains(index.row()))
        {
            rowList.append(index.row());
        }
    }

    std::sort(rowList.begin(), rowList.end());

    int zNumber;
    if(!xh_currentX_Number(zNumber))
    {
        return;
    }

    QStringList branch;
    xh_currentChemicalElementSectionBrunch(branch);
    if(!branch.isEmpty())
    {
        branch.removeFirst();
    }

    for(int i = rowList.count() - 1; i >= 0;  i--)
    {
        model->xp_removeChemicalElementProperty(zNumber, branch, rowList.at(i));
    }

}
//======================================================
void X_ChemicalElementProprtyEditor::xh_copyPropertyToClipboard()
{
    // TODO Copy Selected Properties
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_pastePropertyFromClipboard()
{
    if(!xv_clipboard->mimeData()->hasText()
            || xv_clipboard->text().isEmpty())
    {
        return;
    }

    // devide text to rows (separator - \n)
    QRegularExpression rowSplitterRegExp("\n");
    QString txt = xv_clipboard->text();
    QStringList rowList = xv_clipboard->text().split(rowSplitterRegExp, Qt::SkipEmptyParts);

    // devide every row to columns (separator - \t or ';')
    QRegularExpression columnSplitterRegExp("[\t;]");
    QList<QStringList> slittedRowList;
    QStringList currentRow;
    int columnCount = 0;
    for(int i = 0; i < rowList.count(); i++)
    {
        currentRow = rowList.at(i).split(columnSplitterRegExp);
        slittedRowList.append(currentRow);
        if(columnCount < currentRow.count())
        {
            columnCount = currentRow.count();
        }
    }

    if(columnCount <= 0)
    {
        return;
    }
    else if(columnCount == 1)
    {
        // insert in selection only

    }
    else // columnCount > 1
    {
        // selections ignored
        PropertyList propertyList;

        for(int row = 0; row < slittedRowList.count(); row++)
        {
            propertyList.append(QPair<QString, QString>(slittedRowList.at(row).value(0), slittedRowList.at(row).value(1)));
        }

        xh_insertChemicalElementPropertiesToCurrentBranch(propertyList);
    }
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_onTreeElementSelectionChange(const QItemSelection& selected,
                                                                    const QItemSelection& deselected)
{
    xh_adjustRemoveSectionAbility(selected);
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_onPropertyTableElementSelectionChange(const QItemSelection& selected,
                                                                             const QItemSelection& deselected)
{
    xh_adjustRemoveAndCopyPropertyAbility(selected);
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_setPropertiesSourceFilePath(const QString& fileName)
{
    xv_sourceFileNameLabel->setText(fileName);
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_onClipboardDataChange()
{
    xh_adjustPastePropertyAbility();
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_onModelColumnsInserted(const QModelIndex& parent, int first, int last)
{
    xh_treeColumnVisibilityControl();
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_onLoadButtonClick()
{
    X_ChemicalElementPropertyTreeModel* model =
            qobject_cast<X_ChemicalElementPropertyTreeModel*>(xv_propertySectionTreeView->model());
    if(!model)
    {
        QString msg = tr("There is no chemical elment model for load data.");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return;
    }

    if(model->xp_isDirty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Chemical element properties were changed."));
        msgBox.setInformativeText(tr("Do you want to save them to file?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int answer = msgBox.exec();

        if(answer == QMessageBox::Yes)
        {
            xh_savePropertyDataToFile();
        }
    }

    QString dataFileFolder = xh_dataFileFolder();
    QString fileDataPath = QFileDialog::getOpenFileName(this, tr("Open data file"), dataFileFolder, tr("XML Files (*.xml);;All Files (*.*)"));
    if(fileDataPath.isEmpty())
    {
        return;
    }

    if(!model->xp_loadDataFromFile(fileDataPath))
    {
        QMessageBox::critical(this, tr("Error"), model->xp_lastError(), QMessageBox::Ok);
    }

}
//======================================================
void X_ChemicalElementProprtyEditor::xh_onCloseButtonClick()
{
    X_ChemicalElementPropertyTreeModel* model =
            qobject_cast<X_ChemicalElementPropertyTreeModel*>(xv_propertySectionTreeView->model());
    if(!model)
    {
        reject();
        return;
    }

    if(model->xp_isDirty())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Chemical element properties were changed."));
        msgBox.setInformativeText(tr("Do you want to save them to file?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int answer = msgBox.exec();

        if(answer == QMessageBox::Yes)
        {
            xh_savePropertyDataToFile();
        }
    }

    accept();
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_treeColumnVisibilityControl()
{
    QAbstractItemModel* model = xv_propertySectionTreeView->model();
    if(!model)
    {
        return;
    }

    for(int column = 1; column < model->columnCount(); column++)
    {
        xv_propertySectionTreeView->setColumnHidden(column, true);
    }

    xv_propertySectionTreeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
}
//======================================================
bool X_ChemicalElementProprtyEditor::xh_addChemicalElementPropertySectionBranch(const QList<int>& zNumberList,
                                                                               const QStringList& sectionBranch)
{
    if(sectionBranch.isEmpty())
    {
        return false;
    }

    X_ChemicalElementPropertyTreeModel* model =
            qobject_cast<X_ChemicalElementPropertyTreeModel*>(xv_propertySectionTreeView->model());

    if(!model)
    {
        return false;
    }

    QString errorMsg;
    bool continueFlag = false;
    for(int i = 0; i < zNumberList.count(); i++)
    {
        if(!model->xp_addChemicalElementPropertyBranch(zNumberList.at(i),
                                                       sectionBranch) && !continueFlag)
        {
            // get chem element name
            int row = zNumberList.at(i) - 1;
            QModelIndex index = model->index(row, 0, QModelIndex());
            QVariant vData = index.data(Qt::DisplayRole);

            QString chemicalElement;
            if(!vData.isValid() || !vData.canConvert<QString>())
            {
                chemicalElement = QString::number(zNumberList.at(i));
            }
            else
            {
                chemicalElement = vData.toString();
            }

            QMessageBox msgBox;
            msgBox.setText(tr("Cannot add chemical element property branch to chemical element %1 with error \"%2\".")
                           .arg(chemicalElement, errorMsg));
            msgBox.setInformativeText("Do you want to continue anyway?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            int answer = msgBox.exec();

            switch(answer)
            {
            case QMessageBox::Yes:
                continueFlag = true;
                continue;
            case QMessageBox::No:
                return false;
            }
        }
    }

    xh_adjustRemoveSectionAbility(xv_propertySectionTreeView->selectionModel()->selection());
    return true;
}
//======================================================
bool X_ChemicalElementProprtyEditor::xh_currentChemicalElementSectionBrunch(QStringList& branch) const
{
    X_ChemicalElementPropertyTreeModel* model =
            qobject_cast<X_ChemicalElementPropertyTreeModel*>(xv_propertySectionTreeView->model());

    if(!model)
    {
        return false;
    }

    return model->xp_chemicalElementPropertySectionBranch(xv_propertySectionTreeView->currentIndex(), branch);
}
//======================================================
bool X_ChemicalElementProprtyEditor::xh_selectedX_NumberList(QList<int>& zNumberList) const
{
    X_ChemicalElementPropertyTreeModel* model =
            qobject_cast<X_ChemicalElementPropertyTreeModel*>(xv_propertySectionTreeView->model());

    if(!model)
    {
        return false;
    }

    // get selected indexes
    QModelIndexList selectedIndexList = xv_propertySectionTreeView->selectionModel()->selectedIndexes();
    // get zNumbers

    int zNumber;
    foreach(QModelIndex index, selectedIndexList)
    {
        if(!model->xp_zNumberForIndex(index, zNumber))
        {
            continue;
        }

        if(!zNumberList.contains(zNumber))
        {
            zNumberList.append(zNumber);
        }
    }

    std::sort(zNumberList.begin(), zNumberList.end());

    return !zNumberList.isEmpty();
}
//======================================================
bool X_ChemicalElementProprtyEditor::xh_currentX_Number(int& currentX_Number) const
{
    X_ChemicalElementPropertyTreeModel* model =
            qobject_cast<X_ChemicalElementPropertyTreeModel*>(xv_propertySectionTreeView->model());

    if(!model)
    {
        return false;
    }

    return model->xp_zNumberForIndex(xv_propertySectionTreeView->currentIndex(), currentX_Number);
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_adjustRemoveSectionAbility(const QItemSelection& selected)
{
    X_ChemicalElementPropertyTreeModel* model =
            qobject_cast<X_ChemicalElementPropertyTreeModel*>(xv_propertySectionTreeView->model());

    if(!model || selected.isEmpty())
    {
        xv_removeChemicalElementPropertySectionButton->setEnabled(false);
        xv_removeChemicalElementPropertySectionsAction->setEnabled(false);
        return;
    }

    QModelIndexList selectedIndexList = selected.indexes();
    foreach(QModelIndex index, selectedIndexList)
    {
        if(index.column() == 0 &&  (model->hasChildren(index) || model->xp_isRemovable(index)))
        {
            xv_removeChemicalElementPropertySectionButton->setEnabled(true);
            xv_removeChemicalElementPropertySectionsAction->setEnabled(true);

            return;
        }
    }

    xv_removeChemicalElementPropertySectionButton->setEnabled(false);
    xv_removeChemicalElementPropertySectionsAction->setEnabled(false);
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_adjustRemoveAndCopyPropertyAbility(const QItemSelection& selected)
{
    X_ChemicalElementPropertyTreeModel* model =
            qobject_cast<X_ChemicalElementPropertyTreeModel*>(xv_propertySectionTreeView->model());

    if(!model || selected.isEmpty())
    {
        xv_removeChemicalElementPropertiesButton->setEnabled(false);
        xv_removeChemicalElementPropertiesAction->setEnabled(false);
        xv_copyChemicalElementPropertiesAction->setEnabled(false);
        return;
    }

    bool enabled = !selected.indexes().isEmpty();
    xv_removeChemicalElementPropertiesButton->setEnabled(enabled);
    xv_removeChemicalElementPropertiesAction->setEnabled(enabled);
    xv_copyChemicalElementPropertiesAction->setEnabled(enabled);

}
//======================================================
void X_ChemicalElementProprtyEditor::xh_adjustPastePropertyAbility()
{
    xv_pasteChemicalElementPropertiesAction->setEnabled(xv_clipboard->mimeData()->hasText()
                                                        && !xv_clipboard->text().isEmpty());
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_onSaveButtonClick()
{
    xh_savePropertyDataToFile();
}
//======================================================
void X_ChemicalElementProprtyEditor::xh_savePropertyDataToFile()
{
    X_ChemicalElementPropertyTreeModel* model =
            qobject_cast<X_ChemicalElementPropertyTreeModel*>(xv_propertySectionTreeView->model());

    if(!model)
    {
        QString msg = tr("There is no property model!");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return;
    }

    // get the file
    QString dataFileFolder = xh_dataFileFolder();
    QString fileDataPath = QFileDialog::getSaveFileName(this, tr("Save data to file"), dataFileFolder, tr("XML Files (*.xml);;All Files (*.*)"));
    if(fileDataPath.isEmpty())
    {
        return;
    }

    QString errorMsg;
    if(!model->xp_saveDataToXMLFile(fileDataPath))
    {
        QMessageBox::critical(this, tr("Error"), model->xp_lastError(), QMessageBox::Ok);
        return;
    }

    QString msg = tr("File \"%1\" have been saved successfully.").arg(fileDataPath);

    QMessageBox msgBox;
    msgBox.setWindowTitle(qApp->applicationDisplayName());
    msgBox.setText(msg);
    msgBox.setInformativeText(tr("Do you want to set the file as default chemical element properties source or reset source file to default?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Reset);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int answer = msgBox.exec();

    switch(answer)
    {
    case QMessageBox::No:
        return;
    case QMessageBox::Yes:
        model->xp_setPropertiesSourceFileName(fileDataPath);
        break;
    case QMessageBox::Reset:
        model->xp_setPropertiesSourceFileName(QString());
        break;
    }

}
//======================================================
QString X_ChemicalElementProprtyEditor::xh_dataFileFolder() const
{
    X_ChemicalElementPropertyTreeModel* model =
            qobject_cast<X_ChemicalElementPropertyTreeModel*>(xv_propertySectionTreeView->model());

    if(!model)
    {
        return xh_standardLocation();
    }

    QString currentPropertiesSourceFileName = model->xp_propertiesSourceFilePath();
    QFileInfo fileInfo(currentPropertiesSourceFileName);

    if(fileInfo.absolutePath().startsWith(":"))
    {
        return xh_standardLocation();
    }

    return fileInfo.absolutePath();
}
//======================================================
QString X_ChemicalElementProprtyEditor::xh_standardLocation() const
{
    if(!QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).isEmpty())
    {
        return QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first();
    }

    return QDir::currentPath();
}
//======================================================


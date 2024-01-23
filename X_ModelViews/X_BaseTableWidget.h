//=========================================================================
#ifndef X_BASETABLEWIDGET_H
#define X_BASETABLEWIDGET_H
//=========================================================================
#include <QWidget>
#include <QModelIndexList>
#include <QItemSelection>
//=========================================================================
class QAbstractItemModel;
class QTableView;
class X_ControlAction;
class QLabel;

class QHBoxLayout;
class QVBoxLayout;

//=========================================================================
class X_BaseTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit X_BaseTableWidget(QWidget *parent = 0);

    virtual void xp_setModel(QAbstractItemModel* model, bool alternatingRowColorFlag = true);
    QTableView* xp_tableView() const;
    void xp_setCaption(const QString& caption);
    void xp_setColumnHidden(int column, bool hidden);
    void xp_setHorizontalHeaderHidden(bool hidden);
    void xp_setVerticalHeaderHidden(bool hidden);
    void xp_setStretchLastSection(bool stretch);

    void xp_appendButtonActions(const QList<X_ControlAction*>& actionList);
    void xp_appendContextActions(const QList<X_ControlAction *> &actionList);

    QList<int> xp_selectedRowList();
    QModelIndexList xp_selectedModelIndexList() const;

    void xp_setCurrentRow(int row);
    void xp_currentIndex(QModelIndex& index);

signals:

    void xg_currentChanged(const QModelIndex& current, const QModelIndex& previous) const;
    void xg_selectionChanged();

protected slots:

    void xh_onSelectionChange(const QItemSelection& selected,
                              const QItemSelection& deselected);

protected:

    // VARS
    QTableView* xv_table; /*!< TODO: describe */
    QHBoxLayout* xv_buttonLayout; /*!< TODO: describe */
    QVBoxLayout* xv_mainLayout; /*!< TODO: describe */
    QLabel* xv_captionLabel;
    QList<X_ControlAction*> xv_contextMenuActionList; /*!< TODO: describe */

    // FUNCS
    /*!
     \brief

    */
    void xh_createComponents();
    /*!
     \brief

    */
    void xh_createConnections();


};
//=========================================================================
#endif // X_BASETABLEWIDGET_H

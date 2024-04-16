//==============================================================
#ifndef X_CALIBRATIONARRAYWIDGET_H
#define X_CALIBRATIONARRAYWIDGET_H
//==============================================================
#include <QWidget>
#include <QItemSelection>
//==============================================================
class QTableView;
class QAbstractItemModel;
class QHBoxLayout;
class QVBoxLayout;

//==============================================================
class X_ChemElementWidget : public QWidget
{
    Q_OBJECT
public:
    explicit X_ChemElementWidget(QWidget *parent = 0);

    // FUNCS
    void xp_setModel(QAbstractItemModel* model);
    void xp_appendButtonActions(QList<QAction*>);
    void xp_appendContextMenuActions(QList<QAction*> actionList);

    void xp_setMainLayoutMargin(int margin);

signals:

    void xg_selectedChemElementChanged();

public slots:

    void xp_setCurrentChemElementIndex(int);
    void xp_currentChemElementIndex(int&) const;
    void xp_startCurrentChemElementEdition();
    void xp_selectedChemElementIndexList(QList<int>& selectedIndexList) const;

private:

    // VARS
    QTableView* xv_table;
    QHBoxLayout* xv_buttonLayout;
    QVBoxLayout* xv_mainLayout;
    QList<QAction*> xv_contextMenuActionList;

    // FUNCS
    void xh_createComponents();
    void xh_createConnections();

private slots:

    void xh_onSelectedElementChanged(const QItemSelection & current,
                                     const QItemSelection & previous);
    void xh_onContextMenuRequest(const QPoint &pos);

};
//==============================================================
#endif // X_CALIBRATIONARRAYWIDGET_H

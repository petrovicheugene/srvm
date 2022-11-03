//========================================================
#ifndef X_TERMCORRELATIONTABLEWIDGET_H
#define X_TERMCORRELATIONTABLEWIDGET_H
//========================================================
#include <QWidget>
#include <QModelIndex>

//========================================================
class QTableView;
class QAction;
class QHBoxLayout;
class QVBoxLayout;
class X_TermCorrelationTableModel;
//========================================================
class X_TermCorrelationTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit X_TermCorrelationTableWidget(QWidget *parent = nullptr);

    // FUNCS
    void xp_setModel(X_TermCorrelationTableModel* model);
    void xp_appendButtonActions(QList<QAction*>);
    void xp_appendContextMenuActions(QList<QAction*> actionList);

    void xp_setMainLayoutMargin(int margin);

signals:

    void xg_userChangesTermState(int termLogIndex);
    void xg_currentTermChanged(int currentTermIndex, int previousTermIndex) const;
    void xg_termDoubleClicked(int row) const;

public slots:

    void xp_currentTermIndex(int&);

private slots:

    void zh_editNext(QModelIndex editedIndex);
    void zh_onCurrentTermChanged(QModelIndex current, QModelIndex previous);
    void zh_onTermDoubleClick(const QModelIndex& index);
    void zh_onContextMenuRequest(const QPoint &pos);

private:

    // VARS
    QTableView* xv_table;
    QHBoxLayout* xv_buttonLayout;
    QVBoxLayout* xv_mainLayout;
    QList<QAction*> xv_contextMenuActionList;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

};
//========================================================
#endif // X_TERMCORRELATIONTABLEWIDGET_H

//========================================================
#ifndef ZTERMCORRELATIONTABLEWIDGET_H
#define ZTERMCORRELATIONTABLEWIDGET_H
//========================================================
#include <QWidget>
#include <QModelIndex>

//========================================================
class QTableView;
class QAction;
class QHBoxLayout;
class QVBoxLayout;
class ZTermCorrelationTableModel;
//========================================================
class ZTermCorrelationTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZTermCorrelationTableWidget(QWidget *parent = nullptr);

    // FUNCS
    void zp_setModel(ZTermCorrelationTableModel* model);
    void zp_appendButtonActions(QList<QAction*>);
    void zp_appendContextMenuActions(QList<QAction*> actionList);

    void zp_setMainLayoutMargin(int margin);

signals:

    void zg_userChangesTermState(int termLogIndex);
    void zg_currentTermChanged(int currentTermIndex, int previousTermIndex) const;
    void zg_termDoubleClicked(int row) const;

public slots:

    void zp_currentTermIndex(int&);

private slots:

    void zh_editNext(QModelIndex editedIndex);
    void zh_onCurrentTermChanged(QModelIndex current, QModelIndex previous);
    void zh_onTermDoubleClick(const QModelIndex& index);
    void zh_onContextMenuRequest(const QPoint &pos);

private:

    // VARS
    QTableView* zv_table;
    QHBoxLayout* zv_buttonLayout;
    QVBoxLayout* zv_mainLayout;
    QList<QAction*> zv_contextMenuActionList;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

};
//========================================================
#endif // ZTERMCORRELATIONTABLEWIDGET_H

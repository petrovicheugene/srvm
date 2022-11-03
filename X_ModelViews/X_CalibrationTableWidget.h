//==============================================================
#ifndef X_CALIBRATIONTABLEWIDGET_H
#define X_CALIBRATIONTABLEWIDGET_H
//==============================================================
#include <QWidget>
#include <QItemSelection>
//==============================================================
class QTableView;
class QAbstractItemModel;
class QHBoxLayout;
class QVBoxLayout;
class QAction;
class X_CalibrationRepository;
//==============================================================
class X_CalibrationTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit X_CalibrationTableWidget(QWidget *parent = 0);

     // FUNCS
    void xp_setModel(QAbstractItemModel* model);
    void xp_appendButtonActions(QList<QAction*>);
    void xp_appendContextMenuActions(QList<QAction*> actionList);

    void xp_connectToCalibrationRepository(X_CalibrationRepository*);
    void xp_setMainLayoutMargin(int margin);

signals:

    void xg_currentCalibrationChanged(int current, int previous);
    void xg_selectedCalibrationChanged(QList<int>);
    void xg_requestChemElementList(QStringList&);
    void xg_currentCalibrationWindowName(const QString& name) const;


public slots:

    void xp_selectedCalibrationIndexList(QList<int>& selectedList);
    void xp_currentCalibrationIndex(int&);
    
    void xp_setCurrentCalibrationIndex(int);
    void xp_startCurrentCalibrationEdition();

private:

    // VARS
    QTableView* xv_table;
    QHBoxLayout* xv_buttonLayout;
    QVBoxLayout* xv_mainLayout;
    QList<QAction*> xv_contextMenuActionList;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

private slots:

    void zh_onContextMenuRequest(const QPoint &pos);
    void zh_onCurrentCalibrationChange(const QModelIndex & current, const QModelIndex & previous);
    void zh_onSelectedCalibrationChange(const QItemSelection &selected, const QItemSelection &deselected);
    void zh_onRowCountChange(const QModelIndex& parent, int first, int last);
};
//==============================================================
#endif // X_CALIBRATIONTABLEWIDGET_H

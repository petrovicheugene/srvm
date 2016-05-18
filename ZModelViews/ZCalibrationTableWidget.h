//==============================================================
#ifndef ZCALIBRATIONTABLEWIDGET_H
#define ZCALIBRATIONTABLEWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class QTableView;
class QAbstractItemModel;
class QHBoxLayout;
class QVBoxLayout;
class QAction;
class ZCalibrationRepository;
//==============================================================
class ZCalibrationTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZCalibrationTableWidget(QWidget *parent = 0);

     // FUNCS
    void zp_setModel(QAbstractItemModel* model);
    void zp_appendButtonActions(QList<QAction*>);
    void zp_appendContextMenuActions(QList<QAction*> actionList);

    void zp_connectToCalibrationRepository(ZCalibrationRepository*);
    void zp_setMargin(int margin);

signals:

    void zg_currentCalibrationChanged(int current, int previous);
    void zg_requestChemElementList(QStringList&);

public slots:

    void zp_selectedCalibrationIndexList(QList<int>& selectedList);
    void zp_currentCalibrationIndex(int&);
    
    void zp_setCurrentCalibrationIndex(int);
    void zp_startCurrentCalibrationEdition();

private:

    // VARS
    QTableView* zv_table;
    QHBoxLayout* zv_buttonLayout;
    QVBoxLayout* zv_mainLayout;
    QList<QAction*> zv_contextMenuActionList;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

private slots:

    void zh_onContextMenuRequest(const QPoint &pos);
    void zh_onCurrentCalibrationChanged(const QModelIndex & current, const QModelIndex & previous);
};
//==============================================================
#endif // ZCALIBRATIONTABLEWIDGET_H

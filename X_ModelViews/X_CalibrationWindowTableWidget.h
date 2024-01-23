//==============================================================
#ifndef X_JOINTCALIBRATIONWINDOWTABLEWIDGET_H
#define X_JOINTCALIBRATIONWINDOWTABLEWIDGET_H
//==============================================================
#include <QWidget>
#include <QItemSelection>
//==============================================================
class X_CalibrationWindowModel;
class X_ChannelNumberDelegate;
class X_CalibrationRepository;
class QTableView;
class QHBoxLayout;
class QVBoxLayout;
class QAction;
//==============================================================
class X_CalibrationWindowTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit X_CalibrationWindowTableWidget(QWidget *parent = 0);

    // FUNCS
    void xp_setModel(X_CalibrationWindowModel* model);
    void xp_appendButtonActions(QList<QAction*>);
    void xp_appendContextMenuActions(QList<QAction*> actionList);

    void xp_connectToCalibrationRepository(X_CalibrationRepository*);
    void xp_setMainLayoutMargin(int margin);

signals:

    void xg_currentCalibrationWindowChanged(int, int);
    void xg_calibrationWindowSelectionChanged();
    void xg_calibrationWindowClicked(const QModelIndex& index) const;

public slots:

    void xp_setCurrentWindowIndex(int);
    void xp_currentWindowIndex(int&) const;
    void xp_startCurrentWindowEdition();

    void xp_selectedSpectrumWindowIndexList(QList<int>&);
    void xp_setChannelNumberMinMax(int intensity, int channels);

private:

    // VARS
    QTableView* xv_table;
    X_ChannelNumberDelegate* xv_channelDelegate;
    QHBoxLayout* xv_buttonLayout;
    QVBoxLayout* xv_mainLayout;
    QList<QAction*> xv_contextMenuActionList;

    const int xv_defaultChannelCount = 2048;

    // FUNCS
    void xh_createComponents();
    void xh_createConnections();

private slots:
    void xh_onCurrentCalibrationWindowChanged(const QModelIndex & current,
                                     const QModelIndex & previous);
    void xh_onCalibrationWindowSelectionChanged(const QItemSelection & selected,
                                                                               const QItemSelection & deselected);
    void xh_onCalibrationWindowClick(const QModelIndex& index ) const;
    void xh_onContextMenuRequest(const QPoint &pos);


};
//==============================================================
#endif // X_JOINTCALIBRATIONWINDOWTABLEWIDGET_H

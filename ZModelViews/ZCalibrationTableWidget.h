//==============================================================
#ifndef ZCALIBRATIONTABLEWIDGET_H
#define ZCALIBRATIONTABLEWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class QTableView;
class QAbstractItemModel;
class QHBoxLayout;
class QAction;
//==============================================================
class ZCalibrationTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZCalibrationTableWidget(QWidget *parent = 0);

     // FUNCS
    void zp_setModel(QAbstractItemModel* model);
    void zp_appendButtonActions(QList<QAction*>);

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

    // FUNCS
    void zh_createComponents();

private slots:

    void zh_onCurrentCalibrationChanged(const QModelIndex & current, const QModelIndex & previous);
};
//==============================================================
#endif // ZCALIBRATIONTABLEWIDGET_H

//==============================================================
#ifndef ZJOINTCALIBRATIONWINDOWTABLEWIDGET_H
#define ZJOINTCALIBRATIONWINDOWTABLEWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class QTableView;
class QHBoxLayout;
class QAction;

//==============================================================
class ZJointCalibrationWindowTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZJointCalibrationWindowTableWidget(QWidget *parent = 0);

    // FUNCS
    void zp_setModel(ZJointSpectraModel* model);

signals:

public slots:


private:
    // VARS
    QTableView* zv_table;
    QHBoxLayout* zv_buttonLayout;

    // FUNCS
    void zh_createComponents();


};
//==============================================================
#endif // ZJOINTCALIBRATIONWINDOWTABLEWIDGET_H

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
class ZJointCalibrationWindowtableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZJointCalibrationWindowtableWidget(QWidget *parent = 0);

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

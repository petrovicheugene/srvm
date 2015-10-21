//==============================================================
#ifndef ZCALIBRATIONTABLEWIDGET_H
#define ZCALIBRATIONTABLEWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class QTableView;
//==============================================================
class ZCalibrationTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZCalibrationTableWidget(QWidget *parent = 0);

signals:

public slots:


private:

    // VARS
    QTableView* zv_table;

    // FUNCS
    void zh_createComponents();

};
//==============================================================
#endif // ZCALIBRATIONTABLEWIDGET_H

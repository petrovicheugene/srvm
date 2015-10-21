//==============================================================
#ifndef ZCALIBRATIONTABLEWIDGET_H
#define ZCALIBRATIONTABLEWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class QTableView;
class QAbstractItemModel;
//==============================================================
class ZCalibrationTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZCalibrationTableWidget(QWidget *parent = 0);

     // FUNCS
    void zp_setModel(QAbstractItemModel* model);


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

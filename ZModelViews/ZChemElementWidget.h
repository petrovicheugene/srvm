//==============================================================
#ifndef ZCALIBRATIONARRAYWIDGET_H
#define ZCALIBRATIONARRAYWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class QTableView;
//==============================================================
class ZCalibrationArrayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZCalibrationArrayWidget(QWidget *parent = 0);

signals:

public slots:


private:

    // VARS
    QTableView* zv_table;

    // FUNCS
    void zh_createComponents();

};
//==============================================================
#endif // ZCALIBRATIONARRAYWIDGET_H

//==============================================================
#ifndef ZCALIBRATIONARRAYWIDGET_H
#define ZCALIBRATIONARRAYWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class QTableView;
class QAbstractItemModel;

//==============================================================
class ZChemElementWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZChemElementWidget(QWidget *parent = 0);

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
#endif // ZCALIBRATIONARRAYWIDGET_H

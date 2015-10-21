//==============================================================
#ifndef ZSPECTRUMARRAYWIDGET_H
#define ZSPECTRUMARRAYWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class QTableView;
class QAbstractItemModel;

//==============================================================
class ZSpectrumArrayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZSpectrumArrayWidget(QWidget *parent = 0);

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
#endif // ZSPECTRUMARRAYWIDGET_H

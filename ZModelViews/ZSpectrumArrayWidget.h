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

   void zg_currentArrayChenged(int, int);

public slots:


private:

    // VARS
    QTableView* zv_table;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

private slots:

    void zh_onCurrentArrayChanged(const QModelIndex & current, const QModelIndex & previous);

};
//==============================================================
#endif // ZSPECTRUMARRAYWIDGET_H

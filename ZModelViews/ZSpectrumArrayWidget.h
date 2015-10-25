//==============================================================
#ifndef ZSPECTRUMARRAYWIDGET_H
#define ZSPECTRUMARRAYWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class QTableView;
class QAbstractItemModel;
class QAction;
class QHBoxLayout;
//==============================================================
class ZSpectrumArrayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZSpectrumArrayWidget(QWidget *parent = 0);

    // FUNCS
   void zp_setModel(QAbstractItemModel* model);
   void zp_appendButtonActions(QList<QAction*>);

signals:

   void zg_currentArrayChenged(int, int);

public slots:

    void zp_setCurrentArrayIndex(int);
    void zp_currentArrayIndex(int&);

private:

    // VARS
    QTableView* zv_table;
    QHBoxLayout* zv_buttonLayout;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

private slots:

    void zh_onCurrentArrayChanged(const QModelIndex & current, const QModelIndex & previous);

};
//==============================================================
#endif // ZSPECTRUMARRAYWIDGET_H

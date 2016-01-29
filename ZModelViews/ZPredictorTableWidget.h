//========================================================
#ifndef ZPREDICTORTABLEWIDGET_H
#define ZPREDICTORTABLEWIDGET_H
//========================================================
#include <QWidget>
//========================================================
class QTableView;
class QAction;
class QHBoxLayout;

class ZPredictorTableModel;
//========================================================
class ZPredictorTableWidget : public QWidget
{
   Q_OBJECT
public:
   explicit ZPredictorTableWidget(QWidget *parent = 0);

   // FUNCS
   void zp_setModel(ZPredictorTableModel* model);
   void zp_appendButtonActions(QList<QAction*>);

signals:

public slots:




private:

   // VARS
   QTableView* zv_table;
   QHBoxLayout* zv_buttonLayout;
   // FUNCS
   void zh_createComponents();
   void zh_createConnections();


};
//========================================================
#endif // ZPREDICTORTABLEWIDGET_H

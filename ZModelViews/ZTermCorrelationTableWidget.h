//========================================================
#ifndef ZTERMCORRELATIONTABLEWIDGET_H
#define ZTERMCORRELATIONTABLEWIDGET_H
//========================================================
#include <QWidget>
#include <QModelIndex>

//========================================================
class QTableView;
class QAction;
class QHBoxLayout;
class QVBoxLayout;
class ZTermCorrelationTableModel;
//========================================================
class ZTermCorrelationTableWidget : public QWidget
{
   Q_OBJECT
public:
   explicit ZTermCorrelationTableWidget(QWidget *parent = 0);

   // FUNCS
   void zp_setModel(ZTermCorrelationTableModel* model);
   void zp_appendButtonActions(QList<QAction*>);
   void zp_setMargin(int margin);

signals:

   void zg_userChangesTermState(int termLogIndex);

public slots:


private slots:

   void zh_editNext(QModelIndex editedIndex);

private:

   // VARS
   QTableView* zv_table;
   QHBoxLayout* zv_buttonLayout;
   QVBoxLayout* zv_mainLayout;
   // FUNCS
   void zh_createComponents();
   void zh_createConnections();

};
//========================================================
#endif // ZTERMCORRELATIONTABLEWIDGET_H

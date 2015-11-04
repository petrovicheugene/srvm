//=============================================================
#ifndef ZSPECTRUMTABLEWIDGET_H
#define ZSPECTRUMTABLEWIDGET_H
//=============================================================
#include <QWidget>
#include <QModelIndex>
//=============================================================
class QTableView;
class ZJointSpectraModel;
class QAction;
class QHBoxLayout;
//=============================================================
class ZJointSpectrumTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZJointSpectrumTableWidget(QWidget *parent = 0);

    // FUNCS
   void zp_setModel(ZJointSpectraModel* model);
   void zp_appendButtonActions(QList<QAction*>);

signals:

public slots:

    void zp_selectedSpectrumIndexList(QList<int>&);

private:

    // VARS
    QTableView* zv_table;
    QHBoxLayout* zv_buttonLayout;
    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

private slots:

    void zp_editNext(QModelIndex editedIndex);

};
//=============================================================
#endif // ZSPECTRUMTABLEWIDGET_H

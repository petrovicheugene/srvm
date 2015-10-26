//=============================================================
#ifndef ZSPECTRUMTABLEWIDGET_H
#define ZSPECTRUMTABLEWIDGET_H
//=============================================================
#include <QWidget>
//=============================================================
class QTableView;
class QAbstractItemModel;
class QAction;
class QHBoxLayout;
//=============================================================
class ZSpectrumTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZSpectrumTableWidget(QWidget *parent = 0);

    // FUNCS
   void zp_setModel(QAbstractItemModel* model);
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

};
//=============================================================
#endif // ZSPECTRUMTABLEWIDGET_H

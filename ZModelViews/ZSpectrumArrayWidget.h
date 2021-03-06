//==============================================================
#ifndef ZSPECTRUMARRAYWIDGET_H
#define ZSPECTRUMARRAYWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class QTableView;
class ZArrayModel;
class QAction;
class QHBoxLayout;
class QVBoxLayout;
class ZSpectrumArrayRepository;
//==============================================================
class ZSpectrumArrayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZSpectrumArrayWidget(QWidget *parent = 0);

    // FUNCS
   void zp_setModel(ZArrayModel* model);
   void zp_appendButtonActions(QList<QAction*>);
   void zp_appendContextMenuActions(QList<QAction*> actionList);

   void zp_connectToSpectrumArrayRepository(ZSpectrumArrayRepository*);
   void zp_setMainLayoutMargin(int margin);

signals:

   void zg_currentArrayChanged(int, int);

public slots:

    void zp_setCurrentArrayIndex(int);
    void zp_currentArrayIndex(int&);
    void zp_startCurrentArrayEdition();

private:

    // VARS
    QTableView* zv_table;
    QHBoxLayout* zv_buttonLayout;
    QVBoxLayout* zv_mainLayout;
    QList<QAction*> zv_contextMenuActionList;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_checkCurrentArray();

private slots:

    void zh_onCurrentArrayChanged(const QModelIndex & current,
                                  const QModelIndex & previous);
    void zh_onContextMenuRequest(const QPoint &pos);

};
//==============================================================
#endif // ZSPECTRUMARRAYWIDGET_H

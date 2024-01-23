//==============================================================
#ifndef X_SPECTRUMARRAYWIDGET_H
#define X_SPECTRUMARRAYWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class QTableView;
class X_ArrayModel;
class QAction;
class QHBoxLayout;
class QVBoxLayout;
class X_SpectrumArrayRepository;
//==============================================================
class X_SpectrumArrayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit X_SpectrumArrayWidget(QWidget *parent = 0);

    // FUNCS
   void xp_setModel(X_ArrayModel* model);
   void xp_appendButtonActions(QList<QAction*>);
   void xp_appendContextMenuActions(QList<QAction*> actionList);

   void xp_connectToSpectrumArrayRepository(X_SpectrumArrayRepository*);
   void xp_setMainLayoutMargin(int margin);

signals:

   void xg_currentArrayChanged(int, int);

public slots:

    void xp_setCurrentArrayIndex(int);
    void xp_currentArrayIndex(int&);
    void xp_startCurrentArrayEdition();

private:

    // VARS
    QTableView* xv_table;
    QHBoxLayout* xv_buttonLayout;
    QVBoxLayout* xv_mainLayout;
    QList<QAction*> xv_contextMenuActionList;

    // FUNCS
    void xh_createComponents();
    void xh_createConnections();
    void xh_checkCurrentArray();

private slots:

    void xh_onCurrentArrayChanged(const QModelIndex & current,
                                  const QModelIndex & previous);
    void xh_onContextMenuRequest(const QPoint &pos);

};
//==============================================================
#endif // X_SPECTRUMARRAYWIDGET_H

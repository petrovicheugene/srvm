//=============================================================
#ifndef ZSPECTRUMTABLEWIDGET_H
#define ZSPECTRUMTABLEWIDGET_H
//=============================================================
#include <QWidget>
#include <QModelIndex>
//=============================================================
class QTableView;
class QAction;
class QHBoxLayout;
class QVBoxLayout;

class ZJointSpectraModel;
class ZSpectrumArrayRepository;
//=============================================================
class ZJointSpectrumTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZJointSpectrumTableWidget(QWidget *parent = 0);

    // FUNCS
    void zp_setModel(ZJointSpectraModel* model);
    void zp_appendButtonActions(QList<QAction*>);

    void zp_connectToSpectrumArrayRepository(ZSpectrumArrayRepository*);
    void zp_setMargin(int margin);

signals:

    void zg_currentSpectrumChanged(int, int);

public slots:

    void zp_selectedSpectrumIndexList(QList<int>&);
    void zp_currentIndex(QModelIndex& ) const;

private:

    // VARS
    QTableView* zv_table;
    QHBoxLayout* zv_buttonLayout;
    QVBoxLayout* zv_mainLayout;

    QList<QAction*> zv_buttonActionList;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

private slots:

    void zh_editNext(QModelIndex editedIndex);
    void zh_onCurrentSpectrumChanged(const QModelIndex & current,
                                     const QModelIndex & previous);

    void zh_onContextMenuRequest(const QPoint &pos);
};
//=============================================================
#endif // ZSPECTRUMTABLEWIDGET_H

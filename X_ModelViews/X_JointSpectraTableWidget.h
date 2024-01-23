//=============================================================
#ifndef X_SPECTRUMTABLEWIDGET_H
#define X_SPECTRUMTABLEWIDGET_H
//=============================================================
#include <QWidget>
#include <QModelIndex>
#include <QItemSelection>
//=============================================================
class QTableView;
class QAction;
class QHBoxLayout;
class QVBoxLayout;

class X_JointSpectraModel;
class X_SpectrumArrayRepository;
class X_CalibrationRepository;
//=============================================================
class X_JointSpectrumTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit X_JointSpectrumTableWidget(QWidget *parent = 0);

    // FUNCS
    void xp_setModel(X_JointSpectraModel* model);
    void xp_appendButtonActions(QList<QAction*>);
    void xp_appendContextMenuActions(QList<QAction*> actionList);

    void xp_connectToSpectrumArrayRepository(X_SpectrumArrayRepository*);
    void xp_connectToCalibrationRepository(X_CalibrationRepository*);

    void xp_setMainLayoutMargin(int margin);

signals:

    void xg_currentSpectrumChanged(int, int) const;
    void xg_selectedIndexListChanged(QModelIndexList) const;

public slots:

    void xp_setCurrentSpectrumIndex(int spectrumIndex);
    void xp_selectedSpectrumIndexList(QList<int>&);
    void xp_currentIndex(QModelIndex& ) const;
    void xp_currentSpectrumRow(int& row, bool* ok) const;

private:

    // VARS
    QTableView* xv_table;
    QHBoxLayout* xv_buttonLayout;
    QVBoxLayout* xv_mainLayout;

    QList<QAction*> xv_contextMenuActionList;

    // FUNCS
    void xh_createComponents();
    void xh_createConnections();

private slots:

    void xh_editNext(QModelIndex editedIndex);
    void xh_onCurrentSpectrumChanged(const QModelIndex & current,
                                     const QModelIndex & previous);

    void xh_onContextMenuRequest(const QPoint &pos);
    void xh_onSelectionChange(const QItemSelection & selected, const QItemSelection & deselected) const;
};
//=============================================================
#endif // X_SPECTRUMTABLEWIDGET_H

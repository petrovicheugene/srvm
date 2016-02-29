//==============================================================
#ifndef ZCALIBRATIONARRAYWIDGET_H
#define ZCALIBRATIONARRAYWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class QTableView;
class QAbstractItemModel;
class QHBoxLayout;
class QVBoxLayout;

//==============================================================
class ZChemElementWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZChemElementWidget(QWidget *parent = 0);

    // FUNCS
    void zp_setModel(QAbstractItemModel* model);
    void zp_appendButtonActions(QList<QAction*>);
    void zp_setMargin(int margin);

signals:

    void zg_currentCalibrationChanged(int, int);

public slots:

    void zp_setCurrentChemElementIndex(int);
    void zp_currentChemElementIndex(int&) const;
    void zp_startCurrentChemElementEdition();
    void zp_selectedChemElementIndexList(QList<int>& selectedIndexList) const;

private:

    // VARS
    QTableView* zv_table;
    QHBoxLayout* zv_buttonLayout;
    QVBoxLayout* zv_mainLayout;

    // FUNCS
    void zh_createComponents();

private slots:

    void zh_onCurrentElementChanged(const QModelIndex & current,
                                    const QModelIndex & previous);


};
//==============================================================
#endif // ZCALIBRATIONARRAYWIDGET_H

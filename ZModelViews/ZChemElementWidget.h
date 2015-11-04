//==============================================================
#ifndef ZCALIBRATIONARRAYWIDGET_H
#define ZCALIBRATIONARRAYWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class QTableView;
class QAbstractItemModel;

//==============================================================
class ZChemElementWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZChemElementWidget(QWidget *parent = 0);

    // FUNCS
    void zp_setModel(QAbstractItemModel* model);

signals:

    void zg_currentCalibrationChanged(int, int);

public slots:


private:

    // VARS
    QTableView* zv_table;

    // FUNCS
    void zh_createComponents();

private slots:

    void zh_onCurrentElementChanged(const QModelIndex & current,
                                    const QModelIndex & previous);


};
//==============================================================
#endif // ZCALIBRATIONARRAYWIDGET_H

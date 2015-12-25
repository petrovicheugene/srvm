//==============================================================
#ifndef ZJOINTCALIBRATIONWINDOWTABLEWIDGET_H
#define ZJOINTCALIBRATIONWINDOWTABLEWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class ZCalibrationWindowModel;
class ZIntDelegate;
class QTableView;
class QHBoxLayout;
class QAction;
//==============================================================
class ZJointCalibrationWindowTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZJointCalibrationWindowTableWidget(QWidget *parent = 0);

    // FUNCS
    void zp_setModel(ZCalibrationWindowModel* model);
    void zp_appendButtonActions(QList<QAction*>);

signals:

public slots:

    void zp_selectedSpectrumWindowIndexList(QList<int>&);
    void zp_setChannelNumberMinMax(int intensity, int channels);

private:
    // VARS
    QTableView* zv_table;
    ZIntDelegate* zv_channelDelegate;
    QHBoxLayout* zv_buttonLayout;
    const int zv_defaultChannelCount = 2048;

    // FUNCS
    void zh_createComponents();

};
//==============================================================
#endif // ZJOINTCALIBRATIONWINDOWTABLEWIDGET_H

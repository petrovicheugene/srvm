//==============================================================
#ifndef ZJOINTCALIBRATIONWINDOWTABLEWIDGET_H
#define ZJOINTCALIBRATIONWINDOWTABLEWIDGET_H
//==============================================================
#include <QWidget>
//==============================================================
class ZCalibrationWindowModel;
class ZChannelNumberDelegate;
class ZCalibrationRepository;
class QTableView;
class QHBoxLayout;
class QAction;
//==============================================================
class ZCalibrationWindowTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZCalibrationWindowTableWidget(QWidget *parent = 0);

    // FUNCS
    void zp_setModel(ZCalibrationWindowModel* model);
    void zp_appendButtonActions(QList<QAction*>);

    void zp_connectToCalibrationRepository(ZCalibrationRepository*);

signals:

    void zg_currentCalibrationWindowChanged(int, int);

public slots:

    void zp_selectedSpectrumWindowIndexList(QList<int>&);
    void zp_setChannelNumberMinMax(int intensity, int channels);

private:
    // VARS
    QTableView* zv_table;
    ZChannelNumberDelegate* zv_channelDelegate;
    QHBoxLayout* zv_buttonLayout;
    const int zv_defaultChannelCount = 2048;

    // FUNCS
    void zh_createComponents();

private slots:
    void zh_onCurrentCalibrationWindowChanged(const QModelIndex & current,
                                     const QModelIndex & previous);

};
//==============================================================
#endif // ZJOINTCALIBRATIONWINDOWTABLEWIDGET_H

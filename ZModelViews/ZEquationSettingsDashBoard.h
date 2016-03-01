//========================================================
#ifndef ZEQUATIONSETTINGSPANELWIDGET_H
#define ZEQUATIONSETTINGSPANELWIDGET_H
//========================================================
#include <QWidget>
//========================================================
class QPushButton;
class QToolButton;
class QLineEdit;
class QComboBox;
class QLabel;

class ZNumericEditor;
class ZCalibrationRepository;
//========================================================
class ZEquationSettingsDashBoard : public QWidget
{
    Q_OBJECT
public:
    explicit ZEquationSettingsDashBoard(QWidget *parent = 0);

    void zp_connectToCalibrationRepository(ZCalibrationRepository*);

signals:

public slots:

private slots:

    void zh_onCurrentCalibrationChange(qreal calibrationId, int calibrationIndex);
    void zh_onNormaTypeChange(int index);

private:

    // VARS
    ZCalibrationRepository* zv_calibrationRepository;
    qint64 zv_currentCalibrationId;
    int zv_currentCalibrationIndex;

    QComboBox* zv_normalizerTypeComboBox;
    QLineEdit* zv_customNormalizerLineEdit;

    QPushButton* zv_equationChooseButton;
    QLabel* zv_equationLabel;

    ZNumericEditor* zv_freeMemberNumericEditor;
    ZNumericEditor* zv_freeFactorNumericEditor;

    QPushButton* zv_recalcFactorsButton;
    QPushButton* zv_toStartButton;
    QPushButton* zv_stepBackwardButton;
    QPushButton* zv_stepForwardButton;
    QPushButton* zv_toEndButton;


    // FUNCS

    void zh_createComponents();
    QWidget* zh_createEquationWidget();
    QWidget* zh_createNormalizerWidget();
    QWidget* zh_createFreeMemeberWidget();

    void zh_createConnections();



};
//========================================================
#endif // ZEQUATIONSETTINGSPANELWIDGET_H

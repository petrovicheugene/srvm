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
class ZNumericEditor;
//========================================================
class ZEquationSettingsDashBoard : public QWidget
{
    Q_OBJECT
public:
    explicit ZEquationSettingsDashBoard(QWidget *parent = 0);

signals:

public slots:



private:

    // VARS
    QComboBox* zv_normalizerTypeComboBox;
    QLineEdit* zv_customNormalizerLineEdit;
    QToolButton* zv_checkCustomNormalizerButton;

    QComboBox* zv_equationTypeComboBox;

    ZNumericEditor* zv_freeMemberNumericEditor;
    ZNumericEditor* zv_freeFactorNumericEditor;

    QPushButton* zv_recalcFactorsButton;
    QPushButton* zv_toStartButton;
    QPushButton* zv_stepBackwardButton;
    QPushButton* zv_stepForwardButton;
    QPushButton* zv_toEndButton;


    // FUNCS

    void zh_createComponents();
    void zh_createConnections();

};
//========================================================
#endif // ZEQUATIONSETTINGSPANELWIDGET_H

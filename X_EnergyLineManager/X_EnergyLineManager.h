//======================================================
#ifndef X_ENERGYLINEMANAGER_H
#define X_ENERGYLINEMANAGER_H
//======================================================
#include <QObject>


#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
//======================================================
class X_EnergyLineManager : public QObject
{
    Q_OBJECT
public:
    explicit X_EnergyLineManager(QObject *parent = nullptr);

signals:

    void xg_lineOperation() const;

public slots:


private slots:

    void zt_setLine();

private:

    // VARS


    // FUNCS

    void zt_createTestWidget();
    QDoubleSpinBox* zt_channelSpinBox;
    QLineEdit* zt_symbolLineEdit;
    QPushButton* zt_setLineButton;
};
//======================================================
#endif // X_ENERGYLINEMANAGER_H

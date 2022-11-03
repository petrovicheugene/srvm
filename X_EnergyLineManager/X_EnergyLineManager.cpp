//======================================================
#include "X_EnergyLineManager.h"
#include <QDebug>
//======================================================
X_EnergyLineManager::X_EnergyLineManager(QObject *parent) : QObject(parent)
{
    zt_createTestWidget();
}
//======================================================
void X_EnergyLineManager::zt_createTestWidget()
{
    QWidget* w = new QWidget;

    QVBoxLayout* mainLayout = new QVBoxLayout;
    w->setLayout(mainLayout);

    zt_channelSpinBox = new QDoubleSpinBox(w);
    mainLayout->addWidget(zt_channelSpinBox);

    zt_symbolLineEdit = new QLineEdit(w);
    mainLayout->addWidget(zt_symbolLineEdit);

    zt_setLineButton = new QPushButton(w);
    mainLayout->addWidget(zt_setLineButton);

    w->show();

    connect(zt_setLineButton, &QPushButton::clicked,
            this, &X_EnergyLineManager::zt_setLine);


}
//======================================================
void X_EnergyLineManager::zt_setLine()
{

}
//======================================================

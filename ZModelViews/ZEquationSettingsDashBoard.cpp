//========================================================
#include "ZEquationSettingsDashBoard.h"
#include "ZNumericEditor.h"

#include <QPushButton>
#include <QToolButton>

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

//========================================================
ZEquationSettingsDashBoard::ZEquationSettingsDashBoard(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
}
//========================================================
void ZEquationSettingsDashBoard::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    // Normalizer
    QHBoxLayout* normalizerLayout = new QHBoxLayout(this);
    mainLayout->addLayout(normalizerLayout);
    QLabel* label = new QLabel(this);
    label->setText(tr("Norma:"));
    normalizerLayout->addWidget(label);

    QStringList normaTypes;
    normaTypes << "None" << "Coherent" << "Incoherent"
               << "Coherent / Incoherent" << "Incoherent / Coherent" << "Custom";
    zv_normalizerTypeComboBox = new QComboBox(this);
    zv_normalizerTypeComboBox->insertItems(0, normaTypes);
    label->setBuddy(zv_normalizerTypeComboBox);
    normalizerLayout->addWidget(zv_normalizerTypeComboBox);

    zv_customNormalizerLineEdit = new QLineEdit(this);
    normalizerLayout->addWidget(zv_customNormalizerLineEdit);

    zv_checkCustomNormalizerButton = new QToolButton(this);
    normalizerLayout->addWidget(zv_checkCustomNormalizerButton);

    // Equation
    QHBoxLayout* equationLayout = new QHBoxLayout(this);
    mainLayout->addLayout(equationLayout);

    QStringList equationTypes;
    equationTypes << QString("C=b*T<sub>base</sub> %1()").arg(QString(QChar(0x03A3)));
    equationTypes << QString("C= b*%1(T<sub>i</sub>*F<sub>i</sub>)+a").arg(QString(QChar(0x03A3)));

    zv_equationTypeComboBox = new QComboBox(this);
    zv_equationTypeComboBox->insertItems(0, equationTypes);
    equationLayout->addWidget(zv_equationTypeComboBox);

    // Free members
    QHBoxLayout* freeMemberLayout = new QHBoxLayout(this);
    mainLayout->addLayout(freeMemberLayout);

    label = new QLabel(this);
    label->setText(tr("Free memeber:"));
    freeMemberLayout->addWidget(label);

    zv_freeMemberNumericEditor = new ZNumericEditor(this);
    label->setBuddy(zv_freeMemberNumericEditor);
    freeMemberLayout->addWidget(zv_freeMemberNumericEditor);

    label = new QLabel(this);
    label->setText(tr("Free factor:"));
    freeMemberLayout->addWidget(label);

    zv_freeFactorNumericEditor = new ZNumericEditor(this);
    label->setBuddy(zv_freeFactorNumericEditor);
    freeMemberLayout->addWidget(zv_freeFactorNumericEditor);


    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);
    zv_recalcFactorsButton = new QPushButton("Fuck", this);
    buttonBox ->addButton(zv_recalcFactorsButton, QDialogButtonBox::ActionRole);

    zv_toStartButton = new QPushButton("<<", this);
    buttonBox ->addButton(zv_toStartButton, QDialogButtonBox::ActionRole);
    zv_stepBackwardButton = new QPushButton("<", this);
    buttonBox ->addButton(zv_stepBackwardButton, QDialogButtonBox::ActionRole);
    zv_stepForwardButton = new QPushButton(">", this);
    buttonBox ->addButton(zv_stepForwardButton, QDialogButtonBox::ActionRole);
    zv_toEndButton = new QPushButton(">>", this);
    buttonBox ->addButton(zv_toEndButton, QDialogButtonBox::ActionRole);

}
//========================================================
void ZEquationSettingsDashBoard::zh_createConnections()
{

}
//========================================================


//========================================================
#include "ZEquationSettingsDashBoard.h"
#include "ZNumericEditor.h"
#include "globalVariables.h"
#include "ZTermNormalizer.h"
#include "ZCalibrationRepository.h"
#include "ZEquationChooseDialog.h"

#include <QPushButton>
#include <QToolButton>

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QDialogButtonBox>
//========================================================
ZEquationSettingsDashBoard::ZEquationSettingsDashBoard(QWidget *parent) : QWidget(parent)
{
    zv_calibrationRepository = 0;
    zv_currentCalibrationId = -1;
    zv_currentCalibrationIndex = -1;

    zh_createComponents();
    zh_createConnections();
}
//========================================================
void ZEquationSettingsDashBoard::zp_connectToCalibrationRepository(ZCalibrationRepository* calibrationRepository)
{
    zv_calibrationRepository = calibrationRepository;
//    connect(zv_calibrationRepository, &ZCalibrationRepository::zg_currentCalibrationChanged,
//            this, &ZEquationSettingsDashBoard::zh_onCurrentCalibrationChange);
}
//========================================================
void ZEquationSettingsDashBoard::zh_onCurrentCalibrationChange(qreal calibrationId, int calibrationIndex)
{
    if(!zv_calibrationRepository || (zv_currentCalibrationId == calibrationId && zv_currentCalibrationIndex == calibrationIndex ))
    {
        return;
    }

    zv_currentCalibrationId = calibrationId;
    zv_currentCalibrationIndex = calibrationIndex;

    QString normaTypeString = ZTermNormalizer::zp_normaTypeString( zv_calibrationRepository->zp_normaTypeForCalibrationId(calibrationId));
    if(normaTypeString.isEmpty())
    {
        zv_normalizerTypeComboBox->setCurrentIndex(0);
    }

    int index = zv_normalizerTypeComboBox->findText(normaTypeString);
    if(index < 0 )
    {
        index = 0;
    }

    if(zv_normalizerTypeComboBox->currentIndex() != index)
    {
        zv_normalizerTypeComboBox->setCurrentIndex(index);
    }
}
//========================================================
void ZEquationSettingsDashBoard::zh_onNormaTypeChange(int index)
{
    if(!zv_calibrationRepository)
    {
        return;
    }

    QString typeString = zv_normalizerTypeComboBox->itemText(index);
    ZTermNormalizer::NormaType newType = ZTermNormalizer::zp_normaTypeForString(typeString);
    zv_calibrationRepository->zp_setNormaTypeForCalibrationId(zv_currentCalibrationId, newType);
}
//========================================================
void ZEquationSettingsDashBoard::zh_onEquationTypeChange()
{
    ZEquationChooseDialog dialog;
    if(dialog.exec() == QDialog::Rejected)
    {
        return;
    }

#ifdef DBG
    qDebug() << "ACCEPTED";
#endif
}
//========================================================
void ZEquationSettingsDashBoard::zh_createComponents()
{
    // layouts
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    QHBoxLayout* sectionLayout = new QHBoxLayout(this);
    mainLayout->addLayout(sectionLayout);

    sectionLayout->addStretch();
    sectionLayout->addWidget(zh_createEquationWidget());
    sectionLayout->addWidget(zh_createNormalizerWidget());
    sectionLayout->addWidget(zh_createFreeMemeberWidget());

    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);
    zv_recalcFactorsButton = new QPushButton("Calc", this);
    zv_recalcFactorsButton->setToolTip(tr("Recalculate equation factors"));
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
QWidget* ZEquationSettingsDashBoard::zh_createEquationWidget()
{
    QWidget* w = new QWidget(this);
    QGridLayout* layout = new QGridLayout(this);
    layout->setMargin(0);
    w->setLayout(layout);

    QLabel* label = new QLabel(w);
    label->setText(QString("<b>%1<b>").arg(tr("Equation type:")));
    zv_equationTypeChooseButton = new QPushButton(this);
    zv_equationTypeChooseButton->setText("...");

    zv_equationLabel = new QLabel(this);

    layout->addWidget(label, 0,0,1,1, Qt::AlignLeft);
    layout->addWidget(zv_equationTypeChooseButton, 0,1,1,1, Qt::AlignLeft);
    layout->addWidget(zv_equationLabel, 1,0,1,2, Qt::AlignLeft);

    return w;
}
//========================================================
QWidget* ZEquationSettingsDashBoard::zh_createNormalizerWidget()
{
    QWidget* w = new QWidget(this);
    QGridLayout* layout = new QGridLayout(this);
    layout->setMargin(0);
    w->setLayout(layout);

    QLabel* typeLabel = new QLabel(w);
    typeLabel->setText(QString("<b>%1<b>").arg(tr("Normalizer:")));

    zv_normalizerTypeComboBox = new QComboBox(w);
    QStringList normaTypes = ZTermNormalizer::zp_normaTypeList();
    zv_normalizerTypeComboBox->insertItems(0, normaTypes);

    QLabel* customLabel = new QLabel(w);
    customLabel->setText(QString("<b>%1<b>").arg(tr("Custom norma:")));

    zv_customNormalizerLineEdit = new QLineEdit(w);

    layout->addWidget(typeLabel, 0,0,1,1, Qt::AlignLeft);
    layout->addWidget(zv_normalizerTypeComboBox, 0,1,1,1, Qt::AlignLeft);
    layout->addWidget(customLabel, 1,0,1,1, Qt::AlignLeft);
    layout->addWidget(zv_customNormalizerLineEdit, 1,1,1,1, Qt::AlignLeft);

    return w;
}
//========================================================
QWidget* ZEquationSettingsDashBoard::zh_createFreeMemeberWidget()
{
    QWidget* w = new QWidget(this);
    QGridLayout* layout = new QGridLayout(this);
    layout->setMargin(0);
    w->setLayout(layout);

    QLabel* freeMemberLabel = new QLabel(w);
    freeMemberLabel->setText(QString("<b>%1<b>").arg(tr("Free member:")));

    zv_freeMemberNumericEditor = new ZNumericEditor(w);
    zv_freeMemberNumericEditor->setMinMax(9.9e99, -9.9e99);
    // zv_freeMemberNumericEditor->setMinimumWidth(zv_freeMemberNumericEditor->sizeHint().width());
    layout->addWidget(freeMemberLabel, 0,0,1,1, Qt::AlignLeft);
    layout->addWidget(zv_freeMemberNumericEditor, 0,1,1,1);

#ifdef DBG
    qDebug() << "SIZE" << zv_freeMemberNumericEditor->sizeHint();
#endif

    QLabel* freeFactorLabel = new QLabel(w);
    freeFactorLabel->setText(QString("<b>%1<b>").arg(tr("Free factor:")));
    zv_freeFactorNumericEditor = new ZNumericEditor(w);
    zv_freeFactorNumericEditor->setMinMax(9.9e99, -9.9e99);

    // zv_freeFactorNumericEditor->setMinimumWidth(zv_freeFactorNumericEditor->sizeHint().width());
    layout->addWidget(freeFactorLabel, 1,0,1,1, Qt::AlignLeft);
    layout->addWidget(zv_freeFactorNumericEditor, 1,1,1,1);

    return w;
}
//========================================================
void ZEquationSettingsDashBoard::zh_createConnections()
{
    connect(zv_normalizerTypeComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(zh_onNormaTypeChange(int)));
    connect(zv_equationTypeChooseButton, &QPushButton::clicked,
            this, &ZEquationSettingsDashBoard::zh_onEquationTypeChange);

}
//========================================================


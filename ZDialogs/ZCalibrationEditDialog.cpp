//===========================================================
#include "ZCalibrationEditDialog.h"
#include "ZAbstractCalibration.h"
#include "glVariables.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QCursor>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>
//===========================================================
ZCalibrationEditDialog::ZCalibrationEditDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle(tr("Calibration"));
//    Qt::WindowFlags flags;
//    flags |= Qt::Tool;
//    setWindowFlags(flags);
    zh_createComponents();
    zh_createConnections();
    zh_fillComboBox();
    //    QPoint dialogPoint;
    //    dialogPoint = QCursor::pos();
    //    this->move(dialogPoint);

}
//===========================================================
QString ZCalibrationEditDialog::zp_calibrationFullName()
{
    QString name = zv_nameLineEdit->text() + "." + zv_typeComboBox->currentText();
    return name;
}
//===========================================================
void ZCalibrationEditDialog::zh_createComponents()
{
    // layouts
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    QVBoxLayout* nameLayout = new QVBoxLayout(this);
    mainLayout->addLayout(nameLayout);

    QVBoxLayout* typeLayout = new QVBoxLayout(this);
    mainLayout->addLayout(typeLayout);

    // name
    QLabel* label = new QLabel(this);
    label->setText(QString("<b><font color=darkgreen>%1</font></b>").arg(tr("Calibration name:")));
    nameLayout->addWidget(label);

    zv_nameLineEdit = new QLineEdit(this);
    nameLayout->addWidget(zv_nameLineEdit);
    label->setBuddy(zv_nameLineEdit);

    // type
    label = new QLabel(this);
    label->setText(QString("<b><font color=darkgreen>%1</font></b>").arg(tr("Calibration Type:")));
    typeLayout->addWidget(label);

    zv_typeComboBox = new QComboBox(this);
    typeLayout->addWidget(zv_typeComboBox);
    label->setBuddy(zv_typeComboBox);

    // basement
    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    // buttons
    zv_okButton = new QPushButton(this);
    zv_okButton->setText(tr("OK"));
    buttonBox->addButton(zv_okButton, QDialogButtonBox::ActionRole);

    zv_cancelButton = new QPushButton(this);
    zv_cancelButton->setText(tr("Cancel"));
    buttonBox->addButton(zv_cancelButton, QDialogButtonBox::ActionRole);

}
//===========================================================
void ZCalibrationEditDialog::zh_createConnections()
{
    connect(zv_cancelButton, &QPushButton::clicked,
            this, &ZCalibrationEditDialog::reject);
    connect(zv_okButton, &QPushButton::clicked,
            this, &ZCalibrationEditDialog::onOkButtonClick);

}
//===========================================================
void ZCalibrationEditDialog::zh_fillComboBox(QString currentSuffix)
{
    foreach(QString suffix, ZAbstractCalibration::suffixMap.values())
    {
        if(suffix.isEmpty())
        {
            continue;
        }
        zv_typeComboBox->addItem(suffix);
    }

    QString textToFind;
    if(currentSuffix.isEmpty())
    {
        QSettings settings;
        QVariant vData;
        settings.beginGroup(glAppVersion);
        settings.beginGroup("Common");
        vData = settings.value("preferredCalibrationType");
        if(vData.isValid() && !vData.isNull() && vData.canConvert<QString>())
        {
            textToFind = vData.toString();
        }
        settings.endGroup();
        settings.endGroup();
    }
    else
    {
        textToFind = currentSuffix;
    }

    int currentIndex = zv_typeComboBox->findText(textToFind);
    //    if(currentIndex < 0 || currentIndex >= zv_typeComboBox->count())
    //    {
    //        currentIndex = 0;
    //    }

    zv_typeComboBox->setCurrentIndex(currentIndex);

}
//===========================================================
bool ZCalibrationEditDialog::zh_checkData()
{
    bool res = true;
    QString msg = tr("You should fill following fields:");
    if(zv_nameLineEdit->text().isEmpty())
    {
        msg += QString("<br>- %1").arg(tr("\"Calibration name\""));
        res = false;
    }

    if(zv_typeComboBox->currentText().isEmpty())
    {
        if(!res)
        {
            msg += ";";
        }
        msg += QString("<br>- %1").arg(tr("\"Calibration type\"."));
        res = false;
    }

    if(!res)
    {
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
    }

    return res;



}
//===========================================================
void ZCalibrationEditDialog::onOkButtonClick()
{
    QSettings settings;
    settings.beginGroup(glAppVersion);
    settings.beginGroup("Common");
    settings.setValue("preferredCalibrationType", QVariant(zv_typeComboBox->currentText()));
    settings.endGroup();
    settings.endGroup();

    if(!zh_checkData())
    {
        return;
    }

    accept();

}
//===========================================================

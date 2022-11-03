//======================================================
#include "X_PropertyEditDialog.h"

#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
//======================================================
X_PropertyEditDialog::X_PropertyEditDialog(QWidget *parent)
    : X_BaseDialog("X_PropertyEditDialog", parent)
{
    setWindowTitle(qApp->applicationDisplayName());

    xh_createComponents();
    xh_createConnections();
}
//======================================================
void X_PropertyEditDialog::xh_createComponents()
{
    xh_addWidgetToMainLayout(xh_createControlWidget());
    xh_addStretchToMainLayout();


    xv_okButton = xh_createBasementButton(tr("Ok"),
                                          tr("Save section and close dialog"),
                                          QIcon());
    xv_cancelButton = xh_createBasementButton(tr("Cancel"),
                                              tr("Close dialog without savimg section"),
                                              QIcon());

}
//======================================================
void X_PropertyEditDialog::xh_createConnections()
{
    connect(xv_okButton, &QPushButton::clicked,
            this, &X_PropertyEditDialog::xh_onOkButtonClick);
    connect(xv_cancelButton, &QPushButton::clicked,
            this, &X_PropertyEditDialog::xh_onCloseButtonClick);

}
//======================================================
QWidget* X_PropertyEditDialog::xh_createControlWidget()
{
    QWidget* widget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    widget->setLayout(mainLayout);

    QLabel* label = new QLabel(this);
    label->setText(tr("Chemical property name:"));
    mainLayout->addWidget(label);

    xv_propertyNameLineEdit =  new QLineEdit(this);
    xv_propertyNameLineEdit->installEventFilter(this);
    mainLayout->addWidget(xv_propertyNameLineEdit);


    label = new QLabel(this);
    label->setText(tr("Chemical property value:"));
    mainLayout->addWidget(label);

    xv_propertyValueLineEdit =  new QLineEdit(this);
    xv_propertyValueLineEdit->installEventFilter(this);
    mainLayout->addWidget(xv_propertyValueLineEdit);

    return widget;
}
//======================================================
void X_PropertyEditDialog::xh_onOkButtonClick()
{
    // check data
    if( xp_propertyName().isEmpty())
    {
        QString msg = tr("Input property name!");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        return;
    }

    accept();
}
//======================================================
void X_PropertyEditDialog::xh_onCloseButtonClick()
{
    reject();
}
//======================================================
QString X_PropertyEditDialog::xp_propertyName() const
{
    return xv_propertyNameLineEdit->text();
}
//======================================================
QString X_PropertyEditDialog::xp_propertyValue() const
{
    return xv_propertyValueLineEdit->text();
}
//======================================================
QPair<QString, QString> X_PropertyEditDialog::xp_property() const
{
    return QPair<QString, QString>(xp_propertyName(), xp_propertyValue());
}
//======================================================


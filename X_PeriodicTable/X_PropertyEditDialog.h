//======================================================
#ifndef X_PROPERTYEDITDIALOG_H
#define X_PROPERTYEDITDIALOG_H
//======================================================
#include <X_BaseDialog.h>
//======================================================
class QLineEdit;
class QPushButton;
//======================================================
class X_PropertyEditDialog : public X_BaseDialog
{
    Q_OBJECT
public:
    explicit X_PropertyEditDialog(QWidget *parent = nullptr);

    QString xp_propertyName() const;
    QString xp_propertyValue() const;
    QPair<QString, QString> xp_property() const;

signals:

public slots:

private slots:

    void zh_onOkButtonClick();
    void zh_onCloseButtonClick();

private:

    // VARS

    QLineEdit* xv_propertyNameLineEdit;
    QLineEdit* xv_propertyValueLineEdit;

    QPushButton* xv_okButton;
    QPushButton* xv_cancelButton;



    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

    QWidget* zh_createControlWidget();

};
//======================================================
#endif // X_PROPERTYEDITDIALOG_H

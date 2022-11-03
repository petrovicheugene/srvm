//======================================================
#ifndef X_PROPERTYSECTIONDIALOG_H
#define X_PROPERTYSECTIONDIALOG_H
//======================================================
#include <X_BaseDialog.h>
#include <QList>
//======================================================
class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;
//======================================================
class X_PropertySectionEditDialog : public X_BaseDialog
{
    Q_OBJECT
public:
    enum AdditionTarget {AT_ALL,
                         AT_SELECTED,
                         AT_CURRENT};
    explicit X_PropertySectionEditDialog(const QStringList& branch,
                                    bool currentVisible,
                                    bool selectedVisible,
                                    QWidget *parent = nullptr);

    AdditionTarget xp_additionTarget() const;
    QStringList xp_newSectionBranch() const;

    void xp_setCurrentSectionBranch(const QStringList& branch);

signals:

public slots:

private slots:

    void zh_onOkButtonClick();
    void zh_onCloseButtonClick();

private:

    // VARS
    QStringList xv_parentSectionBranch;

    QLabel* xv_parentSectionBranchLabel;
    QLineEdit* xv_sectionLineEdit;

    QPushButton* xv_okButton;
    QPushButton* xv_cancelButton;

    QRadioButton* xv_toCurrentChemicalElementRadioButton;
    QRadioButton* xv_toSelectedChemicalElementsRadioButton;
    QRadioButton* xv_toAllChemicalElementsRadioButton;


    // FUNCS
    void zh_createComponents();
    void zh_createConnections();
    void zh_setUpAdditionTargetButtons(bool selectedVisible,
                bool currentVisible);
    QWidget* zh_createControlWidget();


};
//======================================================
#endif // X_PROPERTYSECTIONDIALOG_H

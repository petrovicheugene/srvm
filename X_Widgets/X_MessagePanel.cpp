//==================================================
#include "X_MessagePanel.h"
#include "X_General.h"

#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDialogButtonBox>
//==================================================
X_MessagePanel::X_MessagePanel(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
}
//==================================================
void X_MessagePanel::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    xv_msgTextEdit = new QTextEdit(this);
    xv_msgTextEdit->setReadOnly(true);

    mainLayout->addWidget(xv_msgTextEdit);

    QDialogButtonBox* buttonBox = new QDialogButtonBox;
    mainLayout->addWidget(buttonBox);

    xv_clearButton = new QPushButton(this);
    xv_clearButton->setIcon(QIcon(":/images/X_Images/clear-8.png"));
//    xv_clearButton->setIconSize(glPanelButtonIconSize);
//    xv_clearButton->setMinimumHeight(glPanelButtonMinHeight);
    xv_clearButton->setToolTip(tr("Clear messages"));
    xv_clearButton->setFlat(true);
    buttonBox->addButton(xv_clearButton, QDialogButtonBox::ActionRole);

}
//==================================================
void X_MessagePanel::zh_createConnections()
{
    connect(xv_clearButton, &QPushButton::clicked,
            xv_msgTextEdit, &QTextEdit::clear);
}
//==================================================
void X_MessagePanel::xp_appendMessage(const QString& msg)
{
    xv_msgTextEdit->append(msg);
}
//==================================================
void X_MessagePanel::xp_appendStandardOutput(const QByteArray& byteArray)
{
    xv_msgTextEdit->append("***");
    xv_msgTextEdit->append(byteArray);
}
//==================================================

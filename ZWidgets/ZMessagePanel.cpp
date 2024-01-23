//==================================================
#include "ZMessagePanel.h"
#include "ZGeneral.h"

#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDialogButtonBox>
//==================================================
ZMessagePanel::ZMessagePanel(QWidget *parent) : QWidget(parent)
{
    zh_createComponents();
    zh_createConnections();
}
//==================================================
void ZMessagePanel::zh_createComponents()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    zv_msgTextEdit = new QTextEdit(this);
    zv_msgTextEdit->setReadOnly(true);

    mainLayout->addWidget(zv_msgTextEdit);

    QDialogButtonBox* buttonBox = new QDialogButtonBox;
    mainLayout->addWidget(buttonBox);

    zv_clearButton = new QPushButton(this);
    zv_clearButton->setIcon(QIcon(":/images/ZImages/clear-8.png"));
//    zv_clearButton->setIconSize(glPanelButtonIconSize);
//    zv_clearButton->setMinimumHeight(glPanelButtonMinHeight);
    zv_clearButton->setToolTip(tr("Clear messages"));
    zv_clearButton->setFlat(true);
    buttonBox->addButton(zv_clearButton, QDialogButtonBox::ActionRole);

}
//==================================================
void ZMessagePanel::zh_createConnections()
{
    connect(zv_clearButton, &QPushButton::clicked,
            zv_msgTextEdit, &QTextEdit::clear);
}
//==================================================
void ZMessagePanel::zp_clearMessages()
{
    zv_msgTextEdit->clear();
}
//==================================================
void ZMessagePanel::zp_appendMessage(const QString& msg)
{
    zv_msgTextEdit->append(msg);
}
//==================================================
void ZMessagePanel::zp_appendStandardOutput(const QByteArray& byteArray)
{
    zv_msgTextEdit->append("***");
    zv_msgTextEdit->append(byteArray);
}
//==================================================

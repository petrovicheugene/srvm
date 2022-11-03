//====================================================
#include "X_BaseDialog.h"

#include <QApplication>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFrame>
#include <QKeyEvent>
#include <QLayout>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>
//====================================================
X_BaseDialog::X_BaseDialog(QWidget *parent) : QDialog(parent)
{
    xh_createBaseComponents();
    xh_restoreSettings();
}
//====================================================
X_BaseDialog::X_BaseDialog(const QString& subclassName,
                         QWidget *parent) : QDialog(parent), xv_settingsGroupName(subclassName)
{
    xh_createBaseComponents();
    xh_restoreSettings();
}
//====================================================
X_BaseDialog::~X_BaseDialog()
{
    xh_saveSettings();
}
//====================================================
bool X_BaseDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(!xh_ctrlXHandling(event))
    {
        return QDialog::eventFilter(watched, event);
    }
    return true;
}
//====================================================
bool X_BaseDialog::event(QEvent *event)
{
    if(!xh_ctrlXHandling(event))
    {
        return QDialog::event(event);
    }
    return true;
}
//====================================================
bool X_BaseDialog::xh_ctrlXHandling(QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if(!keyEvent)
        {
            return false;
        }

        if(keyEvent->key() == Qt::Key_X && (keyEvent->modifiers() & Qt::ControlModifier))
        {
            reject();
            return false;
        }
    }

    return false;
}
//====================================================
void X_BaseDialog::xh_createBaseComponents()
{
    xv_mainLayout = new QVBoxLayout;
    setLayout(xv_mainLayout);

    // basement sparator line
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    xv_mainLayout->addWidget(line);

    // basement
    xv_buttonBox = new QDialogButtonBox(this);
    xv_mainLayout->addWidget(xv_buttonBox);

    xv_baseComponentCount = xv_mainLayout->count();
}
//====================================================
void X_BaseDialog::xh_saveSettings()
{
    QSettings settings;
    if(!xh_openDialogSettingsGroup(&settings))
    {
        return;
    }

    settings.setValue(xv_geometryName, saveGeometry());

    xh_closeDialogSettingsGroup(&settings);
}
//====================================================
void X_BaseDialog::xh_restoreSettings()
{
    QSettings settings;
    if(!xh_openDialogSettingsGroup(&settings))
    {
        return;
    }

    if(settings.value(xv_geometryName).canConvert<QByteArray>())
    {
        restoreGeometry(settings.value(xv_geometryName).toByteArray());
    }

    xh_closeDialogSettingsGroup(&settings);
}
//====================================================
bool X_BaseDialog::xh_openDialogSettingsGroup(QSettings* settings)
{
    if(settings == nullptr)
    {
        return false;
    }
    // open version group
    if(!qApp->applicationVersion().isEmpty())
    {
        settings->beginGroup(qApp->applicationVersion());
    }

    // open dialog group
    if(!xv_settingsGroupName.isEmpty())
    {
        settings->beginGroup(xv_settingsGroupName);
    }
    return true;
}
//====================================================
bool X_BaseDialog::xh_closeDialogSettingsGroup(QSettings *settings)
{
    if(settings == nullptr)
    {
        return false;
    }

    // close groups
    while(!settings->group().isEmpty())
    {
        settings->endGroup();
    }

    return true;
}
//====================================================
void X_BaseDialog::xh_insertLayoutToMainLayout(int index, QLayout* layout,
                                              int stretch)
{
    xv_mainLayout->insertLayout(index, layout, stretch);
}
//====================================================
void X_BaseDialog::xh_insertWidgetToMainLayout(int index, QWidget* widget,
                                              int stretch, Qt::Alignment alignment)
{
    xv_mainLayout->insertWidget(index, widget, stretch, alignment);
}
//====================================================
void X_BaseDialog::xh_addLayoutToMainLayout(QLayout* layout,
                                           int stretch)
{
    xh_insertLayoutToMainLayout(xv_mainLayout->count() - xv_baseComponentCount,
                                layout, stretch);
}
//====================================================
void X_BaseDialog::xh_addWidgetToMainLayout(QWidget* widget, int stretch,
                                           Qt::Alignment alignment)
{
    xh_insertWidgetToMainLayout(xv_mainLayout->count() - xv_baseComponentCount,
                                widget,  stretch, alignment);
}
//====================================================
void X_BaseDialog::xh_addStretchToMainLayout(int stretch)
{
    xv_mainLayout->insertStretch(xv_mainLayout->count() - xv_baseComponentCount,
                                 stretch);
}
//====================================================
QPushButton* X_BaseDialog::xh_createBasementButton(QString text,
                                                  QString toolTip, QIcon icon)
{
    QPushButton* button = new QPushButton(this);
    if(!text.isEmpty())
    {
        button->setText(text);
    }

    if(!toolTip.isEmpty())
    {
        button->setToolTip(toolTip);
    }

    if(!icon.isNull())
    {
        button->setIcon(icon);
    }

    xv_buttonBox->addButton(button, QDialogButtonBox::ActionRole);
    button->setFocusPolicy(Qt::NoFocus);
    return button;
}
//====================================================

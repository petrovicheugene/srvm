//====================================================
#ifndef X_BASEDIALOG_H
#define X_BASEDIALOG_H
//====================================================
#include <QDialog>
#include <QIcon>
//====================================================
class QDialogButtonBox;
class QEvent;
class QSettings;
class QPushButton;
class QVBoxLayout;
//====================================================
class X_BaseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit X_BaseDialog(QWidget *parent = nullptr);
    explicit X_BaseDialog(const QString& settingsGroupName,
                         QWidget *parent = nullptr); // xv_settingsGroupName - subclass name
    ~X_BaseDialog();

    bool eventFilter(QObject *watched, QEvent *event);
    bool event(QEvent *event);

signals:

public slots:

protected:

    // VARS
    const QString xv_settingsGroupName;
    const QString xv_geometryName = "geometry";

    QVBoxLayout* xv_mainLayout;
    QDialogButtonBox* xv_buttonBox;
    int xv_baseComponentCount;

    // FUNCS

    void xh_createBaseComponents();
    virtual void xh_saveSettings();
    virtual void xh_restoreSettings();
    bool xh_openDialogSettingsGroup(QSettings* settings);
    bool xh_closeDialogSettingsGroup(QSettings* settings);

    void xh_insertLayoutToMainLayout(int index,
                                     QLayout* layout,
                                     int stretch = 0);
    void xh_insertWidgetToMainLayout(int index,
                                     QWidget* widget ,
                                     int stretch = 0,
                                     Qt::Alignment alignment = Qt::Alignment());
    void xh_addLayoutToMainLayout(QLayout* layout,
                                  int stretch = 0);
    void xh_addWidgetToMainLayout(QWidget* widget,
                                  int stretch = 0,
                                  Qt::Alignment alignment = Qt::Alignment());
    void xh_addStretchToMainLayout(int stretch = 0);

    QPushButton* xh_createBasementButton(QString text = QString(),
                                         QString toolTip = QString(),
                                         QIcon icon = QIcon());

    bool xh_ctrlXHandling(QEvent *event);

};
//====================================================
#endif // X_BASEDIALOG_H

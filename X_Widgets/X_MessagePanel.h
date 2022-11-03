//==================================================
#ifndef X_MESSAGEPANEL_H
#define X_MESSAGEPANEL_H
//==================================================
#include <QWidget>
//==================================================
class QTextEdit;
class QPushButton;
//==================================================
class X_MessagePanel : public QWidget
{
    Q_OBJECT
public:
    explicit X_MessagePanel(QWidget *parent = 0);

signals:

public slots:

    void xp_appendMessage(const QString&);
    void xp_appendStandardOutput(const QByteArray& byteArray);

private:

    // VARS
    QTextEdit* xv_msgTextEdit;
    QPushButton* xv_clearButton;


    // FUNCS
    void zh_createComponents();
    void zh_createConnections();


};
//==================================================
#endif // X_MESSAGEPANEL_H

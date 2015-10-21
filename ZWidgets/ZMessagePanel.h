//==================================================
#ifndef ZMESSAGEPANEL_H
#define ZMESSAGEPANEL_H
//==================================================
#include <QWidget>
//==================================================
class QTextEdit;
class QPushButton;
//==================================================
class ZMessagePanel : public QWidget
{
    Q_OBJECT
public:
    explicit ZMessagePanel(QWidget *parent = 0);

signals:

public slots:

    void zp_appendMessage(const QString&);
    void zp_appendStandardOutput(const QByteArray& byteArray);

private:

    // VARS
    QTextEdit* zv_msgTextEdit;
    QPushButton* zv_clearButton;


    // FUNCS
    void zh_createComponents();
    void zh_createConnections();


};
//==================================================
#endif // ZMESSAGEPANEL_H

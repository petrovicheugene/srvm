//=============================================================
#ifndef ZSPECTRUMTABLEWIDGET_H
#define ZSPECTRUMTABLEWIDGET_H
//=============================================================
#include <QWidget>
//=============================================================
class QTableView;

//=============================================================
class ZSpectrumTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZSpectrumTableWidget(QWidget *parent = 0);


signals:

public slots:


private:

    // VARS
    QTableView* zv_table;

    // FUNCS
    void zh_createComponents();

};
//=============================================================
#endif // ZSPECTRUMTABLEWIDGET_H

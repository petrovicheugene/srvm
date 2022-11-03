//==============================================================
#ifndef X_LESGAUSSSOLVER_H
#define X_LESGAUSSSOLVER_H
//==============================================================
#include <QObject>
//==============================================================
class X_Column;
class X_Row;
//==============================================================
class X_Column
{
public:
    X_Column(qreal* factor,const QList<qreal>& elements)
    {
        this->factor = factor;
        this->elements = elements;
    }

    qreal* factor;
    QList<qreal> elements;
};
//==============================================================
class X_LESGaussSolver : public QObject
{
    Q_OBJECT
public:
    explicit X_LESGaussSolver(QObject *parent = 0);

    int xp_rowCount() const;
    int xp_columnCount() const;

    bool xp_solve();

    void xp_clear();
    void xp_appendTermColumn(qreal* factorPtr, const QList<qreal>& termVriableValueList);
    void xp_appendFreeTermList(const QList<qreal> &);

    QString xp_lastError() const;

signals:

    void xg_message(QString);

public slots:

private:

    // VARS
    QList<X_Column> xv_columnList;
    QList<qreal> xv_freeTermList;
    QString xv_lastErrorString;

    // FUNCS
    void zh_swapRows(int row1, int row2);
    void zh_swapColumns(int col1, int col2);
    bool zh_putMaxValueOnDiagonal(int startIndex);
    void zh_addMultipliedRow(int targetRow, int sourceRow, qreal factor);

};
//==============================================================
#endif // X_LESGAUSSSOLVER_H

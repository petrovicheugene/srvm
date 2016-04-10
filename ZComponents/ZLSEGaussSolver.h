//==============================================================
#ifndef ZLSEGAUSSSOLVER_H
#define ZLSEGAUSSSOLVER_H
//==============================================================
#include <QObject>
//==============================================================
class ZColumn;
class ZRow;
//==============================================================
class ZColumn
{
public:
    ZColumn(qreal* factor,const QList<qreal>& elements)
    {
        this->factor = factor;
        this->elements = elements;
    }

    qreal* factor;
    QList<qreal> elements;
};
//==============================================================
class ZLSEGaussSolver : public QObject
{
    Q_OBJECT
public:
    explicit ZLSEGaussSolver(QObject *parent = 0);

    int zp_rowCount() const;
    int zp_columnCount() const;

    bool zp_solve();

    void zp_clear();
    void zp_appendTermColumn(qreal* factorPtr, const QList<qreal>& termVriableValueList);
    void zp_appendFreeTermList(const QList<qreal> &);

signals:

    void zg_message(QString);

public slots:

private:

    // VARS
    QList<ZColumn> zv_columnList;
    QList<qreal> zv_freeTermList;

    // FUNCS
    void zh_swapRows(int row1, int row2);
    void zh_swapColumns(int col1, int col2);
    bool zh_putMaxValueOnDiagonal(int startIndex);
    void zh_addMultipliedRow(int targetRow, int sourceRow, qreal factor);

};
//==============================================================
#endif // ZLSEGAUSSSOLVER_H

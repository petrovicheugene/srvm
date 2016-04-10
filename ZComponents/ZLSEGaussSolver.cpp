//==============================================================
#include "ZLSEGaussSolver.h"
//==============================================================
ZLSEGaussSolver::ZLSEGaussSolver(QObject *parent) : QObject(parent)
{

}
//==============================================================
int ZLSEGaussSolver::zp_rowCount() const
{
    return zv_freeTermList.count();
}
//==============================================================
int ZLSEGaussSolver::zp_columnCount() const
{
    return zv_columnList.count();
}
//==============================================================
bool ZLSEGaussSolver::zp_solve()
{
    if(zv_freeTermList.count() != zv_columnList.count())
    {
        QString error = "LSE solver: The number of factor is not equal to the number of equation. The system has no solution.";
        emit zg_message(error);
        return false;
    }

    // forward pass
    qreal rowFactor;
    qreal maxValue;
    for(int i = 0; i < zv_freeTermList.count(); i++)
    {
        if(!zh_putMaxValueOnDiagonal(i))
        {
            return false;
        }
        // maxValue is on the diagonal
        maxValue = zv_columnList.at(i).elements.at(i);
        for(int r = i + 1; r < zv_freeTermList.count(); r++)
        {
            rowFactor = -1 * zv_columnList.at(i).elements.at(r) / maxValue;
            zh_addMultipliedRow(r, i, rowFactor);
        }
    }

    // backward pass
    qreal numerator;
    for(int i = zv_freeTermList.count() - 1; i >= 0; i--)
    {
        if(zv_columnList.at(i).elements.at(i) == 0)
        {
            QString error = "LSE solver: Attempt of dividing by zero. The system has no solution. ";
            emit zg_message(error);
            return false;
        }

        numerator = zv_freeTermList.at(i);
        for(int r = zv_freeTermList.count() - 1; r > i; r--)
        {
            numerator -= zv_columnList[r].elements[i] * (*zv_columnList[r].factor);
        }

        *zv_columnList[i].factor =  numerator / zv_columnList[i].elements[i];
    }

    return true;
}
//==============================================================
void ZLSEGaussSolver::zp_clear()
{
    zv_columnList.clear();
    zv_freeTermList.clear();
}
//==============================================================
void ZLSEGaussSolver::zp_appendTermColumn(qreal* factorPtr, const QList<qreal>& termVriableValueList)
{
    ZColumn column(factorPtr, termVriableValueList);
    zv_columnList.append(column);
}
//==============================================================
void ZLSEGaussSolver::zp_appendFreeTermList(const QList<qreal>& freeTermList)
{
    zv_freeTermList = freeTermList;
}
//==============================================================
void ZLSEGaussSolver::zh_swapRows(int row1, int row2)
{
    for(int col = 0; col < zv_columnList.count(); col++)
    {
        zv_columnList[col].elements.swap(row1, row2);
    }

    zv_freeTermList.swap(row1, row2);
}
//==============================================================
void ZLSEGaussSolver::zh_swapColumns(int col1, int col2)
{
    zv_columnList.swap(col1, col2);
}
//==============================================================
bool ZLSEGaussSolver::zh_putMaxValueOnDiagonal(int startIndex)
{
    qreal maxAbsValue = 0.0;
    int maxValueColumn = -1;
    int maxValueRow = -1;
    qreal currentAbsValue;
    for(int col = startIndex; col < zv_columnList.count(); col++)
    {
        for(int row = startIndex; row < zv_freeTermList.count(); row++)
        {
            currentAbsValue = qAbs(zv_columnList.at(col).elements.at(row));
            if(maxAbsValue < currentAbsValue)
            {
                maxAbsValue = currentAbsValue;
                maxValueColumn = col;
                maxValueRow = row;
            }
        }
    }

    if(maxValueRow < startIndex || maxValueRow > zv_freeTermList.count()
            || maxValueColumn < startIndex || maxValueColumn > zv_columnList.count())
    {
        QString error = "LSE solver: Error matrix element manipulation.";
        emit zg_message(error);
        return false;
    }

    if(maxAbsValue == 0)
    {
        QString error = "LSE solver: The system has no solution.";
        emit zg_message(error);
        return false;
    }

    zh_swapRows(startIndex, maxValueRow);
    zh_swapColumns(startIndex, maxValueColumn);
    return true;
}
//==============================================================
void ZLSEGaussSolver::zh_addMultipliedRow(int targetRow, int sourceRow, qreal factor)
{
    for(int col = 0; col < zv_columnList.count(); col++)
    {
//        qreal addNumber = zv_columnList.at(col).elements.at(sourceRow) * factor;
//        qreal target = zv_columnList[col].elements[targetRow];
        zv_columnList[col].elements[targetRow] += zv_columnList.at(col).elements.at(sourceRow) * factor;
    }

    zv_freeTermList[targetRow] += zv_freeTermList.at(sourceRow) * factor;
}
//==============================================================

//==============================================================
#include "X_LESGaussSolver.h"
#include <QDebug>
//==============================================================
X_LESGaussSolver::X_LESGaussSolver(QObject *parent) : QObject(parent)
{

}
//==============================================================
int X_LESGaussSolver::xp_rowCount() const
{
    return xv_freeTermList.count();
}
//==============================================================
int X_LESGaussSolver::xp_columnCount() const
{
    return xv_columnList.count();
}
//==============================================================
bool X_LESGaussSolver::xp_solve()
{
    xv_lastErrorString.clear();
    if(xv_freeTermList.count() != xv_columnList.count())
    {
        xv_lastErrorString = "LES solver: The number of factor is not equal to the number of equation. The system has no solution.";
        emit xg_message(xv_lastErrorString);
        qCritical().noquote() <<  xv_lastErrorString;

        return false;
    }

    // forward pass
    // if more than one term
    if(xv_columnList.count() > 1)
    {
        qreal rowFactor;
        qreal maxValue;
        for(int i = 0; i < xv_freeTermList.count(); i++)
        {
            if(!xh_putMaxValueOnDiagonal(i))
            {
                return false;
            }
            // maxValue is on the diagonal
            maxValue = xv_columnList.at(i).elements.at(i);
            for(int r = i + 1; r < xv_freeTermList.count(); r++)
            {
                rowFactor = -1.0 * xv_columnList.at(i).elements.at(r) / maxValue;
                xh_addMultipliedRow(r, i, rowFactor);
            }
        }
    }

    // backward pass
    qreal numerator;
    for(int i = xv_freeTermList.count() - 1; i >= 0; i--)
    {
        if(xv_columnList.at(i).elements.at(i) == 0.0)
        {
            xv_lastErrorString = "LES solver: Attempt of dividing by zero. The system has no solution. ";
            emit xg_message(xv_lastErrorString);
            qCritical().noquote() <<  xv_lastErrorString;
            return false;
        }

        numerator = xv_freeTermList.at(i);
        for(int r = xv_freeTermList.count() - 1; r > i; r--)
        {
            numerator -= xv_columnList[r].elements[i] * (*(xv_columnList[r].factor));
        }

//        qreal fVal = numerator / xv_columnList[i].elements[i];
//        qreal* pointer = xv_columnList[i].factor;
//        *pointer = fVal;
        *(xv_columnList[i].factor) =  numerator / xv_columnList[i].elements[i];
    }

    return true;
}
//==============================================================
QString X_LESGaussSolver::xp_lastError() const
{
    return xv_lastErrorString;
}
//==============================================================
void X_LESGaussSolver::xp_clear()
{
    xv_columnList.clear();
    xv_freeTermList.clear();
}
//==============================================================
void X_LESGaussSolver::xp_appendTermColumn(qreal* factorPtr, const QList<qreal>& termVriableValueList)
{
    X_Column column(factorPtr, termVriableValueList);
    xv_columnList.append(column);
}
//==============================================================
void X_LESGaussSolver::xp_appendFreeTermList(const QList<qreal>& freeTermList)
{
    xv_freeTermList = freeTermList;
}
//==============================================================
void X_LESGaussSolver::xh_swapRows(int row1, int row2)
{
    for(int col = 0; col < xv_columnList.count(); col++)
    {
        //xv_columnList[col].elements.swap(row1, row2);
        xv_columnList[col].elements.swapItemsAt(row1, row2);
    }

    xv_freeTermList.swapItemsAt(row1, row2);
}
//==============================================================
void X_LESGaussSolver::xh_swapColumns(int col1, int col2)
{
    xv_columnList.swapItemsAt(col1, col2);
}
//==============================================================
bool X_LESGaussSolver::xh_putMaxValueOnDiagonal(int startIndex)
{
    qreal maxAbsValue = 0.0;
    int maxValueColumn = -1;
    int maxValueRow = -1;
    qreal currentAbsValue = 0.0;
    for(int col = startIndex; col < xv_columnList.count(); col++)
    {
        for(int row = startIndex; row < xv_freeTermList.count(); row++)
        {
            currentAbsValue = qAbs(xv_columnList.at(col).elements.at(row));
            if(maxAbsValue < currentAbsValue)
            {
                maxAbsValue = currentAbsValue;
                maxValueColumn = col;
                maxValueRow = row;
            }
        }
    }

    if(maxValueRow < startIndex || maxValueRow > xv_freeTermList.count()
            || maxValueColumn < startIndex || maxValueColumn > xv_columnList.count())
    {
        xv_lastErrorString = "LES solver: Error matrix element manipulation.";
        emit xg_message(xv_lastErrorString);
        qCritical().noquote() <<  xv_lastErrorString;
        return false;
    }

    if(maxAbsValue == 0.0)
    {
        xv_lastErrorString = "LES solver: The system has no solution.";
        emit xg_message(xv_lastErrorString);
        qCritical().noquote() <<  xv_lastErrorString;
        return false;
    }

    xh_swapRows(startIndex, maxValueRow);
    xh_swapColumns(startIndex, maxValueColumn);
    return true;
}
//==============================================================
void X_LESGaussSolver::xh_addMultipliedRow(int targetRow, int sourceRow, qreal factor)
{
    for(int col = 0; col < xv_columnList.count(); col++)
    {
//        qreal addNumber = xv_columnList.at(col).elements.at(sourceRow) * factor;
//        qreal target = xv_columnList[col].elements[targetRow];
        xv_columnList[col].elements[targetRow] += xv_columnList.at(col).elements.at(sourceRow) * factor;
    }

    xv_freeTermList[targetRow] += xv_freeTermList.at(sourceRow) * factor;
}
//==============================================================

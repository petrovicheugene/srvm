//====================================================
#include "ZPasteData.h"
#include <QRegularExpression>
//====================================================
ZPasteData::ZPasteData()
{
    zp_reset();
}
//====================================================
void ZPasteData::zp_reset()
{
    zv_dataTable.clear();
    zv_columnCount = 0;
    zv_firstLineType = LT_NOT_DEFINED;
}
//====================================================
bool ZPasteData::zp_loadData(const QString& dataString, const QStringList &chemElementList)
{
    zp_reset();
    if(dataString.isEmpty())
    {
        zp_reset();
        return false;
    }

    // Devide string to terms
    if(!zh_devideStringToTerms(dataString))
    {
        zp_reset();
        return false;
    }

    // check first line
    zv_firstLineType = zh_checkClipboardLine(zv_dataTable.value(0), chemElementList);

    // check other lines
    if(zv_dataTable.count() == 1)
    {
        // if there is the only line it must be data line
        if(zv_firstLineType != LT_DATA)
        {
            zp_reset();
            return false;
        }
    }
    else
    {
        // check other data in table
        LineType lineType;
        for(int i = 1; i < zv_dataTable.count(); i++)
        {
            lineType = zh_checkClipboardLine(zv_dataTable.value(i), chemElementList);
            if(lineType != LT_DATA)
            {
                zp_reset();
                return false;
            }
        }
    }

    return true;
}
//====================================================
bool ZPasteData::zh_devideStringToTerms(const QString& dataString)
{
    // devide into strings and remove empty lines
    QStringList lines =  dataString.split(QRegularExpression("\\n|\\v"));
    if(lines.isEmpty())
    {
        return false;
    }

    // remove last empty line
    while(lines.last().isEmpty())
    {
        lines.removeLast();
    }

    // devide lines into parts
    foreach(QString line, lines)
    {
        // remove whitespace from the start and end
        line = line.simplified();
        QStringList lineValueList = line.split(QRegularExpression("\\t|\\s{1,}"));
        // define column count
        if(zv_columnCount < lineValueList.count())
        {
            zv_columnCount = lineValueList.count();
        }
        zv_dataTable.append(lineValueList);
    }

    return !zv_dataTable.isEmpty();
}
//====================================================
ZPasteData::LineType ZPasteData::zh_checkClipboardLine(const QStringList& line,
                                                       const QStringList& chemElementList) const
{
    LineType lineType = LT_NOT_DEFINED;
    bool ok;

    foreach(QString lineTerm, line)
    {
        if(lineTerm.isEmpty())
        {
            lineTerm = "0.0";
        }

        lineTerm.toDouble(&ok);
        if(ok)
        {
            if(lineType == LT_NOT_DEFINED)
            {
                lineType = LT_DATA;
            }
            else if(lineType == LT_HEADER)
            {
                lineType = LT_MIXED;
                break;
            }
            continue;
        }

        // term contains string
        if(chemElementList.contains(lineTerm))
        {
            if(lineType == LT_NOT_DEFINED)
            {
                lineType = LT_HEADER;
            }
        }
        else
        {
            lineType = LT_MIXED;
            break;
        }
    }

    return lineType;
}
//====================================================
int ZPasteData::zp_dataRowCount() const
{
    if(zv_firstLineType == LT_NOT_DEFINED)
    {
        return 0;
    }

    if(zv_firstLineType == LT_DATA)
    {
        return zv_dataTable.count();
    }

    return zv_dataTable.count() - 1;
}
//====================================================
int ZPasteData::zp_columnCount() const
{
    return zv_columnCount;
}
//====================================================
QString ZPasteData::zp_columnHeader(int column) const
{
    if(zv_firstLineType == LT_NOT_DEFINED || zv_firstLineType == LT_DATA
            || zv_columnCount <= 0 || column < 0 || column >= zv_columnCount)
    {
        return QString();
    }

    return zv_dataTable.value(0, QStringList()).value(column, QString());
}
//====================================================
QStringList ZPasteData::zp_columnData(int column) const
{
    if(zv_firstLineType == LT_NOT_DEFINED || column < 0 || column >= zv_columnCount)
    {
        return QStringList();
    }

    QStringList concentrationColumn;
    int startRow;
    if(zv_firstLineType == LT_DATA)
    {
        startRow = 0;
    }
    else
    {
        startRow = 1;
    }

    for(int row = startRow; row < zv_dataTable.count(); row++)
    {
        concentrationColumn.append(zv_dataTable.at(row).value(column, "0.0"));
    }

    return concentrationColumn;
}
//====================================================
QStringList ZPasteData::zp_columnData(const QString& chemElementName) const
{
    if(zv_firstLineType == LT_DATA || zv_firstLineType == LT_NOT_DEFINED
            || zv_dataTable.count() <= 1 || zv_columnCount == 0)
    {
        return QStringList();
    }

    // find column number
    int columnNumber = -1;
    for(int c = 0; c < zv_dataTable.at(0).count(); c++)
    {
        if(zv_dataTable.at(0).at(c) == chemElementName)
        {
            columnNumber = c;
            break;
        }
    }

    return zp_columnData(columnNumber);
}
//====================================================
bool ZPasteData::zp_isValid() const
{
    return !(zv_dataTable.isEmpty() || zv_firstLineType == LT_NOT_DEFINED || zv_columnCount == 0);
}
//====================================================
bool ZPasteData::zp_isHeaderListValid() const
{
    return zv_dataTable.count() > 1
            && (zv_firstLineType == LT_HEADER || zv_firstLineType == LT_MIXED);
}
//====================================================
QStringList ZPasteData::zp_absentHeaderList(const QStringList& chemElementList) const
{
    if(zv_firstLineType == LT_DATA || zv_firstLineType == LT_NOT_DEFINED
            || zv_dataTable.count() <= 1 || zv_columnCount == 0)
    {
        return QStringList();
    }

    QStringList absentHeaderList;
    QString header;
    for(int c = 0; c < zv_dataTable.at(0).count(); c++)
    {
        header = zv_dataTable.at(0).value(c);
        if(!chemElementList.contains(header))
        {
            absentHeaderList << header;
        }
    }

    return absentHeaderList;
}
//====================================================

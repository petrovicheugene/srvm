//====================================================
#include "X_PasteData.h"
#include "X_LocaleDoubleConverter.h"

#include <QRegularExpression>
//====================================================
X_PasteData::X_PasteData()
{
    xp_reset();
}
//====================================================
void X_PasteData::xp_reset()
{
    xv_dataTable.clear();
    xv_columnCount = 0;
    xv_firstLineType = LT_NOT_DEFINED;
}
//====================================================
bool X_PasteData::xp_loadData(const QString& dataString, const QStringList &chemElementList)
{
    xp_reset();

    if(dataString.isEmpty())
    {
        return false;
    }

    // Devide string to terms
    if(!xh_devideStringToTerms(dataString))
    {
        return false;
    }

    // check first line
    xv_firstLineType = xh_checkClipboardLine(xv_dataTable.value(0), chemElementList);

    // check other lines
    if(xv_dataTable.count() == 1)
    {
        // if there is the only line it must be data line
        if(xv_firstLineType != LT_DATA)
        {
            xp_reset();
            return false;
        }
    }
    else
    {
        // check other data in table
        LineType lineType;
        for(int i = 1; i < xv_dataTable.count(); i++)
        {
            lineType = xh_checkClipboardLine(xv_dataTable.value(i), chemElementList);
            if(lineType != LT_DATA)
            {
                xp_reset();
                return false;
            }
        }
    }

    return true;
}
//====================================================
bool X_PasteData::xh_devideStringToTerms(const QString& dataString)
{
//    QLocale locale;
//    QString decimalSeparator = locale.decimalPoint();

    // devide into strings and remove empty lines
    QStringList lines =  dataString.split(QRegularExpression("\\n|\\v"));
    if(lines.isEmpty())
    {
        return false;
    }

    // remove last empty line
    while(!lines.isEmpty() && lines.last().isEmpty())
    {
        lines.removeLast();
    }

    // devide lines into parts
    foreach(QString line, lines)
    {
        // remove whitespace from the start and end
        line = line.simplified();

        static QRegularExpression separatorRe("\\t|\\s{1,}");
        QStringList lineValueList = line.split(separatorRe);

        for (int i = 0; i < lineValueList.count(); ++i)
        {
            lineValueList[i] = X_LocaleDoubleConverter::toLocalDoubleString(lineValueList.at(i));
        }

        // define column count
        if(xv_columnCount < lineValueList.count())
        {
            xv_columnCount = lineValueList.count();
        }
        xv_dataTable.append(lineValueList);
    }

    return !xv_dataTable.isEmpty();
}
//====================================================
X_PasteData::LineType X_PasteData::xh_checkClipboardLine(const QStringList& line,
                                                       const QStringList& chemElementList) const
{
    LineType lineType = LT_NOT_DEFINED;

    foreach(QString lineTerm, line)
    {
        if(lineTerm.isEmpty())
        {
            lineTerm = QString::number(0.0, 'f', 1);
        }

//        static QRegularExpression re("^[+-]?\\d{1,}[,.]?(\\d{1,}([eE][+-]?\\d{1,})?)?$");
//        QRegularExpressionMatch match = re.match(lineTerm);
//        hasMatch = match.hasMatch(); // true
        bool ok = false;
        lineTerm = X_LocaleDoubleConverter::toLocalDoubleString(lineTerm, &ok);

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
int X_PasteData::xp_dataRowCount() const
{
    if(xv_firstLineType == LT_NOT_DEFINED)
    {
        return 0;
    }

    if(xv_firstLineType == LT_DATA)
    {
        return xv_dataTable.count();
    }

    return xv_dataTable.count() - 1;
}
//====================================================
int X_PasteData::xp_columnCount() const
{
    return xv_columnCount;
}
//====================================================
QString X_PasteData::xp_columnHeader(int column) const
{
    if(xv_firstLineType == LT_NOT_DEFINED || xv_firstLineType == LT_DATA
            || xv_columnCount <= 0 || column < 0 || column >= xv_columnCount)
    {
        return QString();
    }

    return xv_dataTable.value(0, QStringList()).value(column, QString());
}
//====================================================
QStringList X_PasteData::xp_columnData(int column) const
{
    if(xv_firstLineType == LT_NOT_DEFINED || column < 0 || column >= xv_columnCount)
    {
        return QStringList();
    }

    QStringList concentrationColumn;
    int startRow;
    if(xv_firstLineType == LT_DATA)
    {
        startRow = 0;
    }
    else
    {
        startRow = 1;
    }

    for(int row = startRow; row < xv_dataTable.count(); row++)
    {
        concentrationColumn.append(xv_dataTable.at(row).value(column, "0.0"));
    }

    return concentrationColumn;
}
//====================================================
QStringList X_PasteData::xp_columnData(const QString& chemElementName) const
{
    if(xv_firstLineType == LT_DATA || xv_firstLineType == LT_NOT_DEFINED
            || xv_dataTable.count() <= 1 || xv_columnCount == 0)
    {
        return QStringList();
    }

    // find column number
    int columnNumber = -1;
    for(int c = 0; c < xv_dataTable.at(0).count(); c++)
    {
        if(xv_dataTable.at(0).at(c) == chemElementName)
        {
            columnNumber = c;
            break;
        }
    }

    return xp_columnData(columnNumber);
}
//====================================================
bool X_PasteData::xp_isValid() const
{
    return !(xv_dataTable.isEmpty() || xv_firstLineType == LT_NOT_DEFINED || xv_columnCount == 0);
}
//====================================================
bool X_PasteData::xp_isHeaderListValid() const
{
    return xv_dataTable.count() > 1
            && (xv_firstLineType == LT_HEADER || xv_firstLineType == LT_MIXED);
}
//====================================================
QStringList X_PasteData::xp_absentHeaderList(const QStringList& chemElementList) const
{
    if(xv_firstLineType == LT_DATA || xv_firstLineType == LT_NOT_DEFINED
            || xv_dataTable.count() <= 1 || xv_columnCount == 0)
    {
        return QStringList();
    }

    QStringList absentHeaderList;
    QString header;
    for(int c = 0; c < xv_dataTable.at(0).count(); c++)
    {
        header = xv_dataTable.at(0).value(c);
        if(!chemElementList.contains(header))
        {
            absentHeaderList << header;
        }
    }

    return absentHeaderList;
}
//====================================================

//====================================================
#ifndef ZPASTEDATA_H
#define ZPASTEDATA_H
//====================================================
#include <QList>
#include <QStringList>
//====================================================
class ZPasteData
{
public:
    ZPasteData();
    enum LineType {LT_NOT_DEFINED, LT_DATA, LT_HEADER, LT_MIXED};

    // FUNCS
    void zp_reset();
    bool zp_loadData(const QString&, const QStringList& chemElementList);

    int zp_dataRowCount() const;
    int zp_columnCount() const;

    QString zp_columnHeader(int column) const;
    QStringList zp_columnData(int column) const;
    QStringList zp_columnData(const QString& chemElementName) const;

    bool zp_isValid() const;
    bool zp_isHeaderListValid() const;
    QStringList zp_absentHeaderList(const QStringList& chemElementList) const;

private:

    // VARS
    QList<QStringList> zv_dataTable;
    LineType zv_firstLineType;
    int zv_columnCount;

    // FUNCS
    bool zh_devideStringToTerms(const QString& dataString);
    ZPasteData::LineType zh_checkClipboardLine(const QStringList& line,
                                                           const QStringList& chemElementList) const;
};
//====================================================
#endif // ZPASTEDATA_H

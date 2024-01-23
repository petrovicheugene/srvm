//====================================================
#ifndef X_PASTEDATA_H
#define X_PASTEDATA_H
//====================================================
#include <QList>
#include <QStringList>
//====================================================
class X_PasteData
{
public:
    X_PasteData();
    enum LineType {LT_NOT_DEFINED, LT_DATA, LT_HEADER, LT_MIXED};

    // FUNCS
    void xp_reset();
    bool xp_loadData(const QString&, const QStringList& chemElementList);

    int xp_dataRowCount() const;
    int xp_columnCount() const;

    QString xp_columnHeader(int column) const;
    QStringList xp_columnData(int column) const;
    QStringList xp_columnData(const QString& chemElementName) const;

    bool xp_isValid() const;
    bool xp_isHeaderListValid() const;
    QStringList xp_absentHeaderList(const QStringList& chemElementList) const;

private:

    // VARS
    QList<QStringList> xv_dataTable;
    LineType xv_firstLineType;
    int xv_columnCount;

    // FUNCS
    bool xh_devideStringToTerms(const QString& dataString);
    X_PasteData::LineType xh_checkClipboardLine(const QStringList& line,
                                                           const QStringList& chemElementList) const;
};
//====================================================
#endif // X_PASTEDATA_H

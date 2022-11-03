//======================================================
#ifndef X_CHEMICALELEMENTPROPERTYIOMANAGER_H
#define X_CHEMICALELEMENTPROPERTYIOMANAGER_H
//======================================================
#include <QApplication>
#include <QObject>
#include <QList>
#include <QMap>
#include <QPair>
#include <QStack>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
//======================================================
class X_AbstractChemicalElementPropertyRepository;
//======================================================
class X_ChemicalElementPropertyIOManager : public QObject
{
    Q_OBJECT
public:
    explicit X_ChemicalElementPropertyIOManager(QObject *parent = nullptr);
//    explicit X_ChemicalElementPropertyIOManager(X_AbstractChemicalElementPropertyRepository* repository,
//                                               QObject *parent = nullptr);

//    bool xp_extractFromCSV(const QString& filePath,
//                           QString X_NumberColumnName = "X_",
//                           QString separator = ";");

    bool xp_extractFromCSV(X_AbstractChemicalElementPropertyRepository* repository,
                           const QString& filePath,
                           QString X_NumberColumnName = "X_",
                           QString separator = ";");

    bool xp_extractFromXML(X_AbstractChemicalElementPropertyRepository* repository,
                           const QString& filePath);


    bool xp_saveToXML(X_AbstractChemicalElementPropertyRepository* repository,
                      const QString& filePath);

    bool xp_extractFromFile(X_AbstractChemicalElementPropertyRepository* repository,
                            const QString& filePath);

    QStringList xp_lastErrors() const;

signals:

public slots:

private:

    // VARS
    // static QMap<QChar, QString> xv_escapeControlCharacterMap;

    QStringList xv_errorStringList;

    const QString xv_magicString = "GWSOT76D429YWFP";
    const QString xv_ROOT = "root";
    const QString xv_TYPE = "type";
    const QString xv_SECTION = "section";
    const QString xv_PROPERTY = "property";
    const QString xv_NAME = "name";

    // FUNCS
    // static QMap<QChar, QString> zh_initEscapeControlCharacterMap();
    void zh_writeXMLElement(X_AbstractChemicalElementPropertyRepository* repository,
                            QXmlStreamWriter *writer,
                            int z,
                            QStringList& propertySectionBranch);

    bool zh_detectRoot(QXmlStreamReader &reader) const;
    bool zh_parseXmlElement(X_AbstractChemicalElementPropertyRepository* repository,
                            QXmlStreamReader& reader,
                            QStringList& sectionBranch,
                            QString &propertyName,
                            QStack<QString>& tokenNameStack) const;

//    QString zh_escapeControlCharacters(const QString& srcString) const;
//    QString zh_removeWordSeparator(const QString& srcString) const;

};
//======================================================
#endif // X_CHEMICALELEMENTPROPERTYIOMANAGER_H

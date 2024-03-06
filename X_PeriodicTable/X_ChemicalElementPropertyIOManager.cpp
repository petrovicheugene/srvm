//======================================================
#include "X_ChemicalElementPropertyIOManager.h"
#include "X_AbstractChemicalElementPropertyRepository.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QXmlStreamReader>
//======================================================
X_ChemicalElementPropertyIOManager::X_ChemicalElementPropertyIOManager(QObject *parent)
    : QObject(parent)
{

}
//======================================================
bool X_ChemicalElementPropertyIOManager::xp_extractFromCSV(X_AbstractChemicalElementPropertyRepository* repository,
                                                          const QString& filePath,
                                                          QString X_NumberColumnName,
                                                          QString separator)
{
    xv_errorStringList.clear();
    if(!repository)
    {
        xv_errorStringList.append(tr("There is no repository to load data."));
        return false;
    }

    repository->xp_clearData();

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        xv_errorStringList.append(file.errorString());
        return false;
    }

    QTextStream textStream(&file);
    QString line;
    // first line should contain property names
    if(!textStream.readLineInto(&line))
    {
        file.close();
        xv_errorStringList.append(tr("Cannot read header line from file \"%1\"!").arg(filePath));
        return false;
    }
    // create property name list
    QStringList propertyNameList = line.split(separator);
    if(propertyNameList.isEmpty())
    {
        file.close();
        xv_errorStringList.append(tr("Property name list in file \"%1\" is empty!").arg(filePath));
        return false;
    }

    // find X_number column number
    int X_ColumnIndex = propertyNameList.indexOf(X_NumberColumnName);

    // read properties
    QStringList propertyStringList;
    int X_ = 0;
    bool ok;
    while(textStream.readLineInto(&line))
    {
        if(line.isEmpty())
        {
            continue;
        }

        // X_Number
        // if there is no X_ column define X_ number
        if(X_ColumnIndex < 0)
        {
            ++X_;
        }

        // read and split property line
        propertyStringList = line.split(separator);
        for(int i = 0; i < propertyNameList.count(); i++)
        {
            if(i > propertyStringList.count())
            {
                continue;
            }

            if(i == X_ColumnIndex)
            {
                X_ = propertyStringList.value(i).toInt(&ok);
                if(!ok)
                {
                    ++X_;
                }
            }

            if(!repository->xp_setChemicalElementProperty(X_,
                                                          propertyNameList.value(i),
                                                          propertyStringList.value(i)))
            {
                file.close();
                return false;
            }
        }
    }

    file.close();
    return true;

}
//======================================================
bool X_ChemicalElementPropertyIOManager::xp_extractFromXML(X_AbstractChemicalElementPropertyRepository* repository,
                                                          const QString& filePath)
{
    xv_errorStringList.clear();
    if(!repository)
    {
        xv_errorStringList.append(tr("There is no repository to load data."));
        return false;
    }

    repository->xp_clearData();
    if(!repository)
    {
        xv_errorStringList.append(tr("There is no chemical element repository to which load data."));
        return false;
    }

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        xv_errorStringList.append(tr("Cannot open file \"%1\" in read mode.").arg(filePath));
        return false;
    }

    //repository->xp_clearData();
    QXmlStreamReader reader(&file);

    if(!xh_detectRoot(reader))
    {
        xv_errorStringList.append(tr("Cannot recognize file \"%1\" as data source.<br>"
                                     "Root type is invalid").arg(filePath));
        file.close();
        return false;
    }

    QStringList sectionBranch;
    QString propertyName;
    QStack<QString> tokenNameStack;
    // after xh_detectRoot reader is inside the root section
    tokenNameStack.push(xv_ROOT);
    while(!reader.atEnd())
    {
        reader.readNext();
        if(!xh_parseXmlElement(repository, reader, sectionBranch, propertyName, tokenNameStack))
        {
            file.close();
            return false;
        }
    }

    file.close();
    return true;
}
//======================================================
bool X_ChemicalElementPropertyIOManager::xh_parseXmlElement(X_AbstractChemicalElementPropertyRepository* repository,
                                                           QXmlStreamReader& reader,
                                                           QStringList& sectionBranch,
                                                           QString& propertyName,
                                                           QStack<QString>& tokenNameStack) const
{
    if(reader.tokenType() == QXmlStreamReader::StartElement)
    {
        tokenNameStack.push(reader.name().toString());
        if(reader.name().toString() == xv_SECTION)
        {
            sectionBranch.append(reader.attributes().value(xv_NAME).toString());

            bool ok;
            int X_Number = sectionBranch.first().toInt(&ok);
            if(!ok)
            {
                return false;
            }

            QStringList branch = sectionBranch;
            branch.removeFirst();
            if(!branch.isEmpty())
            {
                if(!repository->xp_addChemicalElementPropertyBranch(X_Number, branch))
                {
                    return false;
                }
            }
        }
        else if(reader.name().toString() == xv_PROPERTY)
        {
            propertyName = reader.attributes().value(xv_NAME).toString();
        }
        return true;
    }

    if(reader.tokenType() == QXmlStreamReader::EndElement)
    {
        if(tokenNameStack.isEmpty())
        {
            // nothing to close
            return false;
        }

        if(tokenNameStack.last() == xv_SECTION) // it was section
        {
            sectionBranch.removeLast();
        }
        else  if(tokenNameStack.last() == xv_PROPERTY) // it was property
        {
            // reset property name
            propertyName.clear();
        }
        else // it was smth else
        {
            // reset property name
            propertyName.clear();
        }

        tokenNameStack.pop();
        return true;
    }

    if(reader.tokenType() == QXmlStreamReader::Characters)
    {
        // if was property
        // create property
        if(!tokenNameStack.isEmpty() && tokenNameStack.last() == xv_PROPERTY && !propertyName.isEmpty())
        {
            if(reader.text().toString().simplified().isEmpty())
            {
                return true;
            }
            else
            {
                // create property
                bool ok;
                int X_Number = sectionBranch.first().toInt(&ok);
                if(!ok)
                {
                    return false;
                }
                QStringList branch = sectionBranch;
                branch.removeFirst();
                PropertyList propertyList;
                propertyList.append(QPair<QString, QString>(propertyName, reader.text().toString()));
                if(!repository->xp_setChemicalElementProperties(X_Number, branch, propertyList))
                {
                    return false;
                }
            }
        }

        return true;
    }

    return true;
}
//======================================================
bool X_ChemicalElementPropertyIOManager::xh_detectRoot(QXmlStreamReader& reader) const
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.tokenType() != QXmlStreamReader::StartElement ||
                reader.name() != xv_ROOT)
        {
            continue;
        }

        QString val = reader.attributes().value(xv_TYPE).toString();
        if(reader.attributes().value(xv_TYPE).toString() != xv_magicString)
        {
            continue;
        }

        return true;
    }

    return false;
}
//======================================================
bool X_ChemicalElementPropertyIOManager::xp_saveToXML(X_AbstractChemicalElementPropertyRepository* repository,
                                                     const QString& filePath)
{
    xv_errorStringList.clear();
    if(!repository)
    {
        xv_errorStringList.append(tr("There is no chemical element repository from which save data."));
        return false;
    }

    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly))
    {
        xv_errorStringList.append(tr("Cannot open file \"%1\" in write mode.").arg(filePath));
        return false;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    // head
    writer.writeStartDocument();
    writer.writeStartElement(xv_ROOT);
    writer.writeAttribute(xv_TYPE, xv_magicString);

    // chemical element property data
    QStringList sectionList;
    if(!repository->xp_chemicalElementPropertySectionList(-1,
                                                          QStringList(),
                                                          sectionList))
    {
        // error
        xv_errorStringList.append(repository->xp_lastError());

        writer.writeEndElement(); // root
        writer.writeEndDocument();
        file.close();

        return false;
    }

    QStringList propertySectionBranch;
    for(int element = 0; element < sectionList.count(); element++)
    {
        propertySectionBranch.clear();
        propertySectionBranch.append(sectionList.at(element));
        xh_writeXMLElement(repository, &writer, -1, propertySectionBranch);
    }

    writer.writeEndElement(); // root
    writer.writeEndDocument();

    bool res = file.error() == QFile::NoError;
    if(!res)
    {
        xv_errorStringList.append(file.errorString());
    }

    file.close();
    return res;
}
//======================================================
bool X_ChemicalElementPropertyIOManager::xp_extractFromFile(X_AbstractChemicalElementPropertyRepository* repository,
                                                           const QString& filePath)
{
    xv_errorStringList.clear();
    if(!repository)
    {
        xv_errorStringList.append(tr("There is no repository to extract data."));
        return false;
    }

    if(filePath.isEmpty())
    {
        xv_errorStringList.append("Data file path id empty");
        return false;
    }

    QFileInfo fileInfo(filePath);
    if(fileInfo.suffix() == "csv")
    {
        return xp_extractFromCSV(repository, filePath);
    }
    else
    {
        return xp_extractFromXML(repository, filePath);
    }
}
//======================================================
QStringList X_ChemicalElementPropertyIOManager::xp_lastErrors() const
{
    return xv_errorStringList;
}
//======================================================
void X_ChemicalElementPropertyIOManager::xh_writeXMLElement(X_AbstractChemicalElementPropertyRepository* repository,
                                                           QXmlStreamWriter* writer,
                                                           int z,
                                                           QStringList &propertySectionBranch)
{
    // start section
    writer->writeStartElement(xv_SECTION);
    if(!propertySectionBranch.isEmpty())
    {
        writer->writeAttribute(xv_NAME, (propertySectionBranch.last()));
    }

    // chemical element properties
    PropertyList propertyList;
    if(!repository->xp_chemicalElementProperties(z,
                                                 propertySectionBranch,
                                                 propertyList))
    {
        // error
        xv_errorStringList.append(repository->xp_lastError());
    }
    else
    {
        foreach(auto& property, propertyList)
        {
            writer->writeStartElement(xv_PROPERTY);
            writer->writeAttribute(xv_NAME, property.first);
            writer->writeCharacters(property.second);
            writer->writeEndElement(); // property
        }
    }

    // property sections
    QStringList sectionList;
    QStringList childPropertySectionBranch;
    if(!repository->xp_chemicalElementPropertySectionList(z,
                                                          propertySectionBranch,
                                                          sectionList))
    {
        // error
        xv_errorStringList.append(repository->xp_lastError());
    }
    else
    {
        foreach(auto& section, sectionList)
        {
            childPropertySectionBranch =  propertySectionBranch;
            childPropertySectionBranch.append(section);

            xh_writeXMLElement(repository,
                               writer,
                               z,
                               childPropertySectionBranch);
        }
    }

    // end section
    writer->writeEndElement(); // chemical element
}
//======================================================
//QString X_ChemicalElementPropertyIOManager::xh_escapeControlCharacters(const QString& srcString) const
//{
//    QString outString = srcString;
//    QList<QChar> controlCharacterList = xv_escapeControlCharacterMap.keys();
//    for(int i = outString.count() - 1; i >= 0; i--)
//    {
//        if(controlCharacterList.contains(outString.at(i)))
//        {
//            outString.replace(i, 1, xv_escapeControlCharacterMap.value(outString.at(i), outString.at(i)));
//        }
//    }

//    return outString;
//}
////======================================================
//QString X_ChemicalElementPropertyIOManager::xh_removeWordSeparator(const QString& srcString) const
//{
//    QString outString = srcString;
//    return outString;
//}
//======================================================


//============================================================
#include "X_XMLSpectrumArrayIOHandler.h"
#include <QDebug>
#include <QDir>
#include <QXmlStreamReader>
#include <QFileInfo>

//============================================================
X_XMLSpectrumArrayIOHandler::X_XMLSpectrumArrayIOHandler(QObject *parent) : QObject(parent)
{

}
//============================================================
X_XMLSpectrumArrayIOHandler::~X_XMLSpectrumArrayIOHandler()
{

}
//============================================================
bool X_XMLSpectrumArrayIOHandler::xp_readSpectrumArray (QFile& file, QList<X_RawSpectrumArray>& rawArrayList)
{
    if(!(file.openMode() & QIODevice::ReadOnly))
    {
        QString errorMsg = tr("File \"%1\" is not open in read mode!").arg(file.fileName());
        emit xg_message(errorMsg);
        qCritical().noquote() <<  errorMsg;
        return false;
    }

    QFileInfo fi(file.fileName());
    xv_fileAbsolutePath = fi.absolutePath();

    // root checking
    QXmlStreamReader reader(&file);
    bool rootDetectedFlag = false;
    bool magicStringDetectionFlag = false;
    //bool parsingErrorFalg = false;

    while(!reader.atEnd())
    {
        reader.readNext();
        // property root element detection section
        if(!rootDetectedFlag)
        {
            if(!zh_detectRoot(reader, magicStringDetectionFlag))
            {
                continue;
            }

            if(!magicStringDetectionFlag)
            {
                QString errorMsg = tr("File \"%1\" is not recognized!").arg(file.fileName());
                emit xg_message(errorMsg);
                qCritical().noquote() <<  errorMsg;
                return false;
            }
            rootDetectedFlag = true;
        }

        // root text data handling section

        if(reader.isCharacters())
        {
            if(reader.text().toString().simplified().isEmpty())
            {
                continue;
            }
        }
        else if(reader.isStartElement())
        {
            zh_parseXMLElement(rawArrayList, -1, -1, reader);
        }
        else if(reader.isEndElement())
        {
            // end of root level
        }
    }

    if(reader.hasError())
    {
        QString errorMsg = tr("File \"%1\" parsing failed! %2").arg(file.fileName(), reader.errorString());
        emit xg_message(errorMsg);
        qCritical().noquote() << errorMsg;
        return false;
    }
    else if(file.error() != QFile::NoError)
    {
        QString errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
        emit xg_message(errorMsg);
        qCritical().noquote() << errorMsg;
        return false;
    }

    return true;
}
//============================================================
// returns true if closing tag opened in calling function is readed
void X_XMLSpectrumArrayIOHandler::zh_parseXMLElement(QList<X_RawSpectrumArray> &array,
                                                    int currentArrayIndex,
                                                    int currentSpectrumIndex,
                                                    QXmlStreamReader& reader) const
{
    QString currentChemElement = QString();
    // bool insertedElementOpened = false;
    // current start tag handling
    // attributes handling
    QString currentTagName = reader.name().toString();
    QString path;

    if(currentTagName == xv_ARRAY)
    {
        X_RawSpectrumArray rawArray;
        rawArray.name = reader.attributes().value(xv_NAME).toString();
        array.append(rawArray);
        currentArrayIndex = array.count() - 1;
        //   array.xp_setArrayName(reader.attributes().value(xv_NAME).toString());
    }
    else if(currentTagName == xv_GAIN_FACTOR)
    {
        if(currentArrayIndex >= 0 && currentArrayIndex < array.count())
        {
            // read gainfactor value
            if(!reader.atEnd())
            {
                reader.readNext();
            }
            QString gainFactorString = reader.text().toString();
            bool ok;
            int gainFactor = gainFactorString.toInt(&ok);
            if(!ok)
            {
                gainFactor = 0;
            }
            array[currentArrayIndex].gainFactor = gainFactor;
        }
    }
    else if(currentTagName == xv_SPECTRUM)
    {
        if(currentArrayIndex >= 0 && currentArrayIndex < array.count())
        {
            QFileInfo fi(reader.attributes().value(xv_PATH).toString());
            path = fi.isRelative()? xv_fileAbsolutePath + fi.filePath().remove(0,1) : fi.filePath();

            X_RawSpectrum rawSpectrum;
            rawSpectrum.path = path;
            rawSpectrum.checked = reader.attributes().value(xv_CHECKED).toString() == xv_YES? true : false;
            rawSpectrum.visible = reader.attributes().value(xv_VISIBLE).toString() == xv_YES? true : false;

            array[currentArrayIndex].spectrumList.append(rawSpectrum);
            currentSpectrumIndex = array[currentArrayIndex].spectrumList.count() - 1;
        }
    }
    else if(currentTagName == xv_CONCENTRATION)
    {
        if(currentArrayIndex >= 0 && currentArrayIndex < array.count() &&
                currentSpectrumIndex >= 0 && currentSpectrumIndex < array.value(currentArrayIndex).spectrumList.count())
        {
            currentChemElement = reader.attributes().value(xv_NAME).toString();
            array[currentArrayIndex].spectrumList[currentSpectrumIndex].
                    concentrationMap.insert(currentChemElement, QString());
        }
    }

    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            // insertedElementOpened = true;
            zh_parseXMLElement(array, currentArrayIndex, currentSpectrumIndex, reader);
            continue;
//            if(closingTagReadedFlag)
//            {

//                // continue;
//                return false;
//            }
        }
        else if(reader.isCharacters())
        {
            if(reader.text().toString().simplified().isEmpty())
            {
                continue;
            }

            if(currentTagName == xv_CONCENTRATION)
            {
                QString concentration = reader.text().toString();
                array[currentArrayIndex].spectrumList[currentSpectrumIndex].
                        concentrationMap[currentChemElement] = concentration;
            }
        }
        else if(reader.isEndElement())
        {
            return;
            // return insertedElementOpened;
        }
    }
}
//============================================================
bool X_XMLSpectrumArrayIOHandler::xp_writeSpectrumArray(QFile& file, const QList<X_RawSpectrumArray>& rawArrayList)
{
    if(!(file.openMode() & QIODevice::WriteOnly))
    {
        QString errorMsg = tr("File \"%1\" is not open in write mode!").arg(file.fileName());
        emit xg_message(errorMsg);
        qCritical().noquote() << errorMsg;
        return false;
    }

    QFileInfo fi(file.fileName());
    xv_fileAbsolutePath = fi.absolutePath();
    QString path;

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    // head
    writer.writeStartDocument();
    // root
    writer.writeStartElement(xv_ROOT);
    writer.writeAttribute(xv_TYPE, xv_magicString);
    // data

    for(int a = 0; a < rawArrayList.count(); a++)
    {
        writer.writeStartElement(xv_ARRAY);
        writer.writeAttribute(xv_NAME, rawArrayList.at(a).name);

        writer.writeStartElement(xv_GAIN_FACTOR);
        writer.writeCharacters(QString::number(rawArrayList.at(a).gainFactor));
        writer.writeEndElement(); // gain factor

        for(int s = 0; s < rawArrayList.at(a).spectrumList.count(); s++)
        {
            if(rawArrayList.value(a).spectrumList.value(s).path.startsWith(xv_fileAbsolutePath, Qt::CaseInsensitive))
            {
                path = (rawArrayList.value(a).spectrumList.value(s).path.remove(0, xv_fileAbsolutePath.length()).prepend("."));
            }
            else
            {
                path = rawArrayList.value(a).spectrumList.value(s).path;
            }

            writer.writeStartElement(xv_SPECTRUM);
            writer.writeAttribute(xv_PATH, path);
            writer.writeAttribute(xv_CHECKED, rawArrayList.value(a).spectrumList.value(s).checked ? xv_YES : xv_NO);
            writer.writeAttribute(xv_VISIBLE, rawArrayList.value(a).spectrumList.value(s).visible ? xv_YES : xv_NO);

            QMap<QString, QString>::const_iterator it;

            for(it = rawArrayList[a].spectrumList[s].concentrationMap.begin();
                it != rawArrayList[a].spectrumList[s].concentrationMap.end(); it++ )
            {
                writer.writeStartElement(xv_CONCENTRATION);
                writer.writeAttribute(xv_NAME, it.key());
                writer.writeCharacters(it.value());
                writer.writeEndElement(); // concentration
            }
             writer.writeEndElement(); // spectrum
        }
          writer.writeEndElement(); // array
    }

    writer.writeEndElement(); // root
    writer.writeEndDocument();

    if(file.error() != QFile::NoError)
    {
        QString errorMsg = tr("Cannot write to file \"%1\"! %2").arg(file.fileName(), file.errorString());
        emit xg_message(errorMsg);
        qCritical().noquote() << errorMsg;
        return false;
    }

    return true;
}
//============================================================
bool X_XMLSpectrumArrayIOHandler::zh_detectRoot(const QXmlStreamReader& reader, bool& magicStringDetectionFlag) const
{
    // true - element name == root, error - trnado magic string existing flag
    if(reader.tokenType() != QXmlStreamReader::StartElement ||
            reader.name() != xv_ROOT)
    {
        return false;
    }

    if(reader.attributes().value(xv_TYPE).toString() == xv_magicString)
    {
        magicStringDetectionFlag = true;
    }
    else
    {
        magicStringDetectionFlag = false;
    }

    return true;
}

//============================================================
bool X_XMLSpectrumArrayIOHandler::zh_checkfilePath(const QString& path) const
{
    QFileInfo fileInfo(path);
    if((!fileInfo.exists()) || (!fileInfo.isFile()))
    {
        return false;
    }

    return true;
}
//============================================================

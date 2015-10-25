//============================================================
#include "ZXMLArrayIOHandler.h"
#include <QXmlStreamReader>
#include <QFileInfo>
//============================================================
ZXMLArrayIOHandler::ZXMLArrayIOHandler(QObject *parent) : ZAbstractArrayIOHandler(parent)
{

}
//============================================================
//bool ZXMLArrayIOHandler::zp_readSpectrumArray (ZSpectrumArray& array, QFile& file)
//{
//    if(!(file.openMode() & QIODevice::ReadOnly))
//    {
//        QString errorMsg = tr("File \"%1\" is not open in read mode!").arg(file.fileName());
//        emit zg_message(errorMsg);
//        return false;
//    }

//    // root checking
//    QXmlStreamReader reader(&file);
//    bool rootDetectedFlag = false;
//    bool magicStringDetectionFlag = false;
//    //bool parsingErrorFalg = false;

//    while(!reader.atEnd())
//    {
//        reader.readNext();
//        // property root element detection section
//        if(!rootDetectedFlag)
//        {
//            if(!zh_detectRoot(reader, magicStringDetectionFlag))
//            {
//                continue;
//            }

//            if(!magicStringDetectionFlag)
//            {
//                QString errorMsg = tr("File \"%1\" is not recognized!").arg(file.fileName());
//                emit zg_message(errorMsg);
//                return false;
//            }

//            rootDetectedFlag = true;
//        }

//        // root text data handling section
//        if(reader.isCharacters())
//        {
//            if(reader.text().toString().simplified().isEmpty())
//            {
//                continue;
//            }
//        }

//        if(reader.isStartElement())
//        {
//            zh_parseXMLElement(array, reader);
//        }
//    }

//    if(reader.hasError())
//    {
//        QString errorMsg = tr("File \"%1\" parsing failed! %2").arg(file.fileName(), reader.errorString());
//        emit zg_message(errorMsg);
//        return false;
//    }
//    else if(file.error() != QFile::NoError)
//    {
//        QString errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
//        emit zg_message(errorMsg);
//        return false;
//    }

//    return true;

//}
//============================================================
bool ZXMLArrayIOHandler::zp_readSpectrumArray (QList<ZRawArray>& rawArrayList, QFile& file)
{
    if(!(file.openMode() & QIODevice::ReadOnly))
    {
        QString errorMsg = tr("File \"%1\" is not open in read mode!").arg(file.fileName());
        emit zg_message(errorMsg);
        return false;
    }

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
                emit zg_message(errorMsg);
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

        if(reader.isStartElement())
        {
            zh_parseXMLElement(rawArrayList, -1, -1, reader);
        }
    }

#ifdef DBG
    qDebug() << "Stop";
#endif

    if(reader.hasError())
    {
        QString errorMsg = tr("File \"%1\" parsing failed! %2").arg(file.fileName(), reader.errorString());
        emit zg_message(errorMsg);
        return false;
    }
    else if(file.error() != QFile::NoError)
    {
        QString errorMsg = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
        emit zg_message(errorMsg);
        return false;
    }

    return true;
}
//============================================================
bool ZXMLArrayIOHandler::zh_detectRoot(const QXmlStreamReader& reader, bool& magicStringDetectionFlag) const
{
    // true - element name == root, error - trnado magic string existing flag
    if(reader.tokenType() != QXmlStreamReader::StartElement ||
            reader.name() != zv_ROOT)
    {
        return false;
    }

    if(reader.attributes().value(zv_TYPE).toString() == zv_magicString)
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
// returns true if closing tag opened in calling function is readed
bool ZXMLArrayIOHandler::zh_parseXMLElement(QList<ZRawArray> &array,
                                            int currentArrayIndex,
                                            int currentSpectrumIndex,
                                            QXmlStreamReader& reader)
{
    QString currentChemElement = QString();
    bool insertedElementOpened = false;
    // current start tag handling
    QString currentTagName = reader.name().toString();
    if(currentTagName == zv_ARRAY)
    {
        ZRawArray rawArray;
        rawArray.name = reader.attributes().value(zv_NAME).toString();
        array.append(rawArray);
        currentArrayIndex = array.count() - 1;
        //   array.zp_setArrayName(reader.attributes().value(zv_NAME).toString());
    }
    else if(currentTagName == zv_SPECTRUM)
    {
        if(currentArrayIndex >= 0 && currentArrayIndex < array.count())
        {
            ZRawSpectrum rawSpectrum;
            rawSpectrum.path = reader.attributes().value(zv_PATH).toString();
            array[currentArrayIndex].spectrumList.append(rawSpectrum);
            currentSpectrumIndex = array[currentArrayIndex].spectrumList.count() - 1;
        }
    }
    else if(currentTagName == zv_CONCENTRATION)
    {
        if(currentArrayIndex >= 0 && currentArrayIndex < array.count() &&
                currentSpectrumIndex >= 0 && currentSpectrumIndex < array.value(currentArrayIndex).spectrumList.count())
         {
            currentChemElement = reader.attributes().value(zv_NAME).toString();
            array[currentArrayIndex].spectrumList[currentSpectrumIndex].
                    concentrationMap.insert(currentChemElement, QString());
        }
    }

    while(!reader.atEnd())
    {
        reader.readNext();

        if(reader.isStartElement())
        {
            insertedElementOpened = true;
            bool closingTagReadedFlag = zh_parseXMLElement(array, currentArrayIndex, currentSpectrumIndex, reader);
            continue;
            if(closingTagReadedFlag)
            {
                continue;
                // return false;
            }
        }

        if(reader.isCharacters())
        {
            if(reader.text().toString().simplified().isEmpty())
            {
                continue;
            }

            if(currentTagName == zv_CONCENTRATION)
            {
                QString concentration = reader.text().toString();
                array[currentArrayIndex].spectrumList[currentSpectrumIndex].
                        concentrationMap[currentChemElement] = concentration;
//                if(!zh_checkfilePath(path))
//                {
//                    // error
//                    // TODO error message: path is not valid
//                }
//                else
//                {
//                    if(array.isEmpty())
//                    {
//                        // TODO error handling
//                        //                    QString errorMsg;
//                        //                    if(!array.zp_applySpectrum(path, errorMsg))
//                        //                    {
//                        //                        emit zg_message(errorMsg);
//                        //                    }
//                    }
//                    else
//                    {
//                        if(currentArrayIndex >= 0 || currentArrayIndex < array.count())
//                        {
//                           // array[currentIndex].spectrumList << path;
//                        }
//                    }
//                }


            }
        }

        if(reader.isEndElement())
        {
            return insertedElementOpened;
        }
    }

    //    if(reader.hasError())
    //    {
    //        return false;
    //    }

    return false;
}
//============================================================
bool ZXMLArrayIOHandler::zh_checkfilePath(const QString& path) const
{
    QFileInfo fileInfo(path);
    if((!fileInfo.exists()) || (!fileInfo.isFile()))
    {
        return false;
    }

    return true;
}
//============================================================

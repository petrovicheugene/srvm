//==========================================================
#include "ZXMLCalibrationIOHandler.h"
#include "ZXMLCalibration.h"
#include <QFileInfo>
#include <QXmlStreamReader>

//==========================================================
ZXMLCalibrationIOHandler::ZXMLCalibrationIOHandler(QObject *calibrationParent,QObject *parent)
    : ZAbstractCalibrationIOHandler(calibrationParent, parent)
{

}
//==========================================================
ZXMLCalibrationIOHandler::~ZXMLCalibrationIOHandler()
{

}
//==========================================================
bool ZXMLCalibrationIOHandler::zp_getCalibrationFromFile(const QString& fileName,
                                                              ZAbstractCalibration*& calibration)
{
    calibration = new ZXMLCalibration(fileName, zv_calibrationParent);
    return true;
}
//==========================================================
bool ZXMLCalibrationIOHandler::zp_getCalibrationFromFile(QFile & file,
                                                              ZAbstractCalibration*& calibration)
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
            //zh_parseXMLElement(calibration, -1, reader);
        }
    }

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
bool ZXMLCalibrationIOHandler::zh_detectRoot(const QXmlStreamReader& reader, bool& magicStringDetectionFlag) const
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
//bool ZXMLCalibrationIOHandler::zh_parseXMLElement(ZRawCalibration& rawCalibration,
//                                                       int currentArrayIndex,
//                                                       QXmlStreamReader& reader)
//{
//    //QString currentChemElement = QString();
//    QString currentCalibrationName = QString();
//    bool insertedElementOpened = false;
//    // current start tag handling
//    QString currentTagName = reader.name().toString();
//    if(currentTagName == zv_CHEMELEMENT)
//    {
//        ZRawCalibration rawArray;
//        rawArray.chemElement = reader.attributes().value(zv_NAME).toString();
//        array.append(rawArray);
//        currentArrayIndex = array.count() - 1;
//        //        //   array.zp_setArrayName(reader.attributes().value(zv_NAME).toString());
//    }
//    else if(currentTagName == zv_CALIBRATION)
//    {
//        if(currentArrayIndex >= 0 && currentArrayIndex < array.count())
//        {
//            currentCalibrationName = reader.attributes().value(zv_NAME).toString();
//            // array[currentArrayIndex].calibrationPathMap.insert(calibrationName, QString());
//        }
//    }

//    while(!reader.atEnd())
//    {
//        reader.readNext();

//        if(reader.isStartElement())
//        {
//            insertedElementOpened = true;
//            bool closingTagReadedFlag = zh_parseXMLElement(array, currentArrayIndex, reader);
//            continue;
//            if(closingTagReadedFlag)
//            {
//                continue;
//            }
//        }

//        if(reader.isCharacters())
//        {
//            if(reader.text().toString().simplified().isEmpty())
//            {
//                continue;
//            }

//            if(currentTagName == zv_CALIBRATION
//                    && currentArrayIndex >= 0
//                    && currentArrayIndex < array.count())
//            {
//                QString path = reader.text().toString();
//                array[currentArrayIndex].calibrationPathMap.insert(currentCalibrationName, path);
//            }
//        }

//        if(reader.isEndElement())
//        {
//            return insertedElementOpened;
//        }
//    }

    //    if(reader.hasError())
    //    {
    //        return false;
    //    }

//    return false;
//}
//============================================================
bool ZXMLCalibrationIOHandler::zh_checkfilePath(const QString& path) const
{
    QFileInfo fileInfo(path);
    if((!fileInfo.exists()) || (!fileInfo.isFile()))
    {
        return false;
    }

    return true;
}
//============================================================

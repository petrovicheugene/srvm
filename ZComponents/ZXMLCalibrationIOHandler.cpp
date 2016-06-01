//==========================================================
#include "ZXMLCalibrationIOHandler.h"
#include "ZCalibration.h"
#include <QFileInfo>
#include <QXmlStreamReader>
#include <QDateTime>
//==========================================================
ZXMLCalibrationIOHandler::ZXMLCalibrationIOHandler(QObject *calibrationParent,QObject *parent)
    : QObject(parent)
{
    zv_calibrationParent = calibrationParent;
}
//==========================================================
ZXMLCalibrationIOHandler::~ZXMLCalibrationIOHandler()
{

}
//==========================================================
bool ZXMLCalibrationIOHandler::zp_writeCalibrationToFile(QFile& file, const ZCalibration* calibration)
{
    if(!(file.openMode() & QIODevice::WriteOnly))
    {
        QString errorMsg = tr("File \"%1\" is not open in write mode!").arg(file.fileName());
        emit zg_message(errorMsg);
        return false;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    // head
    writer.writeStartDocument();
    writer.writeStartElement(zv_ROOT);
    writer.writeAttribute(zv_TYPE, zv_magicString);

    // date
    writer.writeStartElement(zv_DATE_TIME);
    writer.writeCharacters(calibration->zp_dateTime().toString(zv_dateTimeFormat));
    writer.writeEndElement(); // date-time

    // chem element
    writer.writeStartElement(zv_CHEMELEMENT);
    writer.writeCharacters(calibration->zp_chemElement());
    writer.writeEndElement(); // chem element

    // Quality data
    // R2
    writer.writeStartElement(zv_DETERMINATION_R2);
    writer.writeCharacters(calibration->zp_calibrationQualityData().determination);
    writer.writeEndElement(); // R2

    // R2adj
    writer.writeStartElement(zv_ADJUSTED_DETERMINATION_R2ADJ);
    writer.writeCharacters(calibration->zp_calibrationQualityData().adj_determination);
    writer.writeEndElement(); // R2adj

    // standard deviation s
    writer.writeStartElement(zv_STANDARD_DEVIATION);
    writer.writeCharacters(calibration->zp_calibrationQualityData().standardDeviation);
    writer.writeEndElement(); // s

    // EQUATION DATA
    // windows
    writer.writeStartElement(zv_WINDOW_LIST);
    for(int w = 0; w < calibration->zp_calibrationWindowCount(); w++)
    {
        // start window + name
        writer.writeStartElement(zv_WINDOW);
        writer.writeAttribute(zv_NAME, calibration->zp_calibrationWindowName(w));
        // window type
        writer.writeStartElement(zv_TYPE);
        writer.writeCharacters(ZCalibrationWindow::zp_typeName(calibration->zp_calibrationWindowType(w)));
        writer.writeEndElement(); // type
        // first channel
        writer.writeStartElement(zv_FIRST_CHANNEL);
        writer.writeCharacters(QString::number(calibration->zp_calibrationWindowFirstChannel(w)));
        writer.writeEndElement(); // first channel
        // last channel
        writer.writeStartElement(zv_LAST_CHANNEL);
        writer.writeCharacters(QString::number(calibration->zp_calibrationWindowLastChannel(w)));
        writer.writeEndElement(); // last channel

        writer.writeEndElement(); // window
    }
    writer.writeEndElement(); // windows

    // terms
    writer.writeStartElement(zv_TERM_LIST);
    for(int t = 0; t < calibration->zp_termCount(); t++)
    {
        // start term + name
        writer.writeStartElement(zv_TERM);
        writer.writeAttribute(zv_NAME, calibration->zp_termName(t));
        // term type
        writer.writeStartElement(zv_TYPE);
        writer.writeCharacters(ZAbstractTerm::zp_termTypeName(calibration->zp_termType(t)));
        writer.writeEndElement(); // type
        // term state
        writer.writeStartElement(zv_STATE);
        writer.writeCharacters(ZAbstractTerm::zp_termStateName(calibration->zp_termState(t)));
        writer.writeEndElement(); // term state

        // term custom string
        if(calibration->zp_termType(t) == ZAbstractTerm::TT_CUSTOM)
        {
            // TODO write Term Custom string
        }

        // term windows
        QStringList windowList = calibration->zp_termWindowsNameList(t);
        foreach(QString window, windowList)
        {
            writer.writeStartElement(zv_TERM_WINDOW);
            writer.writeCharacters(window);
            writer.writeEndElement(); // term window
        }

        // term factor
        writer.writeStartElement(zv_FACTOR);
        writer.writeCharacters(calibration->zp_termFactorString(t));
        writer.writeEndElement(); // term factor

        // end term
        writer.writeEndElement(); // term
    }
    writer.writeEndElement(); // terms

    // common term normalizer
    writer.writeStartElement(zv_NORMALIZER);
    // type
    writer.writeStartElement(zv_TYPE);
    writer.writeCharacters(ZTermNormalizer::zp_normaTypeString(calibration->zp_normaType()));
    writer.writeEndElement(); // type
    // custom string
    writer.writeStartElement(zv_CUSTOM_STRING);
    writer.writeCharacters(calibration->zp_normaCustomString());
    writer.writeEndElement(); // custom string
    writer.writeEndElement(); // common normalizer

    // equation
    writer.writeStartElement(zv_EQUATION);
    // type
    writer.writeStartElement(zv_TYPE);
    writer.writeCharacters(ZCalibration::zp_equationTypeString(calibration->zp_equationType()));
    writer.writeEndElement(); // type
    // base term
    writer.writeStartElement(zv_BASE_TERM);
    writer.writeCharacters(calibration->zp_baseTermString());
    writer.writeEndElement(); // base term

    // base term normalizer
    writer.writeStartElement(zv_FRACTIONAL_BASE_NORMALIZER);
    // type
    writer.writeStartElement(zv_TYPE);
    writer.writeCharacters(ZTermNormalizer::zp_normaTypeString(calibration->zp_baseTermNormaType()));
    writer.writeEndElement(); // type
    // custom string
    writer.writeStartElement(zv_CUSTOM_STRING);
    writer.writeCharacters(calibration->zp_baseTermNormaCustomString());
    writer.writeEndElement(); // custom string

    writer.writeEndElement(); // base term normalizer

    // intercept
    writer.writeStartElement(zv_INTERCEPT);
    writer.writeCharacters(calibration->zp_equationInterceptString());
    writer.writeEndElement(); // intercept

    writer.writeEndElement(); // equation

    writer.writeEndElement(); // root
    writer.writeEndDocument();

    if(file.error() != QFile::NoError)
    {
        QString errorMsg = tr("Cannot write to file \"%1\"! %2").arg(file.fileName(), file.errorString());
        emit zg_message(errorMsg);
        return false;
    }

    return true;
}
//==========================================================
bool ZXMLCalibrationIOHandler::zp_getCalibrationFromFile(QFile & file,
                                                         ZCalibration*& calibration)
{
    calibration = 0;
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
    parentTagStack.clear();
    // handling root level
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
//            calibration = new ZCalibration(file.fileName(), zv_calibrationParent);
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
            // handling inserted levels
            zh_parseXMLElement(calibration, reader);
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

    return false;
    return true;
}
//============================================================
// returns true if closing tag that was opened in calling function is closed
void ZXMLCalibrationIOHandler::zh_parseXMLElement(ZCalibration* calibration,
                                                  QXmlStreamReader& reader)
{
    // current start tag handling
    QString currentTagName = reader.name().toString();
    parentTagStack.push(currentTagName);
//    // attributes handling
    if(currentTagName == zv_WINDOW)
    {
        // create new raw window
        zv_rawWindow = ZRawWindow();
        zv_rawWindow.name = reader.attributes().value(zv_NAME).toString();
    }
    else if(currentTagName == zv_TERM)
    {
        // create new raw term
        zv_rawTerm = ZRawTerm();
    }

    while(!reader.atEnd())
    {
        reader.readNext();

        if(reader.isStartElement())
        {
            zh_parseXMLElement(calibration, reader);
            continue;
        }
        else if(reader.isCharacters())
        {
            if(reader.text().toString().simplified().isEmpty())
            {
                continue;
            }

            if(currentTagName == zv_DATE_TIME)
            {
                QString dateTimeString = reader.text().toString();
                QDateTime dateTime = QDateTime::fromString(dateTimeString, zv_dateTimeFormat);
                calibration->zp_setDateTime(dateTime);
            }
            else if(currentTagName == zv_CHEMELEMENT)
            {
                calibration->zp_setChemElement(reader.text().toString());
            }
            else if(currentTagName == zv_DETERMINATION_R2)
            {
                calibration->zp_setChemElement(reader.text().toString());
            }
            else if(currentTagName == zv_ADJUSTED_DETERMINATION_R2ADJ)
            {
                calibration->zp_setChemElement(reader.text().toString());
            }
            else if(currentTagName == zv_STANDARD_DEVIATION)
            {
                calibration->zp_setChemElement(reader.text().toString());
            }
            else if(currentTagName == zv_TYPE)
            {
                if(parentTagStack.last() == zv_WINDOW)
                {
                    zv_rawWindow.windowType = ZCalibrationWindow::zp_typeForName(reader.text().toString());
                }
                else if(parentTagStack.last() == zv_TERM)
                {

                }
                else if(parentTagStack.last() == zv_NORMALIZER)
                {

                }
                else if(parentTagStack.last() == zv_FRACTIONAL_BASE_NORMALIZER)
                {

                }
            }
            else if(currentTagName == zv_FIRST_CHANNEL)
            {
                zv_rawWindow.firstChannel = reader.text().toString();
            }
            else if(currentTagName == zv_LAST_CHANNEL)
            {
                zv_rawWindow.lastChannel = reader.text().toString();
            }
            else if(currentTagName == zv_STATE)
            {

            }
            else if(currentTagName == zv_FACTOR)
            {
            }
            else if(currentTagName == zv_CUSTOM_STRING)
            {
                if(parentTagStack.last() == zv_NORMALIZER)
                {

                }
                else if(parentTagStack.last() == zv_FRACTIONAL_BASE_NORMALIZER)
                {

                }
            }
            else if(currentTagName == zv_BASE_TERM)
            {
            }
            else if(currentTagName == zv_INTERCEPT)
            {
            }
        }
        else  if(reader.isEndElement())
        {
            if(currentTagName == zv_WINDOW)
            {
                calibration->zp_createNewCalibrationWindow(zv_rawWindow);
            }
            else if(currentTagName == zv_TERM)
            {
                calibration->zp_createTerm(zv_rawTerm);
            }
            parentTagStack.pop();
            return; // parent tag is closed
        }
    }

//    if(reader.hasError())
//    {
//        return;
//    }
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

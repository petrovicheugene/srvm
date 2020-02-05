//======================================================
#include "ZXMLCalibrationProjectIOHandler.h"

#include <QDebug>
#include <QFileInfo>
#include <QXmlStreamReader>
//======================================================
ZXMLCalibrationProjectIOHandler::ZXMLCalibrationProjectIOHandler(QObject *parent) : QObject(parent)
{

}
//======================================================
bool ZXMLCalibrationProjectIOHandler::zp_writeProjectData(QFile&file,
                                                          const QString& calibrationFilePath,
                                                          const QString &spectrumArrayFilePath)
{
    if(!(file.openMode() & QIODevice::WriteOnly))
    {
        zv_message = tr("File \"%1\" is not open in write mode!").arg(file.fileName());
        emit zg_message(zv_message);
        qCritical().noquote() <<  zv_message;
        return false;
    }

    QFileInfo fi(file.fileName());
    zv_fileAbsolutePath = fi.absolutePath();

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    // head
    writer.writeStartDocument();
    writer.writeStartElement(zv_ROOT);
    writer.writeAttribute(zv_TYPE, zv_magicString);

    writer.writeStartElement(zv_CALIBRATION_PATH);

    QString path = calibrationFilePath;
    if(path.startsWith(zv_fileAbsolutePath, Qt::CaseInsensitive))
    {
        path = (path.remove(0, zv_fileAbsolutePath.length()).prepend("."));
    }

    writer.writeCharacters(path);
    writer.writeEndElement(); // type

    writer.writeStartElement(zv_SPECTRUM_ARRAY_PATH);
    path =spectrumArrayFilePath;
    if(path.startsWith(zv_fileAbsolutePath, Qt::CaseInsensitive))
    {
        path = (path.remove(0, zv_fileAbsolutePath.length()).prepend("."));
    }

    writer.writeCharacters(path);
    writer.writeEndElement(); // type

    writer.writeEndElement(); // root
    writer.writeEndDocument();

    if(file.error() != QFile::NoError)
    {
        zv_message = tr("Cannot write to file \"%1\"! %2").arg(file.fileName(), file.errorString());
        emit zg_message(zv_message);
        qCritical().noquote() <<  zv_message;
        return false;
    }

    return true;
}
//======================================================
bool ZXMLCalibrationProjectIOHandler::zh_detectRoot(const QXmlStreamReader& reader,
                                                    bool& magicStringDetectionFlag) const
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
//======================================================
bool ZXMLCalibrationProjectIOHandler::zp_readProjectData(QFile&file, QString& calibrationFilePath,
                        QString& spectrumArrayFilePath)
{
    if(!(file.openMode() & QIODevice::ReadOnly))
    {
        zv_message = tr("File \"%1\" is not open in read mode!").arg(file.fileName());
        emit zg_message(zv_message);
        qCritical().noquote() <<  zv_message;
        return false;
    }

    QFileInfo fi(file.fileName());
    zv_fileAbsolutePath = fi.absolutePath();

    // root checking
    QXmlStreamReader reader(&file);
    bool rootDetectedFlag = false;
    bool magicStringDetectionFlag = false;
    //bool parsingErrorFalg = false;
    parentTagStack.clear();
    QString readerText;
    QString currentTagName;
    QString path;

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
                zv_message = tr("File \"%1\" is not recognized!").arg(file.fileName());
                emit zg_message(zv_message);
                return false;
            }

            rootDetectedFlag = true;
            //calibration = new ZCalibration(file.fileName(), zv_calibrationParent);
        }

        // root text data handling section
        if(reader.isCharacters())
        {
            if(reader.text().toString().simplified().isEmpty())
            {
                continue;
            }

            readerText = reader.text().toString().simplified();

            if(currentTagName == zv_CALIBRATION_PATH)
            {
                QFileInfo fi(readerText);
                path = fi.isRelative()? zv_fileAbsolutePath + fi.filePath().remove(0,1) : fi.filePath();
                calibrationFilePath = path;
            }
            else if(currentTagName == zv_SPECTRUM_ARRAY_PATH)
            {
                QFileInfo fi(readerText);
                path = fi.isRelative()? zv_fileAbsolutePath + fi.filePath().remove(0,1) : fi.filePath();
                spectrumArrayFilePath = path;
            }
        }
        else if(reader.isStartElement())
        {
            // handling inserted levels
            parentTagStack.push(reader.name().toString());
            currentTagName = reader.name().toString();

        }
    }

    if(reader.hasError())
    {
        zv_message = tr("File \"%1\" parsing failed! %2").arg(file.fileName(), reader.errorString());
        emit zg_message(zv_message);
        qCritical().noquote() <<  zv_message;
        return false;
    }
    else if(file.error() != QFile::NoError)
    {
        zv_message = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
        emit zg_message(zv_message);
        qCritical().noquote() <<  zv_message;
        return false;
    }

    return true;
}
//======================================================


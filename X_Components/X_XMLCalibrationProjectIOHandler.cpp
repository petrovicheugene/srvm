//======================================================
#include "X_XMLCalibrationProjectIOHandler.h"

#include <QDebug>
#include <QFileInfo>
#include <QXmlStreamReader>
//======================================================
X_XMLCalibrationProjectIOHandler::X_XMLCalibrationProjectIOHandler(QObject *parent) : QObject(parent)
{

}
//======================================================
bool X_XMLCalibrationProjectIOHandler::xp_writeProjectData(QFile&file,
                                                          const QString& calibrationFilePath,
                                                          const QString &spectrumArrayFilePath)
{
    if(!(file.openMode() & QIODevice::WriteOnly))
    {
        xv_message = tr("File \"%1\" is not open in write mode!").arg(file.fileName());
        emit xg_message(xv_message);
        qCritical().noquote() <<  xv_message;
        return false;
    }

    QFileInfo fi(file.fileName());
    xv_fileAbsolutePath = fi.absolutePath();

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    // head
    writer.writeStartDocument();
    writer.writeStartElement(xv_ROOT);
    writer.writeAttribute(xv_TYPE, xv_magicString);

    writer.writeStartElement(xv_CALIBRATION_PATH);

    QString path = calibrationFilePath;
    if(path.startsWith(xv_fileAbsolutePath, Qt::CaseInsensitive))
    {
        path = (path.remove(0, xv_fileAbsolutePath.length()).prepend("."));
    }

    writer.writeCharacters(path);
    writer.writeEndElement(); // type

    writer.writeStartElement(xv_SPECTRUM_ARRAY_PATH);
    path =spectrumArrayFilePath;
    if(path.startsWith(xv_fileAbsolutePath, Qt::CaseInsensitive))
    {
        path = (path.remove(0, xv_fileAbsolutePath.length()).prepend("."));
    }

    writer.writeCharacters(path);
    writer.writeEndElement(); // type

    writer.writeEndElement(); // root
    writer.writeEndDocument();

    if(file.error() != QFile::NoError)
    {
        xv_message = tr("Cannot write to file \"%1\"! %2").arg(file.fileName(), file.errorString());
        emit xg_message(xv_message);
        qCritical().noquote() <<  xv_message;
        return false;
    }

    return true;
}
//======================================================
bool X_XMLCalibrationProjectIOHandler::zh_detectRoot(const QXmlStreamReader& reader,
                                                    bool& magicStringDetectionFlag) const
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
//======================================================
bool X_XMLCalibrationProjectIOHandler::xp_readProjectData(QFile&file, QString& calibrationFilePath,
                        QString& spectrumArrayFilePath)
{
    if(!(file.openMode() & QIODevice::ReadOnly))
    {
        xv_message = tr("File \"%1\" is not open in read mode!").arg(file.fileName());
        emit xg_message(xv_message);
        qCritical().noquote() <<  xv_message;
        return false;
    }

    QFileInfo fi(file.fileName());
    xv_fileAbsolutePath = fi.absolutePath();

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
                xv_message = tr("File \"%1\" is not recognized!").arg(file.fileName());
                emit xg_message(xv_message);
                return false;
            }

            rootDetectedFlag = true;
            //calibration = new X_Calibration(file.fileName(), xv_calibrationParent);
        }

        // root text data handling section
        if(reader.isCharacters())
        {
            if(reader.text().toString().simplified().isEmpty())
            {
                continue;
            }

            readerText = reader.text().toString().simplified();

            if(currentTagName == xv_CALIBRATION_PATH)
            {
                QFileInfo fi(readerText);
                path = fi.isRelative()? xv_fileAbsolutePath + fi.filePath().remove(0,1) : fi.filePath();
                calibrationFilePath = path;
            }
            else if(currentTagName == xv_SPECTRUM_ARRAY_PATH)
            {
                QFileInfo fi(readerText);
                path = fi.isRelative()? xv_fileAbsolutePath + fi.filePath().remove(0,1) : fi.filePath();
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
        xv_message = tr("File \"%1\" parsing failed! %2").arg(file.fileName(), reader.errorString());
        emit xg_message(xv_message);
        qCritical().noquote() <<  xv_message;
        return false;
    }
    else if(file.error() != QFile::NoError)
    {
        xv_message = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
        emit xg_message(xv_message);
        qCritical().noquote() <<  xv_message;
        return false;
    }

    return true;
}
//======================================================


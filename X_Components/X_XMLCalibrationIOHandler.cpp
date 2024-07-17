//==========================================================
#include "X_XMLCalibrationIOHandler.h"
#include "X_Calibration.h"

#include <QDebug>
#include <QDateTime>
#include <QFileDialog>
#include <QFileInfo>
#include <QStandardPaths>
#include <QXmlStreamReader>
//==========================================================
//==========================================================
X_XMLCalibrationIOHandler::X_XMLCalibrationIOHandler(QObject* parent) : QObject(parent)
{
    // xv_calibrationParent = calibrationParent;
}
//==========================================================
X_XMLCalibrationIOHandler::~X_XMLCalibrationIOHandler() {}
//==========================================================
QString X_XMLCalibrationIOHandler::xp_getCalibrationOpenFile(const QString& calibrationFolderPath)
{
    QString locationDirString = xp_checkDirPath(calibrationFolderPath);
    QString fileName = QFileDialog::getOpenFileName(
        nullptr,
        tr("Select file to open"),
        locationDirString,
        tr("XML Calibration files(*.clbx);;XML files(*.xml);;All files(*.*)"));
    return fileName;
}
//==========================================================
QStringList X_XMLCalibrationIOHandler::xp_getCalibrationOpenFiles(const QString& calibrationFolderPath)
{
    QString locationDirString = xp_checkDirPath(calibrationFolderPath);
    //    QStringList fileNames = QFileDialog::getOpenFileNames(
    //        nullptr,
    //        tr("Open file"),
    //        locationDirString,
    //        tr("XML Calibration files(*.clbx);;XML files(*.xml);;All files(*.*)"));
    //    return fileNames;

//    X_FileDialog dialog(
//        nullptr,
//        tr("Open file"),
//        locationDirString,
//        tr("XML Calibration files(*.clbx);;XML files(*.xml);;All files(*.*)"));
//    dialog.exec();

    QFileDialog xv_fileDialog(nullptr,
        tr("Open file"),
        locationDirString,
        tr("XML Calibration files(*.clbx);;XML files(*.xml);;All files(*.*)"));
//    xv_fileDialog.setOptions(QFileDialog::DontUseNativeDialog);
    xv_fileDialog.setFileMode(QFileDialog::ExistingFiles);
    xv_fileDialog.exec();
    QStringList fileNames = xv_fileDialog.selectedFiles();


    // return fileNames;
    //    if(!xv_calibrationDialog)
    //    {
    //        xv_calibrationDialog = new QFileDialog;
    //        xv_calibrationDialog->setDirectory(locationDirString);
    //        xv_calibrationDialog->setNameFilter(tr("XML Calibration files(*.clbx);;XML files(*.xml);;All files(*.*)"));
    //        xv_calibrationDialog->setWindowTitle(tr("Open calibration files"));
    //        xv_calibrationDialog->setFileMode(QFileDialog::ExistingFiles);
    //        xv_calibrationDialog->setModal(true);
    //    }

    //    xv_calibrationDialog->exec();

    //    //QStringList fileNames = dialog->selectedFiles();
    //    //delete dialog;
    //    xv_calibrationDialog->close();
    return fileNames;
}
//==========================================================
QString X_XMLCalibrationIOHandler::xp_getCalibrationSaveFile(const QString& calibrationFilePath)
{
    QString fileName = QFileDialog::getSaveFileName(
        nullptr,
        tr("Save file"),
        calibrationFilePath,
        tr("XML Calibration files(*.clbx);;XML files(*.xml);;All files(*.*)"));
    return fileName;
}
//==========================================================
QString X_XMLCalibrationIOHandler::xp_checkDirPath(const QString& calibrationFolderPath)
{
    QString locationDirString = calibrationFolderPath;
    QFileInfo fileInfo(locationDirString);
    if (!fileInfo.exists() || !fileInfo.isDir())
    {
        QStringList docLocations = QStandardPaths::standardLocations(
            QStandardPaths::DocumentsLocation);
        if (docLocations.isEmpty())
        {
            locationDirString = QDir::currentPath();
        }
        else
        {
            locationDirString = docLocations.value(0);
        }
    }

    return locationDirString;
}
//==========================================================
bool X_XMLCalibrationIOHandler::xp_writeCalibrationToFile(QFile& file,
                                                          const X_Calibration* calibration)
{
    if (!(file.openMode() & QIODevice::WriteOnly))
    {
        xv_message = tr("File \"%1\" is not open in write mode!").arg(file.fileName());
        emit xg_message(xv_message);
        qCritical().noquote() << xv_message;
        return false;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    // head
    writer.writeStartDocument();
    writer.writeStartElement(xv_ROOT);
    writer.writeAttribute(xv_TYPE, xv_magicString);

    // date
    writer.writeStartElement(xv_DATE_TIME);
    writer.writeCharacters(calibration->xp_dateTime().toString(xv_dateTimeFormat));
    writer.writeEndElement(); // date-time

    // chem element
    writer.writeStartElement(xv_CHEMELEMENT);
    writer.writeCharacters(calibration->xp_chemElement());
    writer.writeEndElement(); // chem element

    // Energy calibration
    //    writer.writeStartElement(xv_ENERGY_K0);
    //    writer.writeCharacters(QString::number(calibration->xp_energyCalibrationK0()));
    //    writer.writeEndElement(); // k0

    //    writer.writeStartElement(xv_ENERGY_K1);
    //    writer.writeCharacters(QString::number(calibration->xp_energyCalibrationK1()));
    //    writer.writeEndElement(); // k1

    //    writer.writeStartElement(xv_ENERGY_K2);
    //    writer.writeCharacters(QString::number(calibration->xp_energyCalibrationK2()));
    //    writer.writeEndElement(); // k2

    // gain factor
    writer.writeStartElement(xv_GAIN_FACTOR);
    writer.writeCharacters(QString::number(calibration->xp_gainFactor()));
    writer.writeEndElement(); // gainFactor

    // energy unit
    //    writer.writeStartElement(xv_ENERGY_UNIT);
    //    writer.writeCharacters(calibration->xp_energyUnit());
    //    writer.writeEndElement(); // energy unit

    // exposition
    writer.writeStartElement(xv_EXPOSITION);
    writer.writeCharacters(QString::number(calibration->xp_exposition()));
    writer.writeEndElement(); // exposition

    // Quality data
    // R2
    writer.writeStartElement(xv_DETERMINATION_R2);
    writer.writeCharacters(calibration->xp_calibrationQualityData().determination);
    writer.writeEndElement(); // R2

    // R2adj
    writer.writeStartElement(xv_ADJUSTED_DETERMINATION_R2ADJ);
    writer.writeCharacters(calibration->xp_calibrationQualityData().adj_determination);
    writer.writeEndElement(); // R2adj

    // standard deviation s
    writer.writeStartElement(xv_STANDARD_DEVIATION);
    writer.writeCharacters(calibration->xp_calibrationQualityData().standardDeviation);
    writer.writeEndElement(); // s

    // EQUATION DATA
    // windows
    writer.writeStartElement(xv_WINDOW_LIST);
    for (int w = 0; w < calibration->xp_calibrationWindowCount(); w++)
    {
        // start window + name
        writer.writeStartElement(xv_WINDOW);
        writer.writeAttribute(xv_NAME, calibration->xp_calibrationWindowName(w));
        // window type
        writer.writeStartElement(xv_TYPE);
        writer.writeCharacters(
            X_CalibrationWindow::xp_typeName(calibration->xp_calibrationWindowType(w)));
        writer.writeEndElement(); // type
        // first channel
        writer.writeStartElement(xv_FIRST_CHANNEL);
        writer.writeCharacters(QString::number(calibration->xp_calibrationWindowFirstChannel(w)));
        writer.writeEndElement(); // first channel
        // last channel
        writer.writeStartElement(xv_LAST_CHANNEL);
        writer.writeCharacters(QString::number(calibration->xp_calibrationWindowLastChannel(w)));
        writer.writeEndElement(); // last channel

        writer.writeEndElement(); // window
    }
    writer.writeEndElement(); // windows

    // terms
    writer.writeStartElement(xv_TERM_LIST);
    for (int t = 0; t < calibration->xp_termCount(); t++)
    {
        // start term + name
        writer.writeStartElement(xv_TERM);
        writer.writeAttribute(xv_NAME, calibration->xp_termName(t));
        // term type
        writer.writeStartElement(xv_TYPE);

        writer.writeCharacters(X_AbstractTerm::xp_termTypeName(calibration->xp_termType(t)));
        writer.writeEndElement(); // type
        // term state
        writer.writeStartElement(xv_STATE);
        writer.writeCharacters(X_AbstractTerm::xp_termStateName(calibration->xp_termState(t)));
        writer.writeEndElement(); // term state

        // term custom string
        if (calibration->xp_termType(t) == X_AbstractTerm::TT_CUSTOM)
        {
            writer.writeStartElement(xv_CUSTOM_STRING);
            writer.writeCharacters(calibration->xp_termCustomString(t));
            writer.writeEndElement(); // term state

            writer.writeStartElement(xv_DESCRIPTION);
            writer.writeCharacters(calibration->xp_termDescription(t));
            writer.writeEndElement(); // term state
        }

        // term windows
        QStringList windowList = calibration->xp_termWindowsNameList(t);
        foreach (QString window, windowList)
        {
            writer.writeStartElement(xv_TERM_WINDOW);
            writer.writeCharacters(window);
            writer.writeEndElement(); // term window
        }

        // term factor
        writer.writeStartElement(xv_FACTOR);
        writer.writeCharacters(calibration->xp_termFactorString(t));
        writer.writeEndElement(); // term factor

        // end term
        writer.writeEndElement(); // term
    }
    writer.writeEndElement(); // terms

    // common term normalizer
    writer.writeStartElement(xv_NORMALIX_ER);
    // type
    writer.writeStartElement(xv_TYPE);
    writer.writeCharacters(X_TermNormalizer::xp_normalizerTypeName(calibration->xp_normaType()));
    writer.writeEndElement(); // type
    // custom string
    writer.writeStartElement(xv_CUSTOM_STRING);
    writer.writeCharacters(calibration->xp_normaCustomString());
    writer.writeEndElement(); // custom string
    writer.writeEndElement(); // common normalizer

    // equation
    writer.writeStartElement(xv_EQUATION);
    // type
    writer.writeStartElement(xv_TYPE);
    writer.writeCharacters(X_Calibration::xp_equationTypeString(calibration->xp_equationType()));
    writer.writeEndElement(); // type
    // base term
    writer.writeStartElement(xv_BASE_TERM);
    writer.writeCharacters(calibration->xp_baseTermString());
    writer.writeEndElement(); // base term

    // base term normalizer
    writer.writeStartElement(xv_FRACTIONAL_BASE_NORMALIX_ER);
    // type
    writer.writeStartElement(xv_TYPE);
    writer.writeCharacters(X_TermNormalizer::xp_normalizerTypeName(calibration->xp_baseTermNormaType()));
    writer.writeEndElement(); // type
    // custom string
    writer.writeStartElement(xv_CUSTOM_STRING);
    writer.writeCharacters(calibration->xp_baseTermNormaCustomString());
    writer.writeEndElement(); // custom string

    writer.writeEndElement(); // base term normalizer

    // intercept
    writer.writeStartElement(xv_INTERCEPT);
    writer.writeCharacters(calibration->xp_equationInterceptString());
    writer.writeEndElement(); // intercept

    writer.writeEndElement(); // equation

    writer.writeEndElement(); // root
    writer.writeEndDocument();

    if (file.error() != QFile::NoError)
    {
        xv_message = tr("Cannot write to file \"%1\"! %2").arg(file.fileName(), file.errorString());
        emit xg_message(xv_message);
        qCritical().noquote() << xv_message;
        return false;
    }

    return true;
}
//==========================================================
QString X_XMLCalibrationIOHandler::xp_message() const
{
    return xv_message;
}
//==========================================================
bool X_XMLCalibrationIOHandler::xp_getCalibrationXMLByteArrayFromFile(
    QFile& file, QByteArray& calibrationXMLByteArray)
{
    if (!(file.openMode() & QIODevice::ReadOnly))
    {
        xv_message = tr("File \"%1\" is not open in read mode!")
                         .arg(file.fileName());
        emit xg_message(xv_message);
        qCritical().noquote() <<  xv_message;
        return false;
    }

    // at start position
    file.seek(0);
    calibrationXMLByteArray = file.readAll();

    return true;
}
//==========================================================																	  
bool X_XMLCalibrationIOHandler::xp_getCalibrationFromFile(QFile& file, X_Calibration* calibration)
{
    if (calibration == nullptr)
    {
        return false;
    }

    if (!(file.openMode() & QIODevice::ReadOnly))
    {
        xv_message = tr("File \"%1\" is not open in read mode!").arg(file.fileName());
        emit xg_message(xv_message);
        qCritical().noquote() << xv_message;
        return false;
    }

    // root checking
    QXmlStreamReader reader(&file);
    bool rootDetectedFlag = false;
    bool magicStringDetectionFlag = false;
    //bool parsingErrorFalg = false;
    parentTagStack.clear();
    // handling root level
    while (!reader.atEnd())
    {
        reader.readNext();
        // property root element detection section
        if (!rootDetectedFlag)
        {
            if (!xh_detectRoot(reader, magicStringDetectionFlag))
            {
                continue;
            }

            if (!magicStringDetectionFlag)
            {
                xv_message = tr("File \"%1\" is not recognized!").arg(file.fileName());
                emit xg_message(xv_message);
                return false;
            }

            rootDetectedFlag = true;
            //calibration = new X_Calibration(file.fileName(), xv_calibrationParent);
        }

        // root text data handling section
        if (reader.isCharacters())
        {
            if (reader.text().toString().simplified().isEmpty())
            {
                continue;
            }
        }
        else if (reader.isStartElement())
        {
            // handling inserted levels
            parentTagStack.push(reader.name().toString());
            xh_parseXMLElement(calibration, reader);
        }
    }

    if (reader.hasError())
    {
        xv_message = tr("File \"%1\" parsing failed! %2").arg(file.fileName(), reader.errorString());
        emit xg_message(xv_message);
        qCritical().noquote() << xv_message;
        return false;
    }
    else if (file.error() != QFile::NoError)
    {
        xv_message = tr("Cannot read file \"%1\"! %2").arg(file.fileName(), file.errorString());
        emit xg_message(xv_message);
        qCritical().noquote() << xv_message;
        return false;
    }

    calibration->xp_setDirty(false);
    return true;
}
//==========================================================
bool X_XMLCalibrationIOHandler::xp_getCalibrationFromString(QString& calibrationString, X_Calibration* calibration)
{
    if (calibration == 0 || calibrationString.isEmpty())
    {
        return false;
    }

    QXmlStreamReader reader(calibrationString);

    // root checking
    bool rootDetectedFlag = false;
    bool magicStringDetectionFlag = false;
    //bool parsingErrorFalg = false;
    parentTagStack.clear();
    // handling root level
    while (!reader.atEnd())
    {
        reader.readNext();
        // property root element detection section
        if (!rootDetectedFlag)
        {
            if (!xh_detectRoot(reader, magicStringDetectionFlag))
            {
                continue;
            }

            if (!magicStringDetectionFlag)
            {
                xv_message = tr("Calibration data is not recognized!");
                emit xg_message(xv_message);
                return false;
            }

            rootDetectedFlag = true;
            //calibration = new X_Calibration(file.fileName(), xv_calibrationParent);
        }

        // root text data handling section
        if (reader.isCharacters())
        {
            if (reader.text().toString().simplified().isEmpty())
            {
                continue;
            }
        }
        else if (reader.isStartElement())
        {
            // handling inserted levels
            parentTagStack.push(reader.name().toString());
            xh_parseXMLElement(calibration, reader);
        }
    }

    if (reader.hasError())
    {
        xv_message = tr("Calibration data parsing failed! %1")
                         .arg(reader.errorString());
        emit xg_message(xv_message);
        return false;
    }

    // calibration->xp_setDateTime(xv_dateTime);
    calibration->xp_setDirty(false);
    return true;
}
//============================================================
// returns true if closing tag that was opened in calling function is closed
void X_XMLCalibrationIOHandler::xh_parseXMLElement(X_Calibration* calibration,
                                                   QXmlStreamReader& reader)
{
    // current start tag handling
    QString currentTagName = reader.name().toString();
    //    parentTagStack.push(currentTagName);
    //    // attributes handling
    if (currentTagName == xv_WINDOW)
    {
        // create new raw window
        xv_rawWindow = X_RawWindow();
        xv_rawWindow.name = reader.attributes().value(xv_NAME).toString();
    }
    else if (currentTagName == xv_TERM)
    {
        // create new raw term
        xv_rawTerm = X_RawTerm();
        xv_rawTerm.name = reader.attributes().value(xv_NAME).toString();
    }

    QString readerText;
    while (!reader.atEnd())
    {
        reader.readNext();

        if (reader.isStartElement())
        {
            parentTagStack.push(currentTagName);
            xh_parseXMLElement(calibration, reader);
            continue;
        }
        else if (reader.isCharacters())
        {
            if (reader.text().toString().simplified().isEmpty())
            {
                continue;
            }

            readerText = reader.text().toString().simplified();
            if (currentTagName == xv_DATE_TIME)
            {
                QString dateTimeString = readerText;
                QDateTime dateTime = QDateTime::fromString(dateTimeString, xv_dateTimeFormat);
                calibration->xp_setDateTime(dateTime);
            }
            else if (currentTagName == xv_CHEMELEMENT)
            {
                calibration->xp_setChemElement(readerText);
            }

            else if (currentTagName == xv_GAIN_FACTOR)
            {
                bool ok;
                int intValue = readerText.toInt(&ok);
                if (!ok)
                {
                    intValue = -1;
                }

                calibration->xp_setGainFactor(intValue);
            }
            else if (currentTagName == xv_EXPOSITION)
            {
                bool ok;
                int intValue = readerText.toInt(&ok);
                if (!ok)
                {
                    intValue = -1;
                }

                calibration->xp_setExposition(intValue);
            }
            else if (currentTagName == xv_DETERMINATION_R2)
            {
                calibration->xp_setDetermination(readerText);
            }
            else if (currentTagName == xv_ADJUSTED_DETERMINATION_R2ADJ)
            {
                calibration->xp_setAdjustedDetermination(readerText);
            }
            else if (currentTagName == xv_STANDARD_DEVIATION)
            {
                calibration->xp_setStandardDeviation(readerText);
            }
            else if (currentTagName == xv_TYPE)
            {
                if (parentTagStack.last() == xv_WINDOW)
                {
                    xv_rawWindow.windowType = X_CalibrationWindow::xp_typeFromString(readerText);
                }
                else if (parentTagStack.last() == xv_TERM)
                {
                    xv_rawTerm.termType = X_AbstractTerm::xp_termTypeFromString(readerText);
                }
                else if (parentTagStack.last() == xv_NORMALIX_ER)
                {
                    calibration->xp_setNormaType(X_TermNormalizer::xp_normalizerTypeFromString(readerText));
                }
                else if (parentTagStack.last() == xv_FRACTIONAL_BASE_NORMALIX_ER)
                {
                    calibration->xp_setBaseTermNormaType(
                        X_TermNormalizer::xp_normalizerTypeFromString(readerText));
                }
                else if (parentTagStack.last() == xv_EQUATION)
                {
                    calibration->xp_setEquationType(
                        X_Calibration::xp_equationTypeFromString(readerText));
                }
            }
            else if (currentTagName == xv_FIRST_CHANNEL)
            {
                xv_rawWindow.firstChannel = readerText;
            }
            else if (currentTagName == xv_LAST_CHANNEL)
            {
                xv_rawWindow.lastChannel = readerText;
            }
            else if (currentTagName == xv_STATE)
            {
                xv_rawTerm.termState = X_AbstractTerm::xp_termStateFromString(readerText);
            }
            else if (currentTagName == xv_DESCRIPTION)
            {
                xv_rawTerm.descriptionString = readerText;
            }
            else if (currentTagName == xv_TERM_WINDOW)
            {
                xv_rawTerm.windowList.append(readerText);
            }
            else if (currentTagName == xv_FACTOR)
            {
                xv_rawTerm.factor = readerText;
            }
            else if (currentTagName == xv_CUSTOM_STRING)
            {
                if (parentTagStack.last() == xv_NORMALIX_ER)
                {
                    calibration->xp_setNormaCustomString(readerText);
                }
                else if (parentTagStack.last() == xv_FRACTIONAL_BASE_NORMALIX_ER)
                {
                    calibration->xp_setBaseTermNormaCustomString(readerText);
                }
                else if (parentTagStack.last() == xv_TERM)
                {
                    xv_rawTerm.customString = readerText;
                }
            }
            else if (currentTagName == xv_BASE_TERM)
            {
                calibration->xp_setBaseTermFromName(readerText);
            }
            else if (currentTagName == xv_INTERCEPT)
            {
                calibration->xp_setEquationInterceptString(readerText);
            }
        }
        else if (reader.isEndElement())
        {
            if (currentTagName == xv_WINDOW)
            {
                calibration->xp_createNewCalibrationWindow(xv_rawWindow);
            }
            else if (currentTagName == xv_TERM)
            {
                if (xv_rawTerm.termType == X_AbstractTerm::TT_CUSTOM)
                {
                    calibration->xp_applyRawTerm(xv_rawTerm);
                }
                else
                {
                    calibration->xp_applyRawTerm(xv_rawTerm);
                }
            }
            parentTagStack.pop();
            return; // parent tag is closed
        }
    }
}
//============================================================
bool X_XMLCalibrationIOHandler::xh_detectRoot(const QXmlStreamReader& reader,
                                              bool& magicStringDetectionFlag) const
{
    // true - element name == root, error - trnado magic string existing flag
    if (reader.tokenType() != QXmlStreamReader::StartElement || reader.name() != xv_ROOT)
    {
        return false;
    }

    if (reader.attributes().value(xv_TYPE).toString() == xv_magicString)
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
bool X_XMLCalibrationIOHandler::xh_checkfilePath(const QString& path) const
{
    QFileInfo fileInfo(path);
    if ((!fileInfo.exists()) || (!fileInfo.isFile()))
    {
        return false;
    }

    return true;
}
//============================================================

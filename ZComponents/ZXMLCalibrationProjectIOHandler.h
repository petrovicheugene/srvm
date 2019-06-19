//======================================================
#ifndef ZXMLCALIBRATIONPROJECT_H
#define ZXMLCALIBRATIONPROJECT_H
//======================================================
#include <QObject>
#include <QApplication>
#include <QFile>
#include <QStack>
#include <QXmlStreamReader>

//======================================================
class ZXMLCalibrationProjectIOHandler : public QObject
{
    Q_OBJECT
public:
    explicit ZXMLCalibrationProjectIOHandler(QObject *parent = nullptr);

    bool zp_writeProjectData(QFile&file, const QString& calibrationFilePath,
                             const QString& spectrumArrayFilePath);
    bool zp_readProjectData(QFile&file, QString& calibrationFilePath,
                            QString& spectrumArrayFilePath);

signals:

    void zg_message(const QString&);

public slots:


private:

    // VARS
    QStack<QString> parentTagStack;

    QString zv_message;
    const QString zv_magicString = qApp->organizationName()  + " E5RT73F9JEP2IXMSD5LG";
    // elment names
    const QString zv_ROOT = "root";
    const QString zv_TYPE = "type";
    const QString zv_CALIBRATION_PATH = "calibrationPath";
    const QString zv_SPECTRUM_ARRAY_PATH = "spectrumArrayPath";

    // FUNCS
    bool zh_detectRoot(const QXmlStreamReader& reader,
                       bool& magicStringDetectionFlag) const;

};
//======================================================
#endif // ZXMLCALIBRATIONPROJECT_H

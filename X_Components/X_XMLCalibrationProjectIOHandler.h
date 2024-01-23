//======================================================
#ifndef X_XMLCALIBRATIONPROJECT_H
#define X_XMLCALIBRATIONPROJECT_H
//======================================================
#include <QObject>
#include <QApplication>
#include <QFile>
#include <QStack>
#include <QXmlStreamReader>

//======================================================
class X_XMLCalibrationProjectIOHandler : public QObject
{
    Q_OBJECT
public:
    explicit X_XMLCalibrationProjectIOHandler(QObject *parent = nullptr);

    bool xp_writeProjectData(QFile&file, const QString& calibrationFilePath,
                             const QString& spectrumArrayFilePath);
    bool xp_readProjectData(QFile&file, QString& calibrationFilePath,
                            QString& spectrumArrayFilePath);

signals:

    void xg_message(const QString&);

public slots:


private:

    // VARS
    QStack<QString> parentTagStack;

    QString xv_message;
    const QString xv_magicString = qApp->organizationName()  + " E5RT73F9JEP2IXMSD5LG";
    // elment names
    const QString xv_ROOT = "root";
    const QString xv_TYPE = "type";
    const QString xv_CALIBRATION_PATH = "calibrationPath";
    const QString xv_SPECTRUM_ARRAY_PATH = "spectrumArrayPath";

    QString xv_fileAbsolutePath;

    // FUNCS
    bool xh_detectRoot(const QXmlStreamReader& reader,
                       bool& magicStringDetectionFlag) const;

};
//======================================================
#endif // X_XMLCALIBRATIONPROJECT_H

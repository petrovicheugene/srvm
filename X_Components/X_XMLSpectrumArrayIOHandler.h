//============================================================
#ifndef X_XMLARRAYIOHANDLER_H
#define X_XMLARRAYIOHANDLER_H
//============================================================
#include <QObject>
#include <QApplication>
#include <QFile>
#include "X_RawSpectrumArray.h"
#include "X_General.h"
//============================================================
class QXmlStreamReader;
//============================================================
class X_XMLSpectrumArrayIOHandler : public QObject
{
    Q_OBJECT
public:
    explicit X_XMLSpectrumArrayIOHandler(QObject* parent = 0);
    virtual ~X_XMLSpectrumArrayIOHandler();

    bool xp_readSpectrumArray (QFile&, QList<X_RawSpectrumArray>&);
    bool xp_writeSpectrumArray(QFile&, const QList<X_RawSpectrumArray>&);

signals:

    void xg_message(QString) const;

private:

    // VARS
    const QString xv_magicString = qApp->organizationName() + " 5FG3P72BNW8HS1L9FGX";
    // elment names
    const QString xv_ROOT = "root";
    const QString xv_TYPE = "type";
    const QString xv_NAME = "name";
    const QString xv_GAIN_FACTOR = "gainFactor";

    const QString xv_ARRAY = "array";
    const QString xv_SPECTRUM = "spectrum";
    const QString xv_PATH = "path";
    const QString xv_CONCENTRATION = "concentration";
    const QString xv_CHECKED = "checked";
    const QString xv_VISIBLE = "visible";

    const QString xv_YES = "yes";
    const QString xv_NO = "no";

    QString xv_fileAbsolutePath;

    // FUNCS
    bool zh_detectRoot(const QXmlStreamReader& reader, bool& magicStringDetectionFlag) const;
    void zh_parseXMLElement(QList<X_RawSpectrumArray>&,
                            int currentArrayIndex,
                            int currentSpectrumIndex,
                            QXmlStreamReader&) const;
    bool zh_checkfilePath(const QString& path) const;
};
//============================================================
#endif // X_XMLARRAYIOHANDLER_H

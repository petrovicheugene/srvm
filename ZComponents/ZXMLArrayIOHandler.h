//============================================================
#ifndef ZXMLARRAYIOHANDLER_H
#define ZXMLARRAYIOHANDLER_H
//============================================================
#include "ZAbstractArrayIOHandler.h"
#include "glVariables.h"
//============================================================
class QXmlStreamReader;


//============================================================
class ZXMLArrayIOHandler : public ZAbstractArrayIOHandler
{
    Q_OBJECT
public:
    explicit ZXMLArrayIOHandler(QObject* parent = 0);
    // virtual bool zp_readSpectrumArray (ZSpectrumArray& , QFile&) override;
    virtual bool zp_readSpectrumArray (QList<ZRawArray>&, QFile&) override;


private:

    // VARS
    const QString zv_magicString = glAppCompany + " 5FG3P72BNW8HS1L9FGX";
    // elment names
    const QString zv_ROOT = "root";
    const QString zv_TYPE = "type";
    const QString zv_NAME = "name";
    const QString zv_ARRAY = "array";
    const QString zv_SPECTRUM = "spectrum";
    const QString zv_PATH = "path";
    const QString zv_CONCENTRATION = "concentration";

    const QString zv_YES = "yes";
    const QString zv_NO = "no";


    // FUNCS
    bool zh_detectRoot(const QXmlStreamReader& reader, bool& magicStringDetectionFlag) const;
    bool zh_parseXMLElement(QList<ZRawArray>&,
                            int currentArrayIndex,
                            int currentSpectrumIndex,
                            QXmlStreamReader&);
    bool zh_checkfilePath(const QString& path) const;
};
//============================================================
#endif // ZXMLARRAYIOHANDLER_H

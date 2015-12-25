//====================================================
#ifndef ZCALIBRATIONWINDOW_H
#define ZCALIBRATIONWINDOW_H
//====================================================
#include <QString>
#include <QMap>
//====================================================
class ZCalibrationWindow
{
public:
    enum WindowType{WT_NOT_DEFINED,
                    WT_BASE_PEAK,
                    WT_PEAK,
                    WT_COHERENT,
                    WT_INCOHERENT};

    ZCalibrationWindow();
    ZCalibrationWindow(const QString&,  WindowType = WT_NOT_DEFINED );

    bool zp_isWindowVisible() const;
    void zp_setWindowVisible(bool);

    QString zp_name() const;
    bool zp_setName(const QString&);

    int zp_firstChannel() const;
    int zp_lastChannel() const;

    bool  zp_setWindowMarginChannels(int, int);

    WindowType zp_type() const;
    void zp_setType(WindowType);

    qint64 zp_windowId() const;

    static QString zp_typeName(WindowType);

private:


    QString zv_name;
    int zv_firstChannel;
    int zv_lastChannel;
    WindowType zv_type;
    bool zv_visible;
    qint64 zv_windowId;

    // STATIC
    // VARS
    static qint64 zv_lastWindowId;
    static QMap<WindowType, QString> zv_typeNameMap;

    // FUNCS
    static QMap<WindowType, QString> zh_intTypeNameMap();

};
//====================================================
#endif // ZCALIBRATIONWINDOW_H

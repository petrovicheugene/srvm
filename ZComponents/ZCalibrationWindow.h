//====================================================
#ifndef ZCALIBRATIONWINDOW_H
#define ZCALIBRATIONWINDOW_H
//====================================================
#include <QObject>
#include <QString>
#include <QMap>
//====================================================
class ZCalibrationWindow : public QObject
{
   Q_OBJECT
public:
    enum WindowType{WT_NOT_DEFINED,
                    WT_BASE_PEAK,
                    WT_PEAK,
                    WT_COHERENT,
                    WT_INCOHERENT};

    explicit ZCalibrationWindow(QObject* parent);
    // explicit ZCalibrationWindow(const QString&,  WindowType = WT_NOT_DEFINED );
    explicit ZCalibrationWindow(const QString&,
                                WindowType,
                                int firstChannel,
                                int lastChannel,
                                QObject* parent);

    bool zp_isWindowVisible() const;
    bool zp_setWindowVisible(bool);

    QString zp_windowName() const;
    bool zp_setWindowName(const QString&);

    int zp_firstChannel() const;
    int zp_lastChannel() const;

    bool zp_setWindowFirstChannel(int channel);
    bool zp_setWindowLastChannel(int channel);

    bool  zp_setWindowMarginChannels(int, int);

    WindowType zp_type() const;
    bool zp_setWindowType(WindowType);

    qint64 zp_windowId() const;
    static QString zp_typeName(WindowType);
    static WindowType zp_typeForName(const QString& );
private:


    QString zv_windowName;
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

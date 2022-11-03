//====================================================
#ifndef X_CALIBRATIONWINDOW_H
#define X_CALIBRATIONWINDOW_H
//====================================================
#include <QObject>
#include <QString>
#include <QMap>
#include <QPair>
//====================================================
class X_AbstractTerm;
class X_AbstractSpectrum;
//====================================================
class X_CalibrationWindow : public QObject
{
    Q_OBJECT
public:
    enum WindowType{WT_NOT_DEFINED,
                    //WT_BASE_PEAK,
                    WT_PEAK,
                    WT_COHERENT,
                    WT_INCOHERENT};

    explicit X_CalibrationWindow(QObject* parent);
    // explicit X_CalibrationWindow(const QString&,  WindowType = WT_NOT_DEFINED );
    explicit X_CalibrationWindow(const QString&,
                                WindowType,
                                int firstChannel,
                                int lastChannel,
                                QObject* parent);

    bool xp_isWindowVisible() const;
    bool xp_setWindowVisible(bool);

    QString xp_windowName() const;
    bool xp_setWindowName(const QString&);

    int xp_firstChannel() const;
    int xp_lastChannel() const;

    bool xp_setWindowFirstChannel(int channel);
    bool xp_setWindowLastChannel(int channel);

    bool  xp_setWindowMarginChannels(int, int);

    WindowType xp_windowType() const;
    bool xp_setWindowType(WindowType);
    qint64 xp_windowId() const;

    // STATIC FUNCS
    static QString xp_displayTypeName(WindowType);
    static QString xp_typeName(WindowType);
    static WindowType xp_typeFromString(const QString& );

public slots:

    void xp_calcWindowIntensity(const QObject*, qreal &, bool keepBufferClean, bool *ok);

signals:

    void xg_windowNameChanged() const;
    void xg_windowMarginsChanged() const;
    void xg_windowTypeChanged(X_CalibrationWindow::WindowType previousType, X_CalibrationWindow::WindowType currentType);

private:

    QString xv_windowName;
    int xv_firstChannel;
    int xv_lastChannel;
    WindowType xv_type;
    bool xv_visible;
    qint64 xv_windowId;

    // buffer
    qint64 xv_spectrumId;
    qreal xv_windowIntensityValue;

    // STATIC
    // VARS
    static qint64 xv_lastWindowId;
    static QMap<WindowType, QPair<QString, QString>> xv_typeNameMap;

    // FUNCS
    static QMap<WindowType, QPair<QString, QString> > zh_intTypeNameMap();

};
//====================================================
#endif // X_CALIBRATIONWINDOW_H

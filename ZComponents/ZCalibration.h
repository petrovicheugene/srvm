//=========================================================
#ifndef QABSTRACTCALIBRATION_H
#define QABSTRACTCALIBRATION_H
//=========================================================
#include <QObject>
#include <QMap>
#include <QColor>
#include "ZCalibrationWindow.h"
//=========================================================
class ZAbstractSpectrum;
//=========================================================
class ZCalibration : public QObject
{
    Q_OBJECT

public:

    explicit ZCalibration(const QString& name, QObject *parent = 0);
    virtual ~ZCalibration();

    // VARS
    enum WindowOperationType {WOT_INSERT_WINDOWS,
                        WOT_END_INSERT_WINDOWS,
                        WOT_REMOVE_WINDOWS,
                        WOT_END_REMOVE_WINDOWS,
                        WOT_CHANGED};

    // FUNCS
    QString zp_name() const;
    void zp_setBaseName(const QString&);

    QString zp_path() const;
    void zp_setPath(const QString&);

    QString zp_chemElement() const;
    bool zp_setChemElement(const QString&);

    bool zp_isVisible() const;
    void zp_setVisible(bool);

    bool zp_isDirty() const;
    QColor zp_color() const;
    qint64 zp_calibrationId() const;
    double zp_calcConcentration(const ZAbstractSpectrum* const, bool *ok = 0);

    // windows
    void zp_createNewCalibrationWindow(int firstChannel = 0, int lastChannel = 0);
    bool zp_isCalibrationWindowVisible(int windowIndex);
    bool zp_setCalibrationWindowVisible(int windowIndex, bool visibility);
    int zp_calibrationWindowCount();
    QString zp_calibrationWindowName(int windowIndex);
    bool zp_setCalibrationWindowName(int windowIndex, const QString&);
    ZCalibrationWindow::WindowType zp_calibrationWindowType(int windowIndex);
    bool zp_setCalibrationWindowType(int windowIndex, ZCalibrationWindow::WindowType type);

    int zp_calibrationWindowFirstChannel(int windowIndex);
    bool zp_setCalibrationWindowFirstChannel(int windowIndex, int channel);
    int zp_calibrationWindowLastChannel(int windowIndex);
    bool zp_setCalibrationWindowLastChannel(int windowIndex, int channel);

    qint64 zp_spectrumWindowId(int windowIndex);
    bool zp_removeSpectrumWindow(int);

signals:

    void zg_message(QString) const;
    void zg_windowOperation(ZCalibration::WindowOperationType, int first, int last) const;
    void zg_visibilityChanged(bool) const;
    void zg_dirtyChanged(bool) const;

public slots:


private:

    // VARS
    QString zv_baseName;
    QString zv_path;
    QString zv_chemElement;
    QColor zv_color;
    qint64 zv_calibrationId;

    bool zv_visibility;
    bool zv_dirty;
    QString zv_defaultWindowName;

    QList<ZCalibrationWindow> zv_spectrumWindowList;

    // FUNCS
    bool zh_isWindowExist(const QString&);

    // STATIC
    // VARS
    static QList<QColor> zv_colorList;
    static qint64 zv_lastCalibrationId;
    static int zv_lastColorIndex;

    //  FUNCS
    static QList<QColor> zp_createColorList();
    static bool checkColor(QColor color);

};
//=========================================================
#endif // QABSTRACTCALIBRATION_H

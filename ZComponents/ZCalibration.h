//=========================================================
#ifndef QABSTRACTCALIBRATION_H
#define QABSTRACTCALIBRATION_H
//=========================================================
#include <QObject>
#include <QMap>
#include <QColor>
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

signals:

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

    static QList<QColor> zv_colorList;
    static qint64 zv_lastCalibrationId;
    static int zv_lastColorIndex;

    // FUNCS
    static QList<QColor> zp_createColorList();
    static bool checkColor(QColor color);

};
//=========================================================
#endif // QABSTRACTCALIBRATION_H

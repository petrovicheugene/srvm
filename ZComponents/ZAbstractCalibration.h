//=========================================================
#ifndef QABSTRACTCALIBRATION_H
#define QABSTRACTCALIBRATION_H
//=========================================================
#include <QObject>
#include <QMap>
//=========================================================
class ZAbstractSpectrum;
//=========================================================
class ZAbstractCalibration : public QObject
{
    Q_OBJECT

public:

    explicit ZAbstractCalibration(const QString& path, QObject *parent = 0);
    virtual ~ZAbstractCalibration();

    // VARS
    enum CalibrationType {CT_NA, CT_SD, CT_XML};

    // Static Func
    static QMap<ZAbstractCalibration::CalibrationType, QString> zp_createSuffixMap();

    // continue VARS
    static const QMap<ZAbstractCalibration::CalibrationType, QString> fileSuffixMap;

    // FUNCS
    QString zp_name() const;
    void zp_setBaseName(const QString&);

    QString zp_path() const;
    void zp_setPath(const QString&);

    CalibrationType zp_type() const;

    bool zp_isVisible() const;
    void zp_setVisible(bool);

    bool zp_isDirty() const;
    QString zp_suffix() const;

    virtual double zp_calcConcentration(ZAbstractSpectrum* const, bool *ok = 0) = 0;

signals:

    void zg_visibilityChanged(bool) const;
    void zg_dirtyChanged(bool) const;

public slots:



protected:

    // VARS
    CalibrationType zv_type;
    QString zv_baseName;
    QString zv_path;

    bool zv_visibility;
    bool zv_dirty;

};
//=========================================================
#endif // QABSTRACTCALIBRATION_H

//=========================================================
#ifndef QABSTRACTCALIBRATION_H
#define QABSTRACTCALIBRATION_H
//=========================================================
#include <QObject>
#include <QMap>
//=========================================================
class ZAbstractSpectrum;
//=========================================================
class ZCalibration : public QObject
{
    Q_OBJECT

public:

    explicit ZCalibration(const QString& path, QObject *parent = 0);
    virtual ~ZCalibration();

    // VARS

    // FUNCS
    QString zp_name() const;
    void zp_setBaseName(const QString&);

    QString zp_path() const;
    void zp_setPath(const QString&);

    bool zp_isVisible() const;
    void zp_setVisible(bool);

    bool zp_isDirty() const;

    double zp_calcConcentration(const ZAbstractSpectrum* const, bool *ok = 0);

signals:

    void zg_visibilityChanged(bool) const;
    void zg_dirtyChanged(bool) const;

public slots:



protected:

    // VARS
    QString zv_baseName;
    QString zv_path;

    bool zv_visibility;
    bool zv_dirty;

};
//=========================================================
#endif // QABSTRACTCALIBRATION_H

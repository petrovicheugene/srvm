//======================================================================
#ifndef ZTERMNORMALIZATOR_H
#define ZTERMNORMALIZATOR_H
//======================================================================
#include <QObject>
//======================================================================
class ZTermNormalizer : public QObject
{
    Q_OBJECT
public:

    enum NormaType {NT_NONE,
                    NT_COHERENT,
                    NT_INCOHERENT,
                    NT_COHERENT_INCOHERENT,
                    NT_INCOHERENT_COHERENT,
                    NT_CUSTOM};

                   explicit ZTermNormalizer(QObject *parent = 0);

    bool zp_isValid() const;
    qreal zp_value() const;

signals:

    void zg_normalizerChanged() const;
    void zg_requestCoherentIntensity(qint64&) const;
    void zg_requestIncoherentIntensity(qint64&) const;


public slots:

    void zp_isValid(bool&) const;
    void zp_value(qreal&) const;


private:

    // VARS
    NormaType zv_normaType;


};
//======================================================================
#endif // ZTERMNORMALIZATOR_H

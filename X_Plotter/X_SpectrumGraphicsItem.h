//======================================================
#ifndef X_SPECTRUMGRAPHICSITEM_H
#define X_SPECTRUMGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>
#include "X_GraphicsItemUserTypes.h"
//======================================================
class X_AbstractSpectrum;
//======================================================
class X_SpectrumGraphicsItem : public QGraphicsItem
{
public:

    explicit X_SpectrumGraphicsItem(const X_AbstractSpectrum* spectrum,
                          qreal boundingRectTopFactor,
                          qreal distortionFactor,
                          qreal distortionCorrectionFactor,
                          QGraphicsItem * parent = 0);
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const override;
    int type() const override;
    static void xp_setCurrentSpectrumColor(QColor);
    static void xp_setCurrentSpectrumId(qint64);
    static qint64 xp_currentSpectrumId();
    //
    void xp_setSpectrumData(const QList<quint32> &);
    qint64 xp_spectrumId();
    void xp_updateSpectrumData(const X_AbstractSpectrum *);

    void xp_setDistortion(qreal distortionFactor, qreal distortionCorrectionFactor);
    bool xp_isSpectrumCurrent();

    void xp_updateCurrentSpectrum(bool visible);

    QColor xp_spectrumColor() const;

protected:

    // FUNCS
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:

    // VARS
    QList<quint32> xv_spectrumData;
    QPainterPath xv_spectrumPainterPath;
    QRectF xv_boundingRect;
    QColor xv_color;
    QColor xv_paintColor;
    qint64 xv_spectrumId;
    static QColor xv_currentColor;
    static qint64 xv_currentSpectrumId;

    qreal xv_distortionFactor;
    qreal xv_distortionCorrectionFactor;
    // qreal xv_sceneRectTop;
    qreal xv_boundingRectTopFactor;
    // qreal xv_k;

    // y' = y^xv_logFactor * xv_k
    // max = max^logFactor * xv_k

    // FUNCS
    void xh_calculatePainterPath();
    qreal xh_sceneDistortionPositionForValue(qreal value);

};
//======================================================
#endif // X_SPECTRUMGRAPHICSITEM_H

//======================================================
#ifndef X_SPECTRUMGRAPHICSITEM_H
#define X_SPECTRUMGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>
//======================================================
class X_AbstractSpectrum;
//======================================================
class X_SpectrumGraphicsItem : public QGraphicsItem
{
public:

    explicit X_SpectrumGraphicsItem(const X_AbstractSpectrum* spectrum,
                          double boundingRectTopFactor,
                          double distortionFactor,
                          double distortionCorrectionFactor,
                          QGraphicsItem * parent = nullptr);
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

    void xp_setDistortion(double distortionFactor, double distortionCorrectionFactor);
    bool xp_isSpectrumCurrent();

    void xp_updateSpectrum(bool visible);
    QColor xp_spectrumColor() const;

    double xp_lineWidth() const;
    void xp_setLineWidth(double width);

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

    double xv_distortionFactor;
    double xv_distortionCorrectionFactor;
    double xv_boundingRectTopFactor;
    double xv_lineWidth = 1.0;

    // FUNCS
    void xh_calculatePainterPath();
    double xh_sceneDistortionPositionForValue(double value);

};
//======================================================
#endif // X_SPECTRUMGRAPHICSITEM_H

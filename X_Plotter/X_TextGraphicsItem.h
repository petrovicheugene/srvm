#ifndef X_TEXTGRAPHICSITEM_H
#define X_TEXTGRAPHICSITEM_H
//======================================================
#include <QGraphicsItem>

#include <QColor>
#include <QFont>

#include "X_GraphicsItemUserTypes.h"
//======================================================
class X_TextGraphicsItem: public QGraphicsItem
{
public:
    X_TextGraphicsItem(const QString &text, QGraphicsItem * parent = 0);
    X_TextGraphicsItem(QGraphicsItem * parent = 0);

    // overrided
    QRectF boundingRect() const override;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    QPainterPath shape() const override;
    int type() const override;

    void xp_updateItem();
    void xp_setText(const QString& text);
    void xp_setFont(QFont font);
    void xp_setTextPixelSize(int pixelSize);
    void xp_setColor(QColor color);

protected:

    void xh_recalcShapeAndBoundingRect();

private:

    // VARS
    QRectF xv_boundingRect;
    QPainterPath xv_shape;
    QPainterPath xv_textPainterPath;

    QString xv_text;
    QFont xv_font;
    int xv_textPixelSize;
    QColor xv_color;
    // FUNCS

};
//======================================================
#endif // X_TEXTGRAPHICSITEM_H

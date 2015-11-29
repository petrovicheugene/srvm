//======================================================================
#include "ZSpectrumTableDelegate.h"
#include "ZPrimitivePlot.h"
#include "ZSpectrumPaintData.h"
#include "globalVariables.h"

#include <QPainter>
#include <QStyle>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
//======================================================================
ZSpectrumTableDelegate::ZSpectrumTableDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    zv_plot = new ZPrimitivePlot();
}
//======================================================================
ZSpectrumTableDelegate::~ZSpectrumTableDelegate()
{
    delete zv_plot;
}
//======================================================================
void ZSpectrumTableDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    if(!index.isValid())
    {
        QStyledItemDelegate::paint (painter, option, index);
        return;
    }

    QColor penColor;
    QColor brushColor;
    zv_plot->resize(option.rect.size());
    zv_plot->zp_setAntialiasingFlag(false);
    QPixmap  pixmap(zv_plot->size());
    QPalette palette = option.palette;
    QStyleOptionViewItemV4 newOption(option);

    if(option.state & QStyle::State_Selected)
    {
        QPainter pixmapPainter(&pixmap);
        pixmapPainter.fillRect(pixmap.rect(), option.palette.brush(QPalette::Base));
        newOption.rect = pixmap.rect();

        const QWidget *widget = newOption.widget;
        QStyle *style = widget ? widget->style() : QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &newOption, &pixmapPainter, widget);

        palette.setBrush(QPalette::Window, QBrush(pixmap));

        if(option.state & QStyle::State_Active)
        {
            penColor = QColor(Qt::yellow);
            brushColor = QColor(Qt::yellow);
        }
        else
        {
            penColor = QColor(Qt::darkGreen);
            brushColor = QColor(Qt::darkGreen);
        }
    }
    else
    {
        if(index.row() % 2)
        {
            palette.setBrush(QPalette::Window, option.palette.color(QPalette::AlternateBase));
        }
        else
        {
            palette.setBrush(QPalette::Window, option.palette.color(QPalette::Base));
        }
        //palette.setColor(QPalette::Window, option.palette.color(QPalette::Base));
        penColor = QColor(Qt::darkBlue);
        brushColor = QColor(Qt::darkBlue);
    }

    zv_plot->setPalette(palette);
    zv_plot->zp_setBrushColor(brushColor);
    zv_plot->zp_setPenColor(penColor);
    QVariant vData = index.data(Qt::DisplayRole);
    if(vData.isValid() && !vData.isNull() && vData.canConvert<ZSpectrumPaintData>())
    {
        ZSpectrumPaintData paintData = vData.value<ZSpectrumPaintData>();
        zv_plot->zp_paintData(paintData);
    }

    zv_plot->render(&pixmap);
    painter->drawPixmap(option.rect, pixmap);
}
//======================================================================
QSize	ZSpectrumTableDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}
//======================================================================

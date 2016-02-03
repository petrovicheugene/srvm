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
#include <QAbstractItemView>
#include <QTableView>
#include <QMouseEvent>
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

    painter->save();

    QColor penColor;
    QColor brushColor;
    QPixmap  pixmap(option.rect.size());
    QPainter pixmapPainter(&pixmap);

    QPalette palette = option.palette;

    QStyleOptionViewItemV4 newOption(option);
    newOption.rect = pixmap.rect();
    initStyleOption(&newOption, index);

    const QWidget *widget = option.widget;
    QStyle *style = widget ? widget->style() : QApplication::style();

    if(newOption.state & QStyle::State_Selected)
    {
        pixmapPainter.fillRect(pixmap.rect(), newOption.palette.highlight()) ;
        style->drawControl(QStyle::CE_ItemViewItem, &newOption, &pixmapPainter, widget);

        if(newOption.state & QStyle::State_Active)
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
            pixmapPainter.fillRect(pixmap.rect(), newOption.palette.color(QPalette::AlternateBase));
        }
        else
        {
            pixmapPainter.fillRect(pixmap.rect(), newOption.palette.color(QPalette::Base));
        }

        style->drawControl(QStyle::CE_ItemViewItem, &newOption, &pixmapPainter, widget);
        penColor = QColor(Qt::darkBlue);
        brushColor = QColor(Qt::darkBlue);
    }

    // rectangels
    QRect plotRect = pixmap.rect();
    QRect decorationRect = QRect();
    if(newOption.features & QStyleOptionViewItem::HasDecoration)
    {
        decorationRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &newOption);

        QColor decorationColor;
        QVariant vData = index.data(Qt::DecorationRole);
        if(vData.isValid() && !vData.isNull() && vData.canConvert<QColor>())
        {
            decorationColor = index.data(Qt::DecorationRole).value<QColor>();
        }
        else
        {
            decorationColor = QColor(Qt::transparent);
        }

        pixmapPainter.save();
        pixmapPainter.setPen(newOption.palette.color(QPalette::Mid));
        pixmapPainter.setBrush(QBrush(decorationColor));
        pixmapPainter.drawRect(decorationRect.adjusted(1, 0, -2, 0));

        bool visible = index.data(VisibleRole).toBool();
        if(visible)
        {
            QPixmap pixmap(":/images/eyeSignBlack.png");
            QRect paintRect = pixmap.rect();
            paintRect.moveCenter(decorationRect.adjusted(1, 0, -2, 0).center());
            pixmapPainter.drawPixmap(paintRect, pixmap);
        }

        pixmapPainter.restore();
        // plot rect correction
        plotRect.setLeft(plotRect.left() + decorationRect.width());
    }

    plotRect.adjust(1, 1, -1, -1);
    // painting plot on a separate pixmap

    QPixmap plotPixmap = pixmap.copy(plotRect);
    palette.setBrush(QPalette::Window, QBrush(plotPixmap));

    zv_plot->resize(plotRect.size());
    zv_plot->zp_setAntialiasingFlag(false);
    zv_plot->setPalette(palette);
    zv_plot->zp_setBrushColor(brushColor);
    zv_plot->zp_setPenColor(penColor);
    QVariant vData = index.data(Qt::DisplayRole);
    if(vData.isValid() && !vData.isNull() && vData.canConvert<ZSpectrumPaintData>())
    {
        ZSpectrumPaintData paintData = vData.value<ZSpectrumPaintData>();
        zv_plot->zp_paintData(paintData);
    }

    zv_plot->render(&plotPixmap);

    // painting plot pixmap on the item's common pixmap
    pixmapPainter.drawPixmap(plotRect, plotPixmap, plotPixmap.rect());

    // painting item's pixmap
    painter->drawPixmap(option.rect, pixmap);
    painter->restore();
}
//======================================================================
QSize	ZSpectrumTableDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}
//======================================================================
bool ZSpectrumTableDelegate::editorEvent ( QEvent * event,
                                           QAbstractItemModel * model,
                                           const QStyleOptionViewItem & option,
                                           const QModelIndex & index )
{
    if(!index.isValid() || model == 0 || event->type() != QEvent::MouseButtonRelease)
    {
        return false;
    }

    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
    if(!mouseEvent || mouseEvent->type() != QMouseEvent::MouseButtonRelease)
    {
        return false;
    }

    QStyleOptionViewItem newOption(option);
    initStyleOption(&newOption, index);
    const QAbstractItemView* itemView = qobject_cast<const QAbstractItemView*>(newOption.widget);
    if(!itemView)
    {
        return false;
    }

    QStyle *style = itemView->style();
    QRect decorationRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &newOption);
    QPoint mousePos = mouseEvent->pos();
    if(!decorationRect.contains(mousePos))
    {
        return false;
    }

    QVariant vData = index.data(VisibleRole);
    if(!vData.isValid() || vData.isNull() || !vData.canConvert<bool>())
    {
        return false;
    }

    bool visible = vData.toBool();
    model->setData(index, QVariant(!visible), VisibleRole);

    return true;
}
//======================================================================
bool ZSpectrumTableDelegate::eventFilter(QObject *object, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        //
        QTableView* itemView = qobject_cast<QTableView*>(parent());
        if(itemView == 0)
        {
            return false;
        }

        if(itemView->viewport() != object)
        {
            return false;
        }

        QPoint mousePoint = mouseEvent->pos();
        QModelIndex itemIndex = itemView->indexAt(mousePoint);

        if(!itemIndex.isValid())
        {
            return false;
        }

        if(itemView->itemDelegateForColumn(itemIndex.column()) != this)
        {
            return false;
        }


        QRect itemRect = itemView->visualRect(itemIndex);
        QStyleOptionViewItemV4 newOption;
        newOption.initFrom(itemView);
        newOption.widget = itemView;

        if (itemView->iconSize().isValid())
        {
            newOption.decorationSize = itemView->iconSize();
        }
        else
        {
            int pm = itemView->style()->pixelMetric(QStyle::PM_SmallIconSize, 0, itemView);
            newOption.decorationSize = QSize(pm, pm);
        }

        newOption.decorationAlignment = Qt::AlignCenter;
        initStyleOption(&newOption, itemIndex);
        newOption.rect = itemRect;

        QRect decorationRect = itemView->style()->subElementRect(QStyle::SE_ItemViewItemDecoration, &newOption);

        if(!decorationRect.contains(mousePoint))
        {
            return false;
        }

        // mouse position is in decoration rect
        // MouseButtonPress or MouseButtonDblClick (making item current) - banned
        // visible will be switched by mouseButtonRelease
        return true;
    }

    return false;
}
//======================================================================

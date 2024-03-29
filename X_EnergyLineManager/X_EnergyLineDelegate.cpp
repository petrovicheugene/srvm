//======================================================================
#include "X_EnergyLineDelegate.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include <QTableView>

//======================================================================
//======================================================================
X_EnergyLineDelegate::X_EnergyLineDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    xv_preventMouseReleaseHandle = false;
}
//======================================================================
void X_EnergyLineDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    if(!index.isValid())
    {
        QStyledItemDelegate::paint (painter, option, index);
        return;
    }

    QVariant vData = index.data(NS_DataRole::VisibleRole);
    if(!vData.isValid() || !vData.canConvert<bool>())
    {
        QStyledItemDelegate::paint (painter, option, index);
        return;
    }

    bool visible = vData.toBool();

    painter->save();

    QColor penColor;
    QColor brushColor;
    QPixmap  pixmap(option.rect.size());
    QPainter pixmapPainter(&pixmap);

    QPalette palette = option.palette;

    QStyleOptionViewItem newOption(option);
    newOption.state =  newOption.state | QStyle::State_Active;
    newOption.rect = pixmap.rect();
    initStyleOption(&newOption, index);
    //newOption.text = QString();

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
        if(index.row() % 2 && newOption.features & QStyleOptionViewItem::Alternate)
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
            if(!decorationColor.isValid())
            {
                decorationColor = newOption.palette.color(QPalette::Base);
            }
        }
        else
        {
            decorationColor = QColor(Qt::transparent);
        }

        pixmapPainter.save();
        pixmapPainter.setPen(newOption.palette.color(QPalette::Mid));
        pixmapPainter.setBrush(QBrush(decorationColor));
        pixmapPainter.drawRect(decorationRect.adjusted(1, 0, -2, 0));

        if(visible)
        {
            QPixmap pixmap(":/images/X_Images/eyeSignBlack-1.png");
            QRect paintRect = pixmap.rect();
            paintRect.moveCenter(decorationRect.adjusted(1, 0, -2, 0).center());
            pixmapPainter.drawPixmap(paintRect, pixmap);
        }

        pixmapPainter.restore();
        // plot rect correction
        plotRect.setLeft(plotRect.left() + decorationRect.width());
    }

    plotRect.adjust(1, 1, -1, -1);

    // paint plot on a separate pixmap
    QPixmap plotPixmap = pixmap.copy(plotRect);
    palette.setBrush(QPalette::Window, QBrush(plotPixmap));

//    xv_plot->resize(plotRect.size());
//    xv_plot->xp_setAntialiasingFlag(false);
//    xv_plot->setPalette(palette);
//    xv_plot->xp_setBrushColor(brushColor);
//    xv_plot->xp_setPenColor(penColor);
//    vData = index.data(Qt::DisplayRole);
//    if(vData.isValid() && !vData.isNull() && vData.canConvert<X_SpectrumPaintData>())
//    {
//        X_SpectrumPaintData paintData = vData.value<X_SpectrumPaintData>();
//        xv_plot->xp_paintData(paintData);
//        xv_plot->render(&plotPixmap);
//    }

    // painting plot pixmap on the item's common pixmap
    pixmapPainter.drawPixmap(plotRect, plotPixmap, plotPixmap.rect());

    // painting item's pixmap
    painter->drawPixmap(option.rect, pixmap);
    painter->restore();
}
//======================================================================
QSize X_EnergyLineDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}
//======================================================================
bool X_EnergyLineDelegate::editorEvent ( QEvent * event,
                                           QAbstractItemModel * model,
                                           const QStyleOptionViewItem & option,
                                           const QModelIndex & index )
{
    if(!index.isValid() || model == 0 || event->type() != QEvent::MouseButtonRelease)
    {
        return QStyledItemDelegate::editorEvent(event,
                                                model,
                                                option,
                                                index);
    }

    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
    if(!mouseEvent || mouseEvent->type() != QMouseEvent::MouseButtonRelease)
    {
        return QStyledItemDelegate::editorEvent(event,
                                                model,
                                                option,
                                                index);
    }

    QStyleOptionViewItem newOption(option);
    initStyleOption(&newOption, index);
    const QAbstractItemView* itemView = qobject_cast<const QAbstractItemView*>(newOption.widget);
    if(!itemView)
    {
        return QStyledItemDelegate::editorEvent(event,
                                                model,
                                                option,
                                                index);
    }

    QStyle *style = itemView->style();
    QRect decorationRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &newOption);
    QPoint mousePos = mouseEvent->pos();
    if(!decorationRect.contains(mousePos))
    {
        return QStyledItemDelegate::editorEvent(event,
                                                model,
                                                option,
                                                index);
    }

    QVariant vData = index.data(NS_DataRole::VisibleRole);
    if(!vData.isValid() || vData.isNull() || !vData.canConvert<bool>())
    {
        return QStyledItemDelegate::editorEvent(event,
                                                model,
                                                option,
                                                index);
    }

    bool visible = vData.toBool();
    model->setData(index, QVariant(!visible), NS_DataRole::VisibleRole);

    return true;
}
//======================================================================
bool X_EnergyLineDelegate::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick
        || event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        //
        QTableView* itemView = qobject_cast<QTableView*>(parent());
        if(itemView == 0)
        {
            return QStyledItemDelegate::eventFilter(object, event);
        }

        if(itemView->viewport() != object)
        {
            return QStyledItemDelegate::eventFilter(object, event);
        }

        QPoint mousePoint = mouseEvent->pos();
        QModelIndex itemIndex = itemView->indexAt(mousePoint);

        if(!itemIndex.isValid())
        {
            return QStyledItemDelegate::eventFilter(object, event);
        }

        if((itemView->itemDelegateForColumn(itemIndex.column()) != 0
            && itemView->itemDelegateForColumn(itemIndex.column()) != this)
                || (itemView->itemDelegate()) != this)
        {
            return QStyledItemDelegate::eventFilter(object, event);
        }


        QRect itemRect = itemView->visualRect(itemIndex);
        QStyleOptionViewItem newOption;
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
            if (xv_preventMouseReleaseHandle)
            {
                if (event->type() == QEvent::MouseButtonRelease)
                {
                    // prevent mouse release handling after slide of the keeping down cursor from decoration rectangle
                    xv_preventMouseReleaseHandle = false;
                    return true;
                }
            }

            return QStyledItemDelegate::eventFilter(object, event);
        }

        // mouse position is in decoration rect
        // MouseButtonPress or MouseButtonDblClick (making item current) - banned
        // visible will be switched by mouseButtonRelease
        if (event->type() == QEvent::MouseButtonRelease)
        {
            QAbstractItemView *itemView = dynamic_cast<QAbstractItemView *>(parent());
            if (!itemView)
            {
                return false;
            }

            QModelIndex index = itemView->indexAt(mousePoint);
            QVariant vData = index.data(NS_DataRole::VisibleRole);
            if (!vData.isValid() || vData.isNull() || !vData.canConvert<bool>())
            {
                return false;
            }

            bool visible = vData.toBool();
            QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());
            model->setData(index, QVariant(!visible), NS_DataRole::VisibleRole);
            xv_preventMouseReleaseHandle = false;
        }
        else
        {
            xv_preventMouseReleaseHandle = true;
        }

        return true;
    }

    if (xv_preventMouseReleaseHandle)
    {
        // prevent mouse release handling after slide of the keeping down cursor from decoration rectangle
        return true;
    }

    return QStyledItemDelegate::eventFilter(object, event);
}
//======================================================================

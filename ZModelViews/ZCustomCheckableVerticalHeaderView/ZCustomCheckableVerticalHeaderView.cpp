//=========================================================
#include "ZCustomCheckableVerticalHeaderView.h"
#include "ZConstants.h"

#include <QPainter>
#include <QApplication>
#include <QTableView>
#include <QMouseEvent>
#include <QRect>
//=========================================================
ZCustomCheckableVerticalHeaderView::ZCustomCheckableVerticalHeaderView(QWidget *parent)
    : QHeaderView(Qt::Vertical, parent)
{
    setSectionsClickable(true);
    zv_pressedIndex = -1;
}
//=========================================================
void ZCustomCheckableVerticalHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    if (!rect.isValid())
    {
        return;
    }
    // get the state of the section
    QStyleOptionHeader opt;
    initStyleOption(&opt);
    QStyle::State state = QStyle::State_None;
    if (isEnabled())
    {
        state |= QStyle::State_Enabled;
    }
    if (window()->isActiveWindow())
    {
        state |= QStyle::State_Active;
    }
    if(sectionsClickable())
    {
        QPoint cursorPos = mapFromGlobal(QCursor::pos());
        if (logicalIndex == logicalIndexAt(cursorPos) && this->rect().contains(cursorPos))
        {
            state |= QStyle::State_MouseOver;
        }
        if (logicalIndex == zv_pressedIndex)
        {
            state |= QStyle::State_Sunken;
        }
        else if (selectedIndexes().count())
        {
            if ((selectionModel() ? selectionModel()->rowIntersectsSelection(logicalIndex, QModelIndex()) : false))
            {
                state |= QStyle::State_On;
            }
            QModelIndex sectionIndex = model()->index(logicalIndex, 0);
            if (selectedIndexes().contains(sectionIndex))
            {
                state |= QStyle::State_Sunken;
            }
        }
    }
    if (isSortIndicatorShown() && sortIndicatorSection() == logicalIndex)
    {
        opt.sortIndicator = (sortIndicatorOrder() == Qt::AscendingOrder)
                ? QStyleOptionHeader::SortDown : QStyleOptionHeader::SortUp;
    }
    // setup the style options structure
    QVariant textAlignment = model()->headerData(logicalIndex, Qt::Vertical,
                                                 Qt::TextAlignmentRole);
    opt.rect = rect;
    opt.section = logicalIndex;
    opt.state |= state;
    opt.textAlignment = Qt::Alignment(textAlignment.isValid()
                                      ? Qt::Alignment(textAlignment.toInt())
                                      : this->defaultAlignment());

    opt.iconAlignment = Qt::AlignVCenter;
    opt.text = model()->headerData(logicalIndex, Qt::Vertical,
                                   Qt::DisplayRole).toString();

    int margin = 2 * style()->pixelMetric(QStyle::PM_HeaderMargin, 0, this);

    const Qt::Alignment headerArrowAlignment = static_cast<Qt::Alignment>(style()->styleHint(QStyle::SH_Header_ArrowAlignment, 0, this));
    const bool isHeaderArrowOnTheSide = headerArrowAlignment & Qt::AlignVCenter;
    if (isSortIndicatorShown() && sortIndicatorSection() == logicalIndex && isHeaderArrowOnTheSide)
    {
        margin += style()->pixelMetric(QStyle::PM_HeaderMarkSize, 0, this);
    }
    if (textElideMode() != Qt::ElideNone)
    {
        opt.text = opt.fontMetrics.elidedText(opt.text, textElideMode() , rect.width() - margin);
    }
    QVariant variant = model()->headerData(logicalIndex, Qt::Vertical,
                                           Qt::DecorationRole);
    opt.icon = qvariant_cast<QIcon>(variant);
    if (opt.icon.isNull())
    {
        opt.icon = qvariant_cast<QPixmap>(variant);
    }
    QVariant foregroundBrush = model()->headerData(logicalIndex, Qt::Vertical,
                                                   Qt::ForegroundRole);
    if (foregroundBrush.canConvert<QBrush>())
    {
        opt.palette.setBrush(QPalette::ButtonText, qvariant_cast<QBrush>(foregroundBrush));
    }
    QPointF oldBO = painter->brushOrigin();
    QVariant backgroundBrush = model()->headerData(logicalIndex, Qt::Vertical,
                                                   Qt::BackgroundRole);
    if (backgroundBrush.canConvert<QBrush>())
    {
        opt.palette.setBrush(QPalette::Button, qvariant_cast<QBrush>(backgroundBrush));
        opt.palette.setBrush(QPalette::Window, qvariant_cast<QBrush>(backgroundBrush));
        painter->setBrushOrigin(opt.rect.topLeft());
    }

    // the section position
    int visual = visualIndex(logicalIndex);
    Q_ASSERT(visual != -1);
    bool first = sectionPosition(logicalIndex) == 0 && sectionSize(logicalIndex) > 0;
    bool last = sectionPosition(logicalIndex) + sectionSize(logicalIndex) == this->length()
            && sectionSize(logicalIndex) > 0;
    if (first && last)
    {
        opt.position = QStyleOptionHeader::OnlyOneSection;
    }
    else if (first)
    {
        opt.position = QStyleOptionHeader::Beginning;
    }
    else if (last)
    {
        opt.position = QStyleOptionHeader::End;
    }
    else
    {
        opt.position = QStyleOptionHeader::Middle;
    }
    opt.orientation = Qt::Vertical;
    // the selected position

    QModelIndex prevIndex = model()->index(this->logicalIndex(visual - 1), 0);
    QModelIndex nextIndex = model()->index(this->logicalIndex(visual + 1), 0);

    bool previousSelected = selectedIndexes().contains(prevIndex);
    bool nextSelected =  selectedIndexes().contains(nextIndex);
    if (previousSelected && nextSelected)
    {
        opt.selectedPosition = QStyleOptionHeader::NextAndPreviousAreSelected;
    }
    else if (previousSelected)
    {
        opt.selectedPosition = QStyleOptionHeader::PreviousIsSelected;
    }
    else if (nextSelected)
    {
        opt.selectedPosition = QStyleOptionHeader::NextIsSelected;
    }
    else
    {
        opt.selectedPosition = QStyleOptionHeader::NotAdjacent;
    }
    // draw the section
    //style()->drawControl(QStyle::CE_Header, &opt, painter, this);
    style()->drawControl(QStyle::CE_HeaderSection, &opt, painter, this);

    QRect decorationRect = zh_decorationRect(opt.rect);

    if(decorationRect.normalized().isValid())
    {
        // draw the empty decoration rect
        QStyleOptionViewItem newOption;
        newOption.palette = this->palette();
        newOption.rect = opt.rect;
        newOption.features |= QStyleOptionViewItem::HasDecoration;
        newOption.decorationAlignment = Qt::AlignCenter;
        int pm = style()->pixelMetric(QStyle::PM_SmallIconSize, 0) ;
        newOption.decorationSize = QSize(pm, pm);

        QRect decorationSignRect = style()->subElementRect(QStyle::SE_ItemViewItemDecoration, &newOption);
        // decorationSignRect.adjusted(1, 0, -2, 0);
        painter->save();
        painter->setPen(newOption.palette.color(QPalette::Mid));
        painter->setBrush(QBrush(newOption.palette.color(QPalette::Base)));
        painter->drawRect(decorationSignRect.adjusted(1, 0, -1, 0));

        // draw the sign in deco rect
        QVariant vData = model()->headerData(logicalIndex, Qt::Vertical, IconRole);
        if(vData.isValid() && !vData.isNull() && vData.canConvert<QPixmap>())
        {
            QPixmap iconPixmap = vData.value<QPixmap>();
            // iconPixmap.scaled(decorationSignRect.adjusted(1, 0, -2, 0).size());

            QRect iconRect = iconPixmap.rect();
            iconRect.moveCenter(decorationSignRect.center());
            //            painter->drawPixmap( decorationSignRect /*.adjusted(2, 1, -3, -1)*/,
            //                                iconPixmap/*.scaled(decorationSignRect.adjusted(3, 2, -3, -1).size(),
            //                                                                      Qt::KeepAspectRatio,
            //                                                                      Qt::SmoothTransformation)*/);

            painter->drawPixmap( iconRect /*.adjusted(2, 1, -3, -1)*/,
                                 iconPixmap/*.scaled(decorationSignRect.adjusted(3, 2, -3, -1).size(),
                                                                                                                Qt::KeepAspectRatio,
                                                                                                                Qt::SmoothTransformation)*/);
        }

        painter->restore();
        // define rect for the label text
        opt.rect = opt.rect.adjusted(decorationRect.normalized().width() + zv_labelTextOffset, 0, 0, 0);
    }

    style()->drawControl(QStyle::CE_HeaderLabel, &opt, painter, this);
    painter->setBrushOrigin(oldBO);

}
//=========================================================
void ZCustomCheckableVerticalHeaderView::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        int logIndex = logicalIndexAt(event->pos());
        if(!isSectionHidden(logIndex))
        {
            int sectionPos = sectionViewportPosition(logIndex);
            if(sectionPos >= 0 && sectionPos < length())
            {
                int sectionHeight  = sectionSize(logIndex);
                QSize sectionRectSize = QSize(viewport()->width(), sectionHeight);
                QRect itemRect = QRect(QPoint(0, sectionPos), sectionRectSize);
                QRect decorationRect = zh_decorationRect(itemRect);
                if(decorationRect.contains(event->pos()))
                {
                    // edit
                    emit zg_userChangesTermState(logIndex);
                    return;
                }
            }
        }
    }

    zv_pressedIndex = logicalIndexAt(event->pos());
    QModelIndex pressedIndex = model()->index(zv_pressedIndex, 0);
    setCurrentIndex(pressedIndex);
    QHeaderView::mousePressEvent(event);
}
//=========================================================
void ZCustomCheckableVerticalHeaderView::mouseReleaseEvent(QMouseEvent* event)
{
    zv_pressedIndex = -1;
    QHeaderView::mouseReleaseEvent(event);
}
//=========================================================
QSize ZCustomCheckableVerticalHeaderView::sectionSizeFromContents(int logicalIndex) const
{
    QSize size = QHeaderView::sectionSizeFromContents(logicalIndex);
    QRect sectionRect = QRect(QPoint(0,0), size);
    QRect decorationRect = zh_decorationRect(sectionRect);

    if(decorationRect.normalized().isValid())
    {
        size.setWidth(size.width() + decorationRect.normalized().width() + zv_labelTextOffset);
    }

    return size;
}
//=========================================================
QRect ZCustomCheckableVerticalHeaderView::zh_decorationRect(const QRect& sectionRect) const
{
    QStyle *style = this->style();
    if(!style)
    {
        style = QApplication::style();
    }

    QStyleOptionViewItem option;
    option.features |= QStyleOptionViewItem::HasDecoration;
    int pm = style->pixelMetric(QStyle::PM_SmallIconSize, 0);
    option.decorationSize = QSize(pm, pm);
    option.decorationAlignment = Qt::AlignCenter;
    option.rect = sectionRect;
    QRect decorationRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &option);

    return decorationRect;
}
//=========================================================

//==========================================================
#include "X_VisibilityStringDelegate.h"
#include "X_General.h"

#include <QLineEdit>
#include <QStyleOption>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QAbstractItemView>
#include <QTableView>
//==========================================================
X_VisibilityStringDelegate::X_VisibilityStringDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    xv_preventMouseReleaseHandle = false;
}
//==========================================================
QWidget* X_VisibilityStringDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if(!index.isValid())
    {
        return 0;
    }

    QStyleOptionViewItem newOption(option);
    initStyleOption(&newOption, index);
    const QAbstractItemView* itemView = qobject_cast<const QAbstractItemView*>(newOption.widget);
    if(itemView)
    {
        QStyle *style = itemView->style();
        QRect decorationRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &newOption);
        QPoint mousePos = itemView->viewport()->mapFromGlobal(QCursor::pos());
        if(decorationRect.contains(mousePos))
        {
            return 0;
        }
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}
//==========================================================
void	X_VisibilityStringDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem newOption(option);
    initStyleOption(&newOption, index);
    newOption.state =  newOption.state | QStyle::State_Active;

    QStyledItemDelegate::paint(painter, newOption, index);

    if(newOption.features & QStyleOptionViewItem::HasDecoration)
    {
        QStyle *style = newOption.widget ? newOption.widget->style() : QApplication::style();
        QRect decorationRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &newOption);

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
            decorationColor = newOption.palette.color(QPalette::Base);
        }

        painter->save();
        painter->setPen(newOption.palette.color(QPalette::Mid));
        painter->setBrush(QBrush(decorationColor));
        painter->drawRect(decorationRect.adjusted(1, 0, -1, 0));

        bool visible = index.data(NS_DataRole::VisibleRole).toBool();
        if(visible)
        {
            QPixmap pixmap(NS_Icons::glIconVisible);
            QRect paintRect = pixmap.rect();
            //paintRect.moveCenter(decorationRect.adjusted(1, 0, -2, 0).center());
            paintRect.moveCenter(decorationRect.center());

            painter->drawPixmap(paintRect, pixmap);
        }
    }

    painter->restore();
}
//==========================================================
void	X_VisibilityStringDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    // X_StringDelegate::setEditorData(editor, index);
    if(!index.isValid())
    {
        return;
    }

    QLineEdit* lineEditor = qobject_cast<QLineEdit*>(editor);
    if(lineEditor == 0)
    {
        return;
    }

    QVariant vData = index.data();
    if(!vData.isValid() || vData.isNull() || !vData.canConvert<QString>())
    {
        return;
    }

    QString str = vData.toString();
    lineEditor->setText(str);
}
//==========================================================
void	X_VisibilityStringDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);
}
//==========================================================
QSize X_VisibilityStringDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}
//==========================================================
void	X_VisibilityStringDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}
//==========================================================
bool X_VisibilityStringDelegate::editorEvent(QEvent *event,
                                            QAbstractItemModel *model,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index)
{
    if (!index.isValid() || model == 0 || event->type() != QEvent::MouseButtonRelease)
    {
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }

    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (!mouseEvent || mouseEvent->type() != QMouseEvent::MouseButtonRelease)
    {
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }

    QStyleOptionViewItem newOption(option);
    initStyleOption(&newOption, index);
    const QAbstractItemView *itemView = qobject_cast<const QAbstractItemView *>(newOption.widget);
    if (!itemView)
    {
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }

    QStyle *style = itemView->style();
    QRect decorationRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &newOption);
    QPoint mousePos = mouseEvent->pos();
    if (!decorationRect.contains(mousePos))
    {
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }

    QVariant vData = index.data(NS_DataRole::VisibleRole);
    if (!vData.isValid() || vData.isNull() || !vData.canConvert<bool>())
    {
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }

    bool visible = vData.toBool();
    model->setData(index, QVariant(!visible), NS_DataRole::VisibleRole);

    return true;
}
//==========================================================
bool X_VisibilityStringDelegate::editorEvent1(QEvent *event,
                                             QAbstractItemModel *model,
                                             const QStyleOptionViewItem &option,
                                             const QModelIndex &index)
{
    if (!index.isValid() || model == 0 || event->type() != QEvent::MouseButtonRelease)
    {
        return QStyledItemDelegate::editorEvent(event,
                                                model,
                                                option,
                                                index);
    }

    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
    if(!mouseEvent)
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
//==========================================================
bool X_VisibilityStringDelegate::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick
        || event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        //
        QTableView *itemView = qobject_cast<QTableView *>(parent());
        if (itemView == 0)
        {
            return QStyledItemDelegate::eventFilter(object, event);
        }

        if (itemView->viewport() != object)
        {
            return QStyledItemDelegate::eventFilter(object, event);
        }

        QPoint mousePoint = mouseEvent->pos();
        QModelIndex itemIndex = itemView->indexAt(mousePoint);

        if (!itemIndex.isValid())
        {
            return QStyledItemDelegate::eventFilter(object, event);
        }

        if (itemView->itemDelegateForColumn(itemIndex.column()) != this)
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

        QRect decorationRect = itemView->style()->subElementRect(QStyle::SE_ItemViewItemDecoration,
                                                                 &newOption);

        if (!decorationRect.contains(mousePoint))
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

    if ((event->type() == QEvent::HoverEnter || event->type() == QEvent::HoverMove
         || event->type() == QEvent::HoverLeave)
        && xv_preventMouseReleaseHandle)
    {
        // prevent mouse release handling after slide of the keeping down cursor from decoration rectangle
        return true;
    }

    return QStyledItemDelegate::eventFilter(object, event);
}
//======================================================================

//==========================================================
bool X_VisibilityStringDelegate::eventFilter1(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick
        || event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
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

        if(itemView->itemDelegateForColumn(itemIndex.column()) != this)
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

        QRect decorationRect = itemView->style()->subElementRect(QStyle::SE_ItemViewItemDecoration,
                                                                 &newOption);

        //        if(decorationRect.contains(mousePoint))
        //        {
        //            // mouse position is in decoration rect
        //            // MouseButtonPress or MouseButtonDblClick (making item current) - banned
        //            // visible will be switched by mouseButtonRelease

        //            return true;
        //        }

        if (!decorationRect.contains(mousePoint))
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

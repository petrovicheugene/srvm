//==========================================================
#include "ZVisibilityStringDelegate.h"
#include "globalVariables.h"

#include <QLineEdit>
#include <QStyleOption>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QAbstractItemView>
#include <QTableView>
//==========================================================
ZVisibilityStringDelegate::ZVisibilityStringDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}
//==========================================================
QWidget* ZVisibilityStringDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
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
void	ZVisibilityStringDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);


    QStyleOptionViewItem newOption(option);
    initStyleOption(&newOption, index);

    if(newOption.features & QStyleOptionViewItem::HasDecoration)
    {
        QStyle *style = newOption.widget ? newOption.widget->style() : QApplication::style();
        QRect decorationRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &newOption);

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

        painter->save();
        painter->setPen(newOption.palette.color(QPalette::Mid));
        painter->setBrush(QBrush(decorationColor));
        painter->drawRect(decorationRect.adjusted(1, 0, -2, 0));

        bool visible = index.data(VisibleRole).toBool();
        if(visible)
        {
            QPixmap pixmap(":/images/eyeSignBlack.png");
            QRect paintRect = pixmap.rect();
            paintRect.moveCenter(decorationRect.adjusted(1, 0, -2, 0).center());
            painter->drawPixmap(paintRect, pixmap);
        }
    }

    painter->restore();
}
//==========================================================
void	ZVisibilityStringDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    // ZStringDelegate::setEditorData(editor, index);
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
void	ZVisibilityStringDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);
}
//==========================================================
QSize ZVisibilityStringDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}
//==========================================================
void	ZVisibilityStringDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}
//==========================================================
bool ZVisibilityStringDelegate::editorEvent ( QEvent * event,
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
//==========================================================
bool ZVisibilityStringDelegate::eventFilter(QObject *object, QEvent *event)
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

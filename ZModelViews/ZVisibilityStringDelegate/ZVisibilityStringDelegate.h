//==========================================================
#ifndef ZVISIBILITYSTRINGDELEGATE_H
#define ZVISIBILITYSTRINGDELEGATE_H
//==========================================================
#include <QStyledItemDelegate>

//==========================================================
class ZVisibilityStringDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ZVisibilityStringDelegate(QObject *parent = 0);

    virtual QWidget *createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor,
                              QAbstractItemModel *model,
                              const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void updateEditorGeometry(QWidget *editor,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const;

private:
    //VARS
    bool zv_preventMouseReleaseHandle;
    //FUNCS

    bool editorEvent(QEvent *event,
                     QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index);
    bool editorEvent1(QEvent *event,
                      QAbstractItemModel *model,
                      const QStyleOptionViewItem &option,
                      const QModelIndex &index);
    bool eventFilter(QObject *object, QEvent *event);
    bool eventFilter1(QObject *object, QEvent *event);
};
//==========================================================
#endif // ZVISIBILITYSTRINGDELEGATE_H

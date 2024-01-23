//======================================================================
#ifndef X_SPECTRUMTABLEDELEGATE_H
#define X_SPECTRUMTABLEDELEGATE_H
//======================================================================
#include <QStyledItemDelegate>
//======================================================================
class X_PrimitivePlot;
//======================================================================
class X_SpectrumTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit X_SpectrumTableDelegate(QObject *parent = 0);
    virtual ~X_SpectrumTableDelegate();

    virtual void	paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual QSize	sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

signals:

public slots:


private:

    // VARS
    // QWidget* xv_baseWidget;
    X_PrimitivePlot* xv_plot;
    bool xv_preventMouseReleaseHandle;

    // FUNCS
    bool editorEvent ( QEvent * event,
                       QAbstractItemModel * model,
                       const QStyleOptionViewItem & option,
                       const QModelIndex & index );

    bool eventFilter(QObject *object, QEvent *event);

};
//======================================================================
#endif // X_SPECTRUMTABLEDELEGATE_H

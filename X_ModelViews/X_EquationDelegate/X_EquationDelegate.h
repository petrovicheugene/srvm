//=======================================================
#ifndef X_EQUATIONDELEGATE_H
#define X_EQUATIONDELEGATE_H
//=======================================================
#include <QStyledItemDelegate>
//=======================================================

//=======================================================
class X_EquationDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit X_EquationDelegate(QObject *parent = 0);

    virtual QWidget* createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	setEditorData ( QWidget * editor, const QModelIndex & index ) const;
    virtual void	setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
    virtual QSize	sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;


private slots:

    void zh_onEditorChildDialogOpen(bool);

signals:

    void xg_currentCalibrationWindowName(const QString& windowName) const;

public slots:

private:

    //VARS
    bool xv_editorChildDialogOpen;

    // FUNCS
    // bool eventFilter(QObject *object, QEvent *event);
};
//=======================================================
#endif // X_EQUATIONDELEGATE_H

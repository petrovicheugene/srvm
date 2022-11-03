//=========================================------------------------------------------
#ifndef X_CONCENTRATIONDELEGATE_H
#define X_CONCENTRATIONDELEGATE_H
//=========================================------------------------------------------
#include <QStyledItemDelegate>
//=========================================------------------------------------------
class QDoubleSpinBox;
class X_NumericEditor;
//=========================================------------------------------------------
class X_NumericDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit X_NumericDelegate(QWidget *parent = 0);

    virtual QWidget *	createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	setEditorData ( QWidget * editor, const QModelIndex & index ) const;
    virtual void	setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
    virtual QSize	sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    void xp_setImmediatellyValueChange(bool);
    //bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    void xp_setEditorMinMax(double min, double max) ;
    void xp_editorMinMax(double& min, double& max) const;
    void xp_setFontBold(bool);

protected:

    // VARS

    enum MessageType {MT_NO_MESSAGE, MT_MAX_LIMIT, MT_MIN_LIMIT};
    //X_NumericEditor* m_numericEditor;
    bool xv_fontBoldFlag;
    bool xv_inheritFontBoldFlag;
    double xv_editorMin;
    double xv_editorMax;

    QModelIndex xv_editedIndex;
    bool xv_isEditorAlive;
    bool xv_immediatellyValueChangeFlag;

    // FUNCS

signals:
    
    //void dataStored(QModelIndex) const;
    void xg_editNext(QModelIndex);
    void xg_requestEditorMinMax(X_NumericDelegate*, QModelIndex) const;

public slots:
    
protected slots:

    void zh_onEditorEnterClick();
    void zh_editorValueChanged(double value);

};
//=========================================------------------------------------------
#endif // X_CONCENTRATIONDELEGATE_H

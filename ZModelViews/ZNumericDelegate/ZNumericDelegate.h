//=========================================------------------------------------------
#ifndef ZCONCENTRATIONDELEGATE_H
#define ZCONCENTRATIONDELEGATE_H
//=========================================------------------------------------------
#include <QStyledItemDelegate>
//=========================================------------------------------------------
class QDoubleSpinBox;
class ZNumericEditor;
//=========================================------------------------------------------
class ZNumericDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ZNumericDelegate(QWidget *parent = 0);

    virtual QWidget *	createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	setEditorData ( QWidget * editor, const QModelIndex & index ) const;
    virtual void	setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
    virtual QSize	sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    virtual void	updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    //bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    void setEditorMinMax(double min, double max) ;

    void setFontBold(bool);

protected:

    // VARS

    enum MessageType {MT_NO_MESSAGE, MT_MAX_LIMIT, MT_MIN_LIMIT};
    //ZNumericEditor* m_numericEditor;
    bool m_fontBoldFlag;
    bool m_inheritFontBoldFlag;
    double m_editorMin;
    double m_editorMax;

    QModelIndex m_editedIndex;
    bool m_isEditorAlive;
    // FUNCS

signals:
    
    //void dataStored(QModelIndex) const;
    void editNext(QModelIndex);
    void requestEditorMinMax(ZNumericDelegate*, QModelIndex) const;

public slots:
    
protected slots:
    void onEditorEnterClick();
};
//=========================================------------------------------------------
#endif // ZCONCENTRATIONDELEGATE_H

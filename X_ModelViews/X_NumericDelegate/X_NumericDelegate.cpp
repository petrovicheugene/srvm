//=================================================================
#include "X_NumericDelegate.h"
#include "X_NumericEditor.h"
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QTextEdit>
#include <limits>
#include <QMessageBox>
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>
//=================================================================
X_NumericDelegate::X_NumericDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
    xv_editorMin = (double)std::numeric_limits<long>::min();
    xv_editorMax = (double)std::numeric_limits<long>::max();
    xv_isEditorAlive = false;
    xv_inheritFontBoldFlag = true;
    xv_fontBoldFlag = false;
    xv_immediatellyValueChangeFlag = true;

}
//=================================================================
QWidget* X_NumericDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    X_NumericEditor* editor = new X_NumericEditor(parent);
    connect(editor, SIGNAL(enterClicked()), this, SLOT(zh_onEditorEnterClick()));

    X_NumericDelegate* pThis = const_cast<X_NumericDelegate*>(this);
    emit xg_requestEditorMinMax(pThis, index);
    editor->setMinMax(xv_editorMin, xv_editorMax);

    QModelIndex* pIndex = const_cast<QModelIndex*>(&xv_editedIndex);
    *pIndex = index;

    if(xv_immediatellyValueChangeFlag)
    {
        X_NumericDelegate* pThis = const_cast<X_NumericDelegate*>(this);
        connect(editor, SIGNAL(valueChanged(double)), pThis, SLOT(zh_editorValueChanged(double)));
    }
    return editor;
}
//=================================================================
void	X_NumericDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QStyleOptionViewItem newOption(option);

    if(!xv_inheritFontBoldFlag)
    {
        newOption.font.setBold(xv_fontBoldFlag);
    }

    newOption.state =  newOption.state | QStyle::State_Active;
    QStyledItemDelegate::paint(painter, newOption, index);
}
//=================================================================
void	X_NumericDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
{
    QString val = index.model()->data(index, Qt::DisplayRole).toString();
    X_NumericEditor* numericEditor = qobject_cast<X_NumericEditor*>(editor);
    numericEditor->setText(val);
}
//=================================================================
void	X_NumericDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
    X_NumericEditor* numericEditor = qobject_cast<X_NumericEditor*>(editor);
    QString valString  = numericEditor->text();

    bool ok;
    double val = valString.toDouble(&ok);
    bool removeLastTree = false;
    MessageType messageType = MT_NO_MESSAGE;

    if(val < xv_editorMin )
    {
        // QMessageBox::warning(0, tr("Limit exceed"),  tr("The value exceeds minimum limit."), QMessageBox::Ok) ;
        messageType = MT_MIN_LIMIT;
        valString = QString::number(xv_editorMin, 'g', 18);
        removeLastTree = true;
    }
    else if(val > xv_editorMax)
    {
        // QMessageBox::warning(0, tr("Limit exceed"),  tr("The value exceeds maximum limit."), QMessageBox::Ok) ;
        messageType = MT_MAX_LIMIT;
        valString = QString::number(xv_editorMax, 'g', 18);
        removeLastTree = true;
    }

    valString.replace(QRegularExpression(","), ".");
    valString.replace(QRegularExpression("e"), "E");
    bool powerTypeRecord = valString.contains(QRegularExpression("[eE]"));

    QStringList parts = valString.split(QRegularExpression("[,.Ee]"),Qt::KeepEmptyParts);

    if(removeLastTree && parts.count() > 2)
    {
        QString fractionalPart = parts.value(1);
        int digitsToRemoveNumber = fractionalPart.length() - 15;
        if(digitsToRemoveNumber > 0)
        {
            fractionalPart.remove(fractionalPart.length() - digitsToRemoveNumber , digitsToRemoveNumber);
            parts[1] = fractionalPart;
        }
    }

    for(int i  = 0; i < parts.count(); i++)
    {
        if(parts.value(i) == "")
            parts[i] = "0";
    }

    QString newNumericString;
    for(int i = 0; i < parts.count(); i++)
    {

        newNumericString = newNumericString + parts.value(i);
        if((i == 0 && powerTypeRecord == false && parts.count() == 2) ||
                (i == 0 && powerTypeRecord == true && parts.count() == 3))
        {
            newNumericString = newNumericString + ".";
        }

        if( (i == 0 &&  powerTypeRecord == true && parts.count() == 2) ||
                (i == 1 && powerTypeRecord == true && parts.count() == 3) )
        {
            newNumericString = newNumericString + "E";
        }
    }

    numericEditor->setText(newNumericString);
    model->setData(index, QVariant(newNumericString));

    bool* pIsEditorAlive = const_cast<bool*>(&xv_isEditorAlive);

    if(messageType == MT_MAX_LIMIT)
    {
        QMessageBox::warning(0, tr("Limit exceed"),  tr("The value exceeds maximum limit."), QMessageBox::Ok) ;
        *pIsEditorAlive = false;
    }
    else if(messageType == MT_MIN_LIMIT)
    {
        QMessageBox::warning(0, tr("Limit exceed"),  tr("The value exceeds minimum limit."), QMessageBox::Ok) ;
        *pIsEditorAlive = false;
    }
    else
    {
        *pIsEditorAlive = true;
    }
}
//=================================================================
QSize X_NumericDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    return QStyledItemDelegate::sizeHint (option, index);
}
//=================================================================
void	X_NumericDelegate::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QStyledItemDelegate::updateEditorGeometry (editor, option, index);
}
//=================================================================
void X_NumericDelegate::xp_setImmediatellyValueChange(bool flag)
{
    xv_immediatellyValueChangeFlag = flag;
}
//=================================================================
void X_NumericDelegate::xp_setEditorMinMax(double min, double max)
{
    if(min < max || min < std::numeric_limits<long>::min() || max > std::numeric_limits<long>::max())
    {
        xv_editorMin = min;
        xv_editorMax = max;
    }
}
//=================================================================
void X_NumericDelegate::xp_editorMinMax(double& min, double& max) const
{
    min = xv_editorMin;
    max = xv_editorMax;
}
//=================================================================
void X_NumericDelegate::xp_setFontBold(bool boldFlag)
{
    xv_inheritFontBoldFlag = false;
    xv_fontBoldFlag = boldFlag;
}
//=================================================================
void X_NumericDelegate::zh_onEditorEnterClick()
{
    X_NumericEditor* editor = qobject_cast<X_NumericEditor*>(sender());
    if(editor->text().isEmpty())
    {
        editor->setValue(0);
    }
    emit commitData(editor);
    if(xv_isEditorAlive)
    {
        emit closeEditor(editor);
    }

    emit xg_editNext(xv_editedIndex);
}
//=================================================================
void X_NumericDelegate::zh_editorValueChanged(double value)
{
    X_NumericEditor* editor = qobject_cast<X_NumericEditor*>(sender());
    if(!editor)
    {
        return;
    }

    QAbstractItemModel* model = const_cast<QAbstractItemModel*>(xv_editedIndex.model());
    setModelData (editor, model, xv_editedIndex);
}
//=================================================================

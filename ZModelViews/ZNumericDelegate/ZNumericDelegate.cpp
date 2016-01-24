//=========================================------------------------------------------
#include "ZNumericDelegate.h"
#include "ZNumericEditor.h"
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QTextEdit>
#include <limits>
#include <QMessageBox>
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>
//=========================================------------------------------------------
ZNumericDelegate::ZNumericDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{

    m_editorMin = (double)std::numeric_limits<long>::min();
    m_editorMax = (double)std::numeric_limits<long>::max();
    m_inheritFontBoldFlag = true;
    m_fontBoldFlag = false;
}
//=========================================------------------------------------------
QWidget* ZNumericDelegate::	createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    ZNumericEditor* editor = new ZNumericEditor(parent);
    connect(editor, SIGNAL(enterClicked()), this, SLOT(onEditorEnterClick()));

    ZNumericDelegate* pThis = const_cast<ZNumericDelegate*>(this);
    emit requestEditorMinMax(pThis, index);
    editor->setMinMax(m_editorMin, m_editorMax);

    return editor;

}
//=========================================------------------------------------------
void	ZNumericDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QStyleOptionViewItem newOption(option);

    if(!m_inheritFontBoldFlag)
    {
        newOption.font.setBold(m_fontBoldFlag);
    }

    QStyledItemDelegate::paint(painter, newOption, index);
}
//=========================================------------------------------------------
void	ZNumericDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
{
    QString val = index.model()->data(index, Qt::DisplayRole).toString();
    ZNumericEditor* numericEditor = qobject_cast<ZNumericEditor*>(editor);
    numericEditor->setText(val);
}
//=========================================------------------------------------------
void	ZNumericDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
    ZNumericEditor* numericEditor = qobject_cast<ZNumericEditor*>(editor);
    QString valString  = numericEditor->text();

    bool ok;
    double val = valString.toDouble(&ok);
    bool removeLastTree = false;
    MessageType messageType = MT_NO_MESSAGE;

    if(val < m_editorMin )
    {
        // QMessageBox::warning(0, tr("Limit exceed"),  tr("The value exceeds minimum limit."), QMessageBox::Ok) ;
        messageType = MT_MIN_LIMIT;
        valString = QString::number(m_editorMin, 'g', 18);
        removeLastTree = true;
    }
    else if(val > m_editorMax)
    {
        // QMessageBox::warning(0, tr("Limit exceed"),  tr("The value exceeds maximum limit."), QMessageBox::Ok) ;
        messageType = MT_MAX_LIMIT;
        valString = QString::number(m_editorMax, 'g', 18);
        removeLastTree = true;
    }

    valString.replace(QRegExp(","), ".");
    valString.replace(QRegExp("e"), "E");
    bool powerTypeRecord = valString.contains(QRegExp("[eE]"));

    QStringList parts = valString.split(QRegExp("[,.Ee]"),QString::KeepEmptyParts);

    if(removeLastTree && parts.count() > 2)
    {
        QString fractionalPart = parts.value(1);
        int digitsToRemoveNumber = fractionalPart.count() - 15;
        if(digitsToRemoveNumber > 0)
        {
            fractionalPart.remove(fractionalPart.count() - digitsToRemoveNumber , digitsToRemoveNumber);
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

    if(model->setData(index, QVariant(newNumericString)))
    {

    }

    QModelIndex* pIndex = const_cast<QModelIndex*>(&m_editedIndex);
    *pIndex = index;
    pIndex = 0;

    bool* pIsEditorAlive = const_cast<bool*>(&m_isEditorAlive);

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
//=========================================------------------------------------------
QSize ZNumericDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    return QStyledItemDelegate::sizeHint (option, index);
}
//=========================================------------------------------------------
void	ZNumericDelegate::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QStyledItemDelegate::updateEditorGeometry (editor, option, index);
}
//=========================================------------------------------------------

void ZNumericDelegate::setEditorMinMax(double min, double max)
{
    if(min < max || min < std::numeric_limits<long>::min() || max > std::numeric_limits<long>::max())
    {
        m_editorMin = min;
        m_editorMax = max;
    }
}
//=========================================------------------------------------------
void ZNumericDelegate::setFontBold(bool boldFlag)
{
    m_inheritFontBoldFlag = false;
    m_fontBoldFlag = boldFlag;
}
//=========================================------------------------------------------
void ZNumericDelegate::onEditorEnterClick()
{
    ZNumericEditor* editor = qobject_cast<ZNumericEditor*>(sender());
    if(editor->text().isEmpty())
    {
        editor->setValue(0);

    }
    emit commitData(editor);
    if(m_isEditorAlive)
    {
        emit closeEditor(editor);
    }

    emit editNext(m_editedIndex);
}
//=========================================------------------------------------------

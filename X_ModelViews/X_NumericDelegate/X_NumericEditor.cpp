//================================================================
#include "X_NumericEditor.h"
#include <QLineEdit>
#include <limits>
#include <QDebug>
#include <QLocale>
#include <math.h>
#include <QMessageBox>
#include <QKeyEvent>
#include <QFontMetrics>
#include <QRegularExpression>
#include <QStyle>
#include <QStyleOptionComplex>
//================================================================
X_NumericEditor::X_NumericEditor(QWidget *parent) :
    QAbstractSpinBox(parent)
{
    xv_min = (double)std::numeric_limits<long long>::min();
    xv_max = (double)std::numeric_limits<long long>::max();

    //int maxPartSymbols = QString::number(m_max).count();
    lineEdit()->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    lineEdit()->setValidator(new QDoubleValidator());

    lineEdit()->setText("0.0");
    connect(lineEdit(), &QLineEdit::textChanged,
            this, &X_NumericEditor::xp_checkChangedText);

}
//================================================================
X_NumericEditor::StepEnabled	X_NumericEditor::stepEnabled () const
{
    return X_NumericEditor::StepDownEnabled | X_NumericEditor::StepUpEnabled;
}
//================================================================
void X_NumericEditor::stepBy ( int steps )
{
    QLocale locale;
    QString decimalSeparator = locale.decimalPoint();
    QString numericString = lineEdit()->text();
    int cursorPos = lineEdit()->cursorPosition();

    numericString.replace(QRegularExpression("[,.]"), decimalSeparator);
    //numericString.replace(QRegExp("e"), "E");

    QStringList parts = numericString.split(QRegularExpression("[,.Ee]"),Qt::KeepEmptyParts);

    for(int i  = 0; i < parts.count(); i++)
    {
        if(parts.value(i) == "")
            parts[i] = "0";
    }

    bool powerTypeRecord = numericString.contains(QRegularExpression("[eE]"));

    // defining which part is the cursor on
    if(parts.isEmpty() || parts.count() > 3)
    {
        return;
    }

    QString newNumericString;
    int changedPartNumber = 0;
    //int cursorPos;

    int firstSeparatorPos;
    int secondSeparatorPos;

    switch(parts.count())
    {
    case 1:

        firstSeparatorPos = 0;
        secondSeparatorPos = 0;
        changedPartNumber = 0;
        cursorPos = lineEdit()->cursorPosition();

        break;
    case 2:

        secondSeparatorPos = numericString.length() - 1;
        firstSeparatorPos = numericString.indexOf(QRegularExpression("[,.Ee]"));
        if(cursorPos > firstSeparatorPos)
        {
            changedPartNumber = 1;
            cursorPos = lineEdit()->cursorPosition() - firstSeparatorPos - 1;
        }
        else
        {
            changedPartNumber = 0;
            cursorPos = lineEdit()->cursorPosition();
        }

        break;
    case 3:

        firstSeparatorPos = numericString.indexOf(QRegularExpression("[,.]"));
        secondSeparatorPos = numericString.indexOf(QRegularExpression("[Ee]"));

        if(cursorPos <= firstSeparatorPos )
        {
            changedPartNumber = 0;
            cursorPos = lineEdit()->cursorPosition();
        }
        else if((cursorPos > firstSeparatorPos) && (cursorPos <= secondSeparatorPos))
        {
            changedPartNumber = 1;
            cursorPos = lineEdit()->cursorPosition() - firstSeparatorPos - 1;
        }
        else if(cursorPos > secondSeparatorPos)
        {
            changedPartNumber = 2;
            cursorPos = lineEdit()->cursorPosition() - secondSeparatorPos - 1;
        }

        break;
    default:
        return;
    }

    qint64 changedPartNumeric = parts.value(changedPartNumber).toLongLong();
    qint64 fractionlPartMax = -1;

    if((powerTypeRecord == false &&  changedPartNumber == 1) ||
            (changedPartNumber == 1 && powerTypeRecord == true && parts.count() == 3))
    {
        QString fractionalPartString;
        for(int i = 0; i < parts.value(changedPartNumber).length(); i++)
        {
            fractionalPartString += "9";
        }
        fractionlPartMax = fractionalPartString.toLongLong();
    }

    bool partValueEqualLimit = false;
    int newPartCount;
    for(int s = 0; s < qAbs(steps); s++)
    {
        if(fractionlPartMax > 0)
        {
            if((changedPartNumeric <= 0 && steps < 0) || (changedPartNumeric >= fractionlPartMax && steps > 0))
            {
                partValueEqualLimit = true;
            }
            else
            {
                changedPartNumeric = steps > 0 ? changedPartNumeric+1 : changedPartNumeric-1;
            }
        }
        else
        {
            changedPartNumeric = steps > 0 ? changedPartNumeric+1 : changedPartNumeric-1;
        }

        if(!checkNewNumber(parts, powerTypeRecord, changedPartNumeric, changedPartNumber, newNumericString,  newPartCount) )
        {
            //rewind
            changedPartNumeric = steps > 0 ? changedPartNumeric-1 : changedPartNumeric+1;
            checkNewNumber(parts, powerTypeRecord, changedPartNumeric, changedPartNumber, newNumericString,  newPartCount);

            break;
        }
        if(partValueEqualLimit)
        {
            break;
        }
    }

    lineEdit()->setText(newNumericString);
    bool ok;
    double newValue = locale.toDouble(newNumericString, &ok);
    if(ok)
    {
        emit valueChanged(newValue);
    }

    int newCursorPos = 0;
    switch (changedPartNumber)
    {
    case 0:
        newCursorPos = cursorPos + (newPartCount - parts.value(changedPartNumber).length());
        break;
    case 1:
        newCursorPos = cursorPos + firstSeparatorPos + 1;
        break;
    case 2:
        newCursorPos = cursorPos + secondSeparatorPos + 1;
        break;
    default:

        break;
    }

    if(newCursorPos < 0)
    {
        newCursorPos = 0;
    }
    else if(newCursorPos > newNumericString.length())
    {
        newCursorPos = newNumericString.length();
    }

    lineEdit()->setCursorPosition(newCursorPos);
}
//================================================================
void X_NumericEditor::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        // qDebug() << "Key Press";


        // setText(lineEdit()->text());
        emit enterClicked();
        return;
    }

    QAbstractSpinBox::keyPressEvent(event);
}
//================================================================
bool X_NumericEditor::checkNewNumber(QStringList parts,
                                    bool powerTypeRecord,
                                    qint64 changedPartNumeric,
                                    int changedPartNumber,
                                    QString& newNumericString,
                                    int& newPartCount)
{
    QLocale locale;
    QString decimalSeparator = locale.decimalPoint();
    newNumericString.clear();
    //    newNumericString = QString::number(changedPartNumeric);

    QString partString = QString::number(changedPartNumeric);

    if( (parts.count() == 2 && changedPartNumber == 1 && powerTypeRecord == false) ||
            (parts.count() == 3 && changedPartNumber == 1 ) )
    {
        int charCountDelta = parts.value(changedPartNumber).length() - partString.length();

        for(int i = 0; i < charCountDelta; i++)
        {
            partString = "0"+partString;
        }
    }

    if(parts.count() - 1 == changedPartNumber && powerTypeRecord && changedPartNumeric >= 0)
    {
        partString = "+"+partString;
    }

    for(int i = 0; i < parts.count(); i++)
    {
        newNumericString = i == changedPartNumber ? newNumericString + partString : newNumericString + parts.value(i);
        if((i == 0 && !powerTypeRecord && parts.count() == 2) ||
                (i == 0 && powerTypeRecord && parts.count() == 3))
        {
            // newNumericString = newNumericString + ".";
            newNumericString = newNumericString + decimalSeparator;
        }

        if( (i == 0 &&  powerTypeRecord && parts.count() == 2) ||
                (i == 1 && powerTypeRecord && parts.count() == 3) )
        {
            newNumericString = newNumericString + "e";
        }
    }

    double newValue = locale.toDouble(newNumericString);
    newPartCount = partString.length();

    if(newValue < xv_min || newValue > xv_max)
    {
        return false;
    }

    return true;
}
//================================================================
void X_NumericEditor::setMinMax(double min, double max)
{
    if(min < max || min < std::numeric_limits<long>::min() || max > std::numeric_limits<long>::max())
    {
        xv_min = min;
        xv_max = max;
    }
}
//================================================================
void X_NumericEditor::setText(QString numericString)
{
    QLocale locale;
    QString decimalSeparator = locale.decimalPoint();
    bool removeLastTree = false;
//    if(val < xv_min )
//    {
//        //QMessageBox::warning(0, tr("Limit exceed"),  tr("The value exceeds minimum limit."), QMessageBox::Ok) ;
//        // numericString = QString::number(xv_min, 'g', 18);
//        xv_min = val;
//    }
//    else if(val > xv_max)
//    {
//        //QMessageBox::warning(0, tr("Limit exceed"),  tr("The value exceeds maximum limit."), QMessageBox::Ok) ;
//        // numericString = QString::number(xv_max, 'g', 18);
//        xv_max = val;
//    }

    if(removeLastTree)
    {
        QStringList parts = numericString.split(QRegularExpression("[,.Ee]"),Qt::KeepEmptyParts);

        QString fractionalPart = parts.value(1);
        fractionalPart.remove(fractionalPart.length() - 3 , 3);
        parts[1] = fractionalPart;

        numericString.clear();
        for(int i = 0; i < parts.count(); i++)
        {
            numericString = numericString + parts.value(i);
            if(i == 0)
            {
                numericString = numericString + decimalSeparator;
                // numericString = numericString + ".";
            }

            if(i == 1)
            {
                numericString = numericString + "e";
            }
        }
    }

    bool ok;

    double val = locale.toDouble(numericString, &ok);

    if(val < xv_min )
    {
        xv_min = val;
    }
    else if(val > xv_max)
    {
        xv_max = val;
    }

    lineEdit()->setText(numericString);
    lineEdit()->selectAll();
}
//================================================================
void X_NumericEditor::setValue(double value)
{
    lineEdit()->setText(QString::number(value));
}
//================================================================
QString X_NumericEditor::text()
{
    return lineEdit()->text();
}
//================================================================
double X_NumericEditor::value()
{
    QLocale locale;
    return locale.toDouble(lineEdit()->text());
}
//================================================================
QSize X_NumericEditor::sizeHint() const
{
    QSize size = QAbstractSpinBox::sizeHint();
    QFontMetrics fm(this->font());
    QString maxString = QString::number(xv_max, 'f', 15);
    QString minString = QString::number(xv_min, 'f', 15);
    int stringWidth = qMax(fm.horizontalAdvance(maxString), fm.horizontalAdvance(minString));
    QStyleOptionComplex opt;
    opt.initFrom(this);
    int upDownButtonWidth = this->style()->subControlRect(QStyle::CC_SpinBox, &opt, QStyle::SC_SpinBoxDown, this).width();
    size.setWidth(stringWidth + (upDownButtonWidth));
    return size;
}
//================================================================
void X_NumericEditor::xp_checkChangedText(const QString& valueString)
{
    if(valueString.isEmpty())
    {
        lineEdit()->setText(QString::number(0.0));
        return;
    }

    QLocale locale;
    QString decimalSeparator = locale.decimalPoint();
//    QString newValueString = valueString;
//    qDebug() << "NEW" << newValueString << "EDITLINE" << lineEdit()->text();

    QRegularExpression re("^[+-]?\\d{1,}[,.]?(\\d{1,}([eE][+-]?\\d{1,})?)?$");
    QRegularExpressionMatch match = re.match(valueString);
    if(!match.hasMatch())
    {
        lineEdit()->setText(lineEdit()->text().chopped(1));
    }

    // replace decimalSeparator
    lineEdit()->setText(lineEdit()->text().replace(QRegularExpression("[,.]"), decimalSeparator));

    qreal newValue = locale.toDouble(valueString);

    if(newValue > xv_max)
    {
        lineEdit()->setText(QString::number(xv_max));
    }
    else if(newValue < xv_min)
    {
        lineEdit()->setText(QString::number(xv_min));
    }
}
//================================================================

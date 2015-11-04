//=========================================----------------------
#include "ZNumericEditor.h"
#include <QLineEdit>
#include <limits>
#include <QDebug>
#include <math.h>
#include <QMessageBox>
#include <QKeyEvent>
//=========================================----------------------
ZNumericEditor::ZNumericEditor(QWidget *parent) :
    QAbstractSpinBox(parent)
{
    m_min = (double)std::numeric_limits<long>::min();
    m_max = (double)std::numeric_limits<long>::max();

    //int maxPartSymbols = QString::number(m_max).count();

    lineEdit()->setValidator(new QDoubleValidator());
    lineEdit()->setText("0.0");
}
//=========================================------------------------
ZNumericEditor::StepEnabled	ZNumericEditor::stepEnabled () const
{
    return ZNumericEditor::StepDownEnabled | ZNumericEditor::StepUpEnabled;
}
//=========================================----------------------
void	ZNumericEditor::stepBy ( int steps )
{
    QString numericString = lineEdit()->text();
    int cursorPos = lineEdit()->cursorPosition();

    //double doubleValue = numericString.toDouble();

    numericString.replace(QRegExp(","), ".");
    //numericString.replace(QRegExp("e"), "E");

    QStringList parts = numericString.split(QRegExp("[,.Ee]"),QString::KeepEmptyParts);

    for(int i  = 0; i < parts.count(); i++)
    {
        if(parts.value(i) == "")
            parts[i] = "0";
    }

    bool powerTypeRecord = numericString.contains(QRegExp("[eE]"));

    // определяем в какой части курсор
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

        changedPartNumber = 0;
        cursorPos = lineEdit()->cursorPosition();

        break;
    case 2:

        firstSeparatorPos = numericString.indexOf(QRegExp("[,.Ee]"));
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

        firstSeparatorPos = numericString.indexOf(QRegExp("[,.]"));
        secondSeparatorPos = numericString.indexOf(QRegExp("[Ee]"));

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
        for(int i = 0; i < parts.value(changedPartNumber).count(); i++)
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
            //откат
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
    // qDebug() << "NUMERIC EDITOR DATA CHANGED";
    double newValue = newNumericString.toDouble(&ok);
    if(ok)
    {
        emit valueChanged(newValue);
    }

    int newCursorPos = 0;
    switch (changedPartNumber)
    {
    case 0:
        newCursorPos = cursorPos + (newPartCount - parts.value(changedPartNumber).count());
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
    else if(newCursorPos > newNumericString.count())
    {
        newCursorPos = newNumericString.count();
    }

    lineEdit()->setCursorPosition(newCursorPos);
}
//=========================================----------------------
void ZNumericEditor::keyPressEvent(QKeyEvent *event)
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
//=========================================----------------------
bool ZNumericEditor::checkNewNumber(QStringList parts,
                                    bool powerTypeRecord,
                                    qint64 changedPartNumeric,
                                    int changedPartNumber,
                                    QString& newNumericString,
                                    int& newPartCount)
{
    newNumericString.clear();
    //    newNumericString = QString::number(changedPartNumeric);

    QString partString = QString::number(changedPartNumeric);

    if( (parts.count() == 2 && changedPartNumber == 1 && powerTypeRecord == false) ||
            (parts.count() == 3 && changedPartNumber == 1 ) )
    {
        int charCountDelta = parts.value(changedPartNumber).count() - partString.count();

        for(int i = 0; i < charCountDelta; i++)
        {
            partString = "0"+partString;
        }
    }

    for(int i = 0; i < parts.count(); i++)
    {

        newNumericString = i == changedPartNumber ? newNumericString + partString : newNumericString + parts.value(i);
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

    double newValue = newNumericString.toDouble();

    newPartCount = partString.count();

    if(newValue < m_min || newValue > m_max)
    {
        return false;
    }

    return true;
}
//=========================================----------------------
void ZNumericEditor::setMinMax(double min, double max)
{
    if(min < max || min < std::numeric_limits<long>::min() || max > std::numeric_limits<long>::max())
    {
        m_min = min;
        m_max = max;
    }
}
//=========================================----------------------
void ZNumericEditor::setText(QString numericString)
{
    bool ok;

    double val = numericString.toDouble(&ok);
    bool removeLastTree = false;
    if(val < m_min )
    {
        QMessageBox::warning(0, tr("Limit exceed"),  tr("The value exceeds minimum limit."), QMessageBox::Ok) ;
        numericString = QString::number(m_min, 'g', 18);

    }
    else if(val > m_max)
    {
        QMessageBox::warning(0, tr("Limit exceed"),  tr("The value exceeds maximum limit."), QMessageBox::Ok) ;
        numericString = QString::number(m_max, 'g', 18);
    }

    if(removeLastTree)
    {
        QStringList parts = numericString.split(QRegExp("[,.Ee]"),QString::KeepEmptyParts);

        QString fractionalPart = parts.value(1);
        fractionalPart.remove(fractionalPart.count() - 3 , 3);
        parts[1] = fractionalPart;

        numericString.clear();
        for(int i = 0; i < parts.count(); i++)
        {
            numericString = numericString + parts.value(i);
            if(i == 0)
            {
                numericString = numericString + ".";
            }

            if(i == 1)
            {
                numericString = numericString + "E";
            }
        }
    }

    lineEdit()->setText(numericString);
    lineEdit()->selectAll();
}
//=========================================----------------------
void ZNumericEditor::setValue(double value)
{
    lineEdit()->setText(QString::number(value));
}
//=========================================----------------------
QString ZNumericEditor::text()
{
    return lineEdit()->text();
}
//=========================================----------------------
double ZNumericEditor::value()
{
    return lineEdit()->text().toDouble();
}
//=========================================----------------------

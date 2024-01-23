//=========================================================
#include "X_Calibration.h"
#include "X_AbstractSpectrum.h"
#include "X_CustomTerm.h"
#include "X_General.h"
#include "X_LocaleDoubleConverter.h"
#include "X_MathExpressionHandler.h"
#include "X_MathExpressionVariableListMaker.h"
#include "X_MixedTerm.h"
#include "X_QrealToStringConverter.h"
#include "X_QuadraticTerm.h"
#include "X_RawTerm.h"
#include "X_SimpleTerm.h"
#include "X_TermNormalizer.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QPointer>
#include <QStringList>
//=========================================================
// STATIC
//=========================================================
const QString X_Calibration::simplePolynomEquationString(
    X_Calibration::xh_initPlynomialEquationString());
const QString X_Calibration::fractionalEquationString(
    X_Calibration::xh_initFractionalEquationString1());
const QString X_Calibration::xv_defaultWindowName(QObject::tr("Peak #"));

bool X_Calibration::xv_useBaseTermInFractionalEquationByDefault = false;
QList<QColor> X_Calibration::xv_colorList = X_Calibration::xp_createColorList();
qint64 X_Calibration::xv_lastCalibrationId = 0;
int X_Calibration::xv_lastColorIndex = 0;
int X_Calibration::xv_precision = 15;
QMap<X_Calibration::EquationType, QString> X_Calibration::xv_eqationTypeStringMap;
   //= X_Calibration::xh_initEquationTypeStringMap();
//=========================================================
QMap<X_Calibration::EquationType, QString> X_Calibration::xh_initEquationTypeStringMap()
{
    QMap<X_Calibration::EquationType, QString> map;
    map.insert(ET_NOT_DEFINED, tr("Not defined"));
    map.insert(ET_POLYNOMIAL, tr("Polynomial"));
    map.insert(ET_FRACTIONAL, tr("Fractional"));
    return map;
}
//=========================================================
QString X_Calibration::xh_initPlynomialEquationString()
{
    QString simplePolynomEquationString
        = "<table border=0 cellspacing=0 cellpadding=0>"
          "<tr>"
          "<td></td>"
          "<td align=center><sub><i>n</i></sub></td>"
          "<td></td>"
          "</tr>"

          "<tr>"
          "<td valign=middle>C=</td>"
          "<td valign=middle align=center>\u2211</td>"
          "<td valign=middle>(a<sub><i>i</i></sub>*X<sub><i>i</i></sub>) + a<sub>0</sub></td>"
          "</tr>"

          "<tr>"
          "<td></td>"
          "<td align=center><sup><i>i</i> = 1</sup></td>"
          "<td></td>"
          "</tr>"
          "</table>";
    return simplePolynomEquationString;
}
//=========================================================
QString X_Calibration::xh_initFractionalEquationString()
{
    QString fractionalEquationString
        = "<table border=0 cellspacing=0 cellpadding=0>"
          "<tr>"
          "<td></td>"
          "<td></td>"
          "<td>a<sub><i>base</i></sub>*X<sub><i>base</i></sub></td>"
          "</tr>"

          "<tr>"
          "<td>C= </td>"
          "<td><hr></td>"
          "<td><hr></td>"
          "</tr>"

          "<tr>"
          "<td></td>"
          "<td align=center><sub><i>n</i></sub></td>"
          "<td></td>"
          "<td></td>"

          "<tr>"
          "<td></td>"
          "<td valign=middle align=center>\u2211</td>"
          "<td valign=middle>(a<sub><i>i</i></sub>*X<sub><i>i</i></sub>) + a<sub>0</sub></td>"
          "</tr>"

          "<tr>"
          "<td></td>"
          "<td align=center><sup><i>i</i> = 1</sup></td>"
          "<td></td>"
          "</tr>"
          "</table>";
    return fractionalEquationString;
}
//=========================================================
QString X_Calibration::xh_initFractionalEquationString1()
{
    QString fractionalEquationString
        = "<table border=0 cellspacing=0 cellpadding=0>"
          "<tr>"
          "<td></td>"
          "<td align=center><sub><i>n</i></sub></td>"
          "<td></td>"
          "</tr>"

          "<tr>"
          "<td valign=middle>C= a<sub><i>base</i></sub>*X<sub><i>base</i></sub> / (</td>"
          "<td valign=middle align=center>\u2211</td>"
          "<td valign=middle>(a<sub><i>i</i></sub>*X<sub><i>i</i></sub>) + a<sub>0</sub>)</td>"
          "</tr>"

          "<tr>"
          "<td></td>"
          "<td align=center><sup><i>i</i> = 1</sup></td>"
          "<td></td>"
          "</tr>"
          "</table>";
    return fractionalEquationString;
}
//=========================================================
QString X_Calibration::xp_equationTypeString(X_Calibration::EquationType type)
{
    return xv_eqationTypeStringMap.value(type);
}
//=========================================================
X_Calibration::EquationType X_Calibration::xp_equationTypeFromString(const QString& typestring)
{
    if (!xv_eqationTypeStringMap.values().contains(typestring))
    {
        return X_Calibration::ET_NOT_DEFINED;
    }

    return xv_eqationTypeStringMap.key(typestring);
}
// END STATIC
//=========================================================
//=========================================================
X_Calibration::X_Calibration(const QString& name, QObject* parent) : QObject(parent)
{
    xv_visibility = false;
    xv_useBaseTermInFractionalEquation = xv_useBaseTermInFractionalEquationByDefault;

    QFileInfo fileInfo(name);
    if (fileInfo.exists())
    {
        xv_baseName = fileInfo.baseName();
        xv_suffix = fileInfo.completeSuffix();
        xv_path = fileInfo.absolutePath();
        xv_dirty = false;
    }
    else
    {
        xv_path = QString();
        xv_suffix = "clbx";
        xv_baseName = name;
        xv_dirty = true;
    }

    xv_color = xv_colorList.value(xv_lastColorIndex++);
    if (xv_lastColorIndex >= xv_colorList.count())
    {
        xv_lastColorIndex = 0;
    }

    xv_exposition = 0;
    //    xv_energyCalibrationFactorK0 = 0;
    //    xv_energyCalibrationFactorK1 = 0;
    //    xv_energyCalibrationFactorK2 = 0;
    xv_gainFactor = 0;
    //    xv_energyUnit = "not defined";

    if(xv_lastCalibrationId <= 0)
    {
        xv_eqationTypeStringMap
            = X_Calibration::xh_initEquationTypeStringMap();
    }

    xv_calibrationId = xv_lastCalibrationId++;
    xv_chemElement = glDefaultChemElementString;
    xv_termNormalizer = new X_TermNormalizer(this);
    xv_baseTermNormalizer = new X_TermNormalizer(this);

    xv_equationType = ET_POLYNOMIAL;
    xp_setEquationIntercept(0.0);
    xv_baseTermId = -1;
    xv_dateTime = QDateTime::currentDateTime();
    xh_createConnections();
}
//=========================================================
X_Calibration::X_Calibration(const X_Calibration* calibration, const QString& name, QObject* parent)
    : QObject(parent)
{
    xv_visibility = false;
    xv_useBaseTermInFractionalEquation = calibration->xv_useBaseTermInFractionalEquationByDefault;

    xv_path = calibration->xv_path;
    xv_suffix = calibration->xv_suffix;
    xv_baseName = name;
    xv_dirty = true;

    xv_color = xv_colorList.value(xv_lastColorIndex++);
    if (xv_lastColorIndex >= xv_colorList.count())
    {
        xv_lastColorIndex = 0;
    }

    xv_exposition = calibration->xv_exposition;
    //    xv_energyCalibrationFactorK0 =  calibration->xv_energyCalibrationFactorK0;
    //    xv_energyCalibrationFactorK1 = calibration->xv_energyCalibrationFactorK1;
    //    xv_energyCalibrationFactorK2 = calibration->xv_energyCalibrationFactorK2;
    xv_gainFactor = calibration->xv_gainFactor;
    //    xv_energyUnit = calibration->xv_energyUnit;
    if(xv_lastCalibrationId <= 0)
    {
        xv_eqationTypeStringMap
            = X_Calibration::xh_initEquationTypeStringMap();
    }
    xv_calibrationId = xv_lastCalibrationId++;
    xv_chemElement = calibration->xv_chemElement;

    xv_termNormalizer = new X_TermNormalizer(this);
    xv_baseTermNormalizer = new X_TermNormalizer(this);

    xv_equationType = calibration->xv_equationType;

    xp_setEquationIntercept(0.0);
    xv_baseTermId = -1;
    xv_dateTime = QDateTime::currentDateTime();
    xh_createConnections();
}
//=========================================================
X_Calibration::~X_Calibration() {}


//=========================================================
void X_Calibration::xh_createConnections()
{
    connect(xv_termNormalizer,
            &X_TermNormalizer::xg_calcWindowIntensity,
            this,
            &X_Calibration::xp_calcWindowIntensity);
    connect(xv_termNormalizer,
            &X_TermNormalizer::xg_errorReport,
            this,
            &X_Calibration::xg_message);

}
//=========================================================
QDateTime X_Calibration::xp_dateTime() const
{
    return xv_dateTime;
}
//=========================================================
QString X_Calibration::xp_baseName() const
{
    return xv_baseName;
}
//=========================================================
void X_Calibration::xp_setBaseName(const QString& name)
{
    if (name == xv_baseName || name.isEmpty())
    {
        return;
    }

    xv_baseName = name;
    xv_dirty = true;
    xv_dateTime = QDateTime::currentDateTime();
    emit xg_dirtyChanged(xv_dirty);
}
//=========================================================
QString X_Calibration::xp_fileName() const
{
    return xv_baseName + "." + xv_suffix;
}
//=========================================================
QString X_Calibration::xp_suffix() const
{
    return xv_suffix;
}
//=========================================================
void X_Calibration::xp_setName(const QString& name)
{
    QFileInfo fileInfo(QDir(xv_path), name);
    xv_suffix = fileInfo.completeSuffix();
    xp_setBaseName(fileInfo.baseName());
    xv_dirty = true;
    xv_dateTime = QDateTime::currentDateTime();
    emit xg_dirtyChanged(xv_dirty);
}
//=========================================================
QString X_Calibration::xp_path() const
{
    return xv_path;
}
//=========================================================
void X_Calibration::xp_setPath(const QString& path)
{
    if (path == xv_path || path.isEmpty())
    {
        return;
    }
    QFileInfo fileInfo(path);
    if (fileInfo.absoluteFilePath().isEmpty())
    {
        return;
    }

    xv_path = fileInfo.absolutePath();

    if (fileInfo.isFile())
    {
        xv_baseName = fileInfo.baseName();
        xv_suffix = fileInfo.completeSuffix();
    }

    xv_dirty = true;
    xv_dateTime = QDateTime::currentDateTime();
    emit xg_dirtyChanged(xv_dirty);
}
//=========================================================
QString X_Calibration::xp_chemElement() const
{
    return xv_chemElement;
}
//=========================================================
bool X_Calibration::xp_setChemElement(const QString& chemElement)
{
    if (chemElement.isEmpty())
    {
        return false;
    }

    xv_chemElement = chemElement;
    xv_dirty = true;
    xv_dateTime = QDateTime::currentDateTime();
    emit xg_dirtyChanged(xv_dirty);

    return true;
}
//=========================================================
//void X_Calibration::xp_setEnergyCalibration(qreal K0, qreal K1, qreal K2, const QString& energyUnit)
//{
//    xv_energyCalibrationFactorK0 = K0;
//    xv_energyCalibrationFactorK1 = K1;
//    xv_energyCalibrationFactorK2 = K2;
//    xv_energyUnit = energyUnit;

//}
////=========================================================
//void X_Calibration::xp_setEnergyCalibrationK0(qreal K0)
//{
//    xv_energyCalibrationFactorK0 = K0;
//}
////=========================================================
//void X_Calibration::xp_setEnergyCalibrationK1(qreal K1)
//{
//    xv_energyCalibrationFactorK1 = K1;
//}
////=========================================================
//void X_Calibration::xp_setEnergyCalibrationK2(qreal K2)
//{
//    xv_energyCalibrationFactorK2 = K2;
//}
//=========================================================
//void X_Calibration::xp_setEnergyUnit(const QString& energyUnit)
//{
//    xv_energyUnit = energyUnit;
//}
//=========================================================
void X_Calibration::xp_setGainFactor(int gainFactor)
{
    xv_gainFactor = gainFactor;
}
//=========================================================
void X_Calibration::xp_setExposition(int exposition)
{
    xv_exposition = exposition;
}
//=========================================================
//qreal X_Calibration::xp_energyCalibrationK0() const
//{
//    return xv_energyCalibrationFactorK0;
//}
////=========================================================
//qreal X_Calibration::xp_energyCalibrationK1() const
//{
//    return xv_energyCalibrationFactorK1;
//}
////=========================================================
//qreal X_Calibration::xp_energyCalibrationK2() const
//{
//    return xv_energyCalibrationFactorK2;
//}
////=========================================================
//QString X_Calibration::xp_energyUnit() const
//{
//    return xv_energyUnit;
//}
//=========================================================
int X_Calibration::xp_gainFactor() const
{
    return xv_gainFactor;
}
//=========================================================
int X_Calibration::xp_exposition() const
{
    return xv_exposition;
}
//=========================================================
bool X_Calibration::xp_isVisible() const
{
    return xv_visibility;
}
//=========================================================
void X_Calibration::xp_setVisible(bool visibility)
{
    if (visibility != xv_visibility)
    {
        xv_visibility = visibility;
        emit xg_visibilityChanged(xv_visibility);
    }
}
//=========================================================
bool X_Calibration::xp_isDirty() const
{
    return xv_dirty;
}
//=========================================================
void X_Calibration::xp_setDirty(bool dirty)
{
    xv_dirty = dirty;
    emit xg_dirtyChanged(xv_dirty);
}
//=========================================================
QColor X_Calibration::xp_color() const
{
    return xv_color;
}
//=========================================================
qint64 X_Calibration::xp_calibrationId() const
{
    return xv_calibrationId;
}
//=========================================================
bool X_Calibration::xp_calcConcentration(const X_AbstractSpectrum * const spectrum,
                                        qreal& concentration)
{
    concentration = 0.0;
    if (spectrum == nullptr || xv_equationType == ET_NOT_DEFINED)
    {
        return false;
    }

    // calc norma
    if (!xv_termNormalizer->xp_calcAndSetNormaValue(spectrum))
    {
        return false;
    }

    // calc polynom
    qreal value;
    for (int t = 0; t < xv_termList.count(); t++)
    {
        // exclude base term if !xv_useBaseTermInFractionalEquation
        if (xv_equationType == ET_FRACTIONAL && !xv_useBaseTermInFractionalEquation
            && xv_termList.at(t)->xp_termId() == xv_baseTermId)
        {
            continue;
        }

        // calc all terms
        if (!xv_termList.at(t)->xp_calcValue(spectrum, value))
        {
            return false;
        }

        // normalize terms (xv_termNormalizer defines nesessary)
        if (!xv_termNormalizer->xp_normalizeValue(value))
        {
            return false;
        }
        concentration += value;
    }

    concentration += xv_intercept;

    if (xv_equationType == ET_FRACTIONAL)
    {
        // concentration goes to denominator. check out it
        if (concentration == 0.0)
        {
            return false;
        }

        // calc nominator
        if (!xp_calcBaseTermValue(spectrum, value))
        {
            return false;
        }

        concentration = value / concentration;
    } // end ET_FRACTIONAL

    return true;
}
//=========================================================
bool X_Calibration::xp_calcActiveTermvalueSum(const X_AbstractSpectrum*  const spectrum,
                                              qreal& sumValue)
{
    sumValue = 0.0;
    if (spectrum == nullptr || xv_equationType == ET_NOT_DEFINED)
    {
        return false;
    }

    // calc norma
    if (!xv_termNormalizer->xp_calcAndSetNormaValue(spectrum))
    {
        return false;
    }

    qreal termIntensity = 0.0;
    // calc polynom
    for (int t = 0; t < xv_termList.count(); t++)
    {
        // exclude base term if !xv_useBaseTermInFractionalEquation
        if (xv_equationType == ET_FRACTIONAL && !xv_useBaseTermInFractionalEquation
            && xv_termList.at(t)->xp_termId() == xv_baseTermId)
        {
            continue;
        }

        if(xv_termList.at(t)->xp_termState() == X_AbstractTerm::TS_NOT_DEFINED ||
            xv_termList.at(t)->xp_termState() == X_AbstractTerm::TS_EXAM_WAITING ||
            xv_termList.at(t)->xp_termState() == X_AbstractTerm::TS_EXCEPTED ||
            xv_termList.at(t)->xp_termState() == X_AbstractTerm::TS_CONST_EXCLUDED )
        {
            continue;
        }

        if(xv_termList.at(t)->xp_calcTermIntensity(spectrum, termIntensity))
        {
            sumValue += termIntensity;
        }
        else
        {
            termIntensity = 0.0;
        }
    }
    return true;
}
//=========================================================
int X_Calibration::xp_createNewCalibrationWindow(int firstChannel,
                                                int lastChannel,
                                                X_CalibrationWindow::WindowType windowType,
                                                QString windowName)
{
    int windowNewIndex = -1;

    if (windowName.isEmpty())
    {
        int windowNumber = xv_windowList.count() + 1;
        for (int i = 0; i < xv_windowList.count(); i++)
        {
            QString name = xv_windowList.at(i)->xp_windowName();
            QString digitalPart = name.mid(xv_defaultWindowName.length());
            bool ok;
            int number = digitalPart.toInt(&ok);
            if (!ok)
            {
                continue;
            }

            if (number >= windowNumber)
            {
                windowNumber = ++number;
            }
        }

        windowName = xv_defaultWindowName + QString::number(windowNumber);
    }

    windowNewIndex = xv_windowList.count();
    X_CalibrationWindow* calibrationWindow = new X_CalibrationWindow(windowName,
                                                                   windowType,
                                                                   firstChannel,
                                                                   lastChannel,
                                                                   this);

    xv_termNormalizer->xp_connectToWindow(calibrationWindow);
    xv_baseTermNormalizer->xp_connectToWindow(calibrationWindow);

    emit xg_windowOperation(WOT_BRGIN_INSERT_WINDOWS, windowNewIndex, windowNewIndex);
    xv_windowList.append(calibrationWindow);
    emit xg_windowOperation(WOT_END_INSERT_WINDOWS, windowNewIndex, windowNewIndex);

    if (calibrationWindow->xp_windowType() == X_CalibrationWindow::WT_PEAK)
    {
        xh_createTermsForWindow(calibrationWindow);
    }
    xv_dirty = true;
    xv_dateTime = QDateTime::currentDateTime();
    emit xg_dirtyChanged(xv_dirty);

    return windowNewIndex;
    // xh_createTermsForWindow(calibrationWindow);
}
//=========================================================
int X_Calibration::xp_createNewCalibrationWindow(const X_RawWindow& rawWindow)
{
    int windowNewIndex = xv_windowList.count();
    bool ok;
    int firstChannel = rawWindow.firstChannel.toInt(&ok);
    if (!ok)
    {
        firstChannel = 0;
    }
    int lastChannel = rawWindow.lastChannel.toInt(&ok);
    if (!ok)
    {
        lastChannel = 0;
    }

    X_CalibrationWindow* calibrationWindow = new X_CalibrationWindow(rawWindow.name,
                                                                   rawWindow.windowType,
                                                                   firstChannel,
                                                                   lastChannel,
                                                                   this);

    xv_termNormalizer->xp_connectToWindow(calibrationWindow);
    xv_baseTermNormalizer->xp_connectToWindow(calibrationWindow);

    emit xg_windowOperation(WOT_BRGIN_INSERT_WINDOWS, windowNewIndex, windowNewIndex);
    xv_windowList.append(calibrationWindow);
    emit xg_windowOperation(WOT_END_INSERT_WINDOWS, windowNewIndex, windowNewIndex);

    if (calibrationWindow->xp_windowType() == X_CalibrationWindow::WT_PEAK)
    {
        xh_createTermsForWindow(calibrationWindow);
    }
    xv_dirty = false;
    emit xg_dirtyChanged(xv_dirty);
    return windowNewIndex;
}
//=========================================================
void X_Calibration::xh_createTermsForWindow(const X_CalibrationWindow* window)
{
    // creation simple
    int windowIndex = xv_windowList.indexOf(const_cast<X_CalibrationWindow*>(window));

    if (!xh_windowHasTerms(window, X_AbstractTerm::TT_SIMPLE))
    {
        // define newTermIndex;
        int newTermIndex = -1;
        const X_CalibrationWindow* currentWindow;

        for (int t = 0; t < xv_termList.count(); t++)
        {
            if (xv_termList.at(t)->xp_termType() != X_AbstractTerm::TT_SIMPLE)
            {
                continue;
            }

            const X_SimpleTerm* simpleTerm = qobject_cast<const X_SimpleTerm*>(xv_termList.at(t));
            if (!simpleTerm)
            {
                continue;
            }

            currentWindow = simpleTerm->xp_window();
            if (xv_windowList.indexOf(const_cast<X_CalibrationWindow*>(currentWindow)) > windowIndex)
            {
                break;
            }
            newTermIndex = t;
        }

        newTermIndex++;

        // creation and insertion
        X_SimpleTerm* simpleTerm = new X_SimpleTerm(window, this);
        emit xg_termOperation(TOT_BEGIN_INSERT_TERM, newTermIndex, newTermIndex);
        xv_termList.insert(newTermIndex, simpleTerm);
        emit xg_termOperation(TOT_END_INSERT_TERM, newTermIndex, newTermIndex);
        xv_dirty = true;
        emit xg_dirtyChanged(xv_dirty);
    }

    // quadratic
    if (!xh_windowHasTerms(window, X_AbstractTerm::TT_QUADRATIC))
    {
        // define newTermIndex;
        int newTermIndex = -1;
        int lastSimpleTermIndex = -1;
        const X_CalibrationWindow* currentWindow;

        for (int t = 0; t < xv_termList.count(); t++)
        {
            if (xv_termList.at(t)->xp_termType() == X_AbstractTerm::TT_SIMPLE)
            {
                lastSimpleTermIndex = t;
                continue;
            }

            if (xv_termList.at(t)->xp_termType() != X_AbstractTerm::TT_QUADRATIC)
            {
                continue;
            }

            const X_QuadraticTerm* simpleTerm = qobject_cast<const X_QuadraticTerm*>(
                xv_termList.at(t));
            if (!simpleTerm)
            {
                continue;
            }

            currentWindow = simpleTerm->xp_window();
            if (xv_windowList.indexOf(const_cast<X_CalibrationWindow*>(currentWindow)) > windowIndex)
            {
                break;
            }

            newTermIndex = t;
        }

        if (newTermIndex < 0)
        {
            newTermIndex = lastSimpleTermIndex + 1;
        }
        else
        {
            newTermIndex++;
        }

        // creation and insertion
        X_QuadraticTerm* quadraticTerm = new X_QuadraticTerm(window, this);
        // insertion
        emit xg_termOperation(TOT_BEGIN_INSERT_TERM, newTermIndex, newTermIndex);
        xv_termList.insert(newTermIndex, quadraticTerm);
        emit xg_termOperation(TOT_END_INSERT_TERM, newTermIndex, newTermIndex);
        xv_dirty = true;
        xv_dateTime = QDateTime::currentDateTime();
        emit xg_dirtyChanged(xv_dirty);
    }
}
//=========================================================
bool X_Calibration::xh_windowHasTerms(const X_CalibrationWindow* window,
                                     X_AbstractTerm::TermType type) const
{
    for (int t = 0; t < xv_termList.count(); t++)
    {
        if (xv_termList.at(t)->xp_termType() != type)
        {
            continue;
        }

        if (xv_termList.at(t)->xp_termBelongsToWindow(window))
        {
            return true;
        }
    }

    return false;
}
//=========================================================
int X_Calibration::xh_termIndex(const X_AbstractTerm* term) const
{
    if (!term)
    {
        return -1;
    }

    for (int i = 0; i < xv_termList.count(); i++)
    {
        if (xv_termList.at(i) == term)
        {
            return i;
        }
    }

    return -1;
}
//=========================================================
void X_Calibration::xh_chopTailX_eroesFromInterceptString()
{
    for (int i = xv_interceptString.length() - 1; i > 0; i--)
    {
        if (xv_interceptString.at(i) != '0'
            || (xv_interceptString.at(i - 1) == '.' || xv_interceptString.at(i - 1) == ','))
        {
            break;
        }
        xv_interceptString.chop(1);
    }
}
//=========================================================
qreal* X_Calibration::xh_termFactorPointer(int termIndex) const
{
    if (termIndex < 0 || termIndex >= xv_termList.count())
    {
        return 0;
    }

    return xv_termList.value(termIndex)->xh_termFactorPointer();
}
//=========================================================
int X_Calibration::xh_findNextTermIndex(X_AbstractTerm::TermType termType) const
{
    if (termType == X_AbstractTerm::TT_SIMPLE)
    {
        for (int t = 0; t < xv_termList.count(); t++)
        {
            if (xv_termList.at(t)->xp_termType() != X_AbstractTerm::TT_SIMPLE)
            {
                return t;
            }
        }
    }
    else if (termType == X_AbstractTerm::TT_QUADRATIC)
    {
        for (int t = 0; t < xv_termList.count(); t++)
        {
            if (xv_termList.at(t)->xp_termType() == X_AbstractTerm::TT_SIMPLE
                || xv_termList.at(t)->xp_termType() == X_AbstractTerm::TT_QUADRATIC)
            {
                continue;
            }

            return t;
        }
    }
    else if (termType == X_AbstractTerm::TT_MIXED)
    {
        for (int t = 0; t < xv_termList.count(); t++)
        {
            if (xv_termList.at(t)->xp_termType() == X_AbstractTerm::TT_SIMPLE
                || xv_termList.at(t)->xp_termType() == X_AbstractTerm::TT_QUADRATIC
                || xv_termList.at(t)->xp_termType() == X_AbstractTerm::TT_MIXED)
            {
                continue;
            }

            return t;
        }
    }
    else if (termType == X_AbstractTerm::TT_CUSTOM)
    {
        for (int t = 0; t < xv_termList.count(); t++)
        {
            if (xv_termList.at(t)->xp_termType() == X_AbstractTerm::TT_SIMPLE
                || xv_termList.at(t)->xp_termType() == X_AbstractTerm::TT_QUADRATIC
                || xv_termList.at(t)->xp_termType() == X_AbstractTerm::TT_MIXED
                || xv_termList.at(t)->xp_termType() == X_AbstractTerm::TT_CUSTOM)
            {
                continue;
            }

            return t;
        }
    }

    return xv_termList.count();
}
//=========================================================
bool X_Calibration::xp_termFactor(int termIndex, qreal& factor) const
{
    if (termIndex < 0 || termIndex >= xv_termList.count())
    {
        factor = 0;
        return false;
    }

    factor = xv_termList.at(termIndex)->xp_termFactor();
    return true;
}
//=========================================================
QList<int> X_Calibration::xp_termWindowIndexList(int termIndex) const
{
    QList<int> indexList;
    if (termIndex < 0 || termIndex >= xv_termList.count())
    {
        return indexList;
    }

    QList<qint64> windowsIdList = xv_termList.at(termIndex)->xp_termWindowIdList();

    for (int i = 0; i < windowsIdList.count(); i++)
    {
        for (int w = 0; w < xv_windowList.count(); w++)
        {
            if (xv_windowList.at(w)->xp_windowId() == windowsIdList.at(i))
            {
                indexList.append(w);
                break;
            }
        }
    }

    return indexList;
}
//=========================================================
QMap<QString, int> X_Calibration::xp_termWindowIndexMap(int termIndex) const
{
    QMap<QString, int> indexMap;
    if (termIndex < 0 || termIndex >= xv_termList.count())
    {
        return indexMap;
    }

    QList<qint64> windowsIdList = xv_termList.at(termIndex)->xp_termWindowIdList();

    for (int i = 0; i < windowsIdList.count(); i++)
    {
        for (int w = 0; w < xv_windowList.count(); w++)
        {
            if (xv_windowList.at(w)->xp_windowId() == windowsIdList.at(i))
            {
                //              indexList.append(w);
                break;
            }
        }
    }

    return indexMap;
}
//=========================================================
QStringList X_Calibration::xp_termWindowsNameList(int termIndex) const
{
    QList<int> termIndexList = xp_termWindowIndexList(termIndex);
    if (termIndexList.isEmpty())
    {
        return QStringList();
    }

    QStringList termWindowsNameList;
    for (int w = 0; w < termIndexList.count(); w++)
    {
        termWindowsNameList.append(xv_windowList.at(termIndexList.at(w))->xp_windowName());
    }

    return termWindowsNameList;
}
//=========================================================
QString X_Calibration::xp_termFactorString(int termIndex) const
{
    if (termIndex < 0 || termIndex >= xv_termList.count())
    {
        return "0.0";
    }

    return xv_termList.at(termIndex)->xp_termFactorString();
}
//=========================================================
bool X_Calibration::xp_termFactorString(int termIndex, QString& factorString) const
{
    if (termIndex < 0 || termIndex >= xv_termList.count())
    {
        factorString = "0.0";
        return false;
    }

    factorString = xv_termList.at(termIndex)->xp_termFactorString();
    return true;
}
//=========================================================
bool X_Calibration::xp_setTermFactorString(int termIndex, const QString& factorString)
{
    if (termIndex < 0 || termIndex >= xv_termList.count())
    {
        return false;
    }

    if (xv_termList.at(termIndex)->xh_setTermFactor(factorString))
    {
        emit xg_termOperation(TOT_TERM_FACTOR_CHANGED, termIndex, termIndex);
        xv_dirty = true;
        emit xg_dirtyChanged(xv_dirty);
        xv_dateTime = QDateTime::currentDateTime();
        return true;
    }

    return false;
}
//=========================================================
bool X_Calibration::xp_termVariablePart(int termIndex,
                                       const X_AbstractSpectrum* spectrum,
                                       qreal& value)
{
    if (termIndex < 0 || termIndex >= xv_termList.count() || !spectrum)
    {
        return false;
    }

    value = 0.0;
    bool res = xv_termList.at(termIndex)->xp_calcTermIntensity(spectrum, value);
    if (res)
    {
        res = xv_termNormalizer->xp_normalizeValue(spectrum, value);
    }

    return res;
}
//=========================================================
bool X_Calibration::xp_calcBaseTermValue(const X_AbstractSpectrum* spectrum, qreal& value) const
{
    value = 0.0;
    if (xv_baseTermId < 0)
    {
        return false;
    }

    //qreal baseTermValue = std::numeric_limits<double>::quiet_NaN();
    //qreal baseNormaValue = std::numeric_limits<double>::quiet_NaN();

    for (int t = 0; t < xv_termList.count(); t++)
    {
        if (xv_termList.at(t)->xp_termId() == xv_baseTermId)
        {
            if (!xv_termList.at(t)->xp_calcTermIntensity(spectrum, value))
            {
                return false;
            }
            // baseTerm - ok

            if (!xv_baseTermNormalizer->xp_normalizeValue(spectrum, value))
            {
                return false;
            }

            break;
        }
    }

    return true;
}
//=========================================================
bool X_Calibration::xp_createMixedTerms(int termIndex)
{
    if (termIndex < 0 || termIndex >= xv_termList.count())
    {
        return false;
    }

    if (xv_termList.at(termIndex)->xp_termType() != X_AbstractTerm::TT_SIMPLE)
    {
        return false;
    }

    //  get window from simple term
    X_SimpleTerm* baseTerm = qobject_cast<X_SimpleTerm*>(xv_termList.at(termIndex));
    if (!baseTerm)
    {
        return false;
    }

    const X_CalibrationWindow* baseWindow = baseTerm->xp_window();
    X_MixedTerm* mixedTerm;
    QList<X_AbstractTerm*> mixedTermList;
    for (int w = 0; w < xv_windowList.count(); w++)
    {
        if (xv_windowList.at(w) == baseWindow
            || xv_windowList.at(w)->xp_windowType() != X_CalibrationWindow::WT_PEAK)
        {
            continue;
        }

        // check existing mixed terms
        bool termExists = false;
        for (int t = 0; t < xv_termList.count(); t++)
        {
            if (xv_termList.at(t)->xp_termType() != X_AbstractTerm::TT_MIXED)
            {
                continue;
            }

            if (xv_termList.at(t)->xp_termBelongsToWindow(baseWindow)
                && xv_termList.at(t)->xp_termBelongsToWindow(xv_windowList.at(w)))
            {
                termExists = true;
                break;
            }
        }

        if (termExists)
        {
            continue;
        }

        // create mixed term
        mixedTerm = new X_MixedTerm(baseWindow, xv_windowList.at(w), this);
        mixedTermList.append(mixedTerm);
    }

    if (mixedTermList.isEmpty())
    {
        return false;
    }

    // define first custom term
    int t = 0;
    for (; t < xv_termList.count(); t++)
    {
        if (xv_termList.at(t)->xp_termType() == X_AbstractTerm::TT_CUSTOM)
        {
            break;
        }
    }

    // insert new term
    if (mixedTermList.isEmpty())
    {
        return false;
    }

    emit xg_termOperation(TOT_BEGIN_INSERT_TERM, t, t + mixedTermList.count() - 1);
    for (int i = 0; i < mixedTermList.count(); i++)
    {
        xv_termList.insert(t + i, mixedTermList.at(i));
    }
    emit xg_termOperation(TOT_END_INSERT_TERM, t, t + mixedTermList.count() - 1);
    xv_dirty = true;
    xv_dateTime = QDateTime::currentDateTime();
    emit xg_dirtyChanged(xv_dirty);

    return true;
}
//=========================================================
bool X_Calibration::xp_removeMixedTerms()
{
    for (int t = xv_termList.count() - 1; t >= 0; t--)
    {
        if (xv_termList.at(t)->xp_termType() != X_AbstractTerm::TT_MIXED)
        {
            continue;
        }

        emit xg_termOperation(TOT_BEGIN_REMOVE_TERM, t, t);
        xv_termList.takeAt(t)->deleteLater();
        emit xg_termOperation(TOT_END_REMOVE_TERM, t, t);
        xv_dirty = true;
        xv_dateTime = QDateTime::currentDateTime();
        emit xg_dirtyChanged(xv_dirty);
    }

    return true;
}
//=========================================================
bool X_Calibration::xp_removeCustomTerm(int termIndex)
{
    if (termIndex < 0 || termIndex >= xv_termList.count())
    {
        return false;
    }

    X_AbstractTerm* term = xv_termList.value(termIndex, nullptr);
    if (!term || term->xp_termType() != X_AbstractTerm::TT_CUSTOM)
    {
        return false;
    }

    emit xg_termOperation(TOT_BEGIN_REMOVE_TERM, termIndex, termIndex);
    xv_termList.takeAt(termIndex)->deleteLater();
    emit xg_termOperation(TOT_END_REMOVE_TERM, termIndex, termIndex);
    xv_dirty = true;
    xv_dateTime = QDateTime::currentDateTime();
    emit xg_dirtyChanged(xv_dirty);
    return true;
}
//=========================================================
void X_Calibration::xp_updateCustomTerm(bool& res, X_RawCustomTerm& rawTerm)
{
    if (rawTerm.termType != X_AbstractTerm::TT_CUSTOM)
    {
        res = false;
        emit xg_message(tr("Updated term type is not \"Custom\"!"));
        return;
    }

    if (!xh_checkCustomString(rawTerm))
    {
        res = false;
        return;
    }

    // create a new term
    int id = xp_applyRawTerm(rawTerm);
    res = id < 0 ? false : true;
    rawTerm.termId = id;
}
//=========================================================
void X_Calibration::xp_calcWindowIntensity(const QString& windowName,
                                          const QObject* spectrumObject,
                                          qreal& intensityValue,
                                          bool useBuffer,
                                          bool* ok)
{
    if (windowName.isEmpty())
    {
        if (ok)
        {
            *ok = false;
        }
    }

    for (auto window : xv_windowList)
    {
        if (window->xp_windowName() == windowName)
        {
            window->xp_calcWindowIntensity(spectrumObject, intensityValue, useBuffer, ok);
            return;
        }
    }

    if (ok)
    {
        *ok = false;
    }
}
//=========================================================
int X_Calibration::xp_createCustomTerm(X_AbstractTerm::TermState termState,
                                      const QString& name,
                                      const QString& equation,
                                      const QString& description,
                                      const QString& termFactor)
{
    // define next term index
    int termIndex = xh_findNextTermIndex(X_AbstractTerm::TT_CUSTOM);

    X_CustomTerm* customTerm = new X_CustomTerm(this);
    customTerm->xp_setTermState(termState);
    customTerm->xp_setName(name);
    customTerm->xp_setExpression(equation);
    customTerm->xp_setDescription(description);
    customTerm->xh_setTermFactor(termFactor);

    connect(customTerm,
            &X_CustomTerm::xg_calcWindowIntensity,
            this,
            &X_Calibration::xp_calcWindowIntensity);
    connect(customTerm,
            &X_CustomTerm::xg_errorReport,
            this,
            &X_Calibration::xg_message);

    emit xg_termOperation(TOT_BEGIN_INSERT_TERM, termIndex, termIndex);
    xv_termList.insert(termIndex, customTerm);
    emit xg_termOperation(TOT_END_INSERT_TERM, termIndex, termIndex);
    xv_dirty = true;
    xv_dateTime = QDateTime::currentDateTime();
    emit xg_dirtyChanged(xv_dirty);

    return termIndex;
}
//=========================================================
int X_Calibration::xp_createTerm(QList<int>& windowIndexList,
                                X_AbstractTerm::TermType termType,
                                X_AbstractTerm::TermState termState,
                                const QString& termFactor)
{
    // define next term index
    int termIndex = xh_findNextTermIndex(termType);
    X_AbstractTerm* term = nullptr;
    if (termType == X_AbstractTerm::TT_SIMPLE)
    {
        if (windowIndexList.isEmpty())
        {
            return -1;
        }
        X_CalibrationWindow* window = xv_windowList.value(windowIndexList.at(0));
        if (window->xp_windowType() != X_CalibrationWindow::WT_PEAK)
        {
            return -1;
        }

        term = new X_SimpleTerm(window, this);
        term->xp_setTermState(termState);
        term->xh_setTermFactor(termFactor);
        //        emit xg_termOperation(TOT_BEGIN_INSERT_TERM, termIndex, termIndex);
        //        xv_termList.insert(termIndex, simpleTerm);
        //        emit xg_termOperation(TOT_END_INSERT_TERM, termIndex, termIndex);
    }
    else if (termType == X_AbstractTerm::TT_QUADRATIC)
    {
        if (windowIndexList.isEmpty())
        {
            return -1;
        }
        X_CalibrationWindow* window = xv_windowList.value(windowIndexList.at(0));
        if (window->xp_windowType() != X_CalibrationWindow::WT_PEAK)
        {
            return -1;
        }

        term = new X_QuadraticTerm(window, this);
        term->xp_setTermState(termState);
        term->xh_setTermFactor(termFactor);
    }
    else if (termType == X_AbstractTerm::TT_MIXED)
    {
        if (windowIndexList.count() != 2)
        {
            return -1;
        }

        X_CalibrationWindow* window1 = xv_windowList.value(windowIndexList.at(0));
        if (window1->xp_windowType() != X_CalibrationWindow::WT_PEAK)
        {
            return -1;
        }
        X_CalibrationWindow* window2 = xv_windowList.value(windowIndexList.at(1));
        if (window2->xp_windowType() != X_CalibrationWindow::WT_PEAK)
        {
            return -1;
        }
        term = new X_MixedTerm(window1, window2, this);
        term->xp_setTermState(termState);
        term->xh_setTermFactor(termFactor);
    }
    else if (termType == X_AbstractTerm::TT_NOT_DEFINED)
    {
        return -1;
    }

    emit xg_termOperation(TOT_BEGIN_INSERT_TERM, termIndex, termIndex);
    xv_termList.insert(termIndex, term);
    emit xg_termOperation(TOT_END_INSERT_TERM, termIndex, termIndex);
    xv_dirty = true;
    xv_dateTime = QDateTime::currentDateTime();
    emit xg_dirtyChanged(xv_dirty);

    return termIndex;
}
//=========================================================
int X_Calibration::xp_applyRawTerm(X_RawTerm& rawTerm)
{
    int termIndex = -1;
    if (rawTerm.termType == X_AbstractTerm::TT_CUSTOM)
    {
        X_RawCustomTerm* customTerm = static_cast<X_RawCustomTerm*>(&rawTerm);
        if (customTerm->termId >= 0)
        {
            termIndex = xh_termIndexForTermId(customTerm->termId);
        }
    }

    if (termIndex < 0)
    {
        termIndex = xh_termIndexForRawTerm(rawTerm);
    }

    if (termIndex >= 0)
    {
        xh_updateExistingTerm(termIndex, rawTerm);
        return termIndex;
    }
    else
    {
        // new term
        termIndex = xh_createNewTerm(rawTerm);
    }

    return termIndex;
}
//=========================================================
int X_Calibration::xh_createNewTerm(X_RawTerm& rawTerm)
{
    if (rawTerm.termType == X_AbstractTerm::TT_CUSTOM)
    {
        X_RawCustomTerm* rawCustomTerm = static_cast<X_RawCustomTerm*>(&rawTerm);

        return xp_createCustomTerm(rawCustomTerm->termState,
                                   rawCustomTerm->name,
                                   rawCustomTerm->customString,
                                   rawCustomTerm->descriptionString,
                                   rawCustomTerm->factor);
    }
    else
    {
        QList<int> windowIndexList;

        if (!xh_createWindowIndexList(windowIndexList, rawTerm))
        {
            return -1;
        }
        return xp_createTerm(windowIndexList, rawTerm.termType, rawTerm.termState, rawTerm.factor);
    }
}
//=========================================================
bool X_Calibration::xh_createWindowIndexList(QMap<QString, int>& windowIndexMap, X_RawTerm& rawTerm)
{
    if (!xh_checkCustomString(rawTerm))
    {
        return false;
    }

    // else
    //{
    int index;
    foreach (QString windowName, rawTerm.windowList)
    {
        index = xp_windowIndexForName(windowName);
        if (index < 0 || index >= xv_windowList.count())
        {
            // TODO error msg
            continue;
        }
        windowIndexMap.insert(windowName, index);
    }
    //}

    return true;
}
//=========================================================
bool X_Calibration::xh_checkCustomString(X_RawTerm& rawTerm)
{
    if (rawTerm.termType == X_AbstractTerm::TT_CUSTOM)
    {
        X_MathExpressionHandler mathExpressionHandler;
        X_MathExpressionVariableListMaker mathExpressionVariableListMaker;
        connect(&mathExpressionHandler,
                &X_MathExpressionHandler::xg_requestVariableValue,
                &mathExpressionVariableListMaker,
                &X_MathExpressionVariableListMaker::xp_insertVariableValue);
        connect(&mathExpressionHandler,
                &X_MathExpressionHandler::xg_errorReport,
                &mathExpressionVariableListMaker,
                &X_MathExpressionVariableListMaker::xp_setError);
        connect(&mathExpressionVariableListMaker,
                &X_MathExpressionVariableListMaker::xg_variableCheckRequest,
                this,
                &X_Calibration::xh_windowIsExist);

        double res;
        if (!mathExpressionHandler.xp_calculateExpression(rawTerm.customString, res))
        {
            QString error;
            int errorStartPos;
            int errorEndPos;
            mathExpressionVariableListMaker.xp_error(error, errorStartPos, errorEndPos);

            emit xg_message(
                error
                + tr(" (position %1)")
                      .arg(QString::number(errorStartPos), QString::number(errorStartPos)));
            return false;
        }

        return true;
    }

    return false;
}
//=========================================================
bool X_Calibration::xh_createWindowIndexList(QList<int>& windowIndexList, X_RawTerm& rawTerm)
{
    if (!xh_checkCustomString(rawTerm))
    {
        return false;
    }

    // else
    //{
    int index;
    foreach (QString windowName, rawTerm.windowList)
    {
        index = xp_windowIndexForName(windowName);
        if (index < 0 || index >= xv_windowList.count())
        {
            // TODO error msg
            continue;
        }
        windowIndexList.append(index);
    }
    //}

    return true;
}
//=========================================================
bool X_Calibration::xh_updateExistingTerm(int termIndex, X_RawTerm& rawTerm)
{
    bool res = false;
    if (rawTerm.termType == X_AbstractTerm::TT_CUSTOM)
    {
        X_RawCustomTerm* rawCustomTerm = static_cast<X_RawCustomTerm*>(&rawTerm);

        X_CustomTerm* customTerm = static_cast<X_CustomTerm*>(xv_termList[termIndex]);
        customTerm->xp_setName(rawCustomTerm->name);
        customTerm->xp_setExpression(rawCustomTerm->customString);
        customTerm->xp_setDescription(rawCustomTerm->descriptionString);
        res = true;

        xv_dirty = true;
        xv_dateTime = QDateTime::currentDateTime();
        emit xg_dirtyChanged(xv_dirty);
    }
    else
    {
        xv_termList[termIndex]->xp_setTermState(rawTerm.termState);
        xv_termList[termIndex]->xh_setTermFactor(rawTerm.factor);
        res = true;
    }



    return res;
}
//=========================================================
int X_Calibration::xh_termIndexForRawTerm(const X_RawTerm& rawTerm) const
{
    // check out is the term already exist
    for (int t = 0; t < xv_termList.count(); t++)
    {
        if (rawTerm.name == xv_termList.at(t)->xp_termName()
            && rawTerm.termType == xv_termList.at(t)->xp_termType())
        {
            //            QList<qint64> termWindowIdList = xv_termList.at(t)->xp_termWindowIdList();
            //            if (termWindowIdList.count() == rawTerm.windowList.count())
            //            {
            //                bool res = true;
            //                foreach (qint64 id, termWindowIdList)
            //                {
            //                    if (!rawTerm.windowList.contains(xp_calibrationWindowNameForId(id)))
            //                    {
            //                        res = false;
            //                        break;
            //                    }
            //                }

            //                // term exists.
            //                if (res)
            //                {
            //                    return t;
            //                }
            //            }
            return t;
        }
    }

    return -1;
}
//=========================================================
int X_Calibration::xh_termIndexForTermId(qint64 termId) const
{
    for (int t = 0; t < xv_termList.count(); t++)
    {
        if (xv_termList.at(t)->xp_termId() == termId)
        {
            return t;
        }
    }

    return -1;
}
//=========================================================
X_TermNormalizer::NormaType X_Calibration::xp_normaType() const
{
    return xv_termNormalizer->xp_normaType();
}
//=========================================================
bool X_Calibration::xp_setNormaType(X_TermNormalizer::NormaType type)
{
    bool res = xv_termNormalizer->xp_setNormaType(type);
    if (res)
    {
        xv_dirty = true;
        xv_dateTime = QDateTime::currentDateTime();
        emit xg_dirtyChanged(xv_dirty);
    }
    return res;
}
//=========================================================
bool X_Calibration::xp_setNormaCustomString(const QString& customString)
{
    bool res = xv_termNormalizer->xp_setCustomNormaString(customString);
    if (res)
    {
        xv_dirty = true;
        xv_dateTime = QDateTime::currentDateTime();
        emit xg_dirtyChanged(xv_dirty);
    }
    return res;
}
//=========================================================
QString X_Calibration::xp_normaCustomString() const
{
    return xv_termNormalizer->xp_normaCustomString();
}
//=========================================================
X_TermNormalizer::NormaType X_Calibration::xp_baseTermNormaType() const
{
    return xv_baseTermNormalizer->xp_normaType();
}
//=========================================================
bool X_Calibration::xp_setBaseTermNormaType(X_TermNormalizer::NormaType type)
{
    bool res = xv_baseTermNormalizer->xp_setNormaType(type);
    if (res)
    {
        xv_dirty = true;
        xv_dateTime = QDateTime::currentDateTime();
        emit xg_dirtyChanged(xv_dirty);
    }
    return res;
}
//=========================================================
bool X_Calibration::xp_setBaseTermNormaCustomString(const QString& customString)
{
    bool res = xv_baseTermNormalizer->xp_setCustomNormaString(customString);
    if (res)
    {
        xv_dirty = true;
        xv_dateTime = QDateTime::currentDateTime();
        emit xg_dirtyChanged(xv_dirty);
    }
    return res;
}
//=========================================================
QString X_Calibration::xp_baseTermNormaCustomString() const
{
    return xv_baseTermNormalizer->xp_normaCustomString();
}
//=========================================================
bool X_Calibration::xp_setBaseTermNormalizerParameters(X_TermNormalizer::NormaType type,
                                                      const QString& customString)
{
    bool res = xv_baseTermNormalizer->xp_setNormalizerParameters(type, customString);
    if (res)
    {
        xv_dirty = true;
        xv_dateTime = QDateTime::currentDateTime();
        emit xg_dirtyChanged(xv_dirty);
    }
    return res;
}
//=========================================================
X_Calibration::EquationType X_Calibration::xp_equationType() const
{
    return xv_equationType;
}
//=========================================================
bool X_Calibration::xp_setEquationType(X_Calibration::EquationType type)
{
    if (xv_equationType == type)
    {
        return false;
    }
    xv_equationType = type;
    if (xv_equationType == ET_POLYNOMIAL)
    {
        xv_baseTermId = -1;
    }

    xv_dateTime = QDateTime::currentDateTime();
    xv_dirty = true;
    emit xg_dirtyChanged(xv_dirty);

    return true;
}
//=========================================================
qreal X_Calibration::xp_equationIntercept() const
{
    return xv_intercept;
}
//=========================================================
bool X_Calibration::xp_setEquationIntercept(qreal value)
{
    if (xv_intercept == value)
    {
        return false;
    }

    xv_intercept = value;
    xv_interceptString = QString::number(value, 'f', 15);
    xh_chopTailX_eroesFromInterceptString();
    emit xg_interceptChanged();
    xv_dateTime = QDateTime::currentDateTime();
    xv_dirty = true;
    emit xg_dirtyChanged(xv_dirty);

    return true;
}
//=========================================================
QString X_Calibration::xp_equationInterceptString() const
{
    return xv_interceptString;
}
//=========================================================
bool X_Calibration::xp_setEquationInterceptString(const QString& interceptString)
{
    bool ok;
    qreal intercept =  X_LocaleDoubleConverter::toDouble(interceptString, &ok);

    if (!ok)
    {
        return false;
    }

    xv_dirty = true;
    xv_dateTime = QDateTime::currentDateTime();
    emit xg_dirtyChanged(xv_dirty);
    bool res = xp_setEquationIntercept(intercept);
    if (res)
    {
        xv_dirty = true;
        emit xg_dirtyChanged(xv_dirty);
    }
    return res;
}
//=========================================================
QString X_Calibration::xp_baseTermDisplayString() const
{
    if (xv_baseTermId < 0)
    {
        return QString();
    }

    QString baseTermString;
    for (int t = 0; t < xv_termList.count(); t++)
    {
        if (xv_termList.at(t)->xp_termId() == xv_baseTermId)
        {
            baseTermString = xv_termList.at(t)->xp_termName();
            break;
        }
    }

    if (xv_baseTermNormalizer->xp_normaType() != X_TermNormalizer::NT_NONE)
    {
        QString normaTypeString = X_TermNormalizer::xp_normaTypeString(
            xv_baseTermNormalizer->xp_normaType());

        if (xv_baseTermNormalizer->xp_normaType() == X_TermNormalizer::NT_COHERENT_INCOHERENT
            || xv_baseTermNormalizer->xp_normaType() == X_TermNormalizer::NT_INCOHERENT_COHERENT)
        {
            normaTypeString = "(" + normaTypeString + ")";
        }

        baseTermString = baseTermString + "/" + normaTypeString;
    }

    return baseTermString;
}
//=========================================================
QString X_Calibration::xp_baseTermString() const
{
    if (xv_baseTermId < 0)
    {
        return QString();
    }

    QString baseTermString;
    for (int t = 0; t < xv_termList.count(); t++)
    {
        if (xv_termList.at(t)->xp_termId() == xv_baseTermId)
        {
            baseTermString = xv_termList.at(t)->xp_termName();
            break;
        }
    }

    return baseTermString;
}
//=========================================================
qint64 X_Calibration::xp_baseTermId() const
{
    return xv_baseTermId;
}
//=========================================================
bool X_Calibration::xp_setBaseTermId(qint64 id)
{
    if (xv_equationType == ET_POLYNOMIAL)
    {
        xv_baseTermId = -1;
        return true;
    }

    if (xv_baseTermId == id)
    {
        return false;
    }

    for (int i = 0; i < xv_termList.count(); i++)
    {
        if (xv_termList.at(i)->xp_termId() == id)
        {
            xv_baseTermId = id;
            xv_dirty = true;
            emit xg_dirtyChanged(xv_dirty);
            return true;
        }
    }
    xv_baseTermId = -1;
    xv_dirty = true;
    xv_dateTime = QDateTime::currentDateTime();
    emit xg_dirtyChanged(xv_dirty);

    return true;
}
//=========================================================
int X_Calibration::xp_baseTermIndex() const
{
    if (xv_baseTermId < 0)
    {
        return -1;
    }

    for (int t = 0; t < xv_termList.count(); t++)
    {
        if (xv_termList.at(t)->xp_termId() == xv_baseTermId)
        {
            return t;
        }
    }
    return -1;
}
//=========================================================
bool X_Calibration::xp_setBaseTermFromName(const QString& baseTermName)
{
    for (int t = 0; t < xv_termList.count(); t++)
    {
        if (xv_termList.at(t)->xp_termName() == baseTermName)
        {
            xv_baseTermId = xv_termList.at(t)->xp_termId();
            return true;
        }
    }

    return false;
}
//=========================================================
void X_Calibration::xp_createEquationDataForEquationRecalc(QMap<int, qreal*>& factorMap,
                                                          qreal*& freeTermPtr)
{
    X_AbstractTerm::TermState termState;
    for (int t = 0; t < xv_termList.count(); t++)
    {
        if (xv_equationType == ET_FRACTIONAL && !xv_useBaseTermInFractionalEquation
            && xv_termList.value(t)->xv_termId == xv_baseTermId)
        {
            continue;
        }

        termState = xv_termList.value(t)->xv_termState;
        if (termState == X_AbstractTerm::TS_CONST_INCLUDED || termState == X_AbstractTerm::TS_INCLUDED)
        {
            factorMap.insert(t, xv_termList.value(t)->xh_termFactorPointer());
        }
    }

    freeTermPtr = &xv_intercept;
}
//=========================================================
void X_Calibration::xh_notifyCalibrationRecalc()
{
    // conform free term string to value

    xv_interceptString = X_QrealToStringConverter::xp_convert(xv_intercept);

    // conform equation terms
    for (int t = 0; t < xv_termList.count(); t++)
    {
        xv_termList.value(t)->xh_conformStringWithValue();
    }
    emit xg_termOperation(TOT_TERM_FACTOR_CHANGED, 0, xv_termList.count() - 1);
    xv_dirty = true;
    emit xg_dirtyChanged(xv_dirty);
    xv_dateTime = QDateTime::currentDateTime();
    emit xg_interceptChanged();
}
//=========================================================
void X_Calibration::xp_resetEquationTerms()
{
    bool res = false;
    for (int t = 0; t < xv_termList.count(); t++)
    {
        if (xv_termList[t]->xv_termFactor != 0.0)
        {
            res = res || true;
        }
        xv_termList[t]->xv_termFactor = 0.0;
        xv_termList[t]->xh_conformStringWithValue();
    }

    emit xg_termOperation(TOT_TERM_FACTOR_CHANGED, 0, xv_termList.count() - 1);
    res = res && xp_setEquationIntercept(0.0);

    if (res)
    {
        xv_dirty = true;
        emit xg_dirtyChanged(xv_dirty);
        xv_dateTime = QDateTime::currentDateTime();
    }
}
//=========================================================
X_CalibrationQualityData X_Calibration::xp_calibrationQualityData() const
{
    return xv_calibrationQualityData;
}
//=========================================================
void X_Calibration::xp_setCalibrationQualityData(X_CalibrationQualityData qualityData)
{
    xv_calibrationQualityData = qualityData;
}
//=========================================================
void X_Calibration::xp_setDetermination(const QString& determination)
{
    xv_calibrationQualityData.determination = determination;
}
//=========================================================
void X_Calibration::xp_setAdjustedDetermination(const QString& adjustedDetermination)
{
    xv_calibrationQualityData.adj_determination = adjustedDetermination;
}
//=========================================================
void X_Calibration::xp_setStandardDeviation(const QString& standardDeviation)
{
    xv_calibrationQualityData.standardDeviation = standardDeviation;
}
//=========================================================
void X_Calibration::xp_setCurrentDateTime()
{
    xv_dateTime = QDateTime::currentDateTime();
}
//=========================================================
void X_Calibration::xp_setDateTime(QDateTime dateTime)
{
    xv_dateTime = dateTime;
}
//=========================================================
void X_Calibration::xh_onTermNameChange()
{
    X_AbstractTerm* term = qobject_cast<X_AbstractTerm*>(sender());
    if (term == 0)
    {
        return;
    }

    int termIndex = xh_termIndex(term);

    emit xg_termOperation(TOT_TERM_NAME_CHANGED, termIndex, termIndex);
    xv_dateTime = QDateTime::currentDateTime();
    xv_dirty = true;
    emit xg_dirtyChanged(xv_dirty);
}
//=========================================================
void X_Calibration::xh_onTermWindowMarginChange()
{
    X_AbstractTerm* term = qobject_cast<X_AbstractTerm*>(sender());
    if (!term)
    {
        return;
    }
    int termIndex = xh_termIndex(term);
    emit xg_termOperation(TOT_TERM_WINDOW_MARGIN_CHANGED, termIndex, termIndex);
    xv_dirty = true;
    xv_dateTime = QDateTime::currentDateTime();
    emit xg_dirtyChanged(xv_dirty);
}
//=========================================================
void X_Calibration::xh_removeTerm(X_AbstractTerm* term)
{
    for (int i = 0; i < xv_termList.count(); i++)
    {
        if (xv_termList.at(i) == term)
        {
            emit xg_termOperation(TOT_BEGIN_REMOVE_TERM, i, i);
            xv_termList.takeAt(i)->deleteLater();
            emit xg_termOperation(TOT_END_REMOVE_TERM, i, i);
            xv_dirty = true;
            xv_dateTime = QDateTime::currentDateTime();
            emit xg_dirtyChanged(xv_dirty);
            return;
        }
    }
}
//=========================================================
void X_Calibration::xh_onNormalizerChange()
{
    if (sender() == xv_termNormalizer)
    {
        emit xg_normalizerChanged();
        xv_dirty = true;
        emit xg_dirtyChanged(xv_dirty);
    }
    else if (sender() == xv_baseTermNormalizer)
    {
        // TODO create signal base term changed and emit
        xv_dirty = true;
        xv_dateTime = QDateTime::currentDateTime();
        emit xg_dirtyChanged(xv_dirty);
    }
}
//=========================================================
bool X_Calibration::xh_isWindowExist(const QString& windowName)
{
    for (int i = 0; i < xv_windowList.count(); i++)
    {
        if (xv_windowList.at(i)->xp_windowName() == windowName)
        {
            return true;
        }
    }

    return false;
}
//=========================================================
bool X_Calibration::xp_isCalibrationWindowVisible(int windowIndex) const
{
    if (windowIndex < 0 || windowIndex >= xv_windowList.count())
    {
        return false;
    }

    return xv_windowList.at(windowIndex)->xp_isWindowVisible();
}
//=========================================================
bool X_Calibration::xp_setCalibrationWindowVisible(int windowIndex, bool visibility)
{
    if (windowIndex < 0 || windowIndex >= xv_windowList.count())
    {
        return false;
    }

    bool res = xv_windowList[windowIndex]->xp_setWindowVisible(visibility);
    if (res)
    {
        emit xg_windowOperation(WOT_WINDOW_VISIBLE_CHANGED, windowIndex, windowIndex);
    }
    return res;
}
//=========================================================
int X_Calibration::xp_calibrationWindowCount() const
{
    return xv_windowList.count();
}
//=========================================================
QString X_Calibration::xp_calibrationWindowName(int windowIndex) const
{
    if (windowIndex < 0 || windowIndex >= xv_windowList.count())
    {
        return QString();
    }

    return xv_windowList.at(windowIndex)->xp_windowName();
}
//=========================================================
const X_CalibrationWindow* X_Calibration::xp_calibrationWindow(int windowIndex) const
{
    if (windowIndex < 0 || windowIndex >= xv_windowList.count())
    {
        return 0;
    }

    return xv_windowList.at(windowIndex);
}
//=========================================================
QString X_Calibration::xp_calibrationWindowNameForId(qint64 windowId) const
{
    for (int w = 0; w < xv_windowList.count(); w++)
    {
        if (xv_windowList.at(w)->xp_windowId() == windowId)
        {
            return xv_windowList.at(w)->xp_windowName();
        }
    }

    return QString();
}
//=========================================================
bool X_Calibration::xp_setCalibrationWindowName(int windowIndex, const QString& name)
{
    if (windowIndex < 0 || windowIndex >= xv_windowList.count())
    {
        return false;
    }

    bool res = xv_windowList[windowIndex]->xp_setWindowName(name);
    if (res)
    {
        emit xg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
        xv_dateTime = QDateTime::currentDateTime();
        xv_dirty = true;
        emit xg_dirtyChanged(xv_dirty);
    }
    return res;
}
//=========================================================
X_CalibrationWindow::WindowType X_Calibration::xp_calibrationWindowType(int windowIndex) const
{
    if (windowIndex < 0 || windowIndex >= xv_windowList.count())
    {
        return X_CalibrationWindow::WT_NOT_DEFINED;
    }

    return xv_windowList.at(windowIndex)->xp_windowType();
}
//=========================================================
bool X_Calibration::xp_setCalibrationWindowType(int windowIndex, X_CalibrationWindow::WindowType type)
{
    if (windowIndex < 0 || windowIndex >= xv_windowList.count())
    {
        return false;
    }

    bool res = xv_windowList[windowIndex]->xp_setWindowType(type);
    if (res)
    {
        emit xg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
        if (type == X_CalibrationWindow::WT_PEAK)
        {
            xh_createTermsForWindow(xv_windowList.at(windowIndex));
        }
        xv_dateTime = QDateTime::currentDateTime();
        xv_dirty = true;
        emit xg_dirtyChanged(xv_dirty);
    }
    return res;
}
//=========================================================
int X_Calibration::xp_calibrationWindowFirstChannel(int windowIndex) const
{
    if (windowIndex < 0 || windowIndex >= xv_windowList.count())
    {
        return 0;
    }

    return xv_windowList.at(windowIndex)->xp_firstChannel();
}
//=========================================================
bool X_Calibration::xp_setCalibrationWindowFirstChannel(int windowIndex, int channel)
{
    if (windowIndex < 0 || windowIndex >= xv_windowList.count())
    {
        return 0;
    }

    bool res = xv_windowList[windowIndex]->xp_setWindowFirstChannel(channel);
    if (res)
    {
        emit xg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
        xv_dateTime = QDateTime::currentDateTime();
        xv_dirty = true;
        emit xg_dirtyChanged(xv_dirty);
    }
    return res;
}
//=========================================================
int X_Calibration::xp_calibrationWindowLastChannel(int windowIndex) const
{
    if (windowIndex < 0 || windowIndex >= xv_windowList.count())
    {
        return 0;
    }

    return xv_windowList.at(windowIndex)->xp_lastChannel();
}
//=========================================================
bool X_Calibration::xp_setCalibrationWindowLastChannel(int windowIndex, int channel)
{
    if (windowIndex < 0 || windowIndex >= xv_windowList.count())
    {
        return 0;
    }

    bool res = xv_windowList[windowIndex]->xp_setWindowLastChannel(channel);
    if (res)
    {
        emit xg_windowOperation(WOT_WINDOW_CHANGED, windowIndex, windowIndex);
        xv_dateTime = QDateTime::currentDateTime();
        xv_dirty = true;
        emit xg_dirtyChanged(xv_dirty);
    }
    return res;
}
//=========================================================
qint64 X_Calibration::xp_calibrationWindowId(int windowIndex) const
{
    if (windowIndex < 0 || windowIndex >= xv_windowList.count())
    {
        return 0.0;
    }

    return xv_windowList.at(windowIndex)->xp_windowId();
}
//=========================================================
void X_Calibration::xh_windowIsExist(const QString& windowName, bool& res) const
{
    res = xp_windowIndexForName(windowName) >= 0;
}
//=========================================================
int X_Calibration::xp_windowIndexForName(const QString& windowName) const
{
    for (int w = 0; w < xv_windowList.count(); w++)
    {
        if (xv_windowList.at(w)->xp_windowName() == windowName)
        {
            return w;
        }
    }
    return -1;
}
//=========================================================
bool X_Calibration::xp_removeCalibrationWindow(int windowIndex)
{
    if (windowIndex < 0 || windowIndex >= xv_windowList.count())
    {
        return false;
    }

    emit xg_windowOperation(WOT_BEGIN_REMOVE_WINDOWS, windowIndex, windowIndex);
    xv_windowList.takeAt(windowIndex)->deleteLater();
    emit xg_windowOperation(WOT_END_REMOVE_WINDOWS, windowIndex, windowIndex);
    xv_dateTime = QDateTime::currentDateTime();
    xv_dirty = true;
    emit xg_dirtyChanged(xv_dirty);

    return true;
}
//=========================================================
void X_Calibration::xp_removeCalibrationWindows()
{
    for (int w = xv_windowList.count() - 1; w >= 0; w--)
    {
        xp_removeCalibrationWindow(w);
    }
}
//=========================================================
int X_Calibration::xp_termCount() const
{
    return xv_termList.count();
}
//=========================================================
QString X_Calibration::xp_termDisplayName(int termIndex) const
{
    if (termIndex < 0 || termIndex >= xv_termList.count())
    {
        return QString();
    }

    QString termName = xv_termList.at(termIndex)->xp_termName();
    if (xv_termNormalizer->xp_normaType() != X_TermNormalizer::NT_NONE)
    {
        termName += " / N";
    }
    return termName;
}
//=========================================================
QString X_Calibration::xp_termExpression(int termIndex) const
{
    if (termIndex < 0 || termIndex >= xv_termList.count()
        || xv_termList.value(termIndex)->xp_termType() != X_AbstractTerm::TT_CUSTOM)
    {
        return QString();
    }

    X_CustomTerm* customTerm = static_cast<X_CustomTerm*>(xv_termList.at(termIndex));
    return customTerm->xp_expression();
}
//=========================================================
QString X_Calibration::xp_termDescription(int termIndex) const
{
    if (termIndex < 0 || termIndex >= xv_termList.count()
        || xv_termList.value(termIndex)->xp_termType() != X_AbstractTerm::TT_CUSTOM)
    {
        return QString();
    }

    X_CustomTerm* customTerm = static_cast<X_CustomTerm*>(xv_termList.at(termIndex));
    return customTerm->xp_description();
}
//=========================================================
QString X_Calibration::xp_termName(int termIndex) const
{
    if (termIndex < 0 || termIndex >= xv_termList.count())
    {
        return QString();
    }

    QString termName = xv_termList.at(termIndex)->xp_termName();
    return termName;
}
//=========================================================
X_AbstractTerm::TermType X_Calibration::xp_termType(int termIndex) const
{
    if (termIndex < 0 || termIndex >= xv_termList.count())
    {
        return X_AbstractTerm::TT_NOT_DEFINED;
    }

    return xv_termList.at(termIndex)->xp_termType();
}
//=========================================================
bool X_Calibration::xp_mixedTermExists() const
{
    for (int t = 0; t < xv_termList.count(); t++)
    {
        if (xv_termList.at(t)->xp_termType() == X_AbstractTerm::TT_MIXED)
        {
            return true;
        }
    }

    return false;
}
//=========================================================
qint64 X_Calibration::xp_termId(int termIndex) const
{
    if (termIndex < 0 || termIndex >= xv_termList.count())
    {
        return -1;
    }

    return xv_termList.at(termIndex)->xp_termId();
}
//=========================================================
bool X_Calibration::xp_setTermState(int termIndex, X_AbstractTerm::TermState xp_termState)
{
    if (termIndex < 0 || termIndex >= xv_termList.count())
    {
        return false;
    }

    bool res = xv_termList.at(termIndex)->xp_setTermState(xp_termState);
    if (res)
    {
        emit xg_termOperation(TOT_TERM_STATE_CHANGED, termIndex, termIndex);
        xv_dateTime = QDateTime::currentDateTime();
        xv_dirty = true;
        emit xg_dirtyChanged(xv_dirty);
    }

    return res;
}
//=========================================================
X_AbstractTerm::TermState X_Calibration::xp_termStateForDisplay(int termIndex) const
{
    if (termIndex < 0 || termIndex >= xv_termList.count())
    {
        return X_AbstractTerm::TS_NOT_DEFINED;
    }

    if (xv_termList.at(termIndex)->xp_termId() == xv_baseTermId && xv_equationType == ET_FRACTIONAL)
    {
        return X_AbstractTerm::TS_BASE;
    }

    return xv_termList.at(termIndex)->xp_termState();
}
//=========================================================
X_AbstractTerm::TermState X_Calibration::xp_termState(int termIndex) const
{
    if (termIndex < 0 || termIndex >= xv_termList.count())
    {
        return X_AbstractTerm::TS_NOT_DEFINED;
    }

    return xv_termList.at(termIndex)->xp_termState();
}
//=========================================================
QString X_Calibration::xp_termCustomString(int termIndex) const
{
    if (termIndex < 0 || termIndex >= xv_termList.count()
        || xv_termList.value(termIndex)->xp_termType() != X_AbstractTerm::TT_CUSTOM)
    {
        return QString();
    }

    X_CustomTerm* customTerm = static_cast<X_CustomTerm*>(xv_termList.value(termIndex));

    return customTerm->xp_expression();
}
//=========================================================
void X_Calibration::xp_setNextUsersTermState(int termIndex)
{
    if (termIndex < 0 || termIndex >= xv_termList.count())
    {
        return;
    }

    if (xv_termList.at(termIndex)->xp_termId() == xv_baseTermId && xv_equationType == ET_FRACTIONAL)
    {
        return;
    }

    X_AbstractTerm::TermState state = xv_termList.at(termIndex)->xp_termState();
    bool res = false;
//    switch (state)
//    {
//    case X_AbstractTerm::TS_CONST_INCLUDED:
//        res = xv_termList.at(termIndex)->xp_setTermState(X_AbstractTerm::TS_EXAM_WAITING);
//        break;
//    case X_AbstractTerm::TS_EXAM_WAITING:
//        res = xv_termList.at(termIndex)->xp_setTermState(X_AbstractTerm::TS_CONST_EXCLUDED);
//        break;
//    case X_AbstractTerm::TS_INCLUDED:
//        res = xv_termList.at(termIndex)->xp_setTermState(X_AbstractTerm::TS_CONST_EXCLUDED);
//        break;
//    case X_AbstractTerm::TS_EXCEPTED:
//        res = xv_termList.at(termIndex)->xp_setTermState(X_AbstractTerm::TS_CONST_EXCLUDED);
//        break;
//    case X_AbstractTerm::TS_CONST_EXCLUDED:
//        res = xv_termList.at(termIndex)->xp_setTermState(X_AbstractTerm::TS_CONST_INCLUDED);
//        break;
//    case X_AbstractTerm::TS_BASE:
//    case X_AbstractTerm::TS_NOT_DEFINED:
//    default:
//        return;
//    }

        switch (state)
        {
        case X_AbstractTerm::TS_CONST_INCLUDED:
            res = xv_termList.at(termIndex)->xp_setTermState(X_AbstractTerm::TS_CONST_EXCLUDED);
            break;
        case X_AbstractTerm::TS_EXAM_WAITING:
            res = xv_termList.at(termIndex)->xp_setTermState(X_AbstractTerm::TS_CONST_EXCLUDED);
            break;
        case X_AbstractTerm::TS_INCLUDED:
            res = xv_termList.at(termIndex)->xp_setTermState(X_AbstractTerm::TS_CONST_EXCLUDED);
            break;
        case X_AbstractTerm::TS_EXCEPTED:
            res = xv_termList.at(termIndex)->xp_setTermState(X_AbstractTerm::TS_CONST_EXCLUDED);
            break;
        case X_AbstractTerm::TS_CONST_EXCLUDED:
            res = xv_termList.at(termIndex)->xp_setTermState(X_AbstractTerm::TS_CONST_INCLUDED);
            break;
        case X_AbstractTerm::TS_BASE:
        case X_AbstractTerm::TS_NOT_DEFINED:
        default:
            return;
        }


    if (res)
    {
        emit xg_termOperation(TOT_TERM_STATE_CHANGED, termIndex, termIndex);
        xv_dateTime = QDateTime::currentDateTime();
        xv_dirty = true;
        emit xg_dirtyChanged(xv_dirty);
    }
}
//=========================================================
bool X_Calibration::checkColor(QColor color)
{
    if (!color.isValid() || xv_colorList.contains(color))
    {
        return false;
    }

    int colorSumm = color.red() + color.green() + color.blue();
    int averageVol = colorSumm / 3;

    if (colorSumm > 450)
    {
        return false;
    }

    if (abs(averageVol - color.red()) + abs(averageVol - color.green())
            + abs(averageVol - color.blue())
        < 90)
    {
        return false;
    }

    return true;
}
//======================================================
QList<QColor> X_Calibration::xp_createColorList()
{
    int red;
    int green;
    int blue;
    QColor color;

    QList<QColor> colorList;

    while (colorList.count() < 1500)
    {
        srand(QDateTime::currentMSecsSinceEpoch() + 500);
        red = ((rand() % 24) * 10) + 15;
        green = ((rand() % 24) * 10) + 15;
        blue = ((rand() % 24) * 10) + 15;

        color = QColor(red, green, blue);
        if (X_Calibration::checkColor(color))
        {
            colorList << color;
        }
    }
    return colorList;
}
//======================================================

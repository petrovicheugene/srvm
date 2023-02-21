//======================================================
#include "X_EnergyCalibrationDialogV2.h"

#include "X_ColorButton.h"
#include "X_General.h"
#include "X_GraphicsItemUserTypes.h"
#include "X_EnergyCalibrationLine.h"
#include "X_EnergyCalibrationSpectrumTableModel.h"
#include "X_EnergyLineSelectionDialog.h"
#include "X_LESGaussSolver.h"
#include "X_Plotter.h"
#include "X_SpectrumGraphicsItem.h"
#include "X_SpectrumTableDelegate.h"
#include "X_VerticalLineGraphicsItem.h"
#include "X_CaptionFormatter.h"

#include <math.h>
#include <QApplication>
#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
//#include <QSqlError>
//#include <QSqlQuery>
#include <QTableView>
#include <QToolButton>
#include <QVBoxLayout>
#include <math.h>
//======================================================
X_EnergyCalibrationDialogV2::X_EnergyCalibrationDialogV2(QMap<quint8, QList<X_SpeSpectrum*> >& spectrumMap,
                                                       QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Energy calibration"));
    setWindowFlags(Qt::Tool);

    xh_createComponents();
    xh_createConnections();
    xh_createContextMenu();
    xh_setElementLineControlsAndComponentVisibility();
    xh_restoreSettings();
    xh_loadSpectrumData(spectrumMap);
    xv_askSaveForGainFactor = false;

    if(xv_plotter != nullptr)
    {
        QMetaObject::invokeMethod(xv_plotter, "xp_fitInBoundingRect",
                                  Qt::QueuedConnection);
    }
}
//======================================================
void X_EnergyCalibrationDialogV2::xp_setAskSaveForGainfactorFlag(bool flag)
{
    xv_askSaveForGainFactor = flag;
}
//======================================================
X_EnergyCalibrationDialogV2::~X_EnergyCalibrationDialogV2()
{
    xh_saveSettings();
}
//======================================================
void X_EnergyCalibrationDialogV2::xh_createComponents()
{
    xv_spectrumModel = new X_EnergyCalibrationSpectrumTableModel(this);

    // main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // gain factor
    QHBoxLayout* gainFactorLayout = new QHBoxLayout;
    gainFactorLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(gainFactorLayout);

    QLabel* label = new QLabel;
    QString caption = tr("Gain factor");
    label->setText(X_CaptionFormatter::xp_format(caption));
    gainFactorLayout->addWidget(label);

    xv_gainFactorComboBox = new QComboBox(this);
    gainFactorLayout->addWidget(xv_gainFactorComboBox);

    gainFactorLayout->addStretch();

    // main widget
    mainLayout->addWidget(xh_createMainWidget());

    // Basement

    // sparator line
    QFrame* line = new QFrame(this);
    line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    line->setLineWidth(1);
    mainLayout->addWidget(line);

    // dialog buttons
    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    //    xv_okButton = new QPushButton(NS_Buttons::glButtonOk, this);
    //    buttonBox->addButton(xv_okButton, QDialogButtonBox::ActionRole);

    xv_closeButton = new QPushButton(tr("Close"), this);
    buttonBox->addButton(xv_closeButton, QDialogButtonBox::ActionRole);
}
//======================================================
QWidget* X_EnergyCalibrationDialogV2::xh_createMainWidget()
{
    QWidget* mainWidget = new QWidget;
    QVBoxLayout* mainGroupBoxlayout = new QVBoxLayout;
    mainWidget->setLayout(mainGroupBoxlayout);

    // top  layout
    QHBoxLayout* rightLayout = new QHBoxLayout;
    rightLayout->setContentsMargins(0, 0, 0, 0);
    mainGroupBoxlayout->addLayout(rightLayout);

    //    QDialogButtonBox* mainWidgetButtonBox = new QDialogButtonBox(this);
    //    mainGroupBoxlayout->addWidget(mainWidgetButtonBox);

    //    xv_okButton = new QPushButton(NS_Buttons::glButtonOk, this);
    //    mainWidgetButtonBox->addButton(xv_okButton, QDialogButtonBox::ActionRole);

    // plotter
    xv_plotter = new X_Plotter;
    mainGroupBoxlayout->addWidget(xv_plotter, 99999999);

    // spectrum list
    xv_spectrumTableView = new QTableView(this);
    rightLayout->addWidget(xv_spectrumTableView);

    // element lines
    rightLayout->addWidget(xh_createElementLinesWidgetAndComponents());
    rightLayout->addStretch();

    return mainWidget;
}
//======================================================
QWidget* X_EnergyCalibrationDialogV2::xh_createElementLinesWidgetAndComponents()
{
    QGroupBox* mainControlWidget = new QGroupBox;
    QVBoxLayout* mainLayout = new QVBoxLayout;
    // mainLayout->setMargin(0);
    mainControlWidget->setLayout(mainLayout);

    QHBoxLayout* peakCountLayout = new QHBoxLayout;
    mainLayout->addLayout(peakCountLayout);

    QLabel* label = new QLabel(this);
    QString caption = tr("Peak count in energy calibration:");
    label->setText(X_CaptionFormatter::xp_format(caption));
    peakCountLayout->addWidget(label);
    xv_peakInEnergyCalibrationCountComboBox = new QComboBox(this);
    xv_peakInEnergyCalibrationCountComboBox->addItem(tr("Two"), QVariant(2));
    xv_peakInEnergyCalibrationCountComboBox->addItem(tr("Three"), QVariant(3));

    peakCountLayout->addWidget(xv_peakInEnergyCalibrationCountComboBox);
    peakCountLayout->addStretch();

    // Element lines
    QGridLayout* dataLayout = new QGridLayout;
    // dataLayout->setMargin(0);
    mainLayout->addLayout(dataLayout);

    for(int i = 0; i < xv_calibrationLineNumber; i++)
    {
        xh_createElementLineControlsAndComponents(dataLayout, i);
    }

    mainLayout->addStretch();
    // Calculate buttonBox
    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    mainLayout->addWidget(buttonBox);

    xv_calculateButton = new QPushButton(tr("Calculate"), this);
    xv_calculateButton->setToolTip(tr("Calculate energy calibration"));
    xv_calculateButton->setFocusPolicy(Qt::NoFocus);
    buttonBox->addButton(xv_calculateButton, QDialogButtonBox::ActionRole);

    return mainControlWidget;
}
//======================================================
void X_EnergyCalibrationDialogV2::xh_createElementLineControlsAndComponents(QGridLayout* layout, int elementNumber)
{
    // calibration line
    X_EnergyCalibrationLine* calibrationLine = new X_EnergyCalibrationLine(this);
    connect(calibrationLine, &X_EnergyCalibrationLine::xg_lineColor,
            this, &X_EnergyCalibrationDialogV2::xh_onLineColorChange);
    xv_energyCalibrationLineList.append(calibrationLine);

    // claibration line graphic item
    X_VerticalLineGraphicsItem* verticalLineGraphicsItem = new X_VerticalLineGraphicsItem(calibrationLine);
    xv_plotter->xp_addItem(verticalLineGraphicsItem);

    // channel set up action
    QAction* channelNumberAction = new QAction(calibrationLine);
    channelNumberAction->setText(tr("Energy line #%1").arg(QString::number(elementNumber + 1)));
    connect(channelNumberAction, &QAction::triggered,
            this, &X_EnergyCalibrationDialogV2::xh_onChannelNumberAction);
    xv_channelNumberActionList.append(channelNumberAction);

    QList<QWidget*> widgetList;

    int startRow = elementNumber * 2;
    QLabel* label = new QLabel(this);
    QString caption = tr("Energy line #%1").arg(QString::number(elementNumber + 1));
    label->setText(X_CaptionFormatter::xp_format(caption));
    widgetList.append(label);
    layout->addWidget(label, startRow, 0, 1, 3);

    X_ColorButton* colorBtn = new X_ColorButton(this);
    widgetList.append(colorBtn);
    layout->addWidget(colorBtn, startRow + 1, 0, 1, 1);
    connect(colorBtn, &X_ColorButton::xg_colorChanged,
            calibrationLine, &X_EnergyCalibrationLine::xp_setLineColor);
    connect(calibrationLine, &X_EnergyCalibrationLine::xg_lineColor,
            colorBtn, &X_ColorButton::xp_setColor);

    label = new QLabel(this);
    label->setText(tr("Peak center"));
    widgetList.append(label);
    layout->addWidget(label, startRow + 1, 1, 1, 1);

    QSpinBox* channelNumberSpinBox = new QSpinBox(this);
    channelNumberSpinBox->setRange(0, 2048);
    widgetList.append(channelNumberSpinBox);
    connect(calibrationLine, &X_EnergyCalibrationLine::xg_channelNumber,
            this, &X_EnergyCalibrationDialogV2::xh_onChannelNumberChange);
    connect(calibrationLine, &X_EnergyCalibrationLine::xg_channelNumber,
            channelNumberSpinBox, &QSpinBox::setValue);
    connect(channelNumberSpinBox, SIGNAL(valueChanged(int)),
            calibrationLine, SLOT(xp_setChannelNumber(int)));
    //    connect(channelNumberSpinBox, &QSpinBox::hasFo
    //            this, &X_EnergyCalibrationDialogV2::onChannelspinBoxHasFocus);
    layout->addWidget(channelNumberSpinBox, startRow + 1, 2, 1, 1);

    label = new QLabel(this);
    label->setText("Energy value");
    widgetList.append(label);
    layout->addWidget(label, startRow + 1, 3, 1, 1);

    QDoubleSpinBox* energyValueSpinBox = new QDoubleSpinBox(this);
    energyValueSpinBox->setRange(0, 9999.99);
    energyValueSpinBox->setDecimals(3);
    energyValueSpinBox->setSuffix(tr("kEv"));
    //energyValueSpinBox->setReadOnly(true);
    energyValueSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    widgetList.append(energyValueSpinBox);

    connect(calibrationLine, &X_EnergyCalibrationLine::xg_energyValue,
            energyValueSpinBox, &QDoubleSpinBox::setValue);
    connect(energyValueSpinBox, SIGNAL(valueChanged(double)),
            calibrationLine, SLOT(xp_setEnergyValue(double)));
    layout->addWidget(energyValueSpinBox, startRow + 1, 4, 1, 1);


    label = new QLabel(this);
    label->setText("Chemical element line");
    widgetList.append(label);
    layout->addWidget(label, startRow + 1, 5, 1, 1);

    QLineEdit* elementLineEdit = new QLineEdit(this);
    widgetList.append(elementLineEdit);

    //elementLineEdit->setReadOnly(true);
    connect(calibrationLine, &X_EnergyCalibrationLine::xg_lineName,
            elementLineEdit, &QLineEdit::setText);
    connect(elementLineEdit, &QLineEdit::textChanged,
            calibrationLine, &X_EnergyCalibrationLine::xp_setLineName);
    layout->addWidget(elementLineEdit, startRow + 1, 6, 1, 1);


    QToolButton* selectLineButton = new QToolButton(this);
    selectLineButton->setText("...");
    selectLineButton->setFocusPolicy(Qt::NoFocus);
    widgetList.append(selectLineButton);
    layout->addWidget(selectLineButton, startRow + 1, 7, 1, 1);

    // energy line set up dialog
    QAction* selectEnergyLineAction = new QAction(calibrationLine);
    connect(selectLineButton, &QPushButton::clicked,
            selectEnergyLineAction, &QAction::trigger);
    connect(selectEnergyLineAction, &QAction::triggered,
            this, &X_EnergyCalibrationDialogV2::xh_onSelectEnergyLineAction);

    QToolButton* clearLineButton = new QToolButton(this);
    clearLineButton->setText("X");
    clearLineButton->setFocusPolicy(Qt::NoFocus);
    widgetList.append(clearLineButton);
    layout->addWidget(clearLineButton, startRow + 1, 8, 1, 1);

    QAction* clearEnergyLineAction = new QAction(calibrationLine);
    connect(clearLineButton, &QPushButton::clicked,
            clearEnergyLineAction, &QAction::trigger);
    connect(clearEnergyLineAction, &QAction::triggered,
            this, &X_EnergyCalibrationDialogV2::xh_onClearEnergyLineAction);

    xv_elementLinesWidgetList.append(widgetList);

}
//======================================================
void X_EnergyCalibrationDialogV2::xh_createContextMenu()
{
    xv_plotter->xp_setContextMenu(xv_channelNumberActionList);
}
//======================================================
void X_EnergyCalibrationDialogV2::xh_createConnections()
{
    xv_spectrumTableView->setModel(xv_spectrumModel);
    X_SpectrumTableDelegate* spectrumTableDelegate = new X_SpectrumTableDelegate(xv_spectrumTableView);
    xv_spectrumTableView->viewport()->installEventFilter(spectrumTableDelegate);
    xv_spectrumTableView->setItemDelegate(spectrumTableDelegate);

    connect(xv_calculateButton, &QPushButton::clicked,
            this, &X_EnergyCalibrationDialogV2::xh_calculateAndWriteEnergyCalibration);
    connect(xv_closeButton, &QPushButton::clicked,
            this, &X_EnergyCalibrationDialogV2::close);
    connect(xv_gainFactorComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(xh_onCurrentGainFactorIndexChange(int)));

    connect(xv_spectrumModel, &X_EnergyCalibrationSpectrumTableModel::modelReset,
            this, &X_EnergyCalibrationDialogV2::xh_onModelReset);
    connect(xv_spectrumModel, &X_EnergyCalibrationSpectrumTableModel::dataChanged,
            this, &X_EnergyCalibrationDialogV2::xh_onModelDataChange);

    connect(xv_plotter, &X_Plotter::xg_mousePressedAt,
            this, &X_EnergyCalibrationDialogV2::xh_onMousePressedOnPlotter);

    connect(xv_peakInEnergyCalibrationCountComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(xh_onCurrentPeackCountIndexChange(int)));
}
//======================================================
void X_EnergyCalibrationDialogV2::xh_loadSpectrumData(QMap<quint8, QList<X_SpeSpectrum *> > &spectrumMap)
{
    // gain factor
    QString itemString;
    for(auto &gainFactor : spectrumMap.keys())
    {
        itemString = tr("Gain Factor: %1").arg(QString::number(gainFactor));
        xv_gainFactorComboBox->addItem(itemString, QVariant(static_cast<int>(gainFactor)));
    }
    xv_gainFactorComboBox->setCurrentIndex(0);

    // spectrum model data
    xv_spectrumModel->xp_loadSpectrumData(spectrumMap);
}
//======================================================
//void X_EnergyCalibrationDialogV2::xh_restoreLineColorList()
//{
//    QSettings settings;
//    settings.beginGroup(qApp->applicationVersion());
//    settings.beginGroup(xv_settingsGroupName);

//    int size = settings.beginReadArray(xv_lineColorListSectionName);
//    if(size > 0)
//    {
//        for (int i = 0; i < size; ++i)
//        {
//            settings.setArrayIndex(i);
//            QColor color = settings.value(xv_lineColorValueName).value<QColor>();
//            xv_lineColorList.append(color);
//        }
//    }
//    else
//    {
//        xv_lineColorList.append(QColor(Qt::red));
//        xv_lineColorList.append(QColor(Qt::green));
//        xv_lineColorList.append(QColor(Qt::blue));
//    }

//    settings.endArray();

//    settings.endGroup();
//    settings.endGroup();
//}
//======================================================
void X_EnergyCalibrationDialogV2::xh_restoreSettings()
{
    QSettings settings;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup(xv_settingsGroupName);

    QVariant vData = settings.value(xv_geometrySectionName);
    if(vData.isValid() && !vData.isNull() && vData.canConvert<QByteArray>())
    {
        restoreGeometry(vData.toByteArray());
    }

    int size = settings.beginReadArray(xv_lineColorListSectionName);
    if(size > 0)
    {
        for (int i = 0; i < size; ++i)
        {
            settings.setArrayIndex(i);
            QColor color = settings.value(xv_lineColorValueName).value<QColor>();
            X_EnergyCalibrationLine* line = xv_energyCalibrationLineList.value(i, nullptr);
            if(line)
            {
                line->xp_setLineColor(color);
            }
        }
    }
    else
    {
        QVector<QColor> lineColorVector;
        lineColorVector.append(QColor(Qt::red));
        lineColorVector.append(QColor(Qt::green));
        lineColorVector.append(QColor(Qt::blue));

        //       QColor color;
        for(int i = 0; i < xv_energyCalibrationLineList.count(); i++)
        {
            xv_energyCalibrationLineList.at(i)->xp_setLineColor(lineColorVector.value(i, QColor()));
        }
    }

    settings.endArray();


    settings.endGroup();
    settings.endGroup();
}
//======================================================
void X_EnergyCalibrationDialogV2::xh_saveSettings()
{
    QSettings settings;
    settings.beginGroup(qApp->applicationVersion());
    settings.beginGroup(xv_settingsGroupName);
    settings.setValue(xv_geometrySectionName, QVariant(saveGeometry()));

    settings.beginWriteArray(xv_lineColorListSectionName);
    for (int i = 0; i < xv_energyCalibrationLineList.count(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue(xv_lineColorValueName, QVariant(xv_energyCalibrationLineList.at(i)->xp_lineColor()));
    }
    settings.endArray();

    settings.endGroup();
    settings.endGroup();
}
//======================================================
void X_EnergyCalibrationDialogV2::xh_calculateAndWriteEnergyCalibration()
{
    // calculate energy scale factors
    QList<double> energyCalibrationFactorList;
    if(!xh_calculateEnergyFactors(energyCalibrationFactorList))
    {
        return;
    }

    // write energy calibration to checked files
    xv_spectrumModel->xp_writeEnergyCalibrationToCheckedSpectra(energyCalibrationFactorList);

    if(xv_askSaveForGainFactor)
    {
        // write energy calibration to gain factor
        QString msg = tr("Do you want to bind the energy calibration to the current gain factor equals %1?").arg(xv_gainFactorComboBox->currentText());
        if(QMessageBox::question(this, tr("Energy calibration error"), msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        {
            return;
        }
    }
}
//======================================================
void X_EnergyCalibrationDialogV2::xh_onCurrentGainFactorIndexChange(int currentIndex)
{
    if(currentIndex < 0 || currentIndex >= xv_gainFactorComboBox->count())
    {
        xv_spectrumModel->xp_setGainFactorFilter(0);
        return;
    }

    QVariant vData = xv_gainFactorComboBox->itemData(currentIndex);
    if(!vData.isValid() | !vData.canConvert<quint8>())
    {
        xv_spectrumModel->xp_setGainFactorFilter(0);
        return;
    }

    quint8 currentGainFactor = static_cast<quint8>(vData.toUInt());
    xv_spectrumModel->xp_setGainFactorFilter(currentGainFactor);

}
//======================================================
void X_EnergyCalibrationDialogV2::xh_onCurrentPeackCountIndexChange(int currentIndex)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if(!comboBox)
    {
        return;
    }

    xh_setElementLineControlsAndComponentVisibility();
    xh_updateVerticalLines();
}
//======================================================
void X_EnergyCalibrationDialogV2::xh_onModelReset()
{
    // current plotter state
    qreal distortionFactor;
    qreal distortionCorrectionFactor;
    xv_plotter->xp_verticalDistortionFactors(distortionFactor, distortionCorrectionFactor);
    bool isPlotScaled = xv_plotter->xp_isPlotScaled();

    // clear all previous spectra
    xv_plotter->xp_clearItemsForType(SpectrumItemType);

    //
    X_SpectrumGraphicsItem* spectrumItem;
    X_SpeSpectrum* spectrum;

    for(int row = 0; row < xv_spectrumModel->rowCount(); row++)
    {
        spectrum = xv_spectrumModel->xp_spectrumForRow(row);
        spectrumItem = new X_SpectrumGraphicsItem(spectrum,
                                                 xv_boundingRectTopFactor,
                                                 distortionFactor,
                                                 distortionCorrectionFactor);
        xv_plotter->xp_addItem(spectrumItem);
    }

    if(!isPlotScaled)
    {
        xv_plotter->xp_fitInBoundingRect();
        //            QMetaObject::invokeMethod(xv_plotter, "xp_fitInBoundingRect",
        //                                      Qt::QueuedConnection);
    }

    QRectF boundingRect = xv_plotter->xp_boundingRect();
    X_VerticalLineGraphicsItem::xp_setTopAndButtonMargins(boundingRect.top(),boundingRect.bottom());
    xh_updateVerticalLines();
}
//======================================================
void X_EnergyCalibrationDialogV2::xh_onModelDataChange(const QModelIndex& topLeft,
                                                      const QModelIndex& bottomRight)
{
    // update spectrum Visibility
    QList<QGraphicsItem*> spectrumList = xv_plotter->xp_itemListForType(SpectrumItemType);
    X_SpectrumGraphicsItem* spectrumItem;
    X_SpeSpectrum* spectrum;

    for(int row = 0; row < xv_spectrumModel->rowCount(); row++)
    {
        spectrum = xv_spectrumModel->xp_spectrumForRow(row);
        for(int sp = 0; sp < spectrumList.count(); sp++)
        {
            spectrumItem = qgraphicsitem_cast<X_SpectrumGraphicsItem*>(spectrumList.at(sp));
            if(spectrumItem != nullptr && spectrumItem->xp_spectrumId() == spectrum->xp_spectrumId())
            {
                // spectrumItem found
                // set visibility
                spectrumItem->setVisible(spectrum->xp_isSpectrumVisible());
                // remove spectrum graphic item from the list
                spectrumList.removeAt(sp);
                break;
            }
        }
    }
}
//======================================================
void X_EnergyCalibrationDialogV2::xh_onMousePressedOnPlotter(QPointF scenePos)
{


}
//======================================================
void X_EnergyCalibrationDialogV2::xh_onChannelNumberAction()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if(!action)
    {
        return;
    }

    QVariant vData = action->data();
    if(!vData.isValid() || !vData.canConvert<QPointF>())
    {
        return;
    }

    X_EnergyCalibrationLine* energyCalibrationLine = qobject_cast<X_EnergyCalibrationLine*>(action->parent());
    if(!energyCalibrationLine)
    {
        return;
    }

    energyCalibrationLine->xp_setChannelNumber(static_cast<int>(vData.toPointF().x()));
}
//======================================================
void X_EnergyCalibrationDialogV2::xh_onLineColorChange(QColor color)
{
    xh_updateVerticalLines();
}
//======================================================
void X_EnergyCalibrationDialogV2::xh_onChannelNumberChange(int channelValue)
{
    xh_updateVerticalLines();
}
//======================================================
void X_EnergyCalibrationDialogV2::xh_updateVerticalLines()
{
    QList<QGraphicsItem*>lineList = xv_plotter->xp_itemListForType(VerticalLineItemType);
    foreach(QGraphicsItem* graphicsItem, lineList)
    {
        X_VerticalLineGraphicsItem* lineItem = qgraphicsitem_cast<X_VerticalLineGraphicsItem*>(graphicsItem);
        if(lineItem)
        {
            lineItem->xp_updateItem();
        }
    }
}
//======================================================
void X_EnergyCalibrationDialogV2::xh_onSelectEnergyLineAction()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if(!action)
    {
        return;
    }

    X_EnergyCalibrationLine* energyCalibrationLine = qobject_cast<X_EnergyCalibrationLine*>(action->parent());
    if(!energyCalibrationLine)
    {
        return;
    }

    X_EnergyLineSelectionDialog dialog;
    if(!dialog.exec())
    {
        return;
    }

    energyCalibrationLine->xp_setEnergyValue(dialog.xp_energyValue());
    QString chemicalElementEnergyLineName = dialog.xp_chemicalElementName() + " - " +
            dialog.xp_energyLineName();
    energyCalibrationLine->xp_setLineName(chemicalElementEnergyLineName);


}
//======================================================
void X_EnergyCalibrationDialogV2::xh_onClearEnergyLineAction()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if(!action)
    {
        return;
    }

    X_EnergyCalibrationLine* energyCalibrationLine = qobject_cast<X_EnergyCalibrationLine*>(action->parent());
    if(!energyCalibrationLine)
    {
        return;
    }

    energyCalibrationLine->xp_setChannelNumber(0);
    energyCalibrationLine->xp_setEnergyValue(0.0);
    energyCalibrationLine->xp_setLineName(QString());

}
//======================================================
bool X_EnergyCalibrationDialogV2::xh_calculateEnergyFactors(QList<double>& energyCalibrationFactorList)
{
    QList<QPair<int, double> > calibrationLineSrcDataList;
    bool skipLine;
    for(int k = 0; k < xv_energyCalibrationLineList.count(); k++)
    {
        skipLine = false;
        for(int e = 0; e < k; e++)
        {
            if(xv_energyCalibrationLineList.at(e)->xp_channelNumber() ==
                    xv_energyCalibrationLineList.at(k)->xp_channelNumber() &&
                    xv_energyCalibrationLineList.at(e)->xp_energyValue() -
                    xv_energyCalibrationLineList.at(k)->xp_energyValue() == 0.0)
            {

                skipLine = true;
                break;
            }
            else if(!(xv_energyCalibrationLineList.at(e)->xp_channelNumber() !=
                      xv_energyCalibrationLineList.at(k)->xp_channelNumber() &&
                      xv_energyCalibrationLineList.at(e)->xp_energyValue() -
                      xv_energyCalibrationLineList.at(k)->xp_energyValue() != 0.0))
            {
                QString msg = tr("Cannot calculate energy calibration.<br>Ambiguity in source data of energy lines.");
                QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
                qCritical().noquote() << msg;
                return false;
            }
        }

        if(skipLine)
        {
            continue;
        }

        if(!xv_energyCalibrationLineList.at(k)->xp_isEnabled())
        {
            continue;
        }

        calibrationLineSrcDataList.append(QPair<int, double>(xv_energyCalibrationLineList.at(k)->xp_channelNumber(),
                                                             xv_energyCalibrationLineList.at(k)->xp_energyValue()));
    }

    if(calibrationLineSrcDataList.count() < 2)
    {
        QString msg = tr("Cannot calculate energy calibration.<br>Insufficient source data of energy lines.");
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        qCritical().noquote() << msg;
        return false;
    }


    // create free member list, column list and fill all lists
    //
    QList<double> freeMemberList;
    QList<QPair<double, QList<double> > > matrixColumnList;
    for(int i = 0; i < calibrationLineSrcDataList.count(); i++)
    {
        matrixColumnList.append(QPair<double, QList<double> >(0.0, QList<double>()));
    }

    double memberValue;
    for(int cl = 0; cl < calibrationLineSrcDataList.count(); cl++)
    {
        // fill column lists
        for(int mc = 0; mc < matrixColumnList.count(); mc++)
        {
            memberValue = static_cast<double>(pow(calibrationLineSrcDataList.at(cl).first, mc));
            matrixColumnList[mc].second.append(memberValue);
        }

        // fill free member list
        freeMemberList.append(calibrationLineSrcDataList.at(cl).second);
    }

    X_LESGaussSolver solver;
    for(int mc = 0; mc < matrixColumnList.count(); mc++)
    {
        solver.xp_appendTermColumn(&matrixColumnList[mc].first,  matrixColumnList.at(mc).second);
    }

    solver.xp_appendFreeTermList(freeMemberList);

    if(!solver.xp_solve())
    {
        QString msg = tr("Calculation of energy calibration factors fails. Error: %1.").arg(solver.xp_lastError());
        QMessageBox::critical(this, tr("Error"), msg, QMessageBox::Ok);
        qCritical().noquote() << msg;
        return false;
    }

    for(int mc = 0; mc <  matrixColumnList.count(); mc++)
    {
        energyCalibrationFactorList.append(matrixColumnList[mc].first);
    }

    QString msg = tr("Energy calibration has been calculated successfully<br>"
                     "K0 - %1<br>"
                     "K1 - %2<br>"
                     "K2 - %3<br>").arg(QString::number(energyCalibrationFactorList.value(0, 0.0), 'E', 14),
                                        QString::number(energyCalibrationFactorList.value(1, 0.0), 'E', 14),
                                        QString::number(energyCalibrationFactorList.value(2, 0.0), 'E', 14));
    qInfo().noquote() << msg;
    return true;
}
//======================================================
void X_EnergyCalibrationDialogV2::xh_setElementLineControlsAndComponentVisibility()
{
    QVariant vData = xv_peakInEnergyCalibrationCountComboBox->currentData();
    if(!vData.isValid() || !vData.canConvert<int>())
    {
        return;
    }

    int peackCount = vData.toInt();
    X_EnergyCalibrationLine* line;
    for(int i = 0; i < xv_elementLinesWidgetList.count(); i++)
    {
        line = xv_energyCalibrationLineList.value(i, nullptr);
        if(line)
        {
            line->xp_setEnabled(i < peackCount);
        }

        for(auto& widget : xv_elementLinesWidgetList.at(i))
        {
            widget->setVisible(i < peackCount);
        }

        QAction* action = xv_channelNumberActionList.value(i, nullptr);
        if(action)
        {
            action->setEnabled(i < peackCount);
            action->setVisible(i < peackCount);
        }
    }
}
//======================================================

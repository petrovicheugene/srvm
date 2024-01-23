//======================================================
#include "X_PeriodicTableWidget.h"
#include "X_ChemicalElementPropertyTreeModel.h"

#include <QAbstractItemModel>
#include <QApplication>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QVBoxLayout>
//======================================================
X_PeriodicTableWidget::X_PeriodicTableWidget(QWidget *parent) : QWidget(parent)
{
    xv_model = nullptr;
    xh_createComponents();
    xh_createConnections();
    xh_recalcAndApplyMinButtonSize();
    xp_setSelectionMode(SM_NO_SELECTION);
}
//======================================================
void X_PeriodicTableWidget::xh_createComponents()
{
    xv_mainLayout = new QVBoxLayout;
    setLayout(xv_mainLayout);
    // main button layout
    QGridLayout *buttonGridLayout = new QGridLayout;
    buttonGridLayout->setSpacing(0);
    xv_mainLayout->addLayout(buttonGridLayout);

    // button placing
    X_ChemicalElementButton *chemicalElementButton;
    QLabel *label;
    uint currentX_Number = 0;

    //
    for (int row = 0; row < 10; row++)
    {
        if (row == 7)
        {
            label = new QLabel;
            buttonGridLayout->addWidget(label, row, 0, 1, 17, Qt::AlignHCenter | Qt::AlignBottom);
            continue;
        }

        // buttons
        for (int col = 0; col < 18; col++)
        {
            // I period  (H and He)
            if (row == 0)
            {
                if (col != 0 && col != 17)
                {
                    continue;
                }
            }

            //  II III periods
            if (row > 0 && row < 3)
            {
                if (col > 1 && col < 12)
                {
                    continue;
                }
            }

            // Lanthanide // Actinide
            if (row > 7)
            {
                if (col < 2 || col > 15)
                {
                    continue;
                }
            }

            // Adjust element number
            if (currentX_Number == 57)
            {
                // skip Lanthanide
                currentX_Number = 71;
            }
            else if (currentX_Number == 89)
            {
                // skip Actinide
                currentX_Number = 103;
            }
            else if (currentX_Number == 118)
            {
                // return to lanthanide
                currentX_Number = 57;
            }
            else if (currentX_Number == 71)
            {
                // return to actinide
                currentX_Number = 89;
            }

            // create and place button
            chemicalElementButton = new X_ChemicalElementButton(++currentX_Number);
            chemicalElementButton->setCheckable(true);
            chemicalElementButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            buttonGridLayout->addWidget(chemicalElementButton, row, col, 1, 1);
            // apppend the button to the list
            if (currentX_Number - 1 >= xv_elementButtonList.size())
            {
                xv_elementButtonList.append(chemicalElementButton);
            }
            else
            {
                xv_elementButtonList.insert(static_cast<int>(currentX_Number - 1),
                                            chemicalElementButton);
            }
        }
    }
}
//======================================================
void X_PeriodicTableWidget::xh_createConnections()
{
    foreach (X_ChemicalElementButton *button, xv_elementButtonList)
    {
        connect(button,
                &X_ChemicalElementButton::xg_symbolChanged,
                this,
                &X_PeriodicTableWidget::xh_recalcAndApplyMinButtonSize);
        connect(button,
                &X_ChemicalElementButton::toggled,
                this,
                &X_PeriodicTableWidget::xh_onButtonToggle);
    }
}
//======================================================
void X_PeriodicTableWidget::xp_setMainLayoutMargin(int margin)
{
    if (margin < 0)
    {
        return;
    }

    xv_mainLayout->setContentsMargins(margin, margin, margin, margin);
}
//======================================================
void X_PeriodicTableWidget::xp_setModel(QAbstractItemModel *model)
{
    if (model == nullptr)
    {
        return;
    }

    // disconnect from
    if (xv_model != nullptr)
    {
        disconnect(model,
                   &QAbstractItemModel::modelReset,
                   this,
                   &X_PeriodicTableWidget::xh_updatePeriodicTable);
    }

    // connect to model
    xv_model = model;
    connect(model,
            &QAbstractItemModel::modelReset,
            this,
            &X_PeriodicTableWidget::xh_updatePeriodicTable);

    connect(model,
            &QAbstractItemModel::dataChanged,
            this,
            &X_PeriodicTableWidget::xh_updateButtonCaptions);

    xh_updatePeriodicTable();
}
//======================================================
QList<int> X_PeriodicTableWidget::xp_selectedChemicalElementList() const
{
    QList<int> selectedChemicalElementList;
    foreach (X_ChemicalElementButton *button, xv_elementButtonList)
    {
        if (button->isChecked())
        {
            selectedChemicalElementList.append(static_cast<int>(button->xp_zNumber()));
        }
    }

    return selectedChemicalElementList;
}
//======================================================
void X_PeriodicTableWidget::xp_fillSelectedChemicalElementList(QList<int> &X_NumberList) const
{
    X_NumberList = xp_selectedChemicalElementList();
}
//======================================================
void X_PeriodicTableWidget::xp_setSelectionMode(SelectionMode selectionMode)
{
    xv_selectionMode = selectionMode;
    foreach (X_ChemicalElementButton *button, xv_elementButtonList)
    {
        button->setCheckable(xv_selectionMode != SM_NO_SELECTION);
    }
}
//======================================================
X_PeriodicTableWidget::SelectionMode X_PeriodicTableWidget::xp_selectionMode()
{
    return xv_selectionMode;
}
//======================================================
void X_PeriodicTableWidget::resizeEvent(QResizeEvent *event)
{
    xh_recalcAndApplyPropertyRectSizes();
    QWidget::resizeEvent(event);
}
//======================================================
void X_PeriodicTableWidget::xh_updatePeriodicTable()
{
    QModelIndex index;
    QVariant vData;
    QString dataString;
    QString toolTipString;
    for (int row = 0; row < xv_elementButtonList.count(); row++)
    {
        index = xv_model->index(row, 0, QModelIndex());
        if (!index.isValid())
        {
            continue;
        }

        // symbol
        vData = xv_model->data(index, SymbolRole);
        if (!vData.isValid() || !vData.canConvert<QString>())
        {
            continue;
        }
        xv_elementButtonList[row]->xp_setChemicalElementSymbol(vData.toString());

        // name
        vData = xv_model->data(index, NameRole);
        if (!vData.isValid() || !vData.canConvert<QString>())
        {
            continue;
        }
        xv_elementButtonList[row]->xp_setChemicalElementName(vData.toString());
        toolTipString = QString("%1 - %2").arg(QString::number(
                                                   xv_elementButtonList[row]->xp_zNumber()),
                                               xv_elementButtonList[row]->xp_chemicalElementName());
        xv_elementButtonList[row]->setToolTip(toolTipString);
    }
}
//======================================================
void X_PeriodicTableWidget::xh_updateButtonCaptions(const QModelIndex &topLeft,
                                                   const QModelIndex &bottomRight)
{
    xh_updatePeriodicTable();
}
//======================================================
void X_PeriodicTableWidget::xh_onButtonToggle(bool toggled)
{
    X_ChemicalElementButton *toggledButton = qobject_cast<X_ChemicalElementButton *>(sender());
    if (toggledButton->isChecked() && xv_selectionMode == SM_SINGLE_SELECTION)
    {
        // make other button unchecked
        foreach (X_ChemicalElementButton *button, xv_elementButtonList)
        {
            if (button == toggledButton)
            {
                continue;
            }
            button->setChecked(false);
        }
    }

    emit xg_selectedChemicalElementChanged(toggledButton->xp_zNumber(), toggledButton->isChecked());
}
//======================================================
void X_PeriodicTableWidget::xh_recalcAndApplyMinButtonSize()
{
    QSize maxSymbolSize;
    xh_maxSymbolRectSize(maxSymbolSize);

    foreach (X_ChemicalElementButton *button, xv_elementButtonList)
    {
        button->xp_updateMinimumSizeForSymbolRectSize(maxSymbolSize);
    }
}
//======================================================
bool X_PeriodicTableWidget::xh_setChemicalElementSymbol(X_Number zNumber, const QString &symbol)
{
    X_ChemicalElementButton *button = nullptr;
    if (!xh_buttonForX_Number(zNumber, button))
    {
        return false;
    }

    button->xp_setChemicalElementSymbol(symbol);
    return true;
}
//======================================================
void X_PeriodicTableWidget::xh_recalcAndApplyPropertyRectSizes()
{
    QSize symbolRectSize;
    QSize zNumberRectSize;
    QSize nameRectSize;
    xh_recalcPropertyRectSizes(symbolRectSize, zNumberRectSize, nameRectSize);

    xh_applyPropertyRectSizes(symbolRectSize, zNumberRectSize, nameRectSize);
}
//======================================================
void X_PeriodicTableWidget::xh_recalcPropertyRectSizes(QSize &symbolRectSize,
                                                      QSize &zNumberRectSize,
                                                      QSize &nameRectSize) const
{
    // reset rects
    symbolRectSize = QSize();
    zNumberRectSize = QSize();
    nameRectSize = QSize();

    // get Button rect sizes
    QSize minButtonSize;
    QSize symbolWindowRectSize;
    QSize zNumberWindowRectSize;
    QSize nameWindowRectSize;

    xh_buttonRectSizes(minButtonSize,
                       symbolWindowRectSize,
                       zNumberWindowRectSize,
                       nameWindowRectSize);

    if (minButtonSize.isEmpty() || symbolWindowRectSize.isEmpty())
    {
        return;
    }

    // define included property rect sizes
    if (minButtonSize.height() - symbolWindowRectSize.height() < zNumberWindowRectSize.height())
    {
        // only symbol
        zNumberWindowRectSize = QSize();
        nameWindowRectSize = QSize();
    }
    else if (minButtonSize.height() - symbolWindowRectSize.height() - zNumberWindowRectSize.height()
                 < nameWindowRectSize.height()
             || minButtonSize.width() < nameWindowRectSize.width())
    {
        // symbol and X_
        nameWindowRectSize = QSize();
    }

    // Define increase factor
    //
    int commonWindowHeight = symbolWindowRectSize.height() + zNumberWindowRectSize.height()
                             + nameWindowRectSize.height();

    int commonWindowWidth = qMax(qMax(symbolWindowRectSize.width(), zNumberWindowRectSize.width()),
                                 nameWindowRectSize.width());

    if (commonWindowHeight <= 0 || commonWindowWidth <= 0)
    {
        // no a caption
        return;
    }

    double windowSideRatio = static_cast<double>(commonWindowWidth)
                             / static_cast<double>(commonWindowHeight);

    double buttonSideRatio = static_cast<double>(minButtonSize.width())
                             / static_cast<double>(minButtonSize.height());

    double scaleFactor;
    if (buttonSideRatio > windowSideRatio)
    {
        // fit by height
        scaleFactor = static_cast<double>(minButtonSize.height())
                      / static_cast<double>(commonWindowHeight);
    }
    else
    {
        // fit by width
        scaleFactor = static_cast<double>(minButtonSize.width())
                      / static_cast<double>(commonWindowWidth);

        // calc height of button rect reduced to commonWindowWidth
        int reducedButtonHeight = qRound(static_cast<double>(commonWindowWidth) / buttonSideRatio);
        int freeHeightForSymbol = reducedButtonHeight - zNumberWindowRectSize.height()
                                  - nameWindowRectSize.height();

        double freeWindowForSymbolSideRatio = static_cast<double>(commonWindowWidth)
                                              / static_cast<double>(freeHeightForSymbol);

        double symbolWindowSideRatio = static_cast<double>(symbolWindowRectSize.width())
                                       / static_cast<double>(symbolWindowRectSize.height());

        double symbolScaleFactor; // symbol rect size can be increased if it is possible
        if (freeWindowForSymbolSideRatio > symbolWindowSideRatio)
        {
            // fit by height
            symbolScaleFactor = static_cast<double>(freeHeightForSymbol)
                                / static_cast<double>(symbolWindowRectSize.height());
        }
        else
        {
            // fit by width
            symbolScaleFactor = static_cast<double>(commonWindowWidth)
                                / static_cast<double>(symbolWindowRectSize.width());
        }
        // recalc symbol window rect size
        symbolWindowRectSize.setHeight(qRound(symbolWindowRectSize.height() * symbolScaleFactor));
        symbolWindowRectSize.setWidth(qRound(symbolWindowRectSize.width() * symbolScaleFactor));
    }

    // recalc every rect size
    if (!zNumberWindowRectSize.isEmpty())
    {
        zNumberRectSize.setHeight(qRound(zNumberWindowRectSize.height() * scaleFactor));
        zNumberRectSize.setWidth(qRound(zNumberWindowRectSize.width() * scaleFactor));
    }

    if (!nameWindowRectSize.isEmpty())
    {
        nameRectSize.setHeight(qRound(nameWindowRectSize.height() * scaleFactor));
        nameRectSize.setWidth(qRound(nameWindowRectSize.width() * scaleFactor));
    }

    if (!symbolWindowRectSize.isEmpty())
    {
        symbolRectSize.setHeight(qRound(symbolWindowRectSize.height() * scaleFactor));
        symbolRectSize.setWidth(qRound(symbolWindowRectSize.width() * scaleFactor));
    }
}
//======================================================
void X_PeriodicTableWidget::xh_applyPropertyRectSizes(const QSize &symbolRectSize,
                                                     const QSize &zNumberRectSize,
                                                     const QSize &nameRectSize)
{
    // set rects to chemical element button
    X_ChemicalElementButton::xp_setSymbolBoundingRectSize(symbolRectSize);
    X_ChemicalElementButton::xp_setX_NumberBoundingRectSize(zNumberRectSize);
    X_ChemicalElementButton::xp_setNameBoundingRectSize(nameRectSize);

    // apply rects to buttons
    foreach (X_ChemicalElementButton *button, xv_elementButtonList)
    {
        button->xp_updatePropertyCaptions();
    }
}
//======================================================
void X_PeriodicTableWidget::xh_buttonRectSizes(QSize &minButtonSize,
                                              QSize &maxWidthSymbolRectSize,
                                              QSize &maxWidthX_NumberRectSize,
                                              QSize &maxWidthNameRectSize) const
{
    if (xv_elementButtonList.isEmpty())
    {
        return;
    }
    // min button size
    int width = this->width();
    int height = this->height();

    // max width symbol rect
    int symbolWidth = 0;
    int symbolHeight = 0;
    // max width symbol rect
    int zNumberWidth = 0;
    int zNumberHeight = 0;
    // max width symbol rect
    int nameWidth = 0;
    int nameHeight = 0;

    QSize size;
    foreach (X_ChemicalElementButton *button, xv_elementButtonList)
    {
        // minButtonSize
        size = button->xp_buttonViewPortRect().size();
        if (height > size.height())
        {
            height = size.height();
        }
        if (width > size.width())
        {
            width = size.width();
        }

        // symbol rect
        size = button->xp_symbolWindowRectSize();
        if (symbolWidth < size.width())
        {
            symbolWidth = size.width();
        }
        if (symbolHeight < size.height())
        {
            symbolHeight = size.height();
        }

        // X_ rect
        size = button->xp_zNumberWindowRectSize();
        if (zNumberWidth < size.width())
        {
            zNumberWidth = size.width();
        }
        if (zNumberHeight < size.height())
        {
            zNumberHeight = size.height();
        }

        // name rect
        size = button->xp_nameWindowRectSize();
        if (nameWidth < size.width())
        {
            nameWidth = size.width();
        }
        if (nameHeight < size.height())
        {
            nameHeight = size.height();
        }
    }

    minButtonSize = QSize(width, height);
    maxWidthSymbolRectSize = QSize(symbolWidth, symbolHeight);
    maxWidthX_NumberRectSize = QSize(zNumberWidth, zNumberHeight);
    maxWidthNameRectSize = QSize(nameWidth, nameHeight);
}
//======================================================
void X_PeriodicTableWidget::xh_maxSymbolRectSize(QSize &symbolSize) const
{
    int symbolWidth = 0;
    int symbolHeight = 0;

    QSize size;
    foreach (X_ChemicalElementButton *button, xv_elementButtonList)
    {
        // symbol rect
        size = button->xp_symbolWindowRectSize();
        if (symbolWidth < size.width())
        {
            symbolWidth = size.width();
        }
        if (symbolHeight < size.height())
        {
            symbolHeight = size.height();
        }
    }

    symbolSize = QSize(symbolWidth, symbolHeight);
}
//======================================================
bool X_PeriodicTableWidget::xh_buttonForX_Number(X_Number zNumber, X_ChemicalElementButton *&button)
{
    foreach (X_ChemicalElementButton *currentButton, xv_elementButtonList)
    {
        if (currentButton->xp_zNumber() == zNumber)
        {
            button = currentButton;
            return true;
        }
    }

    button = nullptr;
    return false;
}
//======================================================


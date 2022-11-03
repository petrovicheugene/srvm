//======================================================
#ifndef X_PERIODICTABLEWIDGET_H
#define X_PERIODICTABLEWIDGET_H
//======================================================
#include <QWidget>

#include "X_ChemicalElementButton.h"
#include <QModelIndex>
//======================================================
class QAbstractItemModel;
class QResizeEvent;
class QVBoxLayout;

//======================================================
class X_PeriodicTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit X_PeriodicTableWidget(QWidget *parent = nullptr);

    enum SelectionMode {SM_NO_SELECTION,
                        SM_SINGLE_SELECTION,
                        SM_MULTIPLE_SELECTION};

    void xp_setSelectionMode(SelectionMode selectionMode);
    SelectionMode xp_selectionMode();
    void xp_setMainLayoutMargin(int margin);
    void xp_setModel(QAbstractItemModel* model);

    QList<int> xp_selectedChemicalElementList() const;


signals:

    void xg_selectedChemicalElementChanged(int zNumber, bool selected);

public slots:

    void xp_fillSelectedChemicalElementList(QList<int>& X_NumberList) const;

protected:

    void resizeEvent(QResizeEvent* event);

private slots:

    void zh_updatePeriodicTable();
    void zh_updateButtonCaptions(const QModelIndex& topLeft,
                                 const QModelIndex& bottomRight);
    void zh_recalcAndApplyMinButtonSize();

    void zh_onButtonToggle(bool toggled);

private:

    // VARS
    QAbstractItemModel* xv_model;
    QVBoxLayout* xv_mainLayout;
    QList<X_ChemicalElementButton*> xv_elementButtonList;
    SelectionMode xv_selectionMode;

    // FUNCS
    void zh_createComponents();
    void zh_createConnections();

    bool zh_buttonForX_Number(X_Number zNumber,  X_ChemicalElementButton *&button);
    bool zh_setChemicalElementSymbol(X_Number zNumber, const QString& symbol);
    void zh_recalcAndApplyPropertyRectSizes();
    void zh_recalcPropertyRectSizes(QSize &symbolRectSize,
                                    QSize &zNumberRectSize,
                                    QSize &nameRectSize) const;
    void zh_applyPropertyRectSizes(const QSize& symbolRectSize,
                                   const QSize& zNumberRectSize,
                                   const QSize& nameRectSize);
    void zh_buttonRectSizes(QSize &minButtonSize, QSize &maxWidthSymbolRectSize, QSize &maxWidthX_NumberRectSize, QSize &maxWidthNameRectSize) const;
    void zh_maxSymbolRectSize(QSize& symbolSize) const;


};
//======================================================
#endif // X_PERIODICTABLEWIDGET_H

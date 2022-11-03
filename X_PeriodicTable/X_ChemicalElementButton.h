//======================================================
#ifndef X_CHEMICALELEMENTBUTTON_H
#define X_CHEMICALELEMENTBUTTON_H
//======================================================
#include <QToolButton>

#include <QMap>
//======================================================
class QFrame;
class QPaintEvent;
class QResizeEvent;
//======================================================
typedef  unsigned int X_Number;
//======================================================
class X_ChemicalElementButton : public QToolButton
{
    Q_OBJECT
public:
    explicit X_ChemicalElementButton(X_Number zNumber, QWidget *parent = nullptr);
    ~X_ChemicalElementButton();

    enum PropertyName {PN_SYMBOL,
                       PN_X_NUMBER,
                       PN_NAME};

    static QSize xp_symbolBoundingRectSize();
    static QSize xp_zNumberBoundingRectSize();
    static QSize xp_nameBoundingRectSize();
    static void xp_setSymbolBoundingRectSize(QSize size);
    static void xp_setX_NumberBoundingRectSize(QSize size);
    static void xp_setNameBoundingRectSize(QSize size);
    static int xp_smallFontSizeRatio();
    static bool xp_setSmallFontSizeRatio(int);

    X_Number xp_zNumber() const;

    void xp_setChemicalElementSymbol(const QString& symbol);
    QString xp_chemicalElementSymbol() const;

    void xp_setChemicalElementName(const QString& name);
    QString xp_chemicalElementName() const;

    void xp_updatePropertyCaptions();

    QRect xp_buttonViewPortRect() const;
    QSize xp_symbolWindowRectSize() const;
    QSize xp_zNumberWindowRectSize() const;
    QSize xp_nameWindowRectSize() const;

    void xp_updateMinimumSizeForSymbolRectSize(QSize symbolSize);
    void xp_setPropertyFont(PropertyName proprtyName, QFont font);

signals:

    void xg_symbolChanged(const QString& symbol);

public slots:


protected:

    void resizeEvent(QResizeEvent* event);
    void paintEvent(QPaintEvent *event);

private slots:

    void zh_onAppFontDatabaseChange();

private:

    // VARS
    static QSize xv_symbolBoundingRectSize;
    static QSize xv_zNumberBoundingRectSize;
    static QSize xv_nameBoundingRectSize;
    static double xv_smallFontSizeRatio;

    //QMap<PropertyName, QColor> xv_colorMap;
    QMap<PropertyName, QFont> xv_fontMap;

    QFrame* xv_frame;
    int xv_frameLineWidth;
    int xv_margin;

    X_Number xv_zNumber;
    QString xv_symbol;
    QString xv_name;

    QRect xv_symbolViewPortRect;
    QRect xv_symbolWindowRect;

    QRect xv_zNumberViewPortRect;
    QRect xv_zNumberWindowRect;

    QRect xv_nameViewPortRect;
    QRect xv_nameWindowRect;


    // FUNCS
    void zh_setDefaultFonts();
    void zh_setDefaultColors();
    void zh_createConnections();

    void xp_updatePropertyViewPortRects();
    void zh_recalcPropertyWindowRects();
    void zh_recalcViewPortRectSize(QSize boundingViewPortSize,
                                   QSize windowSize,
                                   QSize &viewPortSize) const;
    int zh_marginSpacing() const;

    QFont zh_symbolFont() const;
    QFont zh_zNumberFont() const;
    QFont zh_nameFont() const;

    QColor zh_textColor() const;
    QPalette zh_currentPalette() const;

};
//======================================================
#endif // X_CHEMICALELEMENTBUTTON_H

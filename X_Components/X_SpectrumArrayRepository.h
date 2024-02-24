//==================================================================
#ifndef X_SPECTRAARRAYREPOSITORY_H
#define X_SPECTRAARRAYREPOSITORY_H
//==================================================================
#include <QObject>
#include <QList>
#include <QMap>
#include <QMenu>
#include "X_RawSpectrumArray.h"
#include "X_SpectrumArray.h"
#include <X_PasteData.h>

//==================================================================
class QAction;
class X_FileActionManager;
//==================================================================
class X_SpectrumArrayRepository : public QObject
{
    Q_OBJECT
public:
    explicit X_SpectrumArrayRepository(QObject *parent = nullptr);

    // VARS
    enum ArrayOperationType {AOT_INSERT_ARRAYS,
                             AOT_END_INSERT_ARRAYS,
                             AOT_REMOVE_ARRAYS,
                             AOT_END_REMOVE_ARRAYS,
                             AOT_DATA_CHANGED
                            };

    enum SpectrumOperationType {SOT_BEGIN_INSERT_SPECTRA,
                                SOT_END_INSERT_SPECTRA,
                                SOT_BEGIN_REMOVE_SPECTRA,
                                SOT_END_REMOVE_SPECTRA,
                                SOT_VISIBLE_CHANGED,
                                SOT_CHECKED_CHANGED,
                                SOT_CONCENTRATION_CHANGED
                               };

    enum ChemElementOperationType {CEOT_INSERT_CHEM_ELEMENT,
                                   CEOT_END_INSERT_CHEM_ELEMENT,
                                   CEOT_REMOVE_CHEM_ELEMENT,
                                   CEOT_END_REMOVE_CHEM_ELEMENT,
                                   CEOT_CHEM_ELEMENT_CHANGED,
                                   CEOT_CHEM_ELEMENT_VISIBILITY_CHANGE,
                                   CEOT_END_CHEM_ELEMENT_VISIBILITY_CHANGE
                                  };


    // FUNCS
    void xp_appendActionsToMenu(QMenu *menu) const;
    QList<QAction*> xp_arrayActions() const;
    QList<QAction*> xp_arrayContextMenuActions() const;

    QList<QAction*>xp_spectrumActions() const;
    QList<QAction*>xp_spectrumContextMenuActions() const;

    QList<QAction*>xp_chemElementActions() const;
    QList<QAction*>xp_chemElementContextMenuActions() const;

    void xp_connectToFileActionManager(X_FileActionManager*);

    bool xp_isEmpty() const;
    void xp_clear();
    bool xp_isDirty() const;

    const X_SpectrumArray* xp_array(int arrayIndex);
    int xp_arrayCount() const;
    QString xp_arrayName(int arrayIndex) const;
    bool xp_setSpectrumArrayName(int arrayIndex, const QString&);

    int xp_spectrumCount(int arrayIndex) const;
    QString xp_spectrumName(int arrayIndex, int spectrumIndex) const;
    qint64 xp_spectrumId(int arrayIndex, int spectrumIndex) const;
    QColor xp_spectrumColor(int arrayIndex, int spectrumIndex) const;
    bool xp_isSpectrumVisible(int arrayIndex, int spectrumIndex) const;
    bool xp_setSpectrumVisible(int arrayIndex, int spectrumIndex, bool visible);

    bool xp_isSpectrumVisible(qint64 arrayId, qint64 spectrumId) const;
    bool xp_setSpectrumVisible(qint64 arrayId, qint64 spectrumId, bool visible);

    bool xp_isSpectrumCheckedForId(qint64 arrayId, qint64 spectrumId) const;
    bool xp_isSpectrumChecked(int arrayIndex, int spectrumIndex) const;
    bool xp_setSpectrumChecked(int arrayIndex, int spectrumIndex, bool checked);
    bool xp_setSpectrumCheckedForId(qint64 arrayId, qint64 spectrumId, bool checked);
    void xp_setSpectrumCurrent(qint64 spectrumId) const;

    const X_AbstractSpectrum *xp_spectrum(int arrayIndex, int spectrumIndex) const;
    QList<X_AbstractSpectrum*> xp_spectrumListForArray(int arrayIndex) const;
    QList<quint32> xp_spectrumData(int arrayIndex, int spectrumIndex) const;
    int xp_arrayChannelCount(int arrayIndex);
    int xp_arrayMaxIntensity(int arrayIndex);

    int xp_chemElementCount(int arrayIndex) const;
    int xp_visibleChemElementCount(int arrayIndex) const;
    int xp_numberVisibleChemElementsBeforeIndex(int arrayindex, int index) const;
    QStringList xp_chemElementList(int arrayIndex) const;
    QString xp_visibleChemElementName(int arrayIndex, int visibleIndex) const;
    QString xp_chemElementName(int arrayIndex, int row) const;
    qint64 xp_visibleChemElementId(int arrayIndex, int visibleIndex) const;
    qint64 xp_chemElementId(int arrayIndex, int row) const;
    qint64 xp_chemElementIdForName(int arrayIndex, const QString&) const;

    bool xp_chemElementIsVisible(int arrayIndex, int row) const;
    bool xp_setChemElementVisible(int arrayIndex, int row, bool visible) const;

    QString xp_chemConcentration(int arrayIndex,
                                 int spectrumIndex, int visibleChemElementIndex) const;

    bool xp_setChemConcentration(int arrayIndex,
                                 int spectrumIndex, int visibleChemElementIndex, const QString &concentration);
    bool xp_setChemElementName(int arrayIndex, int chemElementIndex, const QString&);
 //   bool xp_energyCalibration(int arrayIndex, qreal& K0, qreal& K1, qreal& K2, QString& energyUnit);
   // bool xp_energyCalibrationForArrayId(qint64 arrayId, qreal& K0, qreal& K1, qreal& K2, QString& energyUnit);

    int xp_gainFactor(int arrayIndex) const;
    bool xp_setGainFactor(int arrayIndex, int gainFactor);

    int xp_exposition(int arrayIndex);
    bool xp_expositionForArrayId(qint64 arrayId, int& exposition);

    qint64 xp_arrayIdForArrayIndex(int arrayIndex) const;

    const X_AbstractSpectrum *xp_spectrumForId(qint64 id) const;

signals:

    void xg_message(QString) const;
    void xg_spectrumArrayOperation(ArrayOperationType, int, int) const;
    void xg_spectrumOperation(SpectrumOperationType, int arrayIndex, int first, int last) const;
    void xg_chemElementOperation(ChemElementOperationType, int arrayIndex, int first, int last) const;

    void xg_currentFile(bool dirty, QString fileName) const;
    void xg_fitPlotInBoundingRect();

    void xg_setCurrentArrayIndex(int arrayIndex) const;
    void xg_setCurrentChemElementIndex(int chemElementIndex) const;
    void xg_setCurrentSpectrumIndex(int spectrumIndex) const;

    void xg_requestCurrentArrayIndex(int& arrayIndex) const;
    void xg_requestSelectedSpectrumIndexList(QList<int>&);
    void xg_requestCurrentChemConcentrationCellIndex(int& row, int& visibleChemColumn);
    void xg_requestSelectedChemElementIndexList(QList<int>&);

    void xg_startCurrentArrayEdition();
    void xg_startCurrentChemElementEdition();

    void xg_saveSpectraArrayList(QString&, QList<X_RawSpectrumArray>, bool& res) const;
    void xg_initSpectraAppending(int);

    void xg_currentArrayIsAboutChange(qint64 arrayId, int arrayIndex);
    void xg_currentArrayIdChanged(qint64 arrayId, int arrayIndex);
    void xg_currentSpectrumChanged(qint64 currentSpectrumId, int currentSpectrumIndex,
                                   qint64 previousSpectrumId, int previousSpectrumIndex);

    void xg_energyCalibrationChanged(QList<double> energyCalibration) const;
    void xg_arrayMaxParametersChanged(qint64 arrayId, int intensity, int channels);
    void xg_requestIsPlotScaled(bool& isScaled);

    void xg_requestClearSelected() const;
    void xg_requestSelectedString(QString&) const;

    void xg_arrayListDirtyChanged(bool dirty, bool currentArrayExists) const;

    void xg_requestCurrentSpectrumRow(int& currentRow, bool* ok) const;

public slots:

    void xp_getArrayCount(int&) const;
    void xp_getSpectrumCount(int arrayIndex, int&) const;
    void xp_getArrayName(int arrayIndex, QString&) const;
    void xp_getSpectrumName(int arrayIndex, int spectrumIndex, QString&) const;
    void xp_onCurrentArrayChanged(int current, int previous);
    void xp_onCurrentSpectrumChanged(int currentSpectrumIndex, int previousSpectrumIndex);
    void xp_onSelectedChemElementChange();
    void xp_chemElementListForCurrentArray(QStringList& chemElementList);

    void xp_onSelectionSpectraChange(bool selectionEnabled,
                                     bool concentrationSelected,
                                     bool spectrumSelected);

    void xp_currentSpectrumWindowIntensity(int firstChannel,
                                           int lastChennel, qreal &intensity) const;

private slots:

    void xp_appendArrays(QString path, QList<X_RawSpectrumArray>);
    void xp_appendSpectraToArray(QStringList);
    void xh_onSpectrumArraySaving(QString);

    void xh_onAppendArrayAction();
    void xh_onRemoveArrayAction();
    void xh_onAppendSpectraToArrayAction();
    void xh_onRemoveSpectrumFromArrayAction();
    void xh_onAppendChemElementAction();
    void xh_onRemoveChemElementAction();

    void xh_onChemElementOperation(X_ChemElementList::OperationType, int, int);
    void xh_createRawArrayListAndStartSaving(QString &filePath, bool &res) const;
    void xh_onSpectrumOperation(X_SpectrumArray::OperationType type, int first, int last) const;

    void xh_onClipboardContentChange();

    void xh_onPasteConcentrationDataAction();
    void xh_onCopyConcentrationDataAction();
    void xh_onClearConcentrationDataAction();

    void xh_onSetSpectraVisibleAction();
    void xh_onSetSpectraInvisibleAction();
    void xh_onInvertSpectraVisibilityAction();

    void xh_onSetChemElementsVisibleAction();
    void xh_onSetChemElementsInvisibleAction();
    void xh_onInvertChemElementsVisibilityAction();
    void xh_onEnergyCalibrationAction();

    void xh_writeEnergyCalibrationToSpectra(int gainFactor,
                                   const QList<double> &energyCalibrationFactors);

private:

    // VARS
    QList<X_SpectrumArray*> xv_arrayList;
    QString xv_arrayFilePath;
    bool xv_dirty;
    QString xv_defaultArrayBaseName;
    const QString xv_messageBoxPasteTitle = tr("Paste concentration");
    int xv_currentArrayIndex = -1;

    QAction* xv_appendArrayAction;
    QAction* xv_removeArrayAction;
    QAction* xv_openArrayAction;
    QAction* xv_saveArrayAction;
    QAction* xv_saveAsArrayAction;
    QAction* xv_energyCalibrationAction;

    QAction* xv_appendSpectrumToArrayAction;
    QAction* xv_removeSpectrumFromArrayAction;
    QAction* xv_appendChemElementAction;
    QAction* xv_removeChemElementAction;

    QAction* xv_pasteConcentrationDataAction;
    QAction* xv_copyConcentrationDataAction;
    QAction* xv_clearConcentrationDataAction;

    QAction* xv_setSpectraVisibleAction;
    QAction* xv_setSpectraInvisibleAction;
    QAction* xv_invertSpectraVisibilityAction;

    QAction* xv_setChemElementsVisibleAction;
    QAction* xv_setChemElementsInvisibleAction;
    QAction* xv_invertChemElementsVisibilityAction;

    X_PasteData xv_pasteData;

    // FUNCS
    void xh_createActions();
    void xh_createConnections();
    void xh_createArray(const X_RawSpectrumArray&);

    bool xh_removeArray(int);
    bool xh_removeSpectrum(int, int);
    bool xh_removeChemicalElement(int, int);
    int xh_chemElementIndexForId(int arrayIndex, qint64 chemElementId);
    QList<X_RawSpectrumArray> xh_createRawArrayList() const;
    void xh_actionEnablingControl();

    void xh_setDirty(bool dirty);
};
//==================================================================
#endif // X_SPECTRAARRAYREPOSITORY_H

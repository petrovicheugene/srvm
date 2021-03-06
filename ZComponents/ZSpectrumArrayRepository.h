//==================================================================
#ifndef ZSPECTRAARRAYREPOSITORY_H
#define ZSPECTRAARRAYREPOSITORY_H
//==================================================================
#include <QObject>
#include <QList>
#include <QMap>
#include <QMenu>
#include "ZRawSpectrumArray.h"
#include "ZSpectrumArray.h"
#include <ZPasteData.h>

//==================================================================
class QAction;
class ZFileActionManager;
//==================================================================
class ZSpectrumArrayRepository : public QObject
{
    Q_OBJECT
public:
    explicit ZSpectrumArrayRepository(QObject *parent = nullptr);

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
    void zp_appendActionsToMenu(QMenu *menu) const;
    QList<QAction*> zp_arrayActions() const;
    QList<QAction*> zp_arrayContextMenuActions() const;

    QList<QAction*>zp_spectrumActions() const;
    QList<QAction*>zp_spectrumContextMenuActions() const;

    QList<QAction*>zp_chemElementActions() const;
    QList<QAction*>zp_chemElementContextMenuActions() const;

    void zp_connectToFileActionManager(ZFileActionManager*);

    bool zp_isEmpty() const;
    void zp_clear();
    bool zp_isDirty() const;

    const ZSpectrumArray* zp_array(int arrayIndex);
    int zp_arrayCount() const;
    QString zp_arrayName(int arrayIndex) const;
    bool zp_setSpectrumArrayName(int arrayIndex, const QString&);

    int zp_spectrumCount(int arrayIndex) const;
    QString zp_spectrumName(int arrayIndex, int spectrumIndex) const;
    qint64 zp_spectrumId(int arrayIndex, int spectrumIndex) const;
    QColor zp_spectrumColor(int arrayIndex, int spectrumIndex) const;
    bool zp_isSpectrumVisible(int arrayIndex, int spectrumIndex) const;
    bool zp_setSpectrumVisible(int arrayIndex, int spectrumIndex, bool visible);

    bool zp_isSpectrumVisible(qint64 arrayId, qint64 spectrumId) const;
    bool zp_setSpectrumVisible(qint64 arrayId, qint64 spectrumId, bool visible);

    bool zp_isSpectrumCheckedForId(qint64 arrayId, qint64 spectrumId) const;
    bool zp_isSpectrumChecked(int arrayIndex, int spectrumIndex) const;
    bool zp_setSpectrumChecked(int arrayIndex, int spectrumIndex, bool checked);
    bool zp_setSpectrumCheckedForId(qint64 arrayId, qint64 spectrumId, bool checked);
    void zp_setSpectrumCurrent(qint64 spectrumId) const;

    const ZAbstractSpectrum* zp_spectrum(int arrayIndex, int spectrumIndex) const;
    QList<ZAbstractSpectrum*> zp_spectrumListForArray(int arrayIndex) const;
    QList<quint32> zp_spectrumData(int arrayIndex, int spectrumIndex) const;
    int zp_arrayChannelCount(int arrayIndex);
    int zp_arrayMaxIntensity(int arrayIndex);

    int zp_chemElementCount(int arrayIndex) const;
    int zp_visibleChemElementCount(int arrayIndex) const;
    int zp_numberVisibleChemElementsBeforeIndex(int arrayindex, int index) const;
    QStringList zp_chemElementList(int arrayIndex) const;
    QString zp_visibleChemElementName(int arrayIndex, int visibleIndex) const;
    QString zp_chemElementName(int arrayIndex, int row) const;
    qint64 zp_visibleChemElementId(int arrayIndex, int visibleIndex) const;
    qint64 zp_chemElementId(int arrayIndex, int row) const;
    qint64 zp_chemElementIdForName(int arrayIndex, const QString&) const;

    bool zp_chemElementIsVisible(int arrayIndex, int row) const;
    bool zp_setChemElementVisible(int arrayIndex, int row, bool visible) const;

    QString zp_chemConcentration(int arrayIndex,
                                 int spectrumIndex, int visibleChemElementIndex) const;

    bool zp_setChemConcentration(int arrayIndex,
                                 int spectrumIndex, int visibleChemElementIndex, const QString &concentration);
    bool zp_setChemElementName(int arrayIndex, int chemElementIndex, const QString&);
 //   bool zp_energyCalibration(int arrayIndex, qreal& K0, qreal& K1, qreal& K2, QString& energyUnit);
   // bool zp_energyCalibrationForArrayId(qint64 arrayId, qreal& K0, qreal& K1, qreal& K2, QString& energyUnit);

    int zp_gainFactor(int arrayIndex) const;
    bool zp_setGainFactor(int arrayIndex, int gainFactor);

    int zp_exposition(int arrayIndex);
    bool zp_expositionForArrayId(qint64 arrayId, int& exposition);

    qint64 zp_arrayIdForArrayIndex(int arrayIndex) const;

    const ZAbstractSpectrum *zp_spectrumForId(qint64 id) const;

signals:

    void zg_message(QString) const;
    void zg_spectrumArrayOperation(ArrayOperationType, int, int) const;
    void zg_spectrumOperation(SpectrumOperationType, int arrayIndex, int first, int last) const;
    void zg_chemElementOperation(ChemElementOperationType, int arrayIndex, int first, int last) const;

    void zg_currentFile(bool dirty, QString fileName) const;
    void zg_fitPlotInBoundingRect();

    void zg_setCurrentArrayIndex(int arrayIndex) const;
    void zg_setCurrentChemElementIndex(int chemElementIndex) const;
    void zg_setCurrentSpectrumIndex(int spectrumIndex) const;

    void zg_requestCurrentArrayIndex(int& arrayIndex) const;
    void zg_requestSelectedSpectrumIndexList(QList<int>&);
    void zg_requestCurrentChemConcentrationCellIndex(int& row, int& visibleChemColumn);
    void zg_requestSelectedChemElementIndexList(QList<int>&);

    void zg_startCurrentArrayEdition();
    void zg_startCurrentChemElementEdition();

    void zg_saveSpectraArrayList(QString&, QList<ZRawSpectrumArray>, bool& res) const;
    void zg_initSpectraAppending(int);

    void zg_currentArrayIsAboutChange(qint64 arrayId, int arrayIndex);
    void zg_currentArrayIdChanged(qint64 arrayId, int arrayIndex);
    void zg_currentSpectrumChanged(qint64 currentSpectrumId, int currentSpectrumIndex,
                                   qint64 previousSpectrumId, int previousSpectrumIndex);

    void zg_energyCalibrationChanged(QList<double> energyCalibration) const;
    void zg_arrayMaxParametersChanged(qint64 arrayId, int intensity, int channels);
    void zg_requestIsPlotScaled(bool& isScaled);

    void zg_requestClearSelected() const;
    void zg_requestSelectedString(QString&) const;

    void zg_arrayListDirtyChanged(bool dirty, bool currentArrayExists) const;

    void zg_requestCurrentSpectrumRow(int& currentRow, bool* ok) const;

public slots:

    void zp_getArrayCount(int&) const;
    void zp_getSpectrumCount(int arrayIndex, int&) const;
    void zp_getArrayName(int arrayIndex, QString&) const;
    void zp_getSpectrumName(int arrayIndex, int spectrumIndex, QString&) const;
    void zp_currentArrayChanged(int current, int previous);
    void zp_currentSpectrumChanged(int currentSpectrumIndex, int previousSpectrumIndex);
    void zp_chemElementListForCurrentArray(QStringList& chemElementList);

    void zp_onSelectionSpectraChange(bool selectionEnabled,
                                     bool concentrationSelected,
                                     bool spectrumSelected);

    void zp_currentSpectrumWindowIntensity(int firstChannel,
                                           int lastChennel, qreal &intensity) const;


private slots:

    void zp_appendArrays(QString path, QList<ZRawSpectrumArray>);
    void zp_appendSpectraToArray(int, QStringList);
    void zh_onSpectrumArraySaving(QString);

    void zh_onAppendArrayAction();
    void zh_onRemoveArrayAction();
    void zh_onAppendSpectraToArrayAction();
    void zh_onRemoveSpectrumFromArrayAction();
    void zh_onAppendChemElementAction();
    void zh_onRemoveChemElementAction();

    void zh_onChemElementOperation(ZChemElementList::OperationType, int, int);
    void zh_createRawArrayListAndStartSaving(QString &filePath, bool &res) const;
    void zh_onSpectrumOperation(ZSpectrumArray::OperationType type, int first, int last) const;

    void zh_onClipboardContentChange();

    void zh_onPasteConcentrationDataAction();
    void zh_onCopyConcentrationDataAction();
    void zh_onClearConcentrationDataAction();

    void zh_onSetSpectraVisibleAction();
    void zh_onSetSpectraInvisibleAction();
    void zh_onInvertSpectraVisibilityAction();

    void zh_onSetChemElementsVisibleAction();
    void zh_onSetChemElementsInvisibleAction();
    void zh_onInvertChemElementsVisibilityAction();
    void zh_onEnergyCalibrationAction();

    void zh_writeEnergyCalibrationToSpectra(int gainFactor,
                                   const QList<double> &energyCalibrationFactors);

private:

    // VARS
    QList<ZSpectrumArray*> zv_arrayList;
    QString zv_arrayFilePath;
    bool zv_dirty;
    QString zv_defaultArrayBaseName;
    const QString zv_messageBoxPasteTitle = tr("Paste concentration");

    QAction* zv_appendArrayAction;
    QAction* zv_removeArrayAction;
    QAction* zv_openArrayAction;
    QAction* zv_saveArrayAction;
    QAction* zv_saveAsArrayAction;
    QAction* zv_energyCalibrationAction;

    QAction* zv_appendSpectrumToArrayAction;
    QAction* zv_removeSpectrumFromArrayAction;
    QAction* zv_appendChemElementAction;
    QAction* zv_removeChemElementAction;

    QAction* zv_pasteConcentrationDataAction;
    QAction* zv_copyConcentrationDataAction;
    QAction* zv_clearConcentrationDataAction;

    QAction* zv_setSpectraVisibleAction;
    QAction* zv_setSpectraInvisibleAction;
    QAction* zv_invertSpectraVisibilityAction;

    QAction* zv_setChemElementsVisibleAction;
    QAction* zv_setChemElementsInvisibleAction;
    QAction* zv_invertChemElementsVisibilityAction;

    ZPasteData zv_pasteData;

    // FUNCS
    void zh_createActions();
    void zh_createConnections();
    void zh_createArray(const ZRawSpectrumArray&);

    bool zh_removeArray(int);
    bool zh_removeSpectrum(int, int);
    bool zh_removeChemicalElement(int, int);
    int zh_chemElementIndexForId(int arrayIndex, qint64 chemElementId);
    QList<ZRawSpectrumArray> zh_createRawArrayList() const;
    void zh_actionEnablingControl();

    void zh_setDirty(bool dirty);
};
//==================================================================
#endif // ZSPECTRAARRAYREPOSITORY_H

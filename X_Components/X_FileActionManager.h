//======================================================
#ifndef X_FILEACTIONMANAGER_H
#define X_FILEACTIONMANAGER_H
//======================================================
#include <QObject>
#include <QFile>
#include "X_SpectrumArray.h"
#include "X_RawSpectrumArray.h"
//======================================================
class QAction;
class QMenu;
class X_Calibration;
//======================================================
class X_FileActionManager : public QObject
{
    Q_OBJECT
public:
    explicit X_FileActionManager(QObject *parent = 0);

    void xp_appendActionsToMenu(QMenu*) const;
    QList<QAction*> xp_actionList() const;

    void xp_initOpenCalibrationFromFileAction(QAction*&) const;
    void xp_initSaveCalibrationFromFileAction(QAction*&) const;
    void xp_initSaveAsCalibrationFromFileAction(QAction*&) const;

    void xp_initOpenSpectrumArrayAction(QAction*&) const;
    void xp_initSaveSpectrumArrayAction(QAction*&) const;
    void xp_initSaveAsSpectrumArrayAction(QAction*&) const;

    void xp_triggerSaveArrayToFileAction();
    void xp_triggerSaveCalibrationsToFile();

signals:

    void xg_message(QString) const;
    void xg_spectrumRawArrayList(QString, QList<X_RawSpectrumArray>) const;
    void xg_requestRawArrayListAndInitSaving(QString&, bool& res) const;

    void xg_spectrumFileListToOpen(int, QStringList) const;

    void xg_calibrationFileListToOpen(QStringList) const;
    void xg_requestCalibrationDataAndInitSaving(QString path, QString name) const;

    void xg_spectrumArraySaved(QString) const;
    void xg_calibrationSaved(const X_Calibration* calibration,
                             QString absFilePath) const;

    //    void xg_saveCalibrationActionEnabledChanged(bool enabled);
    //    void xg_saveCalibrationAsActionEnabledChanged(bool enabled);

public slots:

    void xp_defineSpectrumFilesAndInitAppending(int arrayIndex);
    void xp_saveSettings() const;
    void xp_openCalibrations() const;

    void xp_saveSpectraArrayListToFile(QString &filePath, QList<X_RawSpectrumArray> rawArrayList, bool &res);
    void xp_saveCalibrationToFile(const X_Calibration *calibration, QString filePath, QString name);

    void xp_onArrayListDirtyChange(bool dirty, bool currentArrayExists);
    void xp_onCurrentCalibrationDirtyChange(bool dirty, bool currentCalibrationExists);

private:

    // VARS
    QAction* xv_openArrayFromFileAction;
    QAction* xv_saveArrayToFileAction;
    QAction* xv_saveArrayAsFileAction;

    QAction* xv_openCalibrationFromFileAction;
    QAction* xv_saveCalibrationToFileAction;
    QAction* xv_saveCalibrationAsFileAction;

    QAction* xv_openCalibrationProjectAction;

    QString xv_spectrumArrayFolderPath;
    QString xv_calibrationFolderPath;
    QString xv_spectrumFolderPath;

    const QString xv_projectFileSuffix = "proc";
    const QString xv_spectrumArrayFileSuffix = "spar";
    const QString xv_calibrationFileSuffix = "clbx";

    // FUNCS
    void zh_createActions();
    void zh_createConnections();

    void zh_restoreSettings();

    bool zh_defineSpectrumArrayFileNameToOpen(QString&) const;
    bool zh_defineSpectrumArrayFileNameToSave(QString& fileName) const;

    bool zh_defineCalibrationFileNamesToOpen(QStringList& fileNameList) const;
    bool zh_defineCalibrationFileNameToSave(QString& fileName) const;
    bool zh_checkFile(const QString&) const;

    // bool zh_getSpectrumArrayFromFile(const QString& fileName, X_SpectrumArray& array);
    // bool zh_getSpectrumFromFile(const QString&, QSharedPointer<X_AbstractSpectrum> &);
    // bool zh_getSpectrumFromFile(const QString&, X_AbstractSpectrum *&);
    bool zh_getRawSpectrumArrayFromFile(const QString& fileName, QList<X_RawSpectrumArray>& rawArray);
    // bool zh_getRawCalibrationArrayFromFile(const QString& fileName, QList<X_RawCalibrationArray>& rawArray);
    void zh_saveCalibrationProjectFile(const QString& absFileName);


private slots:

    void zh_onOpenSpectrumArrayAction();
    void zh_onSaveSpectrumArrayAction() const;
    void zh_onSaveSpectrumArrayAsAction() const;

    void zh_onOpenCalibrationAction();
    void zh_onSaveCalibrationAction() const;
    void zh_onSaveCalibrationAsAction() const;
    void zh_onOpenCalibrationProjectAction();
    //    void zh_synchronizeSaveCalibrationActions();
    //    void zh_synchronizeSaveAsCalibrationActions();
};
//======================================================
#endif // X_FILEACTIONMANAGER_H

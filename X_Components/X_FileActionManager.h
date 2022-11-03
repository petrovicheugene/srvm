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

    void xg_spectrumFileListToOpen(QStringList) const;

    void xg_calibrationFileListToOpen(QStringList) const;
    void xg_requestCalibrationDataAndInitSaving(QString path, QString name) const;

    void xg_spectrumArraySaved(QString) const;
    void xg_calibrationSaved(const X_Calibration* calibration,
                             QString absFilePath) const;

    //    void xg_saveCalibrationActionEnabledChanged(bool enabled);
    //    void xg_saveCalibrationAsActionEnabledChanged(bool enabled);

public slots:

    void xp_defineSpectrumFilesAndInitAppending();
    void xp_saveSettings() const;
    void xp_openCalibrations() const;

    void xp_saveSpectraArrayListToFile(QString &filePath, QList<X_RawSpectrumArray> rawArrayList, bool &res);
    //void xp_initSavingSpectraArrayListToFile(QString &filePath, QList<X_RawSpectrumArray> rawArrayList, bool &res);
    void xp_initSavingCalibrationToFile(const X_Calibration *calibration, QString filePath, QString name);

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
    void xh_createActions();
    void xh_createConnections();

    void xh_restoreSettings();

    bool xh_defineSpectrumArrayFileNameToOpen(QString&) const;
    bool xh_defineSpectrumArrayFileNameToSave(QString& fileName) const;

    bool xh_defineCalibrationFileNamesToOpen(QStringList& fileNameList) const;
    bool xh_defineCalibrationFileNameToSave(QString& fileName) const;
    bool xh_checkFile(const QString&) const;

    // bool xh_getSpectrumArrayFromFile(const QString& fileName, X_SpectrumArray& array);
    // bool xh_getSpectrumFromFile(const QString&, QSharedPointer<X_AbstractSpectrum> &);
    // bool xh_getSpectrumFromFile(const QString&, X_AbstractSpectrum *&);
    bool xh_getRawSpectrumArrayFromFile(const QString& fileName, QList<X_RawSpectrumArray>& rawArray);
    // bool xh_getRawCalibrationArrayFromFile(const QString& fileName, QList<X_RawCalibrationArray>& rawArray);
    void xh_saveCalibrationProjectFile(const QString& absFileName);

private slots:

    void xh_onOpenSpectrumArrayAction();
    void xh_onSaveSpectrumArrayAction() const;
    void xh_onSaveSpectrumArrayAsAction() const;

    void xh_onOpenCalibrationAction();
    void xh_onSaveCalibrationAction() const;
    void xh_onSaveCalibrationAsAction() const;
    void xh_onOpenCalibrationProjectAction();

    void xh_handleOpenCalibrationFiles();
    void xh_handleOpenSpectrumArray();
    void xh_handleOpenSpectra();

    void xh_handleOpenCalibrationProject();
    void xh_handleSaveCalibrationFiles();
    void xh_handleSaveSpectrumArrayFiles();

    void xh_saveCalibrationToFile(const X_Calibration *calibration, QString absFileName);

    //    void xh_synchronizeSaveCalibrationActions();
    //    void xh_synchronizeSaveAsCalibrationActions();
};
//======================================================
#endif // X_FILEACTIONMANAGER_H

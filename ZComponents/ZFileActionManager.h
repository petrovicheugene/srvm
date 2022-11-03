//======================================================
#ifndef ZFILEACTIONMANAGER_H
#define ZFILEACTIONMANAGER_H
//======================================================
#include <QObject>
#include <QFile>
#include "ZSpectrumArray.h"
#include "ZRawSpectrumArray.h"
//======================================================
class QAction;
class QMenu;
class ZCalibration;
//======================================================
class ZFileActionManager : public QObject
{
    Q_OBJECT
public:
    explicit ZFileActionManager(QObject *parent = 0);

    void zp_appendActionsToMenu(QMenu*) const;
    QList<QAction*> zp_actionList() const;

    void zp_initOpenCalibrationFromFileAction(QAction*&) const;
    void zp_initSaveCalibrationFromFileAction(QAction*&) const;
    void zp_initSaveAsCalibrationFromFileAction(QAction*&) const;

    void zp_initOpenSpectrumArrayAction(QAction*&) const;
    void zp_initSaveSpectrumArrayAction(QAction*&) const;
    void zp_initSaveAsSpectrumArrayAction(QAction*&) const;

    void zp_triggerSaveArrayToFileAction();
    void zp_triggerSaveCalibrationsToFile();

signals:

    void zg_message(QString) const;
    void zg_spectrumRawArrayList(QString, QList<ZRawSpectrumArray>) const;
    void zg_requestRawArrayListAndInitSaving(QString&, bool& res) const;

    void zg_spectrumFileListToOpen(int, QStringList) const;

    void zg_calibrationFileListToOpen(QStringList) const;
    void zg_requestCalibrationDataAndInitSaving(QString path, QString name) const;

    void zg_spectrumArraySaved(QString) const;
    void zg_calibrationSaved(const ZCalibration* calibration,
                             QString absFilePath) const;

    //    void zg_saveCalibrationActionEnabledChanged(bool enabled);
    //    void zg_saveCalibrationAsActionEnabledChanged(bool enabled);

public slots:

    void zp_selectSpeFilesAndInitAppending(int arrayIndex);
    void zp_saveSettings() const;
    void zp_openCalibrations() const;

    void zp_saveSpectraArrayListToFile(QString &filePath, QList<ZRawSpectrumArray> rawArrayList, bool &res);
    void zp_saveCalibrationToFile(const ZCalibration *calibration, QString filePath, QString name);

    void zp_onArrayListDirtyChange(bool dirty, bool currentArrayExists);
    void zp_onCurrentCalibrationDirtyChange(bool dirty, bool currentCalibrationExists);

private:

    // VARS
    QAction* zv_openArrayFromFileAction;
    QAction* zv_saveArrayToFileAction;
    QAction* zv_saveArrayAsFileAction;

    QAction* zv_openCalibrationFromFileAction;
    QAction* zv_saveCalibrationToFileAction;
    QAction* zv_saveCalibrationAsFileAction;

    QAction* zv_openCalibrationProjectAction;

    QString zv_spectrumArrayFolderPath;
    QString zv_calibrationFolderPath;
    QString zv_spectrumFolderPath;

    const QString zv_projectFileSuffix = "proc";
    const QString zv_spectrumArrayFileSuffix = "spar";
    const QString zv_calibrationFileSuffix = "clbx";

    // FUNCS
    void zh_createActions();
    void zh_createConnections();

    void zh_restoreSettings();

    bool zh_defineSpectrumArrayFileNameToOpen(QString&) const;
    bool zh_defineSpectrumArrayFileNameToSave(QString& fileName) const;

    bool zh_defineCalibrationFileNamesToOpen(QStringList& fileNameList) const;
    bool zh_defineCalibrationFileNameToSave(QString& fileName) const;
    bool zh_checkFile(const QString&) const;

    // bool zh_getSpectrumArrayFromFile(const QString& fileName, ZSpectrumArray& array);
    // bool zh_getSpectrumFromFile(const QString&, QSharedPointer<ZAbstractSpectrum> &);
    // bool zh_getSpectrumFromFile(const QString&, ZAbstractSpectrum *&);
    bool zh_getRawSpectrumArrayFromFile(const QString& fileName, QList<ZRawSpectrumArray>& rawArray);
    // bool zh_getRawCalibrationArrayFromFile(const QString& fileName, QList<ZRawCalibrationArray>& rawArray);
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
#endif // ZFILEACTIONMANAGER_H

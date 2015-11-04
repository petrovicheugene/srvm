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
//======================================================
class ZFileActionManager : public QObject
{
    Q_OBJECT
public:
    explicit ZFileActionManager(QObject *parent = 0);

    void zp_appendActionsToMenu(QMenu*) const;
    QList<QAction*> zp_actionList() const;

signals:

    void zg_message(QString) const;
    void zg_spectrumRawArrayList(QString, QList<ZRawSpectrumArray>) const;
    void zg_requestRawArrayListAndInitSaving(QString) const;

    void zg_spectrumFileListToOpen(int, QStringList) const;
    void zg_calibrationFileListToOpen(QStringList) const;

public slots:

    void zp_defineSpectrumFilesAndInitAppending(int arrayIndex);
    void zp_saveSettings() const;
    void zp_openCalibrations() const;
    void zp_saveSpectraArrayList(QString filePath, QList<ZRawSpectrumArray> rawArrayList);

private:

    // VARS
    QAction* zv_openArrayFromFileAction;
    QAction* zv_saveArrayToFileAction;
    QAction* zv_saveArrayAsFileAction;

    QAction* zv_openCalibrationFromFileAction;
    QAction* zv_saveCalibrationToFileAction;
    QAction* zv_saveCalibrationAsFileAction;

    //QAction* zv_loadSpectrumFromFileAction;

    QString zv_spectrumArrayFolderPath;
    QString zv_calibrationFolderPath;
    QString zv_spectrumFolderPath;

    // FUNCS
    void zh_createActions();
    void zh_createConnections();

    void zh_restoreSettings();

    bool zh_defineSpectrumArrayFileNameToOpen(QString&) const;
    bool zh_defineSpectrumArrayFileNameToSave(QString& fileName) const;

    bool zh_defineCalibrationArrayFileName(QStringList &) const;
    bool zh_checkFile(const QString&) const;

    // bool zh_getSpectrumArrayFromFile(const QString& fileName, ZSpectrumArray& array);
    // bool zh_getSpectrumFromFile(const QString&, QSharedPointer<ZAbstractSpectrum> &);
    //bool zh_getSpectrumFromFile(const QString&, ZAbstractSpectrum *&);
    bool zh_getRawSpectrumArrayFromFile(const QString& fileName, QList<ZRawSpectrumArray>& rawArray);
    //bool zh_getRawCalibrationArrayFromFile(const QString& fileName, QList<ZRawCalibrationArray>& rawArray);

private slots:

    void zh_onOpenSpectrumArrayAction();
    void zh_onSaveSpectrumArrayAction() const;
    void zh_onSaveSpectrumArrayAsAction() const;

    void zh_onOpenCalibrationAction();
    void zh_onSaveCalibrationAction() const;
    void zh_onSaveCalibrationAsAction() const;
    // void zh_onOpenSpectraAction() const;

};
//======================================================
#endif // ZFILEACTIONMANAGER_H

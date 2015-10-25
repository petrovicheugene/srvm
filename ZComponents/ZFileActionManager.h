//======================================================
#ifndef ZFILEACTIONMANAGER_H
#define ZFILEACTIONMANAGER_H
//======================================================
#include <QObject>
#include <QFile>
#include "ZSpectrumArray.h"
#include "ZRawArray.h"
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

    void zp_restoreSettings();
    void zp_saveSettings() const;

signals:

    void zg_message(QString) const;
    void zg_arrayList(QString, QList<ZRawArray>) const;

public slots:

private:

    // VARS
    QAction* zv_openArrayFromFileAction;
    QAction* zv_saveArrayToFileAction;
    QAction* zv_saveArrayAsFileAction;

    QAction* zv_openCalibrationFromFileAction;
    QAction* zv_saveCalibrationToFileAction;
    QAction* zv_saveCalibrationAsFileAction;

    //QAction* zv_loadSpectrumFromFileAction;

    QString zv_arrayFilePath;

    // FUNCS
    void zh_createActions();
    void zh_createConnections();

    bool zh_defineArrayFileName(QString&) const;
    // bool zh_getSpectrumArrayFromFile(const QString& fileName, ZSpectrumArray& array);
    // bool zh_getSpectrumFromFile(const QString&, QSharedPointer<ZAbstractSpectrum> &);
    bool zh_getSpectrumFromFile(const QString&, ZAbstractSpectrum *&);
    bool zh_getRawSpectrumArrayFromFile(const QString& fileName, QList<ZRawArray>& rawArray);

private slots:

    void zh_onOpenArrayAction();
    void zh_onSaveArrayAction() const;
    void zh_onSaveArrayAsAction() const;

    void zh_onOpenCalibrationAction() const;
    void zh_onSaveCalibrationAction() const;
    void zh_onSaveCalibrationAsAction() const;
    // void zh_onOpenSpectraAction() const;

};
//======================================================
#endif // ZFILEACTIONMANAGER_H

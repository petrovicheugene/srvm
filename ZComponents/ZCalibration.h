//=========================================================
#ifndef QABSTRACTCALIBRATION_H
#define QABSTRACTCALIBRATION_H
//=========================================================
#include <QObject>
#include <QMap>
#include <QColor>
#include "ZCalibrationWindow.h"
#include "ZAbstractPredictor.h"
//=========================================================
class ZAbstractSpectrum;
class ZSimplePredictor;
class ZQuadraticPredictor;
class ZCrossProductPredictor;
class ZCustomPredictor;
class ZPredictorNormalizer;
//=========================================================
class ZCalibration : public QObject
{
   Q_OBJECT

public:

   explicit ZCalibration(const QString& name, QObject *parent = 0);
   virtual ~ZCalibration();

   // VARS
   enum WindowOperationType {WOT_BRGIN_INSERT_WINDOWS,
                             WOT_END_INSERT_WINDOWS,
                             WOT_BEGIN_REMOVE_WINDOWS,
                             WOT_END_REMOVE_WINDOWS,
                             WOT_WINDOW_CHANGED};


   enum PredictorOperationType {POT_BEGIN_INSERT_PREDICTOR,
                                POT_END_INSERT_PREDICTOR,
                                POT_BEGIN_REMOVE_PREDICTOR,
                                POT_END_REMOVE_PREDICTOR,
                                POT_PREDICTOR_CHANGED};

   // FUNCS
   QString zp_name() const;
   void zp_setBaseName(const QString&);

   QString zp_path() const;
   void zp_setPath(const QString&);

   QString zp_chemElement() const;
   bool zp_setChemElement(const QString&);

   bool zp_isVisible() const;
   void zp_setVisible(bool);

   bool zp_isDirty() const;
   QColor zp_color() const;
   qint64 zp_calibrationId() const;
   double zp_calcConcentration(const ZAbstractSpectrum* const, bool *ok = 0);

   // windows
   void zp_createNewCalibrationWindow(int firstChannel = 0, int lastChannel = 0);
   bool zp_isCalibrationWindowVisible(int windowIndex) const;
   bool zp_setCalibrationWindowVisible(int windowIndex, bool visibility);
   int zp_calibrationWindowCount() const;
   QString zp_calibrationWindowName(int windowIndex) const;
   const ZCalibrationWindow* zp_calibrationWindow(int windowIndex) const;

   bool zp_setCalibrationWindowName(int windowIndex, const QString&);
   ZCalibrationWindow::WindowType zp_calibrationWindowType(int windowIndex) const;
   bool zp_setCalibrationWindowType(int windowIndex, ZCalibrationWindow::WindowType type);

   int zp_calibrationWindowFirstChannel(int windowIndex) const;
   bool zp_setCalibrationWindowFirstChannel(int windowIndex, int channel);
   int zp_calibrationWindowLastChannel(int windowIndex) const;
   bool zp_setCalibrationWindowLastChannel(int windowIndex, int channel);

   qint64 zp_calibrationWindowId(int windowIndex) const;
   bool zp_removeSpectrumWindow(int);

   // predictors
   void zp_connectPredictorToCalibration(const ZAbstractPredictor*);
   int zp_predictorCount() const;
   QString zp_predictorName(int predictorIndex) const;

signals:

   void zg_message(QString) const;
   void zg_windowOperation(ZCalibration::WindowOperationType, int first, int last) const;
   void zg_visibilityChanged(bool) const;
   void zg_dirtyChanged(bool) const;

   void zg_predictorOperation(ZCalibration::PredictorOperationType, int first, int last)const;
   void zg_normalizerChanged() const;

public slots:

   void zp_isNormalizerValid(bool&) const;
   void zp_normalizerValue(qreal&) const;

private slots:

   void zh_removePredictor(ZAbstractPredictor*);

private:

   // VARS
   QString zv_baseName;
   QString zv_path;
   QString zv_chemElement;
   QColor zv_color;
   qint64 zv_calibrationId;

   bool zv_visibility;
   bool zv_dirty;
   QString zv_defaultWindowName;

   QList<ZCalibrationWindow*> zv_spectrumWindowList;
   QList<ZAbstractPredictor*> zv_predictorList;
   ZPredictorNormalizer* zv_predictorNormalizator;

   // FUNCS
   bool zh_isWindowExist(const QString&);
   void zh_createPredictorsForWindow(ZCalibrationWindow*);

   // STATIC
   // VARS
   static QList<QColor> zv_colorList;
   static qint64 zv_lastCalibrationId;
   static int zv_lastColorIndex;

   //  FUNCS
   static QList<QColor> zp_createColorList();
   static bool checkColor(QColor color);

};
//=========================================================
#endif // QABSTRACTCALIBRATION_H

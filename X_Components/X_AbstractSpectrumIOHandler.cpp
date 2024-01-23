//=============================================================
#include "X_AbstractSpectrumIOHandler.h"
//=============================================================
X_AbstractSpectrumIOHandler::X_AbstractSpectrumIOHandler(QObject *spectrumParent, QObject *parent) : QObject(parent)
{
    xv_spectrumParent = spectrumParent;
}
//=============================================================
X_AbstractSpectrumIOHandler::~X_AbstractSpectrumIOHandler()
{

}
//=============================================================


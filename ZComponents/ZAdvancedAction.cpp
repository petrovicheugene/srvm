//=========================================-------------------------------
#include "ZAdvancedAction.h"
//=========================================-------------------------------
ZControlAction::ZControlAction(QObject *parent) :
    QAction(parent)
{
}
//=========================================-------------------------------
void	ZControlAction::setActionDisabled (bool disableFlag)
{
    emit setControlDisabled(disableFlag);
    setDisabled(disableFlag);

}
//=========================================-------------------------------

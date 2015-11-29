//=========================================-------------------------------
#include "ZAdvancedAction.h"
//=========================================-------------------------------
ZAdvancedAction::ZAdvancedAction(QObject *parent) :
    QAction(parent)
{
}
//=========================================-------------------------------
void	ZAdvancedAction::setActionDisabled (bool disableFlag)
{
    emit setControlDisabled(disableFlag);
    setDisabled(disableFlag);

}
//=========================================-------------------------------

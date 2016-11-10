//=========================================-------------------------------
#ifndef ZADVANCEDACTION_H
#define ZADVANCEDACTION_H
//=========================================-------------------------------
#include <QAction>
//=========================================-------------------------------

//=========================================-------------------------------
class ZControlAction : public QAction
{
    Q_OBJECT
public:
    explicit ZControlAction(QObject *parent = 0);
    
signals:
    
    void setControlDisabled(bool);

public slots:
    
    void	setActionDisabled ( bool );

};
//=========================================-------------------------------
#endif // ZADVANCEDACTION_H

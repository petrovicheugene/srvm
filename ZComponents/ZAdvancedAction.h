//=========================================-------------------------------
#ifndef ZADVANCEDACTION_H
#define ZADVANCEDACTION_H
//=========================================-------------------------------
#include <QAction>
//=========================================-------------------------------

//=========================================-------------------------------
class ZAdvancedAction : public QAction
{
    Q_OBJECT
public:
    explicit ZAdvancedAction(QObject *parent = 0);
    
signals:
    
    void setControlDisabled(bool);

public slots:
    
    void	setActionDisabled ( bool );

};
//=========================================-------------------------------
#endif // ZADVANCEDACTION_H

//================================================================
#ifndef X_NUMERICEDITOR_H
#define X_NUMERICEDITOR_H
//================================================================
#include <QAbstractSpinBox>
#include <QCloseEvent>
//================================================================

//================================================================
class X_NumericEditor : public QAbstractSpinBox
{
    Q_OBJECT
public:
    explicit X_NumericEditor(QWidget *parent = 0);
    ~X_NumericEditor();

    virtual void	stepBy ( int steps );
    virtual void keyPressEvent(QKeyEvent *event);

    void setMinMax(double min, double max);

    void setText(QString numericString);
    void setValue(double value);

    QString text();
    double value();

    QSize sizeHint() const;

protected:

    // VARS
    double xv_min;
    double xv_max;

    // FUNCS
    virtual StepEnabled stepEnabled () const;
    bool checkNewNumber(QStringList parts,
                        bool powerTypeRecord,
                        qint64 changedPartNumeric,
                        int changedPartNumber,
                        QString &newNumericString,
                        int &newPartCount);


signals:

    void enterClicked();
    void valueChanged(double);

public slots:
    

protected slots:

    void xp_checkChangedText(const QString&valueString);

};
//================================================================
#endif // X_NUMERICEDITOR_H

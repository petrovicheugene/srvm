//===================================================
#ifndef X_MATHEXPRESSIONVARIABLELISTMAKER_H
#define X_MATHEXPRESSIONVARIABLELISTMAKER_H
//===================================================
#include <QObject>
#include <QMap>
//===================================================

//===================================================
class X_MathExpressionVariableListMaker : public QObject
{
    Q_OBJECT
public:
    explicit X_MathExpressionVariableListMaker(QObject* parent = nullptr);

    QStringList xp_variableList() const;
    void xp_error(QString& errorString,
                     int& errorTokenStartPosition,
                     int& errorTokenEndPosition) const;
    void xp_clear();

signals:

    void zs_variableCheckRequest(const QString& name, bool& res);

public slots:

    void xp_insertVariableValue(const QString& varName, double& value, bool& bRes);
    void xp_setError(const QString& errorString,
                     int errorTokenStartPosition,
                     int errorTokenEndPosition);

private:
    // VARS
    QMap<QString, qreal> xv_variableNameMap;
    QString xv_errorString;
    int xv_errorTokenStartPosition;
    int xv_errorTokenEndPosition;
    //FUNCS


};
//===================================================
#endif // X_MATHEXPRESSIONVARIABLELISTMAKER_H

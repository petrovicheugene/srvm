//===================================================
#ifndef X_MATHEXPRESSIONHANDLER_H
#define X_MATHEXPRESSIONHANDLER_H
//===================================================
#include <QChar>
#include <QObject>
#include <QPair>
#include <QSet>
#include <QVector>
//===================================================
class X_MathExpressionHandler : public QObject
{
    Q_OBJECT
public:
    explicit X_MathExpressionHandler(QObject* parent = 0);


signals:

    void zs_requestVariableValue(const QString& varName, double& value, bool& bRes) const;
    void zs_errorReport(const QString& errorString,
                        int errorTokenStartPosition,
                        int errorTokenEndPosition) const;

public slots:

    void xp_defineHighLights(const QString& expression,
                             int cursorPos,
                             QVector<QPair<int, bool>>& highlightedSymbolVector);

    bool xp_checkExpression(const QString& expression,
                            int cursorPos,
                            QVector<QPair<int, bool>>& highlightedSymbolVector);
    bool xp_calculateExpression(const QString& expression, double& result);

protected slots:


protected:
    // VARS

    enum TokenType
    {
        TT_NULL,
        TT_ERROR,
        TT_DOUBLE,
        TT_VARIABLE,
        TT_DELIMITER,
        TT_OPERATOR,
        TT_INTEGER,
        TT_CELL
    } xv_tokenType;

    QVector<QPair<QChar, QChar>> xv_pairSymbolVector;
    QSet<QChar> xv_delimiterSet;
    QSet<QString> xv_operatorSet;
    QSet<QString> xv_extraSymbolSet;

    int xv_charPosition;       // parsing pos in expression
    int xv_tokenStartPosition; // for error report
    int xv_tokenEndPosition;   //  for error report

    QString xv_token;
    QString xv_expression;


    // FUNCS

    void zh_findBracketForward(QChar bracketSymbol,
                               const QString& expression,
                               int pos,
                               QPair<int, bool>& highlight);
    void zh_findBracketBackward(QChar bracketSymbol,
                                const QString& expression,
                                int pos,
                                QPair<int, bool>& highlight);

    bool zh_getToken();
    bool zh_addOrSubtract(double& result);
    bool zh_multiplyOrDevide(double& result);
    bool zh_power(double& result);
    bool zh_unaryOperation(double& result);
    bool zh_recognizeBrackets(double& result);
    bool zh_variableValue(double& result);
    void zh_handleError(int error);
};
//===================================================
#endif // X_MATHEXPRESSIONHANDLER_H

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Calculator;
}
QT_END_NAMESPACE

class Calculator : public QMainWindow
{
    Q_OBJECT
public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private:

    Ui::Calculator *ui;
    enum Operations {NONE,ADD,SUBTRACT,DIVIDE,MULTIPLY,EQUAL};
    Operations operationType {ADD};
    Operations selectedOperatorType {NONE};
    QString defaultOperatorBtnStyleSheet;
    QString displayVal;
    bool bAllClear {true};
    bool bClearScreen {false};
    bool bCopy {true};
    bool bGetVal1 {true};
    bool bGetVal2 {false};
    //Set this to true to activate debug logging
    bool bDebugMode {true};
    //Does not include the (=) operator
    bool bAllowOperatorCalculation {true};
    bool bAllowDecimalOperation {true};
    bool bAllowNegateOperation {false};

    double calcVal {0.0};
    double val1 {0};
    double val2 {0};

    void AssignStyleSheet(Operations* selectedOperation);
    void CalculateVals();
    void AssignDefaultStyleSheet();
    void SetVals();
    void Result();
    void UpdateDisplayValue();
    void Operation(Operations selectedOperation);

private slots:

    void NumPressed();
    void Clear();
    void OperatorBtnPressed();
    void NegateToggle();
    void Decimal();
    void Percent();
    void Debug(QString FunctionType);
};

#endif // CALCULATOR_H

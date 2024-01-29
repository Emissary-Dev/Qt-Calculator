#include "calculator.h"
#include "ui_calculator.h"
#include <QRegularExpression>

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    setFixedSize(250, 370);
    ui->setupUi(this);
    QApplication::setStyle("fusion");
    ui->Display->setText(QString::number(calcVal));

    //Identifying buttons
    QPushButton* numButtons[10];
    for (int i = 0; i < 10; ++i) {
        QString buttonName = "B" + QString::number(i);
        numButtons[i] = Calculator::findChild<QPushButton*>(buttonName);
        //Assigning the button a function
        connect(numButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }
    //Math operator buttons (+, -, /, *, =) should use same color stylesheet
    //This assigns that stylesheet to a variable
    defaultOperatorBtnStyleSheet = ui->BAdd->styleSheet();

    //Assigning buttons a function
    connect(ui->BAdd, SIGNAL(released()), this, SLOT(OperatorBtnPressed()));
    connect(ui->BSubtract, SIGNAL(released()), this, SLOT(OperatorBtnPressed()));
    connect(ui->BMultiply, SIGNAL(released()), this, SLOT(OperatorBtnPressed()));
    connect(ui->BDivide, SIGNAL(released()), this, SLOT(OperatorBtnPressed()));
    connect(ui->BEquals, SIGNAL(released()), this, SLOT( OperatorBtnPressed()));
    connect(ui->BClear, SIGNAL(released()), this, SLOT(Clear()));
    connect(ui->BSignChange, SIGNAL(released()), this, SLOT(NegateToggle()));
    connect(ui->BDecimal, SIGNAL(released()), this, SLOT(Decimal()));
    connect(ui->BPercent, SIGNAL(released()), this, SLOT(Percent()));
}

void Calculator::UpdateDisplayValue(){
    //Calculator does not run on tick, so we have to update the displayVal variable manually
    displayVal = ui->Display->text();
}

void Calculator::Debug(QString FunctionType){

  qDebug() << "DEBUG" << FunctionType <<"----------";
  qDebug() << "CalcVal: " << calcVal;
  qDebug() << "Val1: " << val1;
  qDebug() << "Val2: " << val2;
  qDebug() << "OperationType: " << operationType;
  qDebug() << "Copy?: " << bCopy;
  qDebug() << "getVal1: " << bGetVal1;
  qDebug() << "getVal2: " << bGetVal2;
  qDebug() << "AllowOperatorCalculation: " << bAllowOperatorCalculation;
  qDebug() << "AllowDecimalOperation: " << bAllowDecimalOperation;
  qDebug() << "----------";
}

void Calculator::NegateToggle(){
    UpdateDisplayValue();
    static const QRegularExpression reg("[0-9]*");

    if (bAllowNegateOperation){
        ui->Display->setText((QString::number(-1 * 0)));
    }
    else if (reg.match(displayVal).hasMatch()){
        //negate val1 and since val2 takes from the display, we'll negate the display value.
        //Note that there can be situations where val2 won't be negated. Negating val2 24/7 will create bugs in calculation, which is why we do it this way
        val1 *= -1;
        ui->Display->setText((QString::number(-1 * displayVal.toDouble())));
    }

    if (bDebugMode){
        Debug("NegateToggle()FUNCTION ENDED");
    }
}

void Calculator::Decimal(){
    bCopy = false;
    bClearScreen = false;
    //Update displayVal in case we use it to perform operations
    UpdateDisplayValue();

    if (bAllowDecimalOperation){
        //Set display to 0. and reset "get" variables (we will manually overwrite the values later)
        bGetVal1 = true;
        bGetVal2 = false;
        bAllowDecimalOperation = false;
        displayVal = "0";
        ui->Display->setText(displayVal.append("."));
    }

    if (!displayVal.contains(".")){
        //Just add the decimal
        ui->Display->setText(displayVal.append("."));
        bAllowDecimalOperation = false;
    }

    UpdateDisplayValue();
    if (bDebugMode){
        Debug("Decimal()FUNCTION ENDED");
    }
}

void Calculator::Percent(){
    bAllowDecimalOperation = true;
    UpdateDisplayValue();
    ui->Display->setText((QString::number(displayVal.toDouble() / 100)));
    if (bDebugMode){
        Debug("Percent()FUNCTION ENDED");
    }
}

void Calculator::Clear(){

    //Clears everything
    if (bAllClear){
        val1 = 0;
        val2 = 1;
        calcVal = 0;
        bGetVal1 = true;
        bGetVal2 = false;
        bCopy = true;
        bClearScreen = false;
        bAllowNegateOperation = false;
        bAllowOperatorCalculation = true;
        bAllowDecimalOperation = true;
        selectedOperatorType = NONE;
        operationType = MULTIPLY;
        AssignDefaultStyleSheet();
        ui->Display->setText(QString::number(calcVal));
    }
    //Clears the display and current "get" variable
    else{

        if (bGetVal1)
            val1 = 0;
        else if (bGetVal2)
            val2 = 0;

        ui->Display->setText(QString::number(0));
        UpdateDisplayValue();
        bAllClear = true;
        ui->BClear->setText("AC");
    }

    if (bDebugMode){
        Debug("Clear()FUNCTION ENDED");
    }

}

void Calculator::NumPressed(){
    selectedOperatorType = NONE;
    bCopy = false;

    if (bAllClear){
       bAllClear = false;
       ui->BClear->setText("C");
    }
    bAllowDecimalOperation = false;

    QPushButton* button {(QPushButton*) sender()};
    QString buttonVal {button->text()};
    UpdateDisplayValue();

    if (bDebugMode){
        Debug("NumPressed()FUNCTION STARTED");
    }

    if(bClearScreen){
        bClearScreen = false;
        //bAllowOperatorCalculation = true;
        AssignDefaultStyleSheet();
        ui->Display->setText(buttonVal);
    }
    //Length Checking (Max is 8 digits and 1 decimal)
    else{
        long long decimalIndex {displayVal.indexOf(".")};
        long long minusIndex {displayVal.indexOf("-")};
        //copy the displayVal variable so when we remove the decimal for length checking, we aren't altering the actual value
        QString displayValCopy {displayVal};

        if (displayVal.contains(".")){
            displayValCopy.remove(decimalIndex, 1);
        }
        if (displayVal.contains("-")){
            displayValCopy.remove(minusIndex, 1);
        }
        if (displayValCopy.length() < 8){
            QString newVal {displayVal + buttonVal};
            ui->Display->setText(QString::number(newVal.toDouble(), 'g', 8));
        }
    }

    if (bDebugMode)
    {
        Debug("NumPressed()FUNCTION ENDED");
    }
}

void Calculator::Operation(Operations selectedOperation){
    SetVals();
    operationType = selectedOperation;
    selectedOperatorType = selectedOperation;
    AssignStyleSheet(&operationType);
}

void Calculator::OperatorBtnPressed(){
    QPushButton* selectedButton {(QPushButton*) sender()};
    QString selectedButtonVal = selectedButton->text();
    bAllowDecimalOperation = true;
    UpdateDisplayValue();

    if (bDebugMode){
        Debug("OperatorBtnPressed()FUNCTION STARTED");
    }

    //button type isn't equals sign
    if (QString::compare(selectedButtonVal, "=", Qt::CaseInsensitive) != 0){
        //Calculate and display
        if (!bCopy && bAllowOperatorCalculation){
            Result();
        }
        //Copy
        else {
            val1 = displayVal.toDouble();
            val2 = displayVal.toDouble();
        }

        bCopy = true;
        bAllowNegateOperation = true;
        bAllowOperatorCalculation = true;
    }

    if (QString::compare(selectedButtonVal, "/", Qt::CaseInsensitive) == 0 && selectedOperatorType != DIVIDE){
        Operation(DIVIDE);
    }
    else if(QString::compare(selectedButtonVal, "x", Qt::CaseInsensitive) == 0 && selectedOperatorType != MULTIPLY)
    {
         Operation(MULTIPLY);

    } else if(QString::compare(selectedButtonVal, "+", Qt::CaseInsensitive) == 0 && selectedOperatorType != ADD)
    {
         Operation(ADD);

    }

    else if(QString::compare(selectedButtonVal, "-", Qt::CaseInsensitive) == 0 && selectedOperatorType != SUBTRACT)
    {
         Operation(SUBTRACT);

    }
    else if(QString::compare(selectedButtonVal, "=", Qt::CaseInsensitive) == 0)
    {
        selectedOperatorType = EQUAL;
        AssignDefaultStyleSheet();
        Result();
        bCopy = false;
        bAllowOperatorCalculation = false;
        bAllowNegateOperation = false;

        if (val1 == 0 && val2 == 0){
            bGetVal1 = true;
            bGetVal2 = false;
        }
    }
    if (bDebugMode){
       Debug("OperatorBtnPressed()FUNCTION ENDED");
    }
    bClearScreen = true;
}

void Calculator::Result(){
    SetVals();
    CalculateVals();
    ui->Display->setText(QString::number(calcVal));
    if (bDebugMode)
    {
        Debug("Result()FUNCTION ENDED");
    }
}

void Calculator::SetVals(){
    UpdateDisplayValue();

   if (bGetVal1){
        val1 = displayVal.toDouble();
       calcVal = val1;
       bGetVal1 = false;
       bGetVal2 = true;
    }
    else if (bGetVal2 && bAllowOperatorCalculation){
       //Very strange glitch with qt: enter the equation (9 % * = * 9 - =) this should give you 0 as .0729 - .0729 = 0, but you get this really strange number (-1.38778e-17)
       //setting the two varibles to .0729 manually and subtracting them equals 0/ calculating the variables before the glitch equals 0
       //already ran plenty of tests to try and get around the issue
      //cout << "before the glitch val1 - val2 = " <<  val1 - val2 << endl;
      // cout << "VAL1 " << val1 << endl;
      // cout << "VAL2 " << val2 << endl;
      val2 = displayVal.toDouble();
      // cout << "after the glitch val1 - val2 = " <<  val1 - val2 << endl;
      // cout << "VAL1 " << displayVal << endl;
      // cout << "VAL2 " << val2 << endl;
      bClearScreen = true;
    }

    if (bDebugMode)
    {
        Debug("SetVals()FUNCTION ENDED");
    }

}

void Calculator::CalculateVals(){

    switch (operationType) {
    case ADD:
        calcVal = val1 + val2;
        break;
    case SUBTRACT:
        calcVal =  val1 - val2;
        break;
    case MULTIPLY:
        calcVal = val1 * val2;
        break;
    case DIVIDE:
        calcVal = val1 / val2;

        break;
    default:
        calcVal = ui->Display->text().toDouble();
        break;
    }

    val1 = calcVal;
}

void Calculator::AssignStyleSheet(Operations* selectedOperation){

    //White color
    QString selectedOperatorBtnStyleSheet = "background-color: #ffffff; color: #f4a261; border-radius: 24px;";

    //Everything *but* the selected operator gets default stylesheet (selected operator gets a different color)
    switch (*selectedOperation) {
    case ADD:
        ui->BAdd->setStyleSheet(selectedOperatorBtnStyleSheet);
        ui->BMultiply->setStyleSheet(defaultOperatorBtnStyleSheet);
        ui->BDivide->setStyleSheet(defaultOperatorBtnStyleSheet);
        ui->BSubtract->setStyleSheet(defaultOperatorBtnStyleSheet);
        ui->BEquals->setStyleSheet(defaultOperatorBtnStyleSheet);
        break;
    case SUBTRACT:
        ui->BSubtract->setStyleSheet(selectedOperatorBtnStyleSheet);
        ui->BMultiply->setStyleSheet(defaultOperatorBtnStyleSheet);
        ui->BDivide->setStyleSheet(defaultOperatorBtnStyleSheet);
        ui->BAdd->setStyleSheet(defaultOperatorBtnStyleSheet);
        break;
    case MULTIPLY:
        ui->BMultiply->setStyleSheet(selectedOperatorBtnStyleSheet);
        ui->BDivide->setStyleSheet(defaultOperatorBtnStyleSheet);
        ui->BSubtract->setStyleSheet(defaultOperatorBtnStyleSheet);
        ui->BAdd->setStyleSheet(defaultOperatorBtnStyleSheet);
        ui->BEquals->setStyleSheet(defaultOperatorBtnStyleSheet);
        break;
    case DIVIDE:
        ui->BDivide->setStyleSheet(selectedOperatorBtnStyleSheet);
        ui->BMultiply->setStyleSheet(defaultOperatorBtnStyleSheet);
        ui->BSubtract->setStyleSheet(defaultOperatorBtnStyleSheet);
        ui->BAdd->setStyleSheet(defaultOperatorBtnStyleSheet);
        ui->BEquals->setStyleSheet(defaultOperatorBtnStyleSheet);
        break;
    //Assign everything the default stylesheet
    default:
        AssignDefaultStyleSheet();
        break;
    }
}

void Calculator::AssignDefaultStyleSheet(){
    ui->BMultiply->setStyleSheet(defaultOperatorBtnStyleSheet);
    ui->BSubtract->setStyleSheet(defaultOperatorBtnStyleSheet);
    ui->BAdd->setStyleSheet(defaultOperatorBtnStyleSheet);
    ui->BDivide->setStyleSheet(defaultOperatorBtnStyleSheet);
    ui->BEquals->setStyleSheet(defaultOperatorBtnStyleSheet);
}

Calculator::~Calculator()
{
    delete ui;
}

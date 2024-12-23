#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QString>
#include <QJSEngine>
#include <QJSValue>
#include <QPainter>

class Calculator : public QWidget
{
public:
    Calculator(QWidget *parent = nullptr);

private:
    QLineEdit *display;
    QString currentInput;

    void addDigit(const QString &digit);
    void clearDisplay();
    void calculateResult();
    void backspace();
    void applyStyles();
    void calculatePercentage();
    void toggleSign();
    
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
};

Calculator::Calculator(QWidget *parent) : QWidget(parent)
{
    display = new QLineEdit(this);
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setFixedHeight(60);

    QGridLayout *layout = new QGridLayout(this);

    QStringList buttons = {
        "C", "%", "+/-", "×",
        "7", "8", "9", "÷",
        "4", "5", "6", "-",
        "1", "2", "3", "+",
        "0", ".", "<-", "="
    };

    int row = 1, col = 0;
    for (const QString &buttonText : buttons) {
        if (buttonText.isEmpty()) continue;

        QPushButton *button = new QPushButton(buttonText, this);
        layout->addWidget(button, row, col);

        connect(button, &QPushButton::clicked, [this, buttonText]() {
            if (buttonText == "=") {
                calculateResult();
            } else if (buttonText == "C") {
                clearDisplay();
            } else if (buttonText == "<-") {
                backspace();
            } else if (buttonText == "%") {
                calculatePercentage();
            } else if (buttonText == "+/-") {
                toggleSign();
            } else {
                addDigit(buttonText);
            }
        });

        col++;
        if (col > 3) {
            col = 0;
            row++;
        }
    }

    layout->addWidget(display, 0, 0, 1, 4);
    setLayout(layout);

    setWindowTitle("Calculator");

    applyStyles();

    setFixedSize(minimumSizeHint());
}

void Calculator::addDigit(const QString &digit)
{
    if (digit == "." && currentInput.contains(".")) {
        return;
    }
    currentInput.append(digit);
    display->setText(currentInput);
}

void Calculator::clearDisplay()
{
    currentInput.clear();
    display->clear();
}

void Calculator::calculateResult()
{
    QString modifiedInput = currentInput;
    modifiedInput.replace("×", "*");
    modifiedInput.replace("÷", "/");

    QJSEngine engine;
    QJSValue result = engine.evaluate(modifiedInput);

    if (result.isError() || result.toString() == "Infinity" || result.toString() == "-Infinity") {
        display->setText("Error");
        currentInput.clear();
    } else {
        display->setText(result.toString());
        currentInput = result.toString();
    }
}

void Calculator::backspace()
{
    if (!currentInput.isEmpty()) {
        currentInput.chop(1);
        display->setText(currentInput);
    }
}

void Calculator::calculatePercentage()
{
    bool ok;
    double value = currentInput.toDouble(&ok);
    if (ok) {
        double percentage = value / 100.0;
        currentInput = QString::number(percentage);
        display->setText(currentInput);
    } else {
        display->setText("Error");
        currentInput.clear();
    }
}

void Calculator::toggleSign()
{
    bool ok;
    double value = currentInput.toDouble(&ok);
    if (ok) {
        currentInput = QString::number(-value);
        display->setText(currentInput);
    } else {
        display->setText("Error");
        currentInput.clear();
    }
}

void Calculator::applyStyles()
{
    setStyleSheet("background-color: #FFFFFF;");

    display->setStyleSheet(
        "background-color: #000000; "
        "color: white; "
        "border-radius: 25px; "
        "font-size: 32px; "
        "padding: 10px; "
        "border: none; "
        "box-shadow: inset 0 0 10px rgba(0, 0, 0, 0.5);");

    for (auto button : findChildren<QPushButton *>()) {
        if (button->text() == "=") {
            button->setStyleSheet(
                "QPushButton {"
                "background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, "
                "stop: 0 #121393, stop: 1 #0006ca);"
                "color: white;"
                "border-radius: 25px;"
                "font-size: 20px;"
                "font-weight: bold;"
                "border: none;"
                "padding: 10px;"
                "}"
                "QPushButton:hover {"
                "background-color: #444444;"
                "}"
                "QPushButton:pressed {"
                "background-color: #222222;"
                "}");
        } else {
            button->setStyleSheet(
                "QPushButton {"
                "background-color: #000000;"
                "color: white;"
                "border-radius: 25px;"
                "font-size: 20px;"
                "font-weight: bold;"
                "border: none;"
                "padding: 10px;"
                "}"
                "QPushButton:hover {"
                "background-color: #444444;"
                "}"
                "QPushButton:pressed {"
                "background-color: #222222;"
                "}");
        }

        button->setFixedSize(50, 50);
    }
}

QSize Calculator::minimumSizeHint() const
{
    return QSize(250, 350); 
}

QSize Calculator::sizeHint() const
{
    return minimumSizeHint();
}

void Calculator::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(255, 255, 255));

    QWidget::paintEvent(nullptr);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Calculator calc;
    calc.show();

    return app.exec();
}
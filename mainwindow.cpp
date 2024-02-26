#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QVBoxLayout *vLayout = new QVBoxLayout(ui->centralwidget);
    display = new QLineEdit();
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMinimumHeight(50);
    vLayout->addWidget(display);

    QGridLayout *gridLayout = new QGridLayout();
    vLayout->addLayout(gridLayout);

    QStringList buttons {
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "C", "0", "=", "+"
    };

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            QPushButton *button = new QPushButton(buttons[i * 4 + j]);
            gridLayout->addWidget(button, i, j);
            connect(button, &QPushButton::clicked, this, &MainWindow::onPushButton);
        }
    }

    setFixedSize(300, 400);
}

void MainWindow::onPushButton() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    QString buttonText = button->text();

    if (buttonText == "=") {
        display->setText(QString::number(evalExpression(display->text())));
    } else if (buttonText == "C") {
        display->clear();
    } else {
        display->setText(display->text() + buttonText);
    }
}

double MainWindow::evalExpression(const QString& expression) {
    QTextStream stream(expression.toUtf8());
    double result = 0.0;
    QString currentOp = "";
    bool hasLastOperand = false;

    while (!stream.atEnd()) {
        double operand = 0.0;
        stream >> operand;

        if (stream.status() == QTextStream::Ok) {
            if (hasLastOperand) {
                if (currentOp == "+") result += operand;
                else if (currentOp == "-") result -= operand;
                else if (currentOp == "*") result *= operand;
                else if (currentOp == "/" && operand != 0.0) result /= operand;
                else return 0.0;
                hasLastOperand = false;
            } else {
                result = operand;
            }

            stream >> currentOp;
        } else {
            return 0.0;
        }
    }

    if (hasLastOperand) {
        double lastOperand = currentOp.toDouble();
        if (currentOp == "+") result += lastOperand;
        else if (currentOp == "-") result -= lastOperand;
        else if (currentOp == "*") result *= lastOperand;
        else if (currentOp == "/" && lastOperand != 0.0) result /= lastOperand;
        else return 0.0;
    }

    return result;
}

MainWindow::~MainWindow()
{
    delete ui;
}

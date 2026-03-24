#include "mainwindow.h"
#include "recursion.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    nEdit = new QLineEdit(this);
    nEdit->setPlaceholderText("Введите n (целое положительное)");
    mainLayout->addWidget(nEdit);

    variantCombo = new QComboBox(this);
    variantCombo->addItem("Вариант 10", 10);
    variantCombo->addItem("Вариант 3", 3);
    mainLayout->addWidget(variantCombo);

    calcButton = new QPushButton("Вычислить", this);
    mainLayout->addWidget(calcButton);

    resultLabel = new QLabel("Результат: ", this);
    mainLayout->addWidget(resultLabel);

    callsLabel = new QLabel("Количество вызовов: ", this);
    mainLayout->addWidget(callsLabel);

    formulaLabel = new QLabel("Формула: ", this);
    formulaLabel->setWordWrap(true);
    mainLayout->addWidget(formulaLabel);

    connect(calcButton, &QPushButton::clicked, this, &MainWindow::onCalculateClicked);
    connect(variantCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onVariantChanged);

    updateFormula(variantCombo->currentData().toInt());

    setWindowTitle("Рекурсивный калькулятор");
    resize(500, 300);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onVariantChanged(int index)
{
    Q_UNUSED(index);
    int variant = variantCombo->currentData().toInt();
    updateFormula(variant);
}

void MainWindow::updateFormula(int variant)
{
    QString formulaText;
    if (variant == 10) {
        formulaText = "F(1) = 1<br>"
                      "F(n) = n + 3 * F(n-1)  (если n четное)<br>"
                      "F(n) = 2 + 2 * F(n-2)  (если n > 1 нечетное)";
    } else if (variant == 3) {
        formulaText = "F(n) = 1  (n < 3)<br>"
                      "F(n) = F(n-1) + F(n-2)  (n > 2 и n нечетное)<br>"
                      "F(n) = Σ_{i=1}^{n-1} F(i)  (n > 2 и n четное)";
    } else {
        formulaText = "Неизвестный вариант";
    }
    formulaLabel->setText(formulaText);
}

void MainWindow::onCalculateClicked()
{
    bool ok;
    int n = nEdit->text().toInt(&ok);
    if (!ok || n < 1) {
        QMessageBox::warning(this, "Ошибка ввода", "Введите целое положительное число.");
        return;
    }

    resetCallCount();

    long long result = 0;
    int variant = variantCombo->currentData().toInt();

    if (variant == 10) {
        result = F10(n);
    } else if (variant == 3) {
        result = F3(n);
    } else {
        QMessageBox::critical(this, "Ошибка", "Неизвестный вариант");
        return;
    }

    int calls = getCallCount();

    resultLabel->setText(QString("Результат: %1").arg(result));
    callsLabel->setText(QString("Количество вызовов: %1").arg(calls));
}
#include "mainwindow.h"
#include "recursion.h"
#include <QtWidgets>
#include <vector>
#include <sstream>
#include <QElapsedTimer>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->variantCombo->addItem("Вариант 10", 10);
    ui->variantCombo->addItem("Вариант 3", 3);
    updateFormula(ui->variantCombo->currentData().toInt());

    connect(ui->calcButton, &QPushButton::clicked, this, &MainWindow::onCalculateClicked);
    connect(ui->variantCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onVariantChanged);
    connect(ui->maxButton, &QPushButton::clicked, this, &MainWindow::onMaxClicked);

    setWindowTitle("Рекурсивный калькулятор + максимум последовательности");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onVariantChanged(int index)
{
    Q_UNUSED(index);
    int variant = ui->variantCombo->currentData().toInt();
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
        formulaText = "F(n) = 1  (n &lt; 3)<br>"
                      "F(n) = F(n-1) + F(n-2)  (n &gt; 2 и n нечетное)<br>"
                      "F(n) = Σ_{i=1}^{n-1} F(i)  (n &gt; 2 и n четное)";
    } else {
        formulaText = "Неизвестный вариант";
    }
    ui->formulaLabel->setText(formulaText);
}

void MainWindow::onCalculateClicked()
{
    bool ok;
    int n = ui->nEdit->text().toInt(&ok);
    if (!ok || n < 1) {
        QMessageBox::warning(this, "Ошибка ввода", "Введите целое положительное число.");
        return;
    }

    int variant = ui->variantCombo->currentData().toInt();

    resetCallCount();
    QElapsedTimer timerRec;
    timerRec.start();
    long long recResult = 0;
    if (variant == 10) {
        recResult = F10(n);
    } else if (variant == 3) {
        recResult = F3(n);
    } else {
        QMessageBox::critical(this, "Ошибка", "Неизвестный вариант");
        return;
    }
    qint64 recTime = timerRec.nsecsElapsed();

    QElapsedTimer timerIter;
    timerIter.start();
    long long iterResult = 0;
    if (variant == 10) {
        iterResult = F10Iterative(n);
    } else if (variant == 3) {
        iterResult = F3Iterative(n);
    }
    qint64 iterTime = timerIter.nsecsElapsed();

    ui->resultLabel->setText(QString("Результат вычислений: %1").arg(recResult));
    ui->callsLabel->setText(QString("Количество вызовов: %1").arg(getCallCount()));
    ui->recTimeLabel->setText(QString("Время рекурсивного вычисления: %1 мкс")
                                  .arg(recTime / 1000.0, 0, 'f', 3));
    ui->iterTimeLabel->setText(QString("Время итеративного вычисления: %1 мкс")
                                   .arg(iterTime / 1000.0, 0, 'f', 3));

    if (recResult != iterResult) {
        QMessageBox::warning(this, "Несовпадение",
                             "Рекурсивный и итеративный результаты отличаются!");
    }
}

long long MainWindow::F10Iterative(int n)
{
    if (n == 1) return 1;
    std::vector<long long> f(n + 1);
    f[1] = 1;
    for (int i = 2; i <= n; ++i) {
        if (i % 2 == 0) {
            f[i] = i + 3 * f[i - 1];
        } else {
            f[i] = 2 + 2 * f[i - 2];
        }
    }
    return f[n];
}

long long MainWindow::F3Iterative(int n)
{
    if (n < 3) return 1;
    std::vector<long long> f(n + 1);
    f[1] = 1;
    f[2] = 1;
    long long totalSum = f[1] + f[2];
    for (int i = 3; i <= n; ++i) {
        if (i % 2 == 1) {
            f[i] = f[i - 1] + f[i - 2];
        } else {
            f[i] = totalSum;
        }
        totalSum += f[i];
    }
    return f[n];
}

void MainWindow::onMaxClicked()
{
    QString text = ui->sequenceEdit->toPlainText().trimmed();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите последовательность чисел.");
        return;
    }

    QStringList tokens = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    std::vector<int> numbers;
    bool hasZero = false;
    for (const QString& token : tokens) {
        bool ok;
        int num = token.toInt(&ok);
        if (!ok || num < 0) {
            QMessageBox::warning(this, "Ошибка", "Введите только натуральные числа (0 или положительные).");
            return;
        }
        if (num == 0) {
            hasZero = true;
            break;
        }
        numbers.push_back(num);
    }

    if (numbers.empty()) {
        QMessageBox::warning(this, "Ошибка", "Последовательность не содержит чисел (кроме нуля).");
        return;
    }

    int maxVal = maxRecursive(numbers);
    ui->maxResultLabel->setText(QString("Максимум: %1").arg(maxVal));
}

int MainWindow::maxRecursive(const std::vector<int>& nums)
{
    if (nums.size() == 1) {
        return nums[0];
    }
    std::vector<int> tail(nums.begin() + 1, nums.end());
    int maxTail = maxRecursive(tail);
    return (nums[0] > maxTail) ? nums[0] : maxTail;
}
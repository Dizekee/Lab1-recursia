#include "mainwindow.h"
#include "recursion.h"
#include <QtWidgets>
#include <vector>
#include <sstream>
#include <QElapsedTimer>          // для замера времени

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // --- Existing part: recursive function calculator ---
    nEdit = new QLineEdit(this);
    nEdit->setPlaceholderText("Введите n (целое положительное)");
    mainLayout->addWidget(nEdit);

    variantCombo = new QComboBox(this);
    variantCombo->addItem("Вариант 10", 10);
    variantCombo->addItem("Вариант 3", 3);
    mainLayout->addWidget(variantCombo);

    calcButton = new QPushButton("Вычислить", this);
    mainLayout->addWidget(calcButton);

    resultLabel = new QLabel("Результат (рекурсивно): ", this);
    mainLayout->addWidget(resultLabel);

    callsLabel = new QLabel("Количество вызовов: ", this);
    mainLayout->addWidget(callsLabel);

    formulaLabel = new QLabel("Формула: ", this);
    formulaLabel->setWordWrap(true);
    mainLayout->addWidget(formulaLabel);

    // Новые метки для времени и итеративного результата
    recTimeLabel = new QLabel("Время рекурсивного вычисления: ", this);
    mainLayout->addWidget(recTimeLabel);

    iterResultLabel = new QLabel("Результат (итеративно): ", this);
    mainLayout->addWidget(iterResultLabel);

    iterTimeLabel = new QLabel("Время итеративного вычисления: ", this);
    mainLayout->addWidget(iterTimeLabel);

    // --- New part: variant 10 task (max in sequence) ---
    QFrame *separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(separator);

    QLabel *taskLabel = new QLabel("Задание по варианту 10:", this);
    taskLabel->setStyleSheet("font-weight: bold; margin-top: 10px;");
    mainLayout->addWidget(taskLabel);

    QLabel *descLabel = new QLabel("Введите последовательность натуральных чисел, "
                                   "завершающуюся нулём (ноль в конце не обязателен). "
                                   "Программа найдёт максимальное число.", this);
    descLabel->setWordWrap(true);
    mainLayout->addWidget(descLabel);

    sequenceEdit = new QTextEdit(this);
    sequenceEdit->setPlaceholderText("Пример:\n5\n12\n8\n0");
    sequenceEdit->setMaximumHeight(100);
    mainLayout->addWidget(sequenceEdit);

    maxButton = new QPushButton("Найти максимум", this);
    mainLayout->addWidget(maxButton);

    maxResultLabel = new QLabel("Максимум: ", this);
    mainLayout->addWidget(maxResultLabel);

    // --- Connections ---
    connect(calcButton, &QPushButton::clicked, this, &MainWindow::onCalculateClicked);
    connect(variantCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onVariantChanged);
    connect(maxButton, &QPushButton::clicked, this, &MainWindow::onMaxClicked);

    updateFormula(variantCombo->currentData().toInt());

    setWindowTitle("Рекурсивный калькулятор + максимум последовательности");
    resize(600, 550);
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

    int variant = variantCombo->currentData().toInt();

    // --- Рекурсивное вычисление с замером времени ---
    resetCallCount();                       // обнуляем счётчик вызовов
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
    qint64 recTime = timerRec.nsecsElapsed();   // время в наносекундах

    // --- Итеративное вычисление с замером времени ---
    QElapsedTimer timerIter;
    timerIter.start();
    long long iterResult = 0;
    if (variant == 10) {
        iterResult = F10Iterative(n);
    } else if (variant == 3) {
        iterResult = F3Iterative(n);
    }
    qint64 iterTime = timerIter.nsecsElapsed();

    // Вывод результатов
    resultLabel->setText(QString("Результат (рекурсивно): %1").arg(recResult));
    callsLabel->setText(QString("Количество вызовов: %1").arg(getCallCount()));
    recTimeLabel->setText(QString("Время рекурсивного вычисления: %1 мкс")
                              .arg(recTime / 1000.0, 0, 'f', 3));  // микросекунды

    iterResultLabel->setText(QString("Результат (итеративно): %1").arg(iterResult));
    iterTimeLabel->setText(QString("Время итеративного вычисления: %1 мкс")
                               .arg(iterTime / 1000.0, 0, 'f', 3));

    // Проверка совпадения результатов
    if (recResult != iterResult) {
        QMessageBox::warning(this, "Несовпадение",
                             "Рекурсивный и итеративный результаты отличаются!");
    }
}

// --- Итеративные реализации ---

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
    f[2] = 1;                     // для n=2 сумма f[1] = 1
    long long totalSum = f[1] + f[2]; // сумма f[1..2]
    for (int i = 3; i <= n; ++i) {
        if (i % 2 == 1) {         // нечётное
            f[i] = f[i - 1] + f[i - 2];
        } else {                  // чётное
            f[i] = totalSum;      // сумма f[1..i-1]
        }
        totalSum += f[i];
    }
    return f[n];
}

// --- Задание по варианту 10 (максимум) ---

void MainWindow::onMaxClicked()
{
    QString text = sequenceEdit->toPlainText().trimmed();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите последовательность чисел.");
        return;
    }

    // Разбор чисел
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
    maxResultLabel->setText(QString("Максимум: %1").arg(maxVal));
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
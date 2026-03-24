#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

class QLineEdit;
class QComboBox;
class QPushButton;
class QLabel;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCalculateClicked();
    void onVariantChanged(int index);
    void onMaxClicked();

private:
    void updateFormula(int variant);
    int maxRecursive(const std::vector<int>& nums);

    // Итеративные версии
    long long F10Iterative(int n);
    long long F3Iterative(int n);

    QLineEdit *nEdit;
    QComboBox *variantCombo;
    QPushButton *calcButton;
    QLabel *resultLabel;
    QLabel *callsLabel;
    QLabel *formulaLabel;

    QTextEdit *sequenceEdit;
    QPushButton *maxButton;
    QLabel *maxResultLabel;

    // Новые элементы для времени и итеративного результата
    QLabel *recTimeLabel;
    QLabel *iterResultLabel;
    QLabel *iterTimeLabel;
};

#endif // MAINWINDOW_H
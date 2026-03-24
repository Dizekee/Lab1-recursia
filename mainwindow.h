#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLineEdit;
class QComboBox;
class QPushButton;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCalculateClicked();
    void onVariantChanged(int index);

private:
    void updateFormula(int variant);

    QLineEdit *nEdit;
    QComboBox *variantCombo;
    QPushButton *calcButton;
    QLabel *resultLabel;
    QLabel *callsLabel;
    QLabel *formulaLabel;
};

#endif // MAINWINDOW_H
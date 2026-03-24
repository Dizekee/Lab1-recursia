#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QtWidgets>          // включает все виджеты
#include "ui_mainwindow.h"

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
    long long F10Iterative(int n);
    long long F3Iterative(int n);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
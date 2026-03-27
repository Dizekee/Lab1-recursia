#ifndef TESTS_H
#define TESTS_H

#include <QtTest/QtTest>

class TestRecursion : public QObject
{
    Q_OBJECT

private slots:
    void test_F10_knownValues();
    void test_F10_iterative();
    void test_F10_recursiveCalls();
    void test_F3_knownValues();
    void test_F3_iterative();
    void test_F3_recursiveCalls();
    void test_maxRecursive();
};

#endif // TESTS_H
#include "tests.h"
#include "recursion.h"
#include <vector>

// Вспомогательная функция для поиска максимума (копия из mainwindow.cpp)
int maxRecursive(const std::vector<int>& nums)
{
    if (nums.size() == 1) return nums[0];
    std::vector<int> tail(nums.begin() + 1, nums.end());
    int maxTail = maxRecursive(tail);
    return (nums[0] > maxTail) ? nums[0] : maxTail;
}

// Тесты для варианта 10
void TestRecursion::test_F10_knownValues()
{
    QCOMPARE(F10(1), 1LL);
    QCOMPARE(F10(2), 5LL);
    QCOMPARE(F10(3), 4LL);
    QCOMPARE(F10(4), 16LL);
    QCOMPARE(F10(5), 10LL);
}

void TestRecursion::test_F10_iterative()
{
    QCOMPARE(F10Iterative(1), F10(1));
    QCOMPARE(F10Iterative(2), F10(2));
    QCOMPARE(F10Iterative(3), F10(3));
    QCOMPARE(F10Iterative(4), F10(4));
    QCOMPARE(F10Iterative(5), F10(5));
    QCOMPARE(F10Iterative(10), F10(10));
}

void TestRecursion::test_F10_recursiveCalls()
{
    resetCallCount();
    F10(5);
    QVERIFY(getCallCount() > 0);
}

// Тесты для варианта 3
void TestRecursion::test_F3_knownValues()
{
    QCOMPARE(F3(1), 1LL);
    QCOMPARE(F3(2), 1LL);
    QCOMPARE(F3(3), 2LL);
    QCOMPARE(F3(4), 4LL);
    QCOMPARE(F3(5), 6LL);
    QCOMPARE(F3(6), 14LL);
}

void TestRecursion::test_F3_iterative()
{
    QCOMPARE(F3Iterative(1), F3(1));
    QCOMPARE(F3Iterative(2), F3(2));
    QCOMPARE(F3Iterative(3), F3(3));
    QCOMPARE(F3Iterative(4), F3(4));
    QCOMPARE(F3Iterative(5), F3(5));
    QCOMPARE(F3Iterative(10), F3(10));
}

void TestRecursion::test_F3_recursiveCalls()
{
    resetCallCount();
    F3(5);
    QVERIFY(getCallCount() > 0);
}

void TestRecursion::test_maxRecursive()
{
    std::vector<int> v1 = {5};
    QCOMPARE(maxRecursive(v1), 5);

    std::vector<int> v2 = {3, 8, 1, 10, 2};
    QCOMPARE(maxRecursive(v2), 10);

    std::vector<int> v3 = {7, 7, 7};
    QCOMPARE(maxRecursive(v3), 7);

    std::vector<int> v4 = {100, 1, 2, 3};
    QCOMPARE(maxRecursive(v4), 100);
}

QTEST_MAIN(TestRecursion)
#include "recursion.h"
#include <vector>

int callCount = 0;

void resetCallCount() {
    callCount = 0;
}

int getCallCount() {
    return callCount;
}

long long F3(int n) {
    callCount++;
    if (n < 3) return 1;
    if (n % 2 == 1) { // нечётное
        return F3(n-1) + F3(n-2);
    } else { // чётное
        long long sum = 0;
        for (int i = 1; i <= n-1; i++) {
            sum += F3(i);
        }
        return sum;
    }
}

long long F10(int n) {
    callCount++;
    if (n == 1) return 1;
    if (n % 2 == 0) { // чётное
        return n + 3 * F10(n-1);
    } else { // нечётное (n > 1)
        return 2 + 2 * F10(n-2);
    }
}

long long F10Iterative(int n) {
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

long long F3Iterative(int n) {
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
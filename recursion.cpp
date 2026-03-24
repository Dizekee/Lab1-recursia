#include "recursion.h"

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
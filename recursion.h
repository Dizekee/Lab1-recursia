#ifndef RECURSION_H
#define RECURSION_H

extern int callCount;

void resetCallCount();
int getCallCount();

long long F3(int n);
long long F10(int n);

long long F3Iterative(int n);
long long F10Iterative(int n);

#endif // RECURSION_H
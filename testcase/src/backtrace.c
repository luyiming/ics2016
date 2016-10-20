#include "trap.h"

int A(int a) {
    return a * a;
}

int B(int a, int b) {
    return A(a + b);
}

int C(int a, int b, int c) {
    return B(a, b + c);
}

int D(int a, int b, int c, int d) {
    return C(a, b, c + d);
}

int main() {
    int ret = D(1, 2, 3, 4);
    nemu_assert(ret = 100);
    return 0;
}


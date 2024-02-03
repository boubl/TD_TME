#include <assert.h>
#include <stdio.h>

void echange(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void tri(int *a, int *b) {
    if (*a > *b)
        echange(a, b);
}

void tri_3(int *a, int *b, int *c) {
    tri(a, b);
    tri(b, c);
    tri(a, b);
}

int main() {
    int a = 2;
    int b = 3;
    int c = 1;
    tri_3(&a, &b, &c);
    printf("a: %d, b: %d, c: %d\n", a, b, c);
    assert(a == 1 && b == 2 && c == 3);
}

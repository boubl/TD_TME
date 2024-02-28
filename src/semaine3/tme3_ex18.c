#include <assert.h>
#include <stdio.h>

void echange(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void tri_croissant(int *a, int *b) {
    if (*a > *b)
        echange(a, b);
}

void tri_3(int *a, int *b, int *c) {
    tri_croissant(a, b);
    tri_croissant(b, c);
    tri_croissant(a, b);
}

int main() {
    int a = 2;
    int b = 3;
    int c = 1;
    tri_3(&a, &b, &c);
    printf("a: %d, b: %d, c: %d\n", a, b, c);
    assert(a == 1 && b == 2 && c == 3);
}

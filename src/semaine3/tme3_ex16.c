#include <stdio.h>

void ma_fonction(int v1, int v2) {
    int a;
    int *b = &a; // b est un pointeur vers la valeur a

    a = v1;       // a == v1,            b == &a, *b == v1
    *b = a + v2;  // a == v1 + v2,       b == &a, *b == v1 + v2
    a = 2 * (*b); // a == 2 * (v1 + v2), b == &a, *b == 2 * (v1 + v2)
    printf("a=%d, b=%d\n", a, *b);
}

int main() { ma_fonction(10, 20); }

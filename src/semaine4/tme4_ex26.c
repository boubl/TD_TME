#include <stdio.h>
#include <stdlib.h>

int* fusion(int *tab1, int taille1, int *tab2, int taille2) {
    int *buf = malloc(sizeof(int) * (taille1 + taille2));
    int i1 = 0, i2 = 0, ibuf = 0;
    while (i1 < taille1 && i2 < taille2) {
        if (tab1[i1] < tab2[i2]) {
            buf[ibuf] = tab1[i1];
            i1++;
        } else {
            buf[ibuf] = tab2[i2];
            i2++;
        }
        ibuf++;
    }
    int *tab;
    int start, end;
    if (i1 == taille1) {
        tab = tab2;
        start = i2;
        end = taille2;
    } else {
        tab = tab1;
        start = i1;
        end = taille1;
    }

    for (int i = start; i < end; i++) {
        buf[ibuf] = tab[i];
        ibuf++;
    }
    return buf;
}

void print_tab(int *tab, int size) {
    printf("{");
    for (int i = 0; i < size; i++) {
        if (i != 0)
            printf(", ");
        printf("%d", tab[i]);
    }
    printf("}\n");
}

int main() {
    int odd[] = {1, 3, 5, 7, 9};
    int even[] = {0, 2, 4, 6, 8, 10};
    int *buf = fusion(odd, 5, even, 6);
    print_tab(buf, 11);
}

#include <stdio.h>

void placeElt(float *tab, int i) {
    int value = tab[i];
    while (i - 1 >= 0 && tab[i - 1] > value) {
        tab[i] = tab[i - 1];
        i--;
    }
    tab[i] = value;
}

void print_tab(float *tab, int size) {
    printf("{");
    for (int i = 0; i < size; i++) {
        if (i != 0)
            printf(", ");
        printf("%.2f", tab[i]);
    }
    printf("}\n");
}

void sort(float *tab, int size) {
    for (int i = 1; i < size; i++) {
        placeElt(tab, i);
    }
}

int main() {
    float tab1[12] = {6, -4, 34, 0, -100, 7, 3, -6, 7, -4324, 234, 897};
    print_tab(tab1, 12);
    sort(tab1, 12);
    print_tab(tab1, 12);

    float tab2[1] = {0};
    print_tab(tab2, 1);
    sort(tab2, 1);
    print_tab(tab2, 1);

    float tab3[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    print_tab(tab3, 10);
    sort(tab3, 10);
    print_tab(tab3, 10);

    return 0;
}

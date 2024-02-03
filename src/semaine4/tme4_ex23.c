#include <stdio.h>
#include <stdlib.h>

#define JOURS 31

void init_temp(float *tab) {
    for (int i = 0; i < JOURS; i++) {
        tab[i] = (rand() / (float)RAND_MAX) * 500 - 200;
    }
}

float moy_temp(float *tab) {
    float moy = 0;
    for (int i = 0; i < JOURS; i++) {
        moy += tab[i];
    }
    return moy / JOURS; // devrait s'approcher de 50 sur plus de jours
}

float moy_temp_neg(float *tab) {
    float moy = 0;
    int count = 0;
    for (int i = 0; i < JOURS; i++) {
        if (tab[i] < 0) {
            moy += tab[i];
            count++;
        }
    }
    if (!count) {
        printf("Aucune temperature au-dessous de zero.\n");
    }
    return moy / count; // devrait s'approcher de 50 sur plus de jours
}

int main() {
    float tab[JOURS];
    init_temp(tab);
    for (int i = 0; i < JOURS; i++) {
        printf("%d - %.2f\n", i + 1, tab[i]);
    }
    printf("moyenne = %.2f\n", moy_temp(tab));
    return 0;
}

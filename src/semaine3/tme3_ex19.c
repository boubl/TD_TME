#include <stdio.h>

#define VAL1 -2
#define VAL2 -7
#define VAL3 -5
#define VAL4 9

void min_max(int n, int *min, int *max) {
    if (n < *min)
        *min = n;
    else if (n > *max)
        *max = n;
}

void stats(int v1, int v2, int v3, int v4, float *moyenne, int *min, int *max) {
    if (v1 <= 0) {
        *moyenne = -1.0;
        *min = -1;
        *max = -1;
        return;
    }

    int v[] = {v1, v2, v3, v4};
    *moyenne = v1;
    *min = v1;
    *max = v1;
    for (int i = 1; i < 4; i++) {
        if (v[i] <= 0) {
            return;
        }
        *moyenne = (*moyenne * i + v[i]) / (i + 1);
        min_max(v[i], min, max);
    }
}

void afficher_resultat(float moyenne, int min, int max) {
    printf("max = %d, min = %d, moy = %.2f\n", max, min, moyenne);
}

int main() {
    float moy;
    int min, max;

    stats(VAL1, VAL2, VAL3, VAL4, &moy, &min, &max);

    afficher_resultat(moy, min, max);

    return 0;
}

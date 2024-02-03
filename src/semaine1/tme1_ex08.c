#include <stdio.h>

#define TADULTE 22.7
#define TENFANT 10.75
#define TFAMILLE 57.80

float prixEntree(int adultes, int enfants) {
    float prixFamille = TFAMILLE;
    if (adultes > 2) {
        prixFamille += (adultes - 2) * TADULTE;
    }
    if (enfants > 3) {
        prixFamille += (enfants - 3) * TENFANT;
    }
    float prixSans = adultes * TADULTE + enfants * TENFANT;

    if (prixFamille < prixSans) {
        return prixFamille;
    } else {
        return prixSans;
    }
}

int main() {
    printf("%.2f\n", prixEntree(2, 3));
    printf("%.2f\n", prixEntree(2, 2));
    printf("%.2f\n", prixEntree(2, 1));
    printf("%.2f\n", prixEntree(1, 3));

    return 0;
}

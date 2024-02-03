#include <stdio.h>

#define TADULTE 22.7
#define TENFANT 10.75
#define TFAMILLE 57.80

float prixEntree(int adultes, int enfants) {
    // syntaxe pratique:
    // type valeur = condition ? valeur_si_vrai : valeur_si_faux;
    int nb_familles = (adultes / 2) > (enfants / 3) ? enfants / 3 : adultes / 2;
    adultes -= 2 * nb_familles;
    enfants -= 3 * nb_familles;

    float prixFamille = TFAMILLE;
    if (adultes > 2) {
        prixFamille += (adultes - 2) * TADULTE;
    }
    if (enfants > 3) {
        prixFamille += (enfants - 3) * TENFANT;
    }
    float prixSans = adultes * TADULTE + enfants * TENFANT;

    if (prixFamille < prixSans) {
        return prixFamille + nb_familles * TFAMILLE;
    } else {
        return prixSans + nb_familles * TFAMILLE;
    }
}

int main() {
    printf("56.15  -> %.2f\n", prixEntree(2, 1));
    printf("57.80  -> %.2f\n", prixEntree(2, 2));
    printf("57.80  -> %.2f\n", prixEntree(2, 3));
    printf("148.60 -> %.2f\n", prixEntree(6, 3));
    printf("54.95  -> %.2f\n", prixEntree(1, 3));
    printf("149.05 -> %.2f\n", prixEntree(5, 7));
    printf("173.40 -> %.2f\n", prixEntree(6, 8));
    printf("227.00 -> %.2f\n", prixEntree(10, 0));
    printf("43.00  ->%.2f\n", prixEntree(0, 4));

    return 0;
}

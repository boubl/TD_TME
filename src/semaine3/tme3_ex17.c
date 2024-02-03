#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NB_VALEURS 50
#define VMIN -20
#define VMAX 20

void pos_neg_zero(int num, int *pos, int *neg, int *zero) {
    // attention magie noire
    // Basiquement cette instruction:
    // - Cree une liste des pointeurs: (int *[]){neg, zero, pos}
    // - Determine le signe de 'num' et ajoute 1: (num > 0) - (num < 0) + 1
    // - Grace a ca on accede au pointeur correspondant dans notre liste
    // - Puis on dereference ce pointeur et on incremente la valeur
    (*(((int *[]){neg, zero, pos})[(num > 0) - (num < 0) + 1]))++;
}

int valeur_aleatoire(int min, int max) {
    assert(max >= min);
    // (rand() / (float)RAND_MAX) nous donne un float aléatoire entre 0 et 1
    // Puis on le multiplie par le nombre de valeurs possibles: (max - min)
    // Enfin on ajoute min au resultat pour que les nombres aléatoires commencent bien par min.
    return (int)((rand() / (float)RAND_MAX) * (max - min)) + min;
}

int main() {
    srand(time(NULL));

    int pos = 0;
    int neg = 0;
    int zero = 0;

    for (int i = 0; i < NB_VALEURS; i++) {
        int number = valeur_aleatoire(VMIN, VMAX);
        pos_neg_zero(number, &pos, &neg, &zero);
        printf("%d\n", number);
    }

    printf("pos:  %d\nneg:  %d\nzero: %d\n", pos, neg, zero);

    return 0;
}

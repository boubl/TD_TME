#include <math.h>
#include <stdio.h>

int nb_racines(int a, int b, int c) {
    int delta = b * b - 4 * a * c;
    return (int[]){0, 1, 2}[(delta > 0) - (delta < 0) + 1];
}

int nb_racines_delta(int a, int b, int c, int *delta) {
    *delta = b * b - 4 * a * c;
    return (int[]){0, 1, 2}[(*delta > 0) - (*delta < 0) + 1];
}

int racines(int a, int b, int c, float *r1, float *r2) {
    int delta;
    int nb_racines = nb_racines_delta(a, b, c, &delta);
    if (nb_racines == 2) {
        *r1 = (-b - sqrtf((float)delta)) / (2 * a);
        *r2 = (-b + sqrtf((float)delta)) / (2 * a);
    } else if (nb_racines == 1) {
        *r1 = -b / (float)(2 * a);
    }
    return nb_racines;
}

void affiche_polynome(int a, int b, int c) {
    /* hypothese : a != 0
      affiche le polynome sous la forme a*x*x + b*x + c
      n'affiche pas les monomes dont le coefficient est nul */
    if (a == 1) {
        printf("Polynome : x*x");
    } else {
        if (a == -1) {
            printf("Polynome : - x*x");
        } else {
            printf("Polynome : %d x*x", a);
        }
    }
    if (b != 0) {
        if (b > 0) {
            if (b == 1) {
                printf(" + x");
            } else {
                printf(" + %d x", b);
            }
        } else {
            if (b == -1) {
                printf(" - x");
            } else {
                printf(" - %d x", -1 * b);
            }
        }
    }
    if (c != 0) {
        if (c > 0) {
            printf(" + %d", c);
        } else {
            printf(" - %d", -1 * c);
        }
    }
    printf("\n");
}

void affiche_racines(int a, int b, int c) {
    float rac1 = 0.0, rac2 = 0.0;
    int nb_rac;

    nb_rac = racines(a, b, c, &rac1, &rac2);
    if (nb_rac == 2) {
        printf("Le polynome a 2 racines : %.3f et %.3f\n", rac1, rac2);
    }
    if (nb_rac == 1) {
        printf("Le polynome a 1 racine double : %.3f\n", rac1);
    }
    if (nb_rac == 0) {
        printf("Le polynome n'a pas de racine reelle.\n");
    }
}

int main() {
    int a, b, c;

    /*  1 racine double : -0,5 */
    a = 4;
    b = 4;
    c = 1;
    affiche_polynome(a, b, c);
    affiche_racines(a, b, c);
    printf("\n");

    /*  2 racines  : -0,191 et -1,309 */
    a = 4;
    b = 6;
    c = 1;
    affiche_polynome(a, b, c);
    affiche_racines(a, b, c);
    printf("\n");

    /* Que des coefficients negatifs, pas de racine */
    a = -7;
    b = -5;
    c = -1;
    affiche_polynome(a, b, c);
    affiche_racines(a, b, c);
    printf("\n");

    return 0;
}

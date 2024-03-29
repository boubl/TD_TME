#include <assert.h>
#include <math.h>
#include <stdio.h>

int discriminant(int a, int b, int c) { return (b * b) - (4 * a * c); }

void afficheRacines(int a, int b, int c) {
    int delta = discriminant(a, b, c);
    if (delta < 0) {
        printf("Pas de racine reelle\n");
    } else if (delta == 0) {
        float racine = (float)(-b) / (2 * a);
        printf("La racine double est %.2f\n", racine);
    } else {
        float r1 = (-b - sqrt(delta)) / (2 * a);
        float r2 = (-b + sqrt(delta)) / (2 * a);
        printf("Les deux racines sont %.2f et %.2f\n", r2, r1);
    }
}

int main() {
    // Question 1
    assert(discriminant(4, 4, 1) == 0);

    // Question 2
    afficheRacines(4, 4, 1);
    afficheRacines(4, 6, 1);
    afficheRacines(-7, -5, -1);
}

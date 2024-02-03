#include <assert.h>

int jours(int contaminations, int population, float seuil) {
    int nbContamines = 1;
    int jours = 0;
    while ((100.0 * nbContamines / population) < seuil) {
        nbContamines += nbContamines * contaminations;
        jours++;
    }
    return jours;
}

int main() {
    assert(jours(5, 10000, 100) == 6);
    assert(jours(5, 10000, 50) == 5);
    assert(jours(5, 10000, 25) == 5);
    assert(jours(5, 10000, 10) == 4);
}

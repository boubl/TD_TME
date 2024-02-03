#include <assert.h>

int signeProduit(int a, int b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    // Ici le ptit chapeau c'est un 'XOR', un ou exclusif (eXclusive OR).
    // Il donne vrai seulement si c'est different.
    // A utiliser que avec des 1 et des 0 sauf si vous savez ce que vous
    // faites parce que c'est une operation binaire (cherchez la difference
    // entre & et && en C sur Google pour en savoir plus).
    if ((a < 0) ^ (b < 0)) {
        return -1;
    }
    return 1;
}

int main() {
    assert(signeProduit(0, 0) == 0);
    assert(signeProduit(1, 0) == 0);
    assert(signeProduit(0, -1) == 0);
    assert(signeProduit(-1, 1) == -1);
    assert(signeProduit(1, -1) == -1);
    assert(signeProduit(1, 1) == 1);
    assert(signeProduit(-1, -1) == 1);
}

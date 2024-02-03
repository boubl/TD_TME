#include <stdio.h>

int indiceInsert(int *tab, int taille, int nbEl, int el) {
    if (nbEl == taille)
        return -1;
    int i = 0;
    while (tab[i] < el && i < nbEl) {
        i++;
    }
    if (tab[i] == el && i < nbEl) // deuxieme partie du if pour prevoir un bug
        return -1;
    return i;
}

int insertElt(int *tab, int taille, int *nbEl, int el) {
    int i = indiceInsert(tab, taille, *nbEl, el);
    if (i == -1)
        return 0;
    (*nbEl)++;
    for (int k = i; k < *nbEl; k++) {
        int temp = tab[k];
        tab[k] = el;
        el = temp;
    }
    return 1;
}

void print_tab(int *tab, int taille, int nbEl) {
    printf("{");
    for (int i = 0; i < nbEl; i++) {
        if (i != 0)
            printf(", ");
        printf("%d", tab[i]);
    }
    char *plural = (taille - nbEl > 1) ? "s" : "";
    printf("} + %d case%s vide%s\n", taille - nbEl, plural, plural);
}

int main() {
    int tab[5];
    int taille = 5;
    int nbEl = 0;
    print_tab(tab, taille, nbEl);
    insertElt(tab, taille, &nbEl, 1);
    print_tab(tab, taille, nbEl);
    insertElt(tab, taille, &nbEl, 5);
    print_tab(tab, taille, nbEl);
    insertElt(tab, taille, &nbEl, 3);
    print_tab(tab, taille, nbEl);
    insertElt(tab, taille, &nbEl, 4);
    print_tab(tab, taille, nbEl);
    insertElt(tab, taille, &nbEl, 5); // cas ou le nombre existe deja
    print_tab(tab, taille, nbEl);
    insertElt(tab, taille, &nbEl, 2);
    print_tab(tab, taille, nbEl);
    insertElt(tab, taille, &nbEl, 6); // cas ou le le tableau est plein
    print_tab(tab, taille, nbEl);

    return 0;
}

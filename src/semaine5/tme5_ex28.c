#include <stdio.h>

#define NB_AMIS 5
#define NB_JOURS 7

void init(float tab[][NB_JOURS], int taille) {
  for (int i = 0; i < taille; i++) {
    for (int j = 0; j < NB_JOURS; j++) {
      tab[i][j] = 0.0;
    }
  }
}

void affecte_depenses(float tab[][NB_JOURS], int jour, float montant, int id) {
  for (int i = 0; i < NB_AMIS; i++) {
    tab[i][jour] = (id == i) * montant - (montant / NB_AMIS);
  }
}

float calcul_solde(float tab[][NB_JOURS], int id) {
  float solde = 0;
  for (int i = 0; i < NB_JOURS; i++) {
    solde += tab[id][i];
  }
  return solde;
}

void affiche_soldes(float tab[][NB_JOURS]) {
  for (int i = 0; i < NB_AMIS; i++) {
    float solde = calcul_solde(tab, i);
    printf("Solde pour %d : %.2f\n", i, solde);
  }
}

void print_tab(float tab[]) {
  printf("{");
  for (int i = 0; i < NB_JOURS; i++) {
    printf(" %.2f", tab[i]);
    if (i + 1 < NB_JOURS)
      printf(",");
  }
  printf(" }");
}

void print_dtab(float tab[][NB_JOURS], int size) {
  printf("\{");
  for (int i = 0; i < size; i++) {
    printf("\n  ");
    print_tab(tab[i]);
    if (i + 1 < size)
      printf(",");
  }
  printf("\n}\n");
}

int main() {
  float tab[NB_AMIS][NB_JOURS];
  init(tab, NB_AMIS);
  print_dtab(tab, NB_AMIS);
  return 0;
}


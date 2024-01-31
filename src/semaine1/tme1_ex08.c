#include <stdio.h>

#define ADULTE 22.7
#define ENFANT 10.75
#define ENFANTM5 0
#define FAMILLE 57.80

float prixEntree(int adultes, int enfants) {
  float prixFamille = FAMILLE;
  if (adultes > 2 && enfants > 3) {
    prixFamille += (adultes - 2)*ADULTE + (enfants - 3)*ENFANT;
  }
  float prixSans = adultes*ADULTE + enfants*ENFANT;

  if (prixFamille < prixSans) {
    return prixFamille;
  } else {
    return prixSans;
  }
}

int main() {
  printf("%f\n", prixEntree(2, 3));
  printf("%f\n", prixEntree(2, 2));
  printf("%f\n", prixEntree(2, 1));
  printf("%f\n", prixEntree(1, 3));

  return 0;
}

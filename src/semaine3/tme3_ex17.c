#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NB_VALEURS 50
#define VMIN -20
#define VMAX 20

void pos_neg_zero(int res, int* pos, int* neg, int* zero) {
  (*(((int*[]){neg, zero, pos})[(res > 0) - (res < 0) + 1]))++;
}

int valeur_aleatoire(int min, int max) {
  assert(max >= min);

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

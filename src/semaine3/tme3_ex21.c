#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PIERRE 1
#define FEUILLE PIERRE+1
#define CISEAUX FEUILLE+1
#define NBESSAIS 3
#define POINTSGAGNANTS 3
  /* Il sera probablement nécessaire de modifier les parametres et valeur de retour des fonctions 
    choix_ordinateur, choix_joueur, score et jeu */

  /* Les declarations actuelles permettent de compiler et d'executer le programme */

char* nom_objet(int obj) {
  /* hypothèse : obj = PIERRE ou
     obj = FEUILLE ou obj = CISEAUX
   
   renvoie le nom de l'objet correspondant à obj */
  return (char*[]){"PIERRE", "FEUILLE", "CISEAUX"}[obj - PIERRE];
}

int choix_ordinateur() {
/* hypothèse : PIERRE < FEUILLE < CISEAUX
  toutes les valeurs comprises entre PIERRE et CISEAUX sont "valables"
  renvoie une valeur choisie aléatoirement entre PIERRE, FEUILLE et CISEAUX */
  
  return (int)((rand() / (float)RAND_MAX) * 29 / 10) + 1;
}

int choix_joueur() {
/* hypothèses : PIERRE < FEUILLE < CISEAUX 
  renvoie la valeur choisie par le joueur
  vérifie qu'elle est comprise entre PIERRE et CISEAUX 
  redemande la saisie si ce n'est pas le cas
  
  Si après NBESSAIS le joueur n'a pas saisi de valeur valable,
  cette dernière est tirée au sort */
  int choix = -1;
  for (int i = 0; i < NBESSAIS; i++) {
    printf("Faites votre choix:\n - Pierre (%d)\n - Feuille (%d)\n - Ciseaux (%d)\n?: ", PIERRE, FEUILLE, CISEAUX);
    if (scanf("%d", &choix) && choix > 0 && choix < 4) {
      printf("Vous avez choisi %s\n", nom_objet(choix));
      break;
    } else {
      // TODO: flush stdin to better handle text input
      char *try_again = 2 - i == 0 ? "" : "Reessayez. ";
      choix = -1;
      printf("Choix invalide. %s(il vous reste %d essais)\n", try_again, NBESSAIS - 1 - i);
    }
  }
  if (choix == -1) {
    choix = choix_ordinateur();
    printf("Trop de choix invalides. Votre choix aleatoire: %s\n", nom_objet(choix));
  }
  
  return choix;
}

int score(int ordi, int joueur, int *points_o, int *points_j){
  /* hypothèses : le coup du joueur et celui de l'ordinateur sont valables 
                  (égaux à PIERRE, FEUILLE ou CISEAUX)
    renvoie 0, 1 ou 2 respectivement si il y a egalite, si le joueur est gagnant ou si l'ordinateur est gagnant.
   augmente de 1 le score du joueur si le coup du joueur est gagnant
   augmente de 1 le score de l'ordinateur si le coup de l'ordinateur est gagnant */
  if (ordi == joueur) return 0;
  else if (ordi > joueur && joueur != CISEAUX) {
    (*points_o)++;
    return 2;
  }
  else {
    (*points_j)++;
    return 1;
  }
}


void jeu() {
  /* boucle de jeu, la partie s'arrête dès qu'un des deux joueurs 
   atteint POINTSGAGNANTS points */

  // int compteurs[] = {0, 0, 0, 0};
  // while (1) {
  //   compteurs[choix_ordinateur()]++;
  //   compteurs[0]++;
  //   printf("P: %.2f, F: %.2f, C: %.2f\n", compteurs[1] / (float)compteurs[0], compteurs[2] / (float)compteurs[0], compteurs[3] / (float)compteurs[0]);
  // }

  printf("Jouons a Pierre/Feuille/Ciseaux!\n");

  int points_o, points_j = 0;
  while (points_o < POINTSGAGNANTS && points_j < POINTSGAGNANTS) {
    if (points_o != 0 && points_j != 0) {
      printf("Manche suivante !\n");
    }
    int choix_o = choix_ordinateur();
    printf("L'ordinateur a fait son choix.\n");
    int choix_j = choix_joueur();
    int resultat = score(choix_o, choix_j, &points_o, &points_j);
    const char* qui_gagne = (char*[]){"Personne", "Le joueur", "L'ordinateur"}[resultat];
    printf("%s (ordi) contre %s (joueur): %s gagne la manche.\n", nom_objet(choix_o), nom_objet(choix_j), qui_gagne);
    printf("Scores:\n");
    printf("Ordinateur: %d - Joueur %d\n", points_o, points_j);
  }
  printf("%s gagne la partie!\n", points_o > points_j ? "L'ordinateur" : "Le joueur");
}

int main() {
  
  srand(time(NULL));
 
  jeu();
  return 0;
}

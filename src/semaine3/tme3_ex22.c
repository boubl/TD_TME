#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NBTOURS 5
#define NBLANCERS 2
#define NBQUILLES 10

int lancer(int quilles) {
    int input = -1;
    while (input == -1) {
        if (scanf("%d", &input) && input >= 0 && input <= quilles) {
            return input;
        } else {
            // TODO: handle text input
            // TODO: display something to say it was wrong
        }
    }
    return 0;
}

int lancer_aleatoire(int quilles) {
    return (int)(rand() / (float)RAND_MAX * (quilles * 10 + 9)) / 10;
}

void score(int spare, int strike, int strike2, int quilles, int *score_ptr) {
    *score_ptr += (spare * quilles) + (strike * quilles) + (strike2 * quilles) + quilles;
}

void tour(int *spare, int *strike, int *score_ptr) {
    int quilles = NBQUILLES;
    int strike2 = *strike;

    for (int i = 0; i < 2; i++) {
        int nouveau_lancer = lancer_aleatoire(quilles);
        quilles -= nouveau_lancer;
        printf("Lancer %d: %d quille(s) renversee(s)\n", i + 1, nouveau_lancer);

        score(*spare, *strike && i == 0, strike2 && i == 1, nouveau_lancer, score_ptr);

        if (quilles == 0) {
            if (i == 0) {
                printf("Strike!!\n");
                *spare = 0;
                *strike = 1;
                break; // on break car on ne lancera pas une deuxieme fois
            } else {
                printf("Spare!!\n");
                *strike = 0;
                *spare = 1;
                // pas besoin de break, on est deja au deuxieme lancer
            }
        } else {
            *strike = 0;
            *spare = 0;
        }
    }
}

int jeu() {
    int strike = 0;
    int spare = 0;
    int score = 0;

    for (int i = 0; i < NBTOURS; i++) {
        printf("Tour %d:\n", i + 1);
        tour(&spare, &strike, &score);
        printf("Score: %d\n\n", score);
    }

    // tour supplementaire en dehors de la boucle car c'est plus simple
    if (strike || spare) {
        printf("Tour supplementaire:\n");
        spare = 0;
        strike = 0;
        tour(&spare, &strike, &score);
    }

    printf("Score final: %d\n", score);
    return 0;
}

int main() {
    srand(time(NULL));
    jeu();
    return 0;
}

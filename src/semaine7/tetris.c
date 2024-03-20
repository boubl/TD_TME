#include <SDL2/SDL.h>
#include <cini.h>

#define TAILLE_CASE 20
#define HAUTEUR 22
#define LARGEUR 10
#define VIDE 8 /* n'importe quelle valeur qui n'est pas un indice du tableau des couleurs */
#define BACKGROUND "black"

/*-------------   INSERER ICI LES DEFINITIONS DE TYPES   --------------*/

struct une_case {
    int colonne;
    int ligne;
};

struct piece {
    int pos_ligne, pos_colonne;
    struct une_case la_piece[4];
    int type;
};

/*-------------------   INSERER ICI LES FONCTIONS   -------------------*/

void afficher_plateau(int plateau[LARGEUR][HAUTEUR], char *couleurs[]) {
    for (int x = 0; x < LARGEUR; x++) {
        for (int y = 0; y < HAUTEUR; y++) {
            if (plateau[x][y] < 7)
                CINI_fill_rect(x * TAILLE_CASE, y * TAILLE_CASE, TAILLE_CASE, TAILLE_CASE,
                               couleurs[plateau[x][y]]);
        }
    }
}

int decaler_gauche(int plateau[LARGEUR][HAUTEUR], struct piece *p_tetromino) {
    for (int i = 0; i < 4; i++) {
        int x = p_tetromino->la_piece[i].colonne + p_tetromino->pos_colonne - 1;
        int y = p_tetromino->la_piece[i].ligne + p_tetromino->pos_ligne;

        if (plateau[x][y] != VIDE || x < 0) {
            return 0;
        }
    }

    p_tetromino->pos_colonne -= 1;
    return 1;
}

int decaler_droite(int plateau[LARGEUR][HAUTEUR], struct piece *p_tetromino) {
    for (int i = 0; i < 4; i++) {
        int x = p_tetromino->la_piece[i].colonne + p_tetromino->pos_colonne + 1;
        int y = p_tetromino->la_piece[i].ligne + p_tetromino->pos_ligne;

        if (plateau[x][y] != VIDE || x >= LARGEUR) {
            return 0;
        }
    }

    p_tetromino->pos_colonne += 1;
    return 1;
}

int rotation_droite(int plateau[LARGEUR][HAUTEUR], struct piece *p_tetromino) {
    for (int i = 0; i < 4; i++) {
        int x = -p_tetromino->la_piece[i].ligne + p_tetromino->pos_colonne;
        int y = p_tetromino->la_piece[i].colonne + p_tetromino->pos_ligne;

        if (plateau[x][y] != VIDE || y >= HAUTEUR || y < 0 || x < 0 || x >= LARGEUR) {
            return 0;
        }
    }
    for (int j = 0; j < 4; j++) {
        int x = -p_tetromino->la_piece[j].ligne;
        int y = p_tetromino->la_piece[j].colonne;
        p_tetromino->la_piece[j].colonne = x;
        p_tetromino->la_piece[j].ligne = y;
    }

    return 1;
}

int rotation_gauche(int plateau[LARGEUR][HAUTEUR], struct piece *p_tetromino) {
    for (int i = 0; i < 4; i++) {
        int x = p_tetromino->la_piece[i].ligne + p_tetromino->pos_colonne;
        int y = -p_tetromino->la_piece[i].colonne + p_tetromino->pos_ligne;

        if (plateau[x][y] != VIDE || y >= HAUTEUR || y < 0 || x < 0 || x >= LARGEUR) {
            return 0;
        }
    }
    for (int j = 0; j < 4; j++) {
        int x = p_tetromino->la_piece[j].ligne;
        int y = -p_tetromino->la_piece[j].colonne;
        p_tetromino->la_piece[j].colonne = x;
        p_tetromino->la_piece[j].ligne = y;
    }

    return 1;
}

int descendre(int plateau[LARGEUR][HAUTEUR], struct piece *p_tetromino) {
    for (int i = 0; i < 4; i++) {
        int x = p_tetromino->la_piece[i].colonne + p_tetromino->pos_colonne;
        int y = p_tetromino->la_piece[i].ligne + p_tetromino->pos_ligne + 1;

        if (plateau[x][y] != VIDE || y >= HAUTEUR) {
            for (int j = 0; j < 4; j++) {
                int px = p_tetromino->la_piece[j].colonne + p_tetromino->pos_colonne;
                int py = p_tetromino->la_piece[j].ligne + p_tetromino->pos_ligne;
                plateau[px][py] = p_tetromino->type;
            }

            return 0;
        }
    }

    p_tetromino->pos_ligne += 1;
    return 1;
}

void hard_drop(int plateau[LARGEUR][HAUTEUR], struct piece *p_tetromino) {
    while (descendre(plateau, p_tetromino))
        ;
}

void supprimer_lignes(int plateau[LARGEUR][HAUTEUR]) {
    int offset = 0;
    for (int y = HAUTEUR - 1; y >= 0; y--) {
        int full_line = 1;
        for (int x = 0; x < LARGEUR && full_line; x++) {
            if (plateau[x][y] == VIDE)
                full_line = 0;
        }

        for (int x = 0; x < LARGEUR; x++) {
            if (offset && !full_line)
                plateau[x][y + offset] = plateau[x][y];
            if (offset || full_line)
                plateau[x][y] = VIDE;
        }

        offset += full_line;
    }
}

int partie_perdue(int plateau[LARGEUR][HAUTEUR]) {
    for (int x = 0; x < LARGEUR; x++) {
        if (plateau[x][0] != VIDE)
            return 1;
    }
    return 0;
}

/* Affiche une piece */
void afficher_piece(struct piece tetromino, char *couleur) {
    int i;
    int ligne_deb, colonne_deb;

    for (i = 0; i < 4; i++) {
        ligne_deb = (tetromino.pos_ligne + tetromino.la_piece[i].ligne) * TAILLE_CASE;
        colonne_deb = (tetromino.pos_colonne + tetromino.la_piece[i].colonne) * TAILLE_CASE;
        CINI_fill_rect(colonne_deb, ligne_deb, TAILLE_CASE, TAILLE_CASE, couleur);
    }
}

/* Initialisation d'une piece, elle se trouve initialement en haut et au milieu du plateau */
void initialiser(struct piece *p_tetromino, struct une_case tab[4], int indice) {
    int i;
    p_tetromino->pos_colonne = LARGEUR / 2;
    p_tetromino->pos_ligne = 0;
    for (i = 0; i < 4; i++) {
        p_tetromino->la_piece[i] = tab[i];
    }

    p_tetromino->type = indice;
}

/* Afficher toutes les pieces, permet de tester si les pieces sont bien declarees */
void afficher_toutes_pieces(char *color[], struct une_case tab_pieces[7][4]) {
    int i;
    int lig = 0;
    struct piece tetromino;

    for (i = 0; i < 7; i++) {
        initialiser(&tetromino, tab_pieces[i], i);
        tetromino.pos_ligne = lig;
        lig = lig + 3;
        afficher_piece(tetromino, color[tetromino.type]);
    }
}

/*---------------------------------------------------------------------*/

int main() {

    /*---------------------   les declarations   ---------------------*/

    int indice; /* l'indice (type) de la piece jouee */
    int touche; /* identifie la frappe du joueur */

    char *color[] = {"light salmon", "fuchsia", "lime green", "white", "yellow", "cyan", "grey"};

    struct une_case tab_pieces[7][4] = {{{0, 0}, {1, 0}, {0, 1}, {1, 1}},  // O
                                        {{0, 0}, {0, 1}, {0, 2}, {0, 3}},  // I
                                        {{0, 0}, {0, 1}, {0, 2}, {1, 2}},  // L
                                        {{1, 0}, {1, 1}, {1, 2}, {0, 2}},  // J
                                        {{0, 0}, {1, 0}, {1, 1}, {2, 1}},  // Z
                                        {{0, 1}, {1, 0}, {1, 1}, {2, 0}},  // S
                                        {{0, 0}, {1, 0}, {1, 1}, {2, 0}}}; // T

    /* (a completer) */
    int plateau[LARGEUR][HAUTEUR];

    /*--------------------  les initialisations  ---------------------*/

    srand(time(NULL));

    for (int x = 0; x < LARGEUR; x++) {
        for (int y = 0; y < HAUTEUR; y++) {
            plateau[x][y] = VIDE;
        }
    }

    /*----------------------  ouverture de la fenetre graphique -------*/

    CINI_open_window(LARGEUR * TAILLE_CASE, HAUTEUR * TAILLE_CASE, "black");

    /* Affichage de test des pieces */
    /* Instruction a mettre en commentaire pour la suite */
    // afficher_toutes_pieces(color, tab_pieces);

    /*----------------------  la boucle de jeu  ----------------------*/

    do {
        CINI_fill_window(BACKGROUND);
        afficher_plateau(plateau, color);
        indice = rand() % 7;
        struct piece tetromino;
        initialiser(&tetromino, tab_pieces[indice], indice);
        afficher_piece(tetromino, color[indice]);

        do {
            touche = CINI_loop_until_keyup();
            afficher_piece(tetromino,
                           BACKGROUND); /* "efface" la piece en l'affichant en noir ... */
            descendre(plateau, &tetromino);
            switch (touche) {
            case SDLK_LEFT:
                decaler_gauche(plateau, &tetromino);
                break;
            case SDLK_RIGHT:
                decaler_droite(plateau, &tetromino);
                break;
            case SDLK_d:
                rotation_gauche(plateau, &tetromino);
                break;
            case SDLK_g:
                rotation_droite(plateau, &tetromino);
                break;
            case SDLK_UP:
                hard_drop(plateau, &tetromino);
                break;
            default:
                break;
            }
            afficher_piece(tetromino, color[indice]); /* ... avant de la reafficher
                                                                   apres modification */
        } while ((touche != SDLK_DOWN) && (touche != SDLK_ESCAPE));

        /* on supprime les lignes completes si elles existent */
        supprimer_lignes(plateau);

    } while (touche != SDLK_ESCAPE && !partie_perdue(plateau));

    return 0;
}

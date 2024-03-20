#include <SDL2/SDL.h>
#include <cini.h>
#include <time.h>

#define TAILLE_CASE 20
#define HAUTEUR 22
#define LARGEUR 10
#define VIDE 8 /* n'importe quelle valeur qui n'est pas un indice du tableau des couleurs */
#define BACKGROUND "black"
#define CHANGEMENT_NIVEAU 10
#define DIMINUTION_PERIODE 50

/* (a completer) */

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

int supprimer_lignes(int plateau[LARGEUR][HAUTEUR], int *score, int niveau) {
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

    if (offset) {
        *score += (niveau + 1) * (int[4]){40, 100, 300, 1200}[offset - 1];
    }
    return offset;
}

int partie_perdue(int plateau[LARGEUR][HAUTEUR]) {
    for (int x = 0; x < LARGEUR; x++) {
        if (plateau[x][0] != VIDE)
            return 1;
    }
    return 0;
}

/* Affiche une piece */
void afficher_piece(struct piece tetromino, char *color) {
    int i;
    int ligne_deb, colonne_deb;

    for (i = 0; i < 4; i++) {
        ligne_deb = (tetromino.pos_ligne + tetromino.la_piece[i].ligne) * TAILLE_CASE;
        colonne_deb = (tetromino.pos_colonne + tetromino.la_piece[i].colonne) * TAILLE_CASE;
        CINI_fill_rect(colonne_deb, ligne_deb, TAILLE_CASE, TAILLE_CASE, color);
    }
}

void afficher_score(int score) {
    CINI_draw_int_table(LARGEUR * TAILLE_CASE, (HAUTEUR / 2) * TAILLE_CASE, &score, 1, "black",
                        "white");
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

/*-------------------------   Gestion du temps  ---------------------------*/

Uint32 at_top_horloge(Uint32 interval, void *param) {
    /* ajoute un evenement SDL_USEREVENT dans la file a l'echeance de l'horloge
       et reinitialise celle-ci a interval */
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = NULL;
    userevent.data2 = param;

    event.type = SDL_USEREVENT;
    event.user = userevent;
    SDL_PushEvent(&event);

    return interval;
}

/*------------------------------------------------------------------------*/

int main() {

    /* les variables */
    char *color[] = {"light salmon", "fuchsia", "lime green", "white", "yellow", "cyan", "grey"};

    struct une_case tab_pieces[7][4] = {{{0, 0}, {1, 0}, {0, 1}, {1, 1}},  // O
                                        {{0, 0}, {0, 1}, {0, 2}, {0, 3}},  // I
                                        {{0, 0}, {0, 1}, {0, 2}, {1, 2}},  // L
                                        {{1, 0}, {1, 1}, {1, 2}, {0, 2}},  // J
                                        {{0, 0}, {1, 0}, {1, 1}, {2, 1}},  // Z
                                        {{0, 1}, {1, 0}, {1, 1}, {2, 0}},  // S
                                        {{0, 0}, {1, 0}, {1, 1}, {2, 0}}}; // T

    int plateau[LARGEUR][HAUTEUR];
    for (int x = 0; x < LARGEUR; x++) {
        for (int y = 0; y < HAUTEUR; y++) {
            plateau[x][y] = VIDE;
        }
    }

    /*---------------------   les declarations   ---------------------*/

    int descente_OK = 1; /* determine si la piece peut encore descendre */
    int running = 1;     /* passe a 0 si l'utilsateur utilise la touche ESCAPE
                             permet un arret premature de la partie              */
    int cpt = 0, niveau = 0, score = 0;
    SDL_TimerID horloge;     /* l'identifiant de l'horloge qui cadence la chute des pieces */
    Uint32 intervalle = 500; /* la periode de l'horloge (en ms) */
    SDL_Event event;         /* l'evenement a traiter */

    int indice; /* l'indice (type) de la piece jouee */

    /* (a completer) */

    /*--------------------  les initialisations  ---------------------*/

    srand(time(NULL));

    /* (a completer) */

    /*----------------------  ouverture de la fenetre graphique -------*/

    /* CINI_open_window appelle SDL_Init qui initialise le timer :
       a executer imperativement AVANT la creation de l'horloge  */

    /* (a completer) CREATION DE LA FENETRE */
    CINI_open_window(LARGEUR * TAILLE_CASE * 2, HAUTEUR * TAILLE_CASE, "black");

    // cette fonction n'existe plus avec la SDL2, je la remplace avec une fonction CINI custom
    // screen = SDL_GetVideoSurface(); /* la surface video associee a la fenetre */

    /* at_top_horloge est appelee a chaque echeance de intervalle */
    horloge = SDL_AddTimer(intervalle, at_top_horloge, NULL);

    /*----------------------  la boucle de jeu  ----------------------*/

    do {
        CINI_fill_rect(0, 0, LARGEUR * TAILLE_CASE, HAUTEUR * TAILLE_CASE, BACKGROUND);
        afficher_plateau(plateau, color);
        afficher_score(score);
        CINI_draw_line(LARGEUR * TAILLE_CASE, 0, LARGEUR * TAILLE_CASE, HAUTEUR * TAILLE_CASE,
                       "white");
        indice = rand() % 7;
        struct piece tetromino;
        initialiser(&tetromino, tab_pieces[indice], indice);
        afficher_piece(tetromino, color[indice]);
        descente_OK = 1;

        do {
            CINI_ForceUpdate();    /* maj de l'affichage graphique */
            SDL_WaitEvent(&event); /* on attend qu'il se passe quelque chose */
            afficher_piece(tetromino, BACKGROUND);
            switch (event.type) {
            case SDL_USEREVENT:
                descente_OK = descendre(plateau, &tetromino);
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
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
                case SDLK_SPACE:
                    hard_drop(plateau, &tetromino);
                    break;
                case SDLK_ESCAPE:
                    running = 0;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            afficher_piece(tetromino, color[indice]);
        } while (running && descente_OK);
        cpt += supprimer_lignes(plateau, &score, niveau);

        if (cpt > CHANGEMENT_NIVEAU) {
            SDL_RemoveTimer(horloge);
            intervalle = intervalle - DIMINUTION_PERIODE;
            horloge = SDL_AddTimer(intervalle, at_top_horloge, NULL);
            niveau++;
            cpt = cpt - CHANGEMENT_NIVEAU;
        }

    } while (running);
    /* condition a completer en tenant compte du resultat de partie_perdue */
    /*----------------------------------------------------------------*/

    CINI_loop(); /* on attend (pour visualiser la fin de partie : score et plateau) */
    return 0;
}

#include "cini.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include <SDL.h>
// pour l'affichage de polices
#include <SDL_ttf.h>
#include <ft2build.h>
#include <stdio.h>
#include FT_FREETYPE_H

#include "font.h"

#define tolower(c) ((c >= 'A' && c <= 'Z') ? (c - 'A') + 'a' : c)

#define hex(c)                                                                                     \
    ((tolower(c) >= 'a' && tolower(c) <= 'f') ? tolower(c) - 'a' + 10                              \
                                              : ((c >= '0' && c <= '9') ? c - '0' : -1))

//-----------------------------  LES VARIABLES GLOBALES
// global var used to memorize cursor position when clic

static unsigned int clic_posx;
static unsigned int clic_posy;

// pour garder la compatibilite avec la version precedente. 1 fenetre unique, statique.
static SDL_Window *screen;
static SDL_Renderer *renderer;
static SDL_Texture *texture;

static TTF_Font *font;

//-----------------------------  LE SYSTEME DE FENETRAGE

void CINI_close_display() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);
    SDL_Quit();
}

void CINI_close_window() { CINI_close_display(); }

void CINI_close_fullscreen() { CINI_close_display(); }

void CINI_open_display(int width, int height, char *title, Uint32 flags) {

    // Initialize SDL 2D display component.
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "Erreur: ne peut pas initialiser le système graphique (\"%s\").\n",
                SDL_GetError());
        return;
    }

    // Make sure that SDL_Quit is executed upon exit (especially
    // important if running in fullscreen display).
    atexit(CINI_close_display);

    // Initialized windowed mode with given size and flags.
    screen = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                              flags | SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(
        screen, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width,
                                height);

    if (screen == NULL || renderer == NULL || texture == NULL) {
        fprintf(stderr, "Oups, probleme a la creation de la fenetre...\n");
        exit(1);
    }

    // Enable key repeat (i.e.: when a key is kept down, the keyboard
    // event is repeteadly triggered).

    // SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_INTERVAL / 4, SDL_DEFAULT_REPEAT_DELAY / 12);

    // Gestion de l'affichage de caracteres
    // plus qu'inspire de showfont.c par Sam Lantinga <slouken@libsdl.org>

    // Start TTF engine.
    if (TTF_Init() < 0) {
        fprintf(stderr, "Couldn't initialize TTF: %s\n", SDL_GetError());
        SDL_Quit();
        exit(2);
    }
    atexit(TTF_Quit);

    int ptsize = 18;
    font = TTF_OpenFontRW(SDL_RWFromMem(font_ttf, font_ttf_len), 1, ptsize);
    if (font == NULL) {
        fprintf(stderr, "Erreur: ne peut pas trouver la police '%s' (%s).\n", FONTFILE,
                TTF_GetError());
    }

    SDL_SetRenderTarget(renderer, texture);

    // try to fix rendering artifacts on macos
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void CINI_open_fullscreen() {
    // Open fullscreen display
    CINI_open_display(0, 0, "", SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void CINI_open_window(int width, int height, char *title) {
    // Open window.
    CINI_open_display(width, height, title, SDL_WINDOW_RESIZABLE);
}

//-----------------------------  LES CONVERSIONS  -----------------------------

char digit(int i) {
    /* renvoie le caractere representant le chiffre i */
    if ((0 <= i) && (i <= 9)) {
        return ('0' + i);
    }
    return '?';
}

char *sofi(int n) {
    /* construit une chaine de caracteres representant l'entier n */
    int i, size, neg = 0;
    char *s;

    if (n < 0) {
        n = -n;
        neg = 1;
    }

    /* size = nb chiffres (n) + 1 (\0) + 1 (signe) */
    if (n == 0) {
        size = 3;
    } else {
        size = ceil(log(n) / log(10)) + 2;
    }
    s = (char *)malloc(size * sizeof(char));
    s[size - 1] = '\0';
    i = size - 2;
    do {
        s[i] = digit(n % 10);
        n = n / 10;
        i--;
    } while (n > 0);

    if (neg) {
        s[i] = '-';
        return (s + i);
    }
    return (s + i + 1);
}

char *soff(float f) {
    /* construit une chaine de caracteres representant le float f */
    int i, ent, dec, size, neg = 0;
    char *s;

    if (f < 0) {
        f = -f;
        neg = 1;
    }
    /* size = nb chiffres(E(f)) + 3 (virgule et partie decimale) + 1 (\0) + 1 (signe) */
    if (f == 0) {
        size = 6;
    } else {
        size = ceil(log(f) / log(10)) + 5;
    }
    s = (char *)malloc(size * sizeof(char));
    ent = floor(f);              // partie entiere par valeur inferieure
    dec = rint((f - ent) * 100); // arrondi au plus proche. On garde 2 decimales
    s[size - 1] = '\0';
    i = size - 2;
    do {
        s[i] = digit(dec % 10);
        dec = dec / 10;
        i--;
    } while (dec > 0);
    s[i] = '.';
    i--;
    do {
        s[i] = digit(ent % 10);
        ent = ent / 10;
        i--;
    } while (ent > 0);

    if (neg) {
        s[i] = '-';
        return (s + i);
    }
    return (s + i + 1);
}

//-----------------------------  LES VERIFICATIONS  -----------------------------

int outOfBoundaries(SDL_Renderer *renderer, int x, int y) {
    /* renvoie 0 si les coordonnees (x, y) sont dans la fenetre */

    int w, h; /* les dimensions de la fenetre */

    if (SDL_GetRendererOutputSize(renderer, &w, &h) == 0) {
        char *location = malloc(sizeof(char) * 32);
        int out_x = x >= w ? 1 : (x < 0 ? -1 : 0);
        int out_y = y >= h ? 1 : (y < 0 ? -1 : 0);
        int out = (out_x != 0 || out_y != 0);

        if (out_x == -1 && out_y == -1)
            strcpy(location, "en haut à gauche");
        else if (out_x == 0 && out_y == -1)
            strcpy(location, "en haut");
        else if (out_x == 1 && out_y == -1)
            strcpy(location, "en haut à droite");
        else if (out_x == 1 && out_y == 0)
            strcpy(location, "à droite");
        else if (out_x == 1 && out_y == 1)
            strcpy(location, "en bas à droite");
        else if (out_x == 0 && out_y == 1)
            strcpy(location, "en bas");
        else if (out_x == -1 && out_y == 1)
            strcpy(location, "en bas à gauche");
        else if (out_x == -1 && out_y == 0)
            strcpy(location, "à gauche");

        if (out)
            fprintf(stderr, "Warning: (%d, %d) -> la fenêtre fait %dx%d, le pixel sort %s.\n", x, y,
                    w, h, location);

        /* Alexandre Michalon*/
        /* liberation de la memoire allouee a location */
        free(location);
        return out;
    } else {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }
}

//-----------------------------  LE DESSIN  -----------------------------

//-------------- recuperer des informations

/* les coordonnees sont positionnees par CINI_loop_until_clic */
unsigned int CINI_clic_posx() { return clic_posx; }

unsigned int CINI_clic_posy() { return clic_posy; }

SDL_Keycode CINI_key_pressed(SDL_Event *event) {
    if ((event->type == SDL_KEYDOWN) || (event->type == SDL_KEYUP)) {
        return event->key.keysym.sym;
    }
    return 0;
}

int CINI_key_down() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            return 1;
        }
    }
    return 0;
}

int CINI_check_pixel_color(int x, int y, char *color) {
    /* renvoie 1 si le pixel en position (x, y) est de couleur color, 0 sinon */
    CINI_color *c = CINI_parse_color(color);
    SDL_Rect rect = {x, y, 1, 1};
    Uint32 pixmem;

    if (!SDL_RenderReadPixels(renderer, &rect, SDL_PIXELFORMAT_RGB888, &pixmem, 1)) {
        printf("%x\n", pixmem);
        Uint8 pixb = (Uint8)(pixmem & 0xff);
        Uint8 pixg = (Uint8)((pixmem >> 8) & 0xff);
        Uint8 pixr = (Uint8)((pixmem >> 16) & 0xff);
        if ((pixb == c->b) && (pixr == c->r) && (pixg == c->g)) {
            return 1;
        }
        return 0;
    } else {
        printf("Hmmm... probleme a la lecture du pixel. Hors zone ?\n");
        exit(-1);
    }
}

//-------------- tracer un point

void CINI_draw_pixel_rgb(int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    if (!outOfBoundaries(renderer, x, y)) {
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawPoint(renderer, x, y);
        // SDL_RenderPresent(renderer);
    }
}

void CINI_draw_pixel(int x, int y, char *color) {
    CINI_color *c = CINI_parse_color(color);
    if (c != NULL)
        CINI_draw_pixel_rgb(x, y, c->r, c->g, c->b);
    else
        CINI_error_color(color);
}

//-------------- tracer une ligne

void CINI_draw_line_rgb(int ax, int ay, int bx, int by, Uint8 r, Uint8 g, Uint8 b) {
    /* Test a ajouter : on ne trace que la partie de la ligne dans la fenetre */
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderDrawLine(renderer, ax, ay, bx, by);
    // SDL_RenderPresent(renderer);
}

void CINI_draw_line(int ax, int ay, int bx, int by, char *color) {
    CINI_color *c = CINI_parse_color(color);
    if (c != NULL)
        CINI_draw_line_rgb(ax, ay, bx, by, c->r, c->g, c->b);
    else
        CINI_error_color(color);
}

/*
void CINI_draw_line(int ax, int ay, int bx, int by, char* color)
{
        // Old school, Bresenham pur !

        int dX		= abs(bx-ax);
        int dY 		= abs(by-ay);
        int Xincr	= (ax > bx) ? -1 : 1;
        int Yincr 	= (ay > by) ? -1 : 1;
        int dPr, dPru, P;
        CINI_color *c = CINI_parse_color(color);

        if (dX >= dY) {
                dPr  = dY<<1;
                dPru = dPr - (dX<<1);
                P    = dPr - dX;

                for(; dX >= 0; dX--) {
        CINI_draw_pixel_rgb(ax, ay, c->r, c->g, c->b);
                        ax += Xincr;
                        if (P > 0) {
                                ay += Yincr;
                                P += dPru;
                        }
                        else {
                                P += dPr;
                        }
                }
        }
        else {
                dPr		= dX<<1;
                dPru	= dPr - (dY<<1);
                P		= dPr - dY;

                for (; dY>=0; dY--) {
        CINI_draw_pixel_rgb(ax, ay, c->r, c->g, c->b);
                        ay += Yincr;
                        if (P > 0) {
                                ax += Xincr;
                                P += dPru;
                        }
                        else {
                                P += dPr;
                        }
                }
        }
}
*/

//-------------- dessiner un cercle

void CINI_draw_circle(int x, int y, int rayon, char *color) {
    /* calcule puis affiche le cercle de centre (x, y) et de rayon r */
    /* on peut aussi afficher point par point en utilisant CINI_draw_pixel_rgb
       plutot que de passer par le renderer */
    int vx, vy, r2;
    int distance2_N, distance2_O, distance2_NO;
    int ecart_N, ecart_O, ecart_NO;
    CINI_color *c = CINI_parse_color(color);

    SDL_SetRenderDrawColor(renderer, c->r, c->g, c->b, 255);
    vx = x + rayon;
    vy = y; /* les coordonnees du point de depart */
    r2 = rayon * rayon;

    do {
        /* affichage des 4 points symetriques */
        SDL_RenderDrawPoint(renderer, vx, vy);
        SDL_RenderDrawPoint(renderer, 2 * x - vx, vy);
        SDL_RenderDrawPoint(renderer, vx, 2 * y - vy);
        SDL_RenderDrawPoint(renderer, 2 * x - vx, 2 * y - vy);
        /*      CINI_draw_pixel_rgb(vx, vy, c->r, c->g, c->b);*/
        /*      CINI_draw_pixel_rgb(2*x - vx, vy,c->r, c->g, c->b);*/
        /*      CINI_draw_pixel_rgb(vx, 2*y - vy, c->r, c->g, c->b);*/
        /*      CINI_draw_pixel_rgb(2*x - vx, 2*y - vy,c->r, c->g, c->b);*/

        /* calcul du carre des distances au centre pour les 3 candidats
           N = (vx, vy-1) O = (vx-1, vy) NO = (vx-1, vy-1)  centre = (x, y) */
        distance2_N = ((vx - x) * (vx - x)) + ((vy - 1 - y) * (vy - 1 - y));
        distance2_NO = ((vx - 1 - x) * (vx - 1 - x)) + ((vy - 1 - y) * (vy - 1 - y));
        distance2_O = ((vx - 1 - x) * (vx - 1 - x)) + ((vy - y) * (vy - y));

        /* qui a la distance au carre la plus proche de R2 ? */
        ecart_N = abs(r2 - distance2_N);
        ecart_NO = abs(r2 - distance2_NO);
        ecart_O = abs(r2 - distance2_O);

        if (ecart_O < ecart_N) {
            /* on va vers l'ouest */
            vx = vx - 1;
            if (ecart_NO < ecart_O) {
                /* et aussi vers le nord */
                vy = vy - 1;
            }
        } else {
            /* on va vers le nord */
            vy = vy - 1;
            if (ecart_NO < ecart_N) {
                /* et aussi vers l'ouest */
                vx = vx - 1;
            }
        }
    } while (vx >= x);
    // SDL_RenderPresent(renderer);
}

//-------------- dessiner un disque

void CINI_draw_disc(int x, int y, int rayon, char *color) {
    /* calcule puis affiche le disque de centre (x, y) et de rayon r */
    /* on peut aussi afficher point par point en utilisant CINI_draw_pixel_rgb
       plutot que de passer par le renderer */
    int vx, vy, r2;
    int distance2_N, distance2_O, distance2_NO;
    int ecart_N, ecart_O, ecart_NO;
    CINI_color *c = CINI_parse_color(color);

    SDL_SetRenderDrawColor(renderer, c->r, c->g, c->b, 255);
    vx = x + rayon;
    vy = y; /* les coordonnees du point de depart */
    r2 = rayon * rayon;

    do {
        /* affichage d'une ligne et de la ligne symetrique */
        SDL_RenderDrawLine(renderer, vx, vy, 2 * x - vx, vy);
        SDL_RenderDrawLine(renderer, vx, 2 * y - vy, 2 * x - vx, 2 * y - vy);
        /*		CINI_draw_line_rgb(vx, vy, 2*x - vx, vy, c->r, c->g, c->b);*/
        /*		CINI_draw_line_rgb(vx, 2*y - vy, 2*x - vx, 2*y - vy, c->r, c->g, c->b);*/

        /* calcul du carre des distances au centre pour les 3 candidats
           N = (vx, vy-1) O = (vx-1, vy) NO = (vx-1, vy-1)  centre = (x, y) */
        distance2_N = ((vx - x) * (vx - x)) + ((vy - 1 - y) * (vy - 1 - y));
        distance2_NO = ((vx - 1 - x) * (vx - 1 - x)) + ((vy - 1 - y) * (vy - 1 - y));
        distance2_O = ((vx - 1 - x) * (vx - 1 - x)) + ((vy - y) * (vy - y));

        /* qui a la distance au carre la plus proche de R2 ? */
        ecart_N = abs(r2 - distance2_N);
        ecart_NO = abs(r2 - distance2_NO);
        ecart_O = abs(r2 - distance2_O);

        if (ecart_O < ecart_N) {
            /* on va vers l'ouest */
            vx = vx - 1;
            if (ecart_NO < ecart_O) {
                /* et aussi vers le nord */
                vy = vy - 1;
            }
        } else {
            /* on va vers le nord */
            vy = vy - 1;
            if (ecart_NO < ecart_N) {
                /* et aussi vers l'ouest */
                vx = vx - 1;
            }
        }
    } while (vx >= x);
    // SDL_RenderPresent(renderer);
}

//-------------- remplir un rectangle

void CINI_fill_rect_rgb(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b)
/* remplit avec la couleur rgb un rectangle de largeur w et hauteur h,
   dont le coin superieur gauche est en (x, y) */
{
    /* Test a ajouter : on ne trace que la partie du rectangle dans la fenetre */
    SDL_Rect rectangle = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderFillRect(renderer, &rectangle);
    // SDL_RenderPresent(renderer);
}

void CINI_fill_rect(int x, int y, int w, int h, char *color) {
    /* remplit avec "color" un rectangle de largeur w et hauteur h,
       dont le coin superieur gauche est en (x, y) */
    CINI_color *c = CINI_parse_color(color);
    if (c != NULL)
        CINI_fill_rect_rgb(x, y, w, h, c->r, c->g, c->b);
    else
        CINI_error_color(color);
}

//-------------- remplir la fenetre

void CINI_fill_window(char *color) {
    CINI_color *c = CINI_parse_color(color);
    if (c != NULL) {
        // definit la couleur
        SDL_SetRenderDrawColor(renderer, c->r, c->g, c->b, 255);
        // remplit la fenetre
        SDL_RenderClear(renderer);
        // affiche le contenu
        // SDL_RenderPresent(renderer);
        free(c);
    } else
        CINI_error_color(color);
}

//-----------------------------  LES CHAINES DE CARACTERES  ----------------

/* la gestion des chaines de caracteres est tres inspiree du programme showfont.c
   fourni par Sam Lantinga avec SDL2_ttf */

typedef struct {
    SDL_Texture *message;
    SDL_Rect messageRect;
} Scene;

void createScene(Scene *scene, int x, int y, TTF_Font *font, CINI_color color, char *str) {
    SDL_Surface *text;
    SDL_Color TXT_Color;

    TXT_Color.r = color.r;
    TXT_Color.g = color.g;
    TXT_Color.b = color.b;

    text = TTF_RenderText_Solid(font, str, TXT_Color);
    scene->messageRect.x = x;
    scene->messageRect.y = y;
    scene->messageRect.w = text->w;
    scene->messageRect.h = text->h;
    scene->message = SDL_CreateTextureFromSurface(renderer, text);
    /* printf("Font is generally %d big, and string is %d big\n",
                 TTF_FontHeight(font), text->h); */
    SDL_FreeSurface(text);
}

void CINI_draw_string(int x, int y, char *color, char *str) {
    Scene scene;
    CINI_color *c = CINI_parse_color(color);

    if (c == NULL) {
        CINI_error_color(color);
    } else {
        createScene(&scene, x, y, font, *c, str);
        SDL_RenderCopy(renderer, scene.message, NULL, &scene.messageRect);
        // SDL_RenderPresent(renderer);

        /* un peu de nettoyage */
        SDL_DestroyTexture(scene.message);
    }
}

//------------------------  LES TABLEAUX GRAPHIQUES  -----------------------

void CINI_draw_char_table(int x, int y, char tab[], int size, char *bg_color, char *fg_color) {
    int i;
    int ptsize = 18;
    Scene scene;
    CINI_color *fgc = CINI_parse_color(fg_color);
    char *s = (char *)malloc(2 * sizeof(char)); /* pour stocker la chaine "tab[i]\0" */

    int wTot = 0;              /* la largeur totale du tableau */
    int x0 = x;                /* le coin en haut a gauche */
    int m = 15, mDemi = m / 2; /* marge de trace */
    int hCase;                 /* hauteur d'une case */

    if (fgc != NULL) {
        s[1] = '\0';
        x = x + mDemi;
        hCase = ptsize + m;

        for (i = 0; i < size; i++) {
            s[0] = tab[i];
            createScene(&scene, x, y + mDemi, font, *fgc, s);
            CINI_fill_rect(x - mDemi, y, scene.messageRect.w + m, hCase, bg_color);
            CINI_draw_line_rgb(x - mDemi, y, x - mDemi, y + hCase, fgc->r, fgc->g, fgc->b);
            SDL_RenderCopy(renderer, scene.message, NULL, &scene.messageRect);
            wTot = wTot + scene.messageRect.w;
            x = x + scene.messageRect.w + m;
            SDL_DestroyTexture(scene.message);
        }

        CINI_draw_line_rgb(x0, y, x0, y + hCase, fgc->r, fgc->g, fgc->b);
        CINI_draw_line_rgb(x0, y, x0 + wTot + size * m, y, fgc->r, fgc->g, fgc->b);
        CINI_draw_line_rgb(x0, y + hCase, x0 + wTot + size * m, y + hCase, fgc->r, fgc->g, fgc->b);
        CINI_draw_line_rgb(x0 + wTot + size * m, y, x0 + wTot + size * m, y + hCase, fgc->r, fgc->g,
                           fgc->b);
    }
    free(s);
}

void CINI_draw_int_table(int x, int y, int tab[], int size, char *bg_color, char *fg_color) {
    int i;
    int ptsize = 18;
    Scene scene;
    CINI_color *fgc = CINI_parse_color(fg_color);

    int wTot = 0;              /* la largeur totale du tableau */
    int x0 = x;                /* le coin en haut a gauche */
    int m = 15, mDemi = m / 2; /* marge de trace */
    int hCase;                 /* hauteur d'une case */

    if (fgc != NULL) {
        x = x + mDemi;
        hCase = ptsize + m;

        for (i = 0; i < size; i++) {
            createScene(&scene, x, y + mDemi, font, *fgc, sofi(tab[i]));
            CINI_fill_rect(x - mDemi, y, scene.messageRect.w + m, hCase, bg_color);
            CINI_draw_line_rgb(x - mDemi, y, x - mDemi, y + hCase, fgc->r, fgc->g, fgc->b);
            SDL_RenderCopy(renderer, scene.message, NULL, &scene.messageRect);
            wTot = wTot + scene.messageRect.w;
            x = x + scene.messageRect.w + m;
            SDL_DestroyTexture(scene.message);
        }

        CINI_draw_line_rgb(x0, y, x0, y + hCase, fgc->r, fgc->g, fgc->b);
        CINI_draw_line_rgb(x0, y, x0 + wTot + size * m, y, fgc->r, fgc->g, fgc->b);
        CINI_draw_line_rgb(x0, y + hCase, x0 + wTot + size * m, y + hCase, fgc->r, fgc->g, fgc->b);
        CINI_draw_line_rgb(x0 + wTot + size * m, y, x0 + wTot + size * m, y + hCase, fgc->r, fgc->g,
                           fgc->b);
    }
}

void CINI_draw_float_table(int x, int y, float tab[], int size, char *bg_color, char *fg_color) {
    int i;
    Scene scene;
    int ptsize = 18;
    CINI_color *fgc = CINI_parse_color(fg_color);

    int wTot = 0;              /* la largeur totale du tableau */
    int x0 = x;                /* le coin en haut a gauche */
    int m = 15, mDemi = m / 2; /* marge de trace */
    int hCase;                 /* hauteur d'une case */

    if (fgc != NULL) {
        x = x + mDemi;
        hCase = ptsize + m;

        for (i = 0; i < size; i++) {
            createScene(&scene, x, y + mDemi, font, *fgc, soff(tab[i]));
            CINI_fill_rect(x - mDemi, y, scene.messageRect.w + m, hCase, bg_color);
            CINI_draw_line_rgb(x - mDemi, y, x - mDemi, y + hCase, fgc->r, fgc->g, fgc->b);
            SDL_RenderCopy(renderer, scene.message, NULL, &scene.messageRect);
            wTot = wTot + scene.messageRect.w;
            x = x + scene.messageRect.w + m;
            SDL_DestroyTexture(scene.message);
        }

        CINI_draw_line_rgb(x0, y, x0, y + hCase, fgc->r, fgc->g, fgc->b);
        CINI_draw_line_rgb(x0, y, x0 + wTot + size * m, y, fgc->r, fgc->g, fgc->b);
        CINI_draw_line_rgb(x0, y + hCase, x0 + wTot + size * m, y + hCase, fgc->r, fgc->g, fgc->b);
        CINI_draw_line_rgb(x0 + wTot + size * m, y, x0 + wTot + size * m, y + hCase, fgc->r, fgc->g,
                           fgc->b);
    }
}

//-----------------------------  LES ATTENTES  -----------------------------

//-------------- clic souris

int CINI_loop_until_clic() {
    SDL_Event event;

    while (1) {
        // Poll system for an event.
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
            /* If the window has been closed, then stop the loop. */
            case SDL_QUIT:
                return -1;

            /* A mouse button have been pressed */
            case SDL_MOUSEBUTTONDOWN:
                clic_posx = event.button.x;
                clic_posy = event.button.y;
                return 0;
            }
        }
        /* Let the system process other events (avoids 100% CPU). */
        SDL_Delay(1);
    }
}

//-------------- touche enfoncee

int CINI_loop_until_keydown() {
    SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    SDL_Event event;

    int quit = 0;
    int key = -1;
    while (!quit && key == -1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            // If the window has been closed, then stop the loop.
            case SDL_QUIT:
                quit = true;
            // A key has been pressed.
            case SDL_KEYDOWN:
                key = CINI_key_pressed(&event);
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_SetRenderTarget(renderer, texture);
    return key;
}

//-------------- touche relachee

int CINI_loop_until_keyup() {
    SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    SDL_Event event;

    int quit = 0;
    int key = -1;
    while (!quit && key == -1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            // If the window has been closed, then stop the loop.
            case SDL_QUIT:
                quit = true;
            // A key has been pressed.
            case SDL_KEYUP:
                key = CINI_key_pressed(&event);
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_SetRenderTarget(renderer, texture);
    return key;
}

//-------------- ESC ou fermeture de la fenetre

void CINI_loop() {
    SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    SDL_Event event;

    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || CINI_key_pressed(&event) == SDLK_ESCAPE) {
                quit = 1;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    SDL_SetRenderTarget(renderer, texture);
}

//-------------- Fonction custom

void CINI_ForceUpdate() {
    SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer, texture);
}

//-------------- Partie des couleurs ------------------//

//-------------- couleur non conforme
void CINI_error_color(char *color) {
    fprintf(stderr, "Erreur : \"%s\" n'est pas une couleur valide.\n", color);
    exit(1);
}

//-------------- construction du CINI_color associe a un #rgb
CINI_color *parseHexColor(char *cs) {
    int i;
    CINI_color *c = (CINI_color *)malloc(sizeof(CINI_color));

    if (cs[0] == '#') {

        for (i = 1; cs[i] == '\0'; i++)
            if (hex(cs[i]) == -1)
                return NULL;

        switch (strlen(cs)) {
        case 7:
            c->r = hex(cs[1]) * 16 + hex(cs[2]);
            c->g = hex(cs[3]) * 16 + hex(cs[4]);
            c->b = hex(cs[5]) * 16 + hex(cs[6]);
            break;

        case 4:
            c->r = hex(cs[1]) * 17;
            c->g = hex(cs[2]) * 17;
            c->b = hex(cs[3]) * 17;
            break;
        }
    }

    return c;
}

//-------------- chaine -> #rgb -> CINI_color

#define COLOR_NAME_CMP(cs, name, code)                                                             \
    if (strcmp(cs, name) == 0) {                                                                   \
        free(cs);                                                                                  \
        return parseHexColor(code);                                                                \
    }

CINI_color *CINI_parse_color(char *color_str) {
    int i;
    CINI_color *res;
    /*	Lowercase string.*/
    char *tmp = malloc((strlen(color_str) + 1) * sizeof(char));
    for (i = 0; color_str[i] != '\0'; i++) {
        tmp[i] = tolower(color_str[i]);
    }
    tmp[i] = '\0';

    if (tmp[0] == '#') {
        res = parseHexColor(tmp);
        free(tmp);
        return res;
    } else {
        COLOR_NAME_CMP(tmp, "alice blue", "#f0f8ff");
        COLOR_NAME_CMP(tmp, "antique white", "#faebd7");
        COLOR_NAME_CMP(tmp, "aqua", "#00ffff");
        COLOR_NAME_CMP(tmp, "aquamarine", "#7fffd4");
        COLOR_NAME_CMP(tmp, "azure", "#f0ffff");
        COLOR_NAME_CMP(tmp, "beige", "#f5f5dc");
        COLOR_NAME_CMP(tmp, "bisque", "#ffe4c4");
        COLOR_NAME_CMP(tmp, "black", "#000000");
        COLOR_NAME_CMP(tmp, "blanched almond", "#ffebcd");
        COLOR_NAME_CMP(tmp, "blue", "#0000ff");
        COLOR_NAME_CMP(tmp, "blue violet", "#8a2be2");
        COLOR_NAME_CMP(tmp, "brown", "#a52a2a");
        COLOR_NAME_CMP(tmp, "burlywood", "#deb887");
        COLOR_NAME_CMP(tmp, "cadet blue", "#5f9ea0");
        COLOR_NAME_CMP(tmp, "chartreuse", "#7fff00");
        COLOR_NAME_CMP(tmp, "chocolate", "#d2691e");
        COLOR_NAME_CMP(tmp, "coral", "#ff7f50");
        COLOR_NAME_CMP(tmp, "cornflower blue", "#6495ed");
        COLOR_NAME_CMP(tmp, "cornsilk", "#fff8dc");
        COLOR_NAME_CMP(tmp, "crimson", "#dc143c");
        COLOR_NAME_CMP(tmp, "cyan", "#00ffff");
        COLOR_NAME_CMP(tmp, "dark blue", "#00008b");
        COLOR_NAME_CMP(tmp, "dark cyan", "#008b8b");
        COLOR_NAME_CMP(tmp, "dark goldenrod", "#b8860b");
        COLOR_NAME_CMP(tmp, "dark gray", "#a9a9a9");
        COLOR_NAME_CMP(tmp, "dark green", "#006400");
        COLOR_NAME_CMP(tmp, "dark khaki", "#bdb76b");
        COLOR_NAME_CMP(tmp, "dark magenta", "#8b008b");
        COLOR_NAME_CMP(tmp, "dark olive green", "#556b2f");
        COLOR_NAME_CMP(tmp, "dark orange", "#ff8c00");
        COLOR_NAME_CMP(tmp, "dark orchid", "#9932cc");
        COLOR_NAME_CMP(tmp, "dark red", "#8b0000");
        COLOR_NAME_CMP(tmp, "dark salmon", "#e9967a");
        COLOR_NAME_CMP(tmp, "dark seagreen", "#8dbc8f");
        COLOR_NAME_CMP(tmp, "dark slate blue", "#483d8b");
        COLOR_NAME_CMP(tmp, "dark slate gray", "#2f4f4f");
        COLOR_NAME_CMP(tmp, "dark turquoise", "#00ded1");
        COLOR_NAME_CMP(tmp, "dark violet", "#9400d3");
        COLOR_NAME_CMP(tmp, "deep pink", "#ff1493");
        COLOR_NAME_CMP(tmp, "deep sky blue", "#00bfff");
        COLOR_NAME_CMP(tmp, "dim gray", "#696969");
        COLOR_NAME_CMP(tmp, "dodger blue", "#1e90ff");
        COLOR_NAME_CMP(tmp, "firebrick", "#b22222");
        COLOR_NAME_CMP(tmp, "floral white", "#fffaf0");
        COLOR_NAME_CMP(tmp, "forest green", "#228b22");
        COLOR_NAME_CMP(tmp, "fuchsia", "#ff00ff");
        COLOR_NAME_CMP(tmp, "gainsboro", "#dcdcdc");
        COLOR_NAME_CMP(tmp, "ghost white", "#f8f8ff");
        COLOR_NAME_CMP(tmp, "gold", "#ffd700");
        COLOR_NAME_CMP(tmp, "goldenrod", "#daa520");
        COLOR_NAME_CMP(tmp, "grey", "#808080");
        COLOR_NAME_CMP(tmp, "gray", "#808080");
        COLOR_NAME_CMP(tmp, "green", "#008000");
        COLOR_NAME_CMP(tmp, "green yellow", "#adff2f");
        COLOR_NAME_CMP(tmp, "honeydew", "#f0fff0");
        COLOR_NAME_CMP(tmp, "hot pink", "#ff69b4");
        COLOR_NAME_CMP(tmp, "indian red", "#cd5c5c");
        COLOR_NAME_CMP(tmp, "indigo", "#4b0082");
        COLOR_NAME_CMP(tmp, "ivory", "#fffff0");
        COLOR_NAME_CMP(tmp, "khaki", "#f0e68c");
        COLOR_NAME_CMP(tmp, "lavender", "#e6e6fa");
        COLOR_NAME_CMP(tmp, "lavender blush", "#fff0f5");
        COLOR_NAME_CMP(tmp, "lawngreen", "#7cfc00");
        COLOR_NAME_CMP(tmp, "lemon chiffon", "#fffacd");
        COLOR_NAME_CMP(tmp, "light blue", "#add8e6");
        COLOR_NAME_CMP(tmp, "light coral", "#f08080");
        COLOR_NAME_CMP(tmp, "light cyan", "#e0ffff");
        COLOR_NAME_CMP(tmp, "light goldenrod yellow", "#fafad2");
        COLOR_NAME_CMP(tmp, "light green", "#90ee90");
        COLOR_NAME_CMP(tmp, "light grey", "#d3d3d3");
        COLOR_NAME_CMP(tmp, "light pink", "#ffb6c1");
        COLOR_NAME_CMP(tmp, "light salmon", "#ffa07a");
        COLOR_NAME_CMP(tmp, "light seagreen", "#20b2aa");
        COLOR_NAME_CMP(tmp, "light sky blue", "#87cefa");
        COLOR_NAME_CMP(tmp, "light slate gray", "#778899");
        COLOR_NAME_CMP(tmp, "light steel blue", "#b0c4de");
        COLOR_NAME_CMP(tmp, "light yellow", "#ffffe0");
        COLOR_NAME_CMP(tmp, "lime", "#00ff00");
        COLOR_NAME_CMP(tmp, "lime green", "#32cd32");
        COLOR_NAME_CMP(tmp, "linen", "#faf0e6");
        COLOR_NAME_CMP(tmp, "magenta", "#ff00ff");
        COLOR_NAME_CMP(tmp, "maroon", "#800000");
        COLOR_NAME_CMP(tmp, "medium aquamarine", "#66cdaa");
        COLOR_NAME_CMP(tmp, "medium blue", "#0000cd");
        COLOR_NAME_CMP(tmp, "medium orchid", "#ba55d3");
        COLOR_NAME_CMP(tmp, "medium purple", "#9370db");
        COLOR_NAME_CMP(tmp, "medium sea green", "#3cb371");
        COLOR_NAME_CMP(tmp, "medium slate blue", "#7b68ee");
        COLOR_NAME_CMP(tmp, "medium spring green", "#00fa9a");
        COLOR_NAME_CMP(tmp, "medium turquoise", "#48d1cc");
        COLOR_NAME_CMP(tmp, "medium violet red", "#c71585");
        COLOR_NAME_CMP(tmp, "midnight blue", "#191970");
        COLOR_NAME_CMP(tmp, "mint cream", "#f5fffa");
        COLOR_NAME_CMP(tmp, "misty rose", "#ffe4e1");
        COLOR_NAME_CMP(tmp, "moccasin", "#ffe4b5");
        COLOR_NAME_CMP(tmp, "navajo white", "#ffdead");
        COLOR_NAME_CMP(tmp, "navy", "#000080");
        COLOR_NAME_CMP(tmp, "old lace", "#fdf5e6");
        COLOR_NAME_CMP(tmp, "olive", "#808000");
        COLOR_NAME_CMP(tmp, "olive drab", "#6b8e23");
        COLOR_NAME_CMP(tmp, "orange", "#ffa500");
        COLOR_NAME_CMP(tmp, "orange red", "#ff4500");
        COLOR_NAME_CMP(tmp, "orchid", "#da70d6");
        COLOR_NAME_CMP(tmp, "pale goldenrod", "#eee8aa");
        COLOR_NAME_CMP(tmp, "pale green", "#98fb98");
        COLOR_NAME_CMP(tmp, "pale turquoise", "#afeeee");
        COLOR_NAME_CMP(tmp, "pale violet red", "#db7093");
        COLOR_NAME_CMP(tmp, "papaya whip", "#ffefd5");
        COLOR_NAME_CMP(tmp, "peach puff", "#ffdab9");
        COLOR_NAME_CMP(tmp, "peru", "#cd853f");
        COLOR_NAME_CMP(tmp, "pink", "#ffc8cb");
        COLOR_NAME_CMP(tmp, "plum", "#dda0dd");
        COLOR_NAME_CMP(tmp, "powder blue", "#b0e0e6");
        COLOR_NAME_CMP(tmp, "purple", "#800080");
        COLOR_NAME_CMP(tmp, "red", "#ff0000");
        COLOR_NAME_CMP(tmp, "rosy brown", "#bc8f8f");
        COLOR_NAME_CMP(tmp, "royal blue", "#4169e1");
        COLOR_NAME_CMP(tmp, "saddle brown", "#8b4513");
        COLOR_NAME_CMP(tmp, "salmon", "#fa8072");
        COLOR_NAME_CMP(tmp, "sandy brown", "#f4a460");
        COLOR_NAME_CMP(tmp, "sea green", "#2e8b57");
        COLOR_NAME_CMP(tmp, "sea shell", "#fff5ee");
        COLOR_NAME_CMP(tmp, "sienna", "#a0522d");
        COLOR_NAME_CMP(tmp, "silver", "#c0c0c0");
        COLOR_NAME_CMP(tmp, "sky blue", "#87ceeb");
        COLOR_NAME_CMP(tmp, "slate blue", "#6a5acd");
        COLOR_NAME_CMP(tmp, "slate gray", "#708090");
        COLOR_NAME_CMP(tmp, "snow", "#fffafa");
        COLOR_NAME_CMP(tmp, "spring green", "#00ff7f");
        COLOR_NAME_CMP(tmp, "steelblue", "#4682b4");
        COLOR_NAME_CMP(tmp, "tan", "#d2b48c");
        COLOR_NAME_CMP(tmp, "teal", "#008080");
        COLOR_NAME_CMP(tmp, "thistle", "#d8bfd8");
        COLOR_NAME_CMP(tmp, "tomato", "#ff6347");
        COLOR_NAME_CMP(tmp, "turquoise", "#40e0d0");
        COLOR_NAME_CMP(tmp, "violet", "#ee82ee");
        COLOR_NAME_CMP(tmp, "wheat", "#f5deb3");
        COLOR_NAME_CMP(tmp, "white", "#ffffff");
        COLOR_NAME_CMP(tmp, "whitesmoke", "#f5f5f5");
        COLOR_NAME_CMP(tmp, "yellow", "#ffff00");
        COLOR_NAME_CMP(tmp, "yellow green", "#9acd32");
    }
    return NULL;
}

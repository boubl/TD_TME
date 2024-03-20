#ifndef _CINI_H_
#define _CINI_H_

#include <stdbool.h>
#include <stdint.h>

#ifndef FONTFILE
#define FONTFILE "font.ttf"
#endif

typedef struct CINI_color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} CINI_color;

CINI_color *CINI_parse_color(char *color_str);
void CINI_error_color(char *color);

void CINI_open_window(int width, int height, char *title);
/* ouvre une fenetre de dimensions width x height et de nom title */

void CINI_open_fullscreen();
/* ouvre une fenetre en mode plein ecran */

void CINI_close_window();
/* fermeture de la fenetre */

void CINI_close_fullscreen();
/* fermeture de la fenetre */

int CINI_check_pixel_color(int x, int y, char *color);
/* renvoie 1 si le pixel de coordonnees (x, y) est de couleur color, 0 sinon */

int CINI_key_down();

void CINI_loop();
int CINI_loop_until_keydown();
int CINI_loop_until_keyup();
int CINI_loop_until_clic();

unsigned int CINI_clic_posx();
unsigned int CINI_clic_posy();

void CINI_fill_window(char *color);
void CINI_fill_rect(int x, int y, int w, int h, char *color);

void CINI_draw_pixel(int x, int y, char *color);
void CINI_draw_line(int ax, int ay, int bx, int by, char *color);
void CINI_draw_disc(int x, int y, int rayon, char *color);
void CINI_draw_char(int x, int y, char *color, char *str);

void CINI_draw_int_table(int x, int y, int tab[], int size, char *bg_color, char *fg_color);
void CINI_draw_char_table(int x, int y, char tab[], int size, char *bg_color, char *fg_color);
void CINI_draw_float_table(int x, int y, float tab[], int size, char *bg_color, char *fg_color);

// custom

void CINI_ForceUpdate();

#endif

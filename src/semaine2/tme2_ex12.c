#include "cini.h"
#include <stdio.h>

#define WIDTH 400
#define HEIGHT 300

int position(int a, int b, int x, int y) {
  int res = a * x + b;
  if (res == y) {
    return 0; // sur la droite
  } else if (res > y) {
    return -1; // en dessous de la droite
  } else {
    return 1; // au dessus de la droite
  }
}

void affiche(int a, int b, int width, int height) {
  char *colors[3];
  colors[0] = "blue";
  colors[1] = "black";
  colors[2] = "red";
  
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      CINI_draw_pixel(x, y, colors[1 + position(a, b, x, y)]);
      printf("Rendering... %d%%\n", (x * height + y) * 100 / (width * height)); // fancy progress indicator :)
    }
  }
  printf("Rendering done!\n");
}

int main() {
  CINI_open_window(WIDTH, HEIGHT, "tme2_ex12");
  affiche(1, 10, WIDTH, HEIGHT);
  CINI_loop();
  return 0;
}

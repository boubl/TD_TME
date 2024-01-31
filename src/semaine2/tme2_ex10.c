#include <stdio.h>
#include <cini.h>

void carre(int longueur, int x, int y) {
  for (int i = 0; i < longueur; i++) {
    CINI_draw_pixel(x + i, y + 0, "blue");
    CINI_draw_pixel(x + 0, y + i, "red");
    CINI_draw_pixel(x + i, y + longueur - 1, "green");
    CINI_draw_pixel(x + longueur - 1, y + i, "black");
  }
}

void carres_remontant(int longueur, int x, int y) {
  int nextX = x;
  int nextY = y;
  while (nextX >= 0 && nextY >= 0) {
    carre(longueur, nextX, nextY);
    nextX -= 20;
    nextY -= 20;
  }
}

int main() {
  CINI_open_window(400, 300, "tme2_ex10");
  CINI_fill_window("white");

  carres_remontant(75, 175, 100);

  CINI_loop();
  return 0;
}

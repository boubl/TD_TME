#include <cini.h>

#define WIDTH 400
#define HEIGHT 300

void triangle(int width, int height, float epsilon) {
  int ax = 0;
  int ay = height - 1;
  int bx = width - 1;
  int by = height - 1;
  int cx = width / 2;
  int cy = 0;

  while ((ax - bx) * (ax - bx) + (ay - by) * (ay - by) > epsilon) {
    //draw
    CINI_draw_line(ax, ay, bx, by, "red");
    CINI_draw_line(cx, cy, ax, ay, "blue");
    CINI_draw_line(cx, cy, bx, by, "green");

    // calculate
    int axp = (bx + 9*ax) / 10;
    int ayp = (by + 9*ay) / 10;
    int bxp = (cx + 9*bx) / 10;
    int byp = (cy + 9*by) / 10;
    int cxp = (ax + 9*cx) / 10;
    int cyp = (ay + 9*cy) / 10;
    // put back in the value
    ax = axp;
    ay = ayp;
    bx = bxp;
    by = byp;
    cx = cxp;
    cy = cyp;
  }
}

int main() {
  CINI_open_window(WIDTH, HEIGHT, "tme2_ex15");
  CINI_fill_window("white");
  triangle(WIDTH, HEIGHT, 10);
  CINI_loop();
}

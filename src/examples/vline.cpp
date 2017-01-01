/*
Horizontal line drawing vs. optimized vertical line drawing
*/

#include "../hercules/hercules.h"
#include <conio.h> // getch

Hercules h;

void vfill() {
  for (uint16 x = 0; x < h.maxx; ++x)
    h.line(x, 0, x, h.maxy - 1, 1);
}

void hfill() {
  for (uint16 y = 0; y < h.maxy; ++y)
    h.line(0, y, h.maxx - 1, y);
}

void grid() {
  for (uint16 x = 0; x < h.maxx; x += 16)
    h.line(x, 0, x, h.maxy - 1);
  for (uint16 y = 0; y < h.maxy; y += 16)
    h.line(0, y, h.maxx - 1, y);
}

int main() {
  h.initgraph();

  grid();
  getch();
  vfill();
  getch();
  h.clear_page(0);
  grid();
  getch();
  hfill();
  getch();

  h.closegraph();
}
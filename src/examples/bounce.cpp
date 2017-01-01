/*
Animation with page switching
Bouncing ball
*/


#include "../hercules/hercules.h"

#include <conio.h> // getch

Hercules h;

void bouncing_ball() {
  bool is_running = 1;
  bool clean = 1;
  const uint8 r = 64;
  short x = 100, y = 100;
  short dx = 8, dy = 10;
  short x1[] = {x, x}, y1[] = {y, y}; // prev. values
  uint8 p = 0;                        // page

  while (is_running) {
    if (clean)
      p = !p;
    else
      p = 0;
    h.draw_page(p);

    for (uint8 ri = r - 3; ri < r; ++ri) {
      if (clean)
        h.circle(x1[p], y1[p], ri, 0);
      h.circle(x, y, ri);
    }
    h.view_page(p);

    x1[p] = x;
    y1[p] = y;
    x += dx;
    y += dy;

    if (x + r >= h.maxx - 10 || x - r < 10)
      dx = -dx;

    if (y + r >= h.maxy - 10 || y - r < 10)
      dy = -dy;
    if (kbhit()) {
      unsigned char k = getch();
      switch (k) {
      case 27: // escape
        is_running = 0;
        break;
      case ' ':
        clean = !clean;
        h.clear_page(0);
        break;
      }
    }
  }
}

int main() {
  h.initgraph();

  bouncing_ball();

  h.closegraph();
  return 0;
}

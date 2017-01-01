/*
Demonstration of simple drawing and page switching.
*/

#include "../hercules/hercules.h"
#include "../inc/random.h"

#include <conio.h> // getch

Hercules h;

void rand_rect() {
  for (int i = 0; i < 128; ++i)
    h.rect(rand16() % (h.maxx - 128), rand16() % (h.maxy - 128),
           rand16() % 100 + 3, rand16() % 100 + 20);
}

void rand_circles() {
  for (int i = 0; i < 128; ++i)
    h.circle(rand16() % (h.maxx - 128) + 64, rand16() % (h.maxy - 128) + 64,
             rand16() % 64, 1);
}

int main() {
  h.initgraph();

  rand_circles();
  getch();
  h.view_page(1);
  h.draw_page(1);
  rand_rect();

  getch();
  h.view_page(0);
  getch();
  h.view_page(1);
  getch();

  h.closegraph();
  return 0;
}

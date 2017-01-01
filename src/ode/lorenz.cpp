
#include "../hercules/hercules.h"
#include "../inc/random.h"
#include "ode.h"

#include <conio.h> // getch and kbhit

class Lorenz : public ODE {
  double *dx(double *x) {
    static double dx[3];
    const double s = 10;
    const double r = 28;
    const double b = 8. / 3;

    dx[0] = s * (x[1] - x[0]);
    dx[1] = x[0] * (r - x[2]) - x[1];
    dx[2] = x[0] * x[1] - b * x[2];
    return dx;
  }
};

int main() {
  Hercules h;
  Lorenz ode;
  double x[] = {0.01, 0.01, 0.1};
  double x1[] = {0.01, 0.01, 0.1}; // save prev. value to draw line

  bool is_running = 1;
  bool color = 1;
  unsigned i = 0;

  h.initgraph();
  while (is_running) {
    ode.RK4(x);
    if (++i == 2) {
      i = 0;
      h.draw_page(0);
      const double sx0 = 13, sy0 = 6; // scaling factors
      h.line(360 + sx0 * x[0], 170 + sy0 * x[1], 360 + sx0 * x1[0],
             170 + sy0 * x1[1], color);
      h.draw_page(1);
      h.line(360 + sx0 * x[0], 7 * x[2], 360 + sx0 * x1[0], 7 * x1[2], color);

      for (int j = 0; j < 3; ++j)
        x1[j] = x[j];
    }

    if (kbhit()) {               // int kbhit() from conio.h
      unsigned char k = getch(); // int getch() from conio.h,
      // TODO: replace both with own implementation in asm
      switch (k) {
      case 27: // escape
        is_running = 0;
        break;
      case 'c':
        color = !color;
        break;
      case '0':
        h.clear_page(0);
        h.clear_page(1);
        break;
      case '1':
        h.view_page(0);
        break;
      case '2':
        h.view_page(1);
        break;
      }
    }
  }
  h.closegraph();
  return 0;
}

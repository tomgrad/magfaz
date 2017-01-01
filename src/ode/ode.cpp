#include "ode.h"
#include <cmath>
#include <iostream>
using namespace std;

ODE::ODE() : t(0), dt(0.01) {}

double *ODE::dx(double *x) {
  static double dx[3];

  // example: harmonic oscillator
  dx[0] = x[1];
  dx[1] = -x[0];
  dx[2] = 0;
  return dx;
}

void ODE::set_dt(double step) { dt = step; }

void ODE::RK4(double *x) {
  const unsigned N = 3;
  double k1[N], k2[N], k3[N], k4[N];

  double *x1 = dx(x);
  double x2[N];
  for (int i = 0; i < N; ++i) {
    k1[i] = dt * x1[i];
    x2[i] = x[i] + k1[i] / 2;
  }
  x1 = dx(x2);

  for (int i = 0; i < N; ++i) {
    k2[i] = dt * x1[i];
    x2[i] = x[i] + k2[i] / 2;
  }
  x1 = dx(x2);

  for (int i = 0; i < N; ++i) {
    k3[i] = dt * x1[i];
    x2[i] = x[i] + k3[i];
  }
  x1 = dx(x2);

  for (int i = 0; i < N; ++i)
    k4[i] = dt * x1[i];

  for (int i = 0; i < N; ++i)
    x[i] += (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]) / 6;

  delete x1;

  // range check
  /*
      double top = 1e5;
      for (int i = 0; i < N; ++i)
          if (x[i] > top)
              cout << "Over range!" << endl;
  */
}

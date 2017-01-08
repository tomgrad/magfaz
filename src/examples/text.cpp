#include "../hercules/hercules.h"
#include "conio.h"
Hercules h;

int main() {

  h.initgraph();

  char t1[] = "1234567890!@#$%^&*()";
  char t2[] = "qwertyuiopasdfghjklzxcvbnm";
  char t3[] = "QWERTYUIOPASDFGHJKLZXCVBNM";

  h.text(t1, sizeof(t1) - 1, 10, 10, 1);
  h.text(t2, sizeof(t2) - 1, 60, 100, 1);
  h.text(t3, sizeof(t3) - 1, 120, 200, 1);

  getch();

  h.closegraph();

}
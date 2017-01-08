#include "../hercules/hercules.h"
#include "conio.h"
#include "stdio.h"

char *load_bitmap(const char *fn, uint16 &width, uint16 &height) {
  FILE *f = fopen(fn, "rb");

  if (f == NULL) {
    printf("Error opening file %s", fn);
    width = 0;
    height = 0;
    return NULL;
  }

  fread(&width, 2, 1, f);
  fread(&height, 2, 1, f);

  uint16 size = (width >> 3) * height;
  char *bitmap = new char[size];

  size_t result = fread(bitmap, 1, size, f);
  fclose(f);
  return bitmap;
}

int main() {
  Hercules h;
  uint16 width, height;
  char *bmaps[] = {"einstein.bin", "logopw.bin", "logoif.bin"};

  h.initgraph();

  for (int i = 0; i<sizeof(bmaps)>> 1; ++i) {
    h.clear_page(0);
    char *bitmap = load_bitmap(bmaps[i], width, height);
    if (bitmap == NULL)
      return 1;

    h.bitmap(bitmap, width, height, (h.maxx - width) >> 1,
             (h.maxy - height) >> 1);

    delete bitmap;
    getch();
  }
  h.closegraph();
}
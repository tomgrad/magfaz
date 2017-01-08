#include "hercules.h"
#include "../inc/helpers.h" // abs, swap, sgn
// #include "font8x8.h"
#include <conio.h> // outp
#include <math.h> // sqrt. Watcom has some stupid problems with std:sqrt from cmath
#include <stdio.h> // printf

Hercules::Hercules() {
  base = (addr)0xb0000000;
  maxx = 720;
  maxy = 348;
  font = new char[1024];

  FILE *f = fopen("font8x8.bin", "rb");
  fread(font, 1, 1024, f);
  fclose(f);
}

Hercules::~Hercules() { delete font; }

inline uint16 Hercules::offset(const uint16 x, const uint16 y) const {
  return ((y & 3) << 13) + (y >> 2) * 90 + (x >> 3);
}

void Hercules::initgraph() {

  const uint16 params[] = {
      0x3500, 0x2d01,
      0x2e02, // Linesync at 46th character
      0x0703, // linesync width 7 chrclock ticks
      0x5b04, // height 92 chars (368 lines)
      0x0205, // height adjust
      0x5706, // lines / picture (348 lines)
      0x5707, // picturesync: after 87 character lines
      0x0309  // character height: 4 lines / char
  };

  outp(0x3bf, 1 + 2); // allows both graphics pages
  outp(0x3b8, 0);     // screen off

  for (uint8 i = 0; i < sizeof(params) / 2; ++i)
    outpw(0x3b4, params[i]);

  view_page(0);
  draw_page(0);
  clear_page(0);
  clear_page(1);
}

void Hercules::closegraph() {
  const uint8 params[16] = {0x61, 0x50, 0x52, 0x0f, 0x19, 0x06,
                            0x19, 0x19, 0x02, 0x0d, 0x0b, 0x0c};

  // switch back to text mode
  outp(0x3bf, 0); // lock out graphics mode
  outp(0x3b8, 0); // screen off

  for (char i = 0; i < sizeof(params); i++) {
    outp(0x3b4, i);
    outp(0x3b5, params[i]);
  }
  clear_txt();
  outp(0x3b8, 0x28); // enable blink (0x20) and turn the screen on (8)
}

void Hercules::view_page(int page) {
  if (page)
    outp(0x3b8, 0x8a);
  else
    outp(0x3b8, 0x0a);
  page_viewed = page;
}

void Hercules::draw_page(int page) {
  page_drawn = base + page * 0x8000;
  page_drawn_no = page;
}

void Hercules::screen_off() const { outp(0x3b8, 0); }

void Hercules::clear_buffer(addr buf) {
  for (uint16 i = 0; i < 0x8000; ++i)
    buf[i] = 0;
}

void Hercules::clear_page(int page) { clear_buffer(base + page * 0x8000); }

void Hercules::clear_txt() {
  // incrementing index inside loop
  // not very wise
  for (uint16 i = 0; i < 0x8000;) {
    *(base + i++) = 0x00;
    *(base + i++) = 0x07;
  }
}

bool Hercules::inbound(const uint16 x, const uint16 y) const {
  return (x > 0 || y > 0 || x <= maxx || y <= maxy);
}

void Hercules::putpixel(uint16 x, uint16 y, const bool c) {
  uint16 offset = ((y & 3) << 13) + (y >> 2) * 90 + (x >> 3);
  uint8 mask = 128 >> (x & 7);
  if (c)
    page_drawn[offset] |= mask;
  else
    page_drawn[offset] &= ~mask;
}

void Hercules::hline(uint16 x0, uint16 x1, uint16 y, const bool c) {
  if (x1 < x0)
    swap(x0, x1);
  uint16 points = x1 - x0 + 1;
  uint16 offset = ((y & 3) << 13) + (y >> 2) * 90 + (x0 >> 3);
  uint16 n_beg = 8 - (x0 % 8);           // ile bitow w pierwszym bajcie
  uint8 n_end = x1 % 8;                  // ile w ostatnim
  uint8 mask_b = uint16(1 << n_beg) - 1; // 00000111
  uint8 mask_e = 0xff << (8 - n_end);    // 11100000
  if (n_beg >
      points) // linia nie dopelnia bajtu, poczatek i koniec w tym samym bajcie
  {
    mask_b &= mask_e;
    if (c)
      page_drawn[offset++] |= mask_b;
    else
      page_drawn[offset++] &= ~mask_b;
    return;
  }
  if (c)
    page_drawn[offset++] |= mask_b;
  else
    page_drawn[offset++] &= ~mask_b;

  if (n_beg == points)
    return;
  points -= n_beg + n_end;

  for (uint16 i = 0; i < (points >> 3); ++i)
    if (c)
      page_drawn[offset++] = 0xff;
    else
      page_drawn[offset++] = 0x00;

  if (c)
    page_drawn[offset] |= mask_e;
  else
    page_drawn[offset] &= ~mask_e;
}

void Hercules::rect(uint16 x, uint16 y, uint16 w, uint16 h, bool c) {
  uint16 i = 0;
  hline(x, x + w, y, c);
  line(x, y, x, y + h, c);
  line(x + w, y, x + w, y + h, c);
  hline(x, x + w, y + h, c);
}

// void Hercules::circle(uint16 x, uint16 y, uint16 r, const bool c) {
//   for (uint16 i = x - r; i <= x + r; ++i) {
//     putpixel(i, y + sqrt(r * r - (i - x) * (i - x)), c);
//     putpixel(i, y - sqrt(r * r - (i - x) * (i - x)), c);
//   }
//   for (uint16 i = y - r; i <= y + r; ++i) {
//     putpixel(x + sqrt(r * r - (i - y) * (i - y)), i, c);
//     putpixel(x - sqrt(r * r - (i - y) * (i - y)), i, c);
//   }
// }

// Bresenham's line algorithm
void Hercules::line(uint16 x, uint16 y, uint16 x1, uint16 y1, const bool c) {
  if (y == y1) {
    hline(x, x1, y, c);
    return;
  }
  int h = y1 - y;
  int w = x1 - x;
  int dx1 = sgn(w), dy1 = sgn(h), dx2 = dx1, dy2 = 0;
  uint16 longest = abs(w);
  uint16 shortest = abs(h);
  if (!(longest > shortest)) {
    swap(longest, shortest);
    dy2 = sgn(h);
    dx2 = 0;
  }
  uint16 numerator = longest >> 1;
  for (uint16 i = 0; i <= longest; i++) {
    putpixel(x, y, c);
    numerator += shortest;
    if (!(numerator < longest)) {
      numerator -= longest;
      x += dx1;
      y += dy1;
    } else {
      x += dx2;
      y += dy2;
    }
  }
}

// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void Hercules::circle(const uint16 x0, const uint16 y0, const uint16 radius,
                      const bool c) {
  int x = radius, y = 0, err = 0;

  while (x >= y) {
    putpixel(x0 + x, y0 + y, c);
    putpixel(x0 + y, y0 + x, c);
    putpixel(x0 - y, y0 + x, c);
    putpixel(x0 - x, y0 + y, c);
    putpixel(x0 - x, y0 - y, c);
    putpixel(x0 - y, y0 - x, c);
    putpixel(x0 + y, y0 - x, c);
    putpixel(x0 + x, y0 - y, c);

    if (err <= 0) {
      ++y;
      err += 2 * y + 1;
    }
    if (err > 0) {
      --x;
      err -= 2 * x + 1;
    }
  }
}

void Hercules::text(const char *str, const uint8 len, const uint16 x,
                    const uint16 y, const bool c) {

  uint16 xmod = x - x % 8; // only full bytes for now

  for (uint8 row = 0; row < 8; ++row) {
    uint16 off = offset(xmod, y + row);

    for (uint8 i = 0; i < len; ++i)
      page_drawn[off + i] |= *(font + (str[i] << 3) + row);
  }
}

void Hercules::bitmap(const char *data, const uint16 width, const uint16 height,
                      const uint16 x, const uint16 y, const bool c) {
  uint16 xmod = x - x % 8; // only full bytes for now
  char* ptr = (char*)data;
  uint8 wb = width >> 3;   // width in bytes
  for (uint16 row = 0; row < height; ++row) {
    uint16 off = offset(xmod, y + row);

    for (uint8 i = 0; i < wb; ++i)
      page_drawn[off + i] |= *(ptr++);
  }
}

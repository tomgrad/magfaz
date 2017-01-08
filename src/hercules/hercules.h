#ifndef HERCULES_H
#define HERCULES_H

/*
Hercules Graphics Card driver
based on:
- hercules.c from LOGLAN82
https://github.com/loglan/loglan/blob/master/int/graf/hercules.c
- code by Greg Haer

TODO: make all static
*/

typedef unsigned char far *addr;
typedef unsigned char uint8;
typedef unsigned short uint16;

class Hercules {

public:
  uint16 maxx;
  uint16 maxy;

  Hercules();
  ~Hercules();
  void initgraph();
  void closegraph();
  void draw_page(int);
  void view_page(int);
  void clear_page(int);
  void clear_txt();
  void screen_off() const;
  bool inbound(const uint16, const uint16) const;

  void putpixel(uint16 x, uint16 y, const bool = 1);
  void rect(uint16 x, uint16 y, uint16 w, uint16 h, const bool = 1);
  void circle(const uint16, const uint16, const uint16 r, const bool = 1);
  void line(uint16 x0, uint16 y0, uint16 x1, uint16 y1, const bool = 1);
  void hline(uint16, uint16, uint16, const bool = 1);
  void text(const char*, const uint8, const uint16=0, const uint16=0, const bool=1);
  void bitmap(const char*, const uint16, const uint16, const uint16=0, const uint16=0, const bool=1);

private:
  // disable copy ctor, no delete keyword in c++03 :(
  Hercules(const Hercules &that);
  addr page_drawn;
  addr base;
  int page_drawn_no;
  int page_viewed;
  void clear_buffer(addr buf);
  inline uint16 offset(uint16, uint16) const;
  char* font;
};

#endif // HERCULES_H
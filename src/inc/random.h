#ifndef RANDOM_H
#define RANDOM_H

unsigned long long seed = 123456789;
unsigned long long rand16() {
  seed = (1664525 * seed + 1013904223) & 0xffff;
  return seed;
}

#endif // RANDOM_H
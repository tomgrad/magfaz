#ifndef HELPERS_H
#define HELPERS_H

/*
Some functions from STL.
Watcom sometimes has problems with standard library
when compiling for DOS.
*/

template <class T> inline void swap(T &x, T &y) {
  T tmp(x);
  x = y;
  y = tmp;
}

template <class T> inline T abs(T x) {
  if (x > 0)
    return x;
  else
    return -x;

  /* probably overkill
    static T mask=1<<(sizeof(mask)*8-1);
    if(x&mask) return ~x+1;
    else return x;
  */
}

template <class T> inline T sgn(T x) {
  if (x > 0)
    return 1;
  if (x < 0)
    return -1;
  return 0;
}

#endif // HELPERS_H
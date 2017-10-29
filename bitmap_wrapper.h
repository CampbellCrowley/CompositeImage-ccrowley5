#ifndef BITMAP_WRAPPER_H
#define BITMAP_WRAPPER_H
#include "bitmap.h"
#include <iostream> // Because bitmap is dumb and doesn't do this itself.

// Arithmetic Operators
Pixel operator+(const Pixel &, const Pixel &);
Pixel operator+(const Pixel &, const int &);
Pixel operator+(const int &, const Pixel &);
Pixel operator-(const Pixel &, const Pixel &);
Pixel operator-(const Pixel &, const int &);
Pixel operator/(const Pixel &, const Pixel &);
Pixel operator/(const Pixel &, const int &);
Pixel operator*(const Pixel &, const Pixel &);
Pixel operator*(const Pixel &, const int &);
Pixel operator*(const int &, const Pixel &);

// Comparative Operators
bool operator==(const Pixel &, const Pixel &);

#include "bitmap_wrapper.cpp"
#endif // BITMAP_WRAPPER_H

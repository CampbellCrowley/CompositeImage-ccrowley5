// A wrapper for bitmap.cc and bitmap.h to fix a few issues I have with it, but
// mostly to overload operators for Pixels.
#ifndef BITMAP_WRAPPER_H
#define BITMAP_WRAPPER_H
#include <iostream>  // Because bitmap is dumb and doesn't do this itself.
#include "bitmap.h"

// Arithmetic Operators
// Addition
Pixel operator+(const Pixel &one, const Pixel &two) {
  return Pixel(one.red + two.red, one.green + two.green, one.blue + two.blue);
}
template <typename T>
Pixel operator+(const Pixel &one, const T &two) {
  return Pixel(one.red + two, one.green + two, one.blue + two);
}
template <typename T>
Pixel operator+(const T &one, const Pixel &two) {
  return Pixel(two.red + one, two.green + one, two.blue + one);
}
void operator+=(Pixel &one, const Pixel &two) {
  one.red += two.red;
  one.green += two.green;
  one.blue += two.blue;
}
template <typename T>
void operator+=(Pixel &one, const T &two) {
  one.red += two;
  one.green += two;
  one.blue += two;
}
void operator++(Pixel &one) {
  one.red++;
  one.green++;
  one.blue++;
}

// Subtraction
Pixel operator-(const Pixel &one, const Pixel &two) {
  return Pixel(one.red - two.red, one.green - two.green, one.blue - two.blue);
}
template <typename T>
Pixel operator-(const Pixel &one, const T &two) {
  return Pixel(one.red - two, one.green - two, one.blue - two);
}
void operator-=(Pixel &one, const Pixel &two) {
  one.red -= two.red;
  one.green -= two.green;
  one.blue -= two.blue;
}
template <typename T>
void operator-=(Pixel &one, const T &two) {
  one.red -= two;
  one.green -= two;
  one.blue -= two;
}
void operator--(Pixel &one) {
  one.red--;
  one.green--;
  one.blue--;
}

// Division
Pixel operator/(const Pixel &one, const Pixel &two) {
  return Pixel(one.red / two.red, one.green / two.green, one.blue / two.blue);
}
template <typename T>
Pixel operator/(const Pixel &one, const T &two) {
  return Pixel(one.red / two, one.green / two, one.blue / two);
}
void operator/=(Pixel &one, const Pixel &two) {
  one.red /= two.red;
  one.green /= two.green;
  one.blue /= two.blue;
}
template <typename T>
void operator/=(Pixel &one, const T &two) {
  one.red /= two;
  one.green /= two;
  one.blue /= two;
}
Pixel operator%(const Pixel &one, const Pixel &two) {
  return Pixel(one.red % two.red, one.green % two.green, one.blue % two.blue);
}
template <typename T>
Pixel operator%(const Pixel &one, const T &two) {
  return Pixel(one.red % two, one.green % two, one.blue % two);
}

// Multiplication
Pixel operator*(const Pixel &one, const Pixel &two) {
  return Pixel(one.red * two.red, one.green * two.green, one.blue * two.blue);
}
template <typename T>
Pixel operator*(const Pixel &one, const T &two) {
  return Pixel(one.red * two, one.green * two, one.blue * two);
}
template <typename T>
Pixel operator*(const T &one, const Pixel &two) {
  return Pixel(two.red * one, two.green * one, two.blue * one);
}
void operator*=(Pixel &one, const Pixel &two) {
  one.red *= two.red;
  one.green *= two.green;
  one.blue *= two.blue;
}
template <typename T>
void operator*=(Pixel &one, const T &two) {
  one.red *= two;
  one.green *= two;
  one.blue *= two;
}

// Relational Operators
int cmp(const Pixel &one, const Pixel &two) {
  return one.red + one.green + one.blue - two.red - two.green - two.blue;
}
bool operator==(const Pixel &one, const Pixel &two) { return cmp(one, two) == 0; }
bool operator!=(const Pixel &one, const Pixel &two) { return !(one == two); }
bool operator<(const Pixel &one, const Pixel &two) { return cmp(one, two) < 0; }
bool operator>(const Pixel &one, const Pixel &two) { return two < one; }
bool operator>=(const Pixel &one, const Pixel &two) { return !(one < two); }
bool operator<=(const Pixel &one, const Pixel &two) { return !(one > two); }

// Subscript Operator
// If only...
// int &Pixel::operator[](const int &i) {
//   switch(i) {
//     case 0: return Pixel.red;
//     case 1: return Pixel.green;
//     case 2: return Pixel.blue;
//     default: return -1;
//   }
// }

#endif  // BITMAP_WRAPPER_H

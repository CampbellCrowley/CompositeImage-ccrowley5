#ifndef BITMAP_WRAPPER_H
#define BITMAP_WRAPPER_H
#include "bitmap.h"
#include <iostream> // Because bitmap is dumb and doesn't do this itself.

// Arithmetic Operators
// Addition
Pixel operator+(const Pixel &one, const Pixel &two) {
  return Pixel(one.red + two.red, one.green + two.green, one.blue + two.blue);
}
template<typename T>
Pixel operator+(const Pixel &one, const T &two) {
  return Pixel(one.red + two, one.green + two, one.blue + two);
}
template<typename T>
Pixel operator+(const T &one, const Pixel &two) {
  return Pixel(two.red + one, two.green + one, two.blue + one);
}

// Subtraction
Pixel operator-(const Pixel &one, const Pixel &two) {
  return Pixel(one.red - two.red, one.green - two.green, one.blue - two.blue);
}
template<typename T>
Pixel operator-(const Pixel &one, const T &two) {
  return Pixel(one.red - two, one.green - two, one.blue - two);
}

// Division
Pixel operator/(const Pixel &one, const Pixel &two) {
  return Pixel(one.red / two.red, one.green / two.green, one.blue / two.blue);
}
template<typename T>
Pixel operator/(const Pixel &one, const T &two) {
  return Pixel(one.red / two, one.green / two, one.blue / two);
}

// Multiplication
Pixel operator*(const Pixel &one, const Pixel &two) {
  return Pixel(one.red * two.red, one.green * two.green, one.blue * two.blue);
}
template<typename T>
Pixel operator*(const Pixel &one, const T &two) {
  return Pixel(one.red * two, one.green * two, one.blue * two);
}
template<typename T>
Pixel operator*(const T &one, const Pixel &two) {
  return Pixel(two.red * one, two.green * one, two.blue * one);
}

// Comparative Operators

bool operator==(const Pixel &one, const Pixel &two) {
  return one.red == two.red && one.green == two.green && one.blue == two.blue;
}

// Subscript Operator
// If only...
// int &Pixel::operator[](const int &i) {
//   switch(i) {
//     case 0: return Pixel.red;
//     case 1: return Pixel.green;
//     case 2: return Pixel.blue;
//   }
// }

#endif // BITMAP_WRAPPER_H

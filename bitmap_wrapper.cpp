#include "bitmap_wrapper.h"

// Arithmetic Operators
// Addition
Pixel operator+(const Pixel &one, const Pixel &two) {
  return Pixel(one.red + two.red, one.green + two.green, one.blue + two.blue);
}
Pixel operator+(const Pixel &one, const int &two) {
  return Pixel(one.red + two, one.green + two, one.blue + two);
}
Pixel operator+(const int &one, const Pixel &two) {
  return Pixel(two.red + one, two.green + one, two.blue + one);
}

// Subtraction
Pixel operator-(const Pixel &one, const Pixel &two) {
  return Pixel(one.red - two.red, one.green - two.green, one.blue - two.blue);
}
Pixel operator-(const Pixel &one, const int &two) {
  return Pixel(one.red - two, one.green - two, one.blue - two);
}

// Division
Pixel operator/(const Pixel &one, const Pixel &two) {
  return Pixel(one.red / two.red, one.green / two.green, one.blue / two.blue);
}
Pixel operator/(const Pixel &one, const int &two) {
  return Pixel(one.red / two, one.green / two, one.blue / two);
}

// Multiplication
Pixel operator*(const Pixel &one, const Pixel &two) {
  return Pixel(one.red * two.red, one.green * two.green, one.blue * two.blue);
}
Pixel operator*(const Pixel &one, const int &two) {
  return Pixel(one.red * two, one.green * two, one.blue * two);
}
Pixel operator*(const int &one, const Pixel &two) {
  return Pixel(two.red * one, two.green * one, two.blue * one);
}

// Comparative Operators

bool operator==(const Pixel &one, const Pixel &two) {
  return one.red == two.red && one.green == two.green && one.blue == two.blue;
}

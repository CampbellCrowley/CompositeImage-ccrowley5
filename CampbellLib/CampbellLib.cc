#include "CampbellLib.h"
#include <math.h>

namespace Campbell {
bool Common::isdigit(const char input) {
  if ((int)input >= 48 && (int)input <= 57) return true;
  return false;
}

const char *Color::reset = "\033[1;0m";
const char *Color::black = "\033[1;30m";
const char *Color::red = "\033[1;31m";
const char *Color::green = "\033[1;32m";
const char *Color::yellow = "\033[1;33m";
const char *Color::blue = "\033[1;34m";
const char *Color::magenta = "\033[1;35m";
const char *Color::cyan = "\033[1;36m";
const char *Color::white = "\033[1;37m";

bool Strings::isNumber(const char input[]) {
  bool firstDecimal = true;
  for (int i = 0; i < Arrays::length(input); i++) {
    // Number can contain only one decimal, and if it has a sign, the sign must
    // be at the beginning and there must be only one sign.
    if (!Common::isdigit(input[i]) && input[i] != '.' &&
        !((input[i] == '-' || input[i] == '+') && i == 0)) {
      return false;
    }
    if (input[i] == '.') {
      if (firstDecimal) {
        firstDecimal = false;
      } else {
        return false;
      }
    }
  }
  return true;
}

double Strings::toNumber(const char input[]) {
  if (!isNumber(input)) return 0;
  // Number is calculated as int first to help reduce precision issues.
  unsigned int intermediate = 0;
  bool isSigned = input[0] == '+' || input[0] == '-';
  bool isNegative = input[0] == '-';
  int decimalLocation = Arrays::length(input);

  for (int i = 0; i < Arrays::length(input); i++) {
    if (input[i] == '.') {
      decimalLocation = i;
      break;
    }
  }

  for (int i = 0; i < Arrays::length(input); i++) {
    if (i == decimalLocation || (isSigned && i == 0)) {
      continue;
    }
    // Digit to modify is calculated but must account for the offset created
    // when the string has a decimal but an int does not store a decimal.
    int power = Arrays::length(input) - i + ((i >= decimalLocation) ? 0 : -1);
    // Casting to int gives ASCII codes from 48 to 57 for 0-9. Shifting this
    // gives the correct numbers.
    intermediate += ((int)input[i] - 48) * pow(10, power);
  }

  double output =
      intermediate * pow(10, decimalLocation - Arrays::length(input));
  return isNegative ? -output : output;
}

bool Strings::toYesNo(const char input[], bool defaultYes) {
  if (defaultYes) {
    if (input[0] == 'n' || input[0] == 'N') {
      return false;
    } else {
      return true;
    }
  } else {
    if (input[0] == 'y' || input[0] == 'Y') {
      return true;
    } else {
      return false;
    }
  }
}

bool Strings::getYesNo(bool defaultYes) {
  std::string input;
  getline(std::cin, input);
  return Strings::toYesNo(input.c_str(), defaultYes);
}

}  // namespace Campbell

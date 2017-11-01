#include <iostream>
#include "CampbellLib.h"

int main() {
  // cout rounds to a number that's too small by default.
  std::cout.precision(12);

  // Test string to double conversion.
  std::string num = "-1234.56789";
  Campbell::Test::ASSERT(Campbell::Strings::toNumber(num.c_str()), -1234.56789);
  num = "123";
  Campbell::Test::ASSERT(Campbell::Strings::toNumber(num.c_str()), (double)123);
  num = "1.23";
  Campbell::Test::ASSERT(Campbell::Strings::toNumber(num.c_str()), 1.23);
  num = "-123";
  Campbell::Test::ASSERT(Campbell::Strings::toNumber(num.c_str()),
                         (double)-123);
  num = ".123";
  Campbell::Test::ASSERT_SIMILAR(Campbell::Strings::toNumber(num.c_str()),
                                 .123);
  num = "-.123";
  Campbell::Test::ASSERT_SIMILAR(Campbell::Strings::toNumber(num.c_str()),
                                 -0.123);

  std::cout << std::endl;

  // Test yesno input.
  std::string yes = "Yes";
  std::string no = "No";
  std::string unk = "Uknown";

  Campbell::Test::ASSERT(Campbell::Strings::toYesNo(yes.c_str(), true), true);
  Campbell::Test::ASSERT(Campbell::Strings::toYesNo(no.c_str(), true), false);
  Campbell::Test::ASSERT(Campbell::Strings::toYesNo(unk.c_str(), true), true);
  Campbell::Test::ASSERT(Campbell::Strings::toYesNo(yes.c_str(), false), true);
  Campbell::Test::ASSERT(Campbell::Strings::toYesNo(no.c_str(), false), false);
  Campbell::Test::ASSERT(Campbell::Strings::toYesNo(unk.c_str(), false), false);
}

#include "wocca/choir/position.h"
#include "test/catch.hpp"

#include <iostream>

using pos = wocca::choir::pos8;
using off = wocca::choir::off8;

TEST_CASE("wocca/choir/position", "Position/offset arithmetic"){
  CHECK(pos{2} + off{3} == pos{5});
  CHECK(off{3} + pos{2} == pos{5});
  CHECK(pos{5} - off{3} == pos{2});
  CHECK(pos{5} - pos{3} == off{2});
  CHECK(pos{3} - pos{5} == off{-2});

  CHECK(pos{0xfe} + off{5} == pos{0x03});
  CHECK(pos{0x03} - off{5} == pos{0xfe});
  CHECK(pos{0x03} - pos{0xfe} == off{5});

  CHECK(-off{3} == off{-3});

  CHECK(pos{0} < pos{1});
  CHECK(pos{1} > pos{0});
  CHECK_FALSE(pos{1} < pos{0});
  CHECK_FALSE(pos{0} > pos{1});
  CHECK_FALSE(pos{1} < pos{1});
  CHECK_FALSE(pos{1} > pos{1});

  CHECK(pos{0} <= pos{1});
  CHECK(pos{1} <= pos{1});
  CHECK(pos{1} >= pos{0});
  CHECK(pos{1} >= pos{1});
  CHECK_FALSE(pos{1} <= pos{0});
  CHECK_FALSE(pos{0} >= pos{1});

  CHECK(pos{0x7f} < pos{0x80});
  CHECK(pos{0x80} < pos{0x81});
  CHECK(pos{0xff} < pos{0x00});

  pos p{5};

  CHECK((p += off{2}) == pos{7});
  CHECK(p == pos{7});
  CHECK((p -= off{2}) == pos{5});
  CHECK(p == pos{5});
  CHECK((p -= off{7}) == pos{0xfe});
  CHECK(p == pos{0xfe});
  CHECK((p += off{6}) == pos{4});
  CHECK(p == pos{4});

  off o{3};
  CHECK((o += off{2}) == off{5});
  CHECK(o == off{5});
  CHECK((o -= off{2}) == off{3});
  CHECK(o == off{3});
}


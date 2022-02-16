#include "wocca/choir/span.h"
#include "test/catch.hpp"

TEST_CASE("wocca/choir/span", "Contiguous wrapping range of unsigned values"){
  using span = wocca::choir::span<uint8_t>;

  span empty{};
  CHECK(empty.begin() == 0);
  CHECK(empty.end() == 0);
  CHECK(empty.size() == 0);

  CHECK_FALSE(empty.contains(0x00));
  CHECK_FALSE(empty.contains(0x01));
  CHECK_FALSE(empty.contains(0x7f));
  CHECK_FALSE(empty.contains(0x80));
  CHECK_FALSE(empty.contains(0xff));

  span simple{1,4};
  CHECK(simple.begin() == 1);
  CHECK(simple.end() == 4);
  CHECK(simple.size() == 3);
  CHECK_FALSE(simple.empty());

  CHECK_FALSE(simple.contains(0x00));
  CHECK(simple.contains(0x01));
  CHECK(simple.contains(0x02));
  CHECK(simple.contains(0x03));
  CHECK_FALSE(simple.contains(0x04));
  CHECK_FALSE(simple.contains(0x7f));
  CHECK_FALSE(simple.contains(0x80));
  CHECK_FALSE(simple.contains(0xff));

  span wrap{0xfe, 0x02};

  CHECK(wrap.begin() == 0xfe);
  CHECK(wrap.end() == 0x02);
  CHECK(wrap.size() == 4);
  CHECK_FALSE(wrap.empty());

  CHECK_FALSE(wrap.contains(0xfd));
  CHECK(wrap.contains(0xfe));
  CHECK(wrap.contains(0xff));
  CHECK(wrap.contains(0x00));
  CHECK(wrap.contains(0x01));
  CHECK_FALSE(wrap.contains(0x02));
  CHECK_FALSE(wrap.contains(0x7f));
  CHECK_FALSE(wrap.contains(0x80));
}

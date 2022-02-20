#include "wocca/choir/mesh.h"
#include "test/catch.hpp"

using mesh = wocca::choir::mesh<uint8_t>;
using span = mesh::span;
using spot = mesh::spot;
using iter = mesh::iterator;

TEST_CASE("mesh::empty", "Empty mesh")
{
  mesh m;

  CHECK(m.begin() == m.end());
  CHECK(m.extent() == span{0,0});
}

TEST_CASE("mesh::insert", "Insertion into mesh")
{
  mesh m;
  iter it;

  // Insert first
  CHECK(m.insert({0x80, 0xa0}) == span{0x80, 0xa0});
  it = m.begin();
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x80, 0xa0});
  CHECK(it == m.end());

  // Insert after with gap
  CHECK(m.insert({0xb0, 0xc0}) == span{0xb0, 0xc0});
  it = m.begin();
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x80, 0xa0});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xb0, 0xc0});
  CHECK(it == m.end());

  // Insert after without gap
  CHECK(m.insert({0xc0, 0xd0}) == span{0xc0, 0xd0});
  it = m.begin();
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x80, 0xa0});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xb0, 0xd0});
  CHECK(it == m.end());

  // Insert after with overlap
  CHECK(m.insert({0xb0, 0xe0}) == span{0xd0, 0xe0});
  it = m.begin();
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x80, 0xa0});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xb0, 0xe0});
  CHECK(it == m.end());

  // Insert before with gap
  CHECK(m.insert({0x50, 0x60}, mesh::where::front) == span{0x50, 0x60});
  it = m.begin();
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x50, 0x60});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x80, 0xa0});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xb0, 0xe0});
  CHECK(it == m.end());

  // Insert before without gap
  CHECK(m.insert({0x40, 0x50}, mesh::where::front) == span{0x40, 0x50});
  it = m.begin();
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x40, 0x60});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x80, 0xa0});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xb0, 0xe0});
  CHECK(it == m.end());

  // Insert before with overlap
  CHECK(m.insert({0x30, 0x50}, mesh::where::front) == span{0x30, 0x40});
  it = m.begin();
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x30, 0x60});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x80, 0xa0});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xb0, 0xe0});
  CHECK(it == m.end());

  // Insert empty spans and spans within lumps (no change)
  CHECK(m.insert({0x30, 0x40}).empty());
  CHECK(m.insert({0x40, 0x50}).empty());
  CHECK(m.insert({0x50, 0x60}).empty());
  CHECK(m.insert({0x30, 0x60}).empty());
  CHECK(m.insert({0x80, 0x90}).empty());
  CHECK(m.insert({0x88, 0x98}).empty());
  CHECK(m.insert({0x90, 0xa0}).empty());
  CHECK(m.insert({0x80, 0xa0}).empty());
  CHECK(m.insert({0xb0, 0xc0}).empty());
  CHECK(m.insert({0xb8, 0xc8}).empty());
  CHECK(m.insert({0xc0, 0xd0}).empty());
  CHECK(m.insert({0xb0, 0xd0}).empty());
  for (int i = 0; i < 0x100; ++i)
    CHECK(m.insert({uint8_t(i),uint8_t(i)}).empty());

  it = m.begin();
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x30, 0x60});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x80, 0xa0});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xb0, 0xe0});
  CHECK(it == m.end());

  // Insert before, after and within the first gap (no overlap)
  CHECK(m.insert({0x60, 0x64}) == span{0x60, 0x64});
  CHECK(m.insert({0x7c, 0x80}) == span{0x7c, 0x80});
  CHECK(m.insert({0x68, 0x78}) == span{0x68, 0x78});

  it = m.begin();
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x30, 0x64});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x68, 0x78});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x7c, 0xa0});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xb0, 0xe0});
  CHECK(it == m.end());

  // Insert before, after and within the last gap (with overlap)
  CHECK(m.insert({0x90, 0xa4}) == span{0xa0, 0xa4});
  CHECK(m.insert({0xac, 0xc0}) == span{0xac, 0xb0});
  CHECK(m.insert({0xa6, 0xaa}) == span{0xa6, 0xaa});

  it = m.begin();
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x30, 0x64});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x68, 0x78});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x7c, 0xa4});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xa6, 0xaa});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xac, 0xe0});
  CHECK(it == m.end());

  // Insert covering a first gap (no overlap)
  CHECK(m.insert({0x64, 0x68}) == span{0x64, 0x68});
  it = m.begin();
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x30, 0x78});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x7c, 0xa4});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xa6, 0xaa});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xac, 0xe0});
  CHECK(it == m.end());

  // Insert covering a gap (overlap)
  CHECK(m.insert({0x70, 0x80}) == span{0x78, 0x7c});
  it = m.begin();
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x30, 0xa4});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xa6, 0xaa});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xac, 0xe0});
  CHECK(it == m.end());

  // Insert covering both gaps
  CHECK(m.insert({0x40, 0xb0}) == span{0xa4, 0xa6});
  it = m.begin();
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x30, 0xaa});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xac, 0xe0});
  CHECK(it == m.end());

  // Insert covering the whole mesh
  CHECK(m.insert({0x20, 0xf0}, mesh::where::front) == span{0x20, 0x30});
  it = m.begin();
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x20, 0xaa});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xac, 0xe0});
  CHECK(it == m.end());

  // Insert beyond the end (no overlap)
  CHECK(m.insert({0xe0, 0xf0}) == span{0xe0, 0xf0});
  it = m.begin();
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x20, 0xaa});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xac, 0xf0});
  CHECK(it == m.end());

  // Insert beyond the end (with overlap)
  CHECK(m.insert({0xe0, 0x00}) == span{0xf0, 0x00});
  it = m.begin();
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x20, 0xaa});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xac, 0x00});
  CHECK(it == m.end());

  // Insert beyond the limit
  CHECK(m.insert({0x10, 0x30}) == span{0x10, 0x20});
  CHECK(m.insert({0x10, 0x30}).empty());
  it = m.begin();
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x20, 0xaa});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0xac, 0x00});
  REQUIRE(it != m.end()); CHECK(*it++ == span{0x10, 0x20});
  CHECK(it == m.end());

  CHECK(m.extent() == span{0x20, 0x20});
  CHECK_FALSE(m.empty());
}

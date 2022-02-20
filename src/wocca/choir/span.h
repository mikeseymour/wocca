#pragma once

#include <concepts>
#include <iomanip>

namespace wocca {
namespace choir {

/* A contiguous range of unsigned values from `begin()` up to but not including
 * `end()`. Arithmetic is modular, so `end() < begin()` indicates a range that
 * wraps at the end of the available set of values. This allows a span to
 * represent a sliding view of part of an unbounded stream.
 */
template <std::unsigned_integral Spot = unsigned>
struct span
{
  using spot = Spot;

  spot begin_;
  spot end_;

  spot begin() const {return begin_;}
  spot end() const {return end_;}
  spot size() const {return end_ - begin_;}
  spot offset(spot s) const {return s - begin_;}

  bool empty() const {return size() == 0;}
  bool contains(spot s) const {return offset(s) < size();}
  bool touches(spot s) const {return offset(s) <= size();}

  template <typename Stream>
  friend Stream& operator<<(Stream& stream, span s)
  {
    return stream << std::hex << '[' << +s.begin() << ',' << +s.end() << ')';
  }

  friend bool operator==(span x, span y)
  {
    return x.begin() == y.begin() && x.end() == y.end();
  }

  friend bool operator!=(span x, span y) {return !(x == y);}
};

}}

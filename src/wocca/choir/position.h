#pragma once

#include <type_traits>
#include <cstdint>

namespace wocca {
namespace choir {

// Representation of positions and offsets within a finite section of a
// potentially infinite stream. The types are affine: positions can be
// subtracted to give an offset, which can be applied to other offsets.
//
// For an N-bit offset type, arithmetic and comparisons are valid as long as
// the absolute distance between positions is less than 2^(N-1).
template <typename U, typename S = std::make_signed_t<U>>
struct position_details
{
  struct off
  {
    S val;

    off& operator+=(off o) {val += o.val; return *this;}
    off& operator-=(off o) {val -= o.val; return *this;}
  };

  struct pos
  {
    U val;

    pos& operator+=(off o) {val += o.val; return *this;}
    pos& operator-=(off o) {val -= o.val; return *this;}
  };

  friend pos operator+(pos x, off y) {return x += y;}
  friend pos operator+(off x, pos y) {return y += x;}
  friend pos operator-(pos x, off y) {return x -= y;}

  friend off operator-(pos x, pos y) {return {S(x.val - y.val)};}

  friend off operator+(off x, off y) {return x += y;}
  friend off operator-(off x, off y) {return x -= y;}
  friend off operator-(off x)        {return {S(-x.val)};}

  friend bool operator==(pos x, pos y) {return x.val == y.val;}
  friend bool operator!=(pos x, pos y) {return x.val != y.val;}

  friend bool operator==(off x, off y) {return x.val == y.val;}
  friend bool operator!=(off x, off y) {return x.val != y.val;}

  friend bool operator< (pos x, pos y) {return (x - y).val <  0;}
  friend bool operator<=(pos x, pos y) {return (x - y).val <= 0;}
  friend bool operator> (pos x, pos y) {return (x - y).val >  0;}
  friend bool operator>=(pos x, pos y) {return (x - y).val >= 0;}

  template <typename Stream>
  friend Stream& operator<<(Stream& s, pos x) {return s << +x.val;}
  template <typename Stream>
  friend Stream& operator<<(Stream& s, off x) {return s << +x.val;}
};

using pos64 = position_details<uint64_t>::pos;
using off64 = position_details<uint64_t>::off;

using pos32 = position_details<uint32_t>::pos;
using off32 = position_details<uint32_t>::off;

using pos16 = position_details<uint16_t>::pos;
using off16 = position_details<uint16_t>::off;

using pos8 = position_details<uint8_t>::pos;
using off8 = position_details<uint8_t>::off;

}
}


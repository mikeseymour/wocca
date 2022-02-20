#pragma once

#include "wocca/choir/span.h"
#include <vector>
#include <cassert>

namespace wocca {
namespace choir {

template <std::unsigned_integral Spot = unsigned>
class mesh
{
public:
  using spot = Spot;
  using span = choir::span<spot>;
  using iterator = std::vector<span>::const_iterator;

  enum class where {front, back};

  iterator begin() const {return spans.begin();}
  iterator end() const {return spans.end();}
  span front() const {return spans.front();}
  span back() const {return spans.back();}
  bool empty() const {return spans.empty();}

  /* The total range covered by all pieces of the mesh. Note that this will
   * appear empty if the mesh is empty, or if it covers the entire range
   * allowed by the `spot` type. Use `empty()` to distinguish these cases.
   */
  span extent() const {
    return empty() ? span{} : span{front().begin(), back().end()};
  }

  /* Inserts the first contiguous part of the span which will fit in a gap
   * in the mesh. If the span lies outside the mesh, it is inserted at the
   * front or back according to the `where` parameter.
   *
   * Returns the inserted span, empty if it was all present in the mesh.
   */
  span insert(span s, where where = where::back) {
    if (s.empty())
      return s;

    if (empty()) {
      spans.push_back(s);
      return s;
    }

    const span outside {back().end(), front().begin()};
    span gap_before = where == where::front ? outside : span{};

    for (auto it = spans.begin(); it != end(); ++it) {
      auto next = std::next(it);

      /* If the insertion begins before this piece, insert a new piece or
       * extend this one if they're contiguous. */
      if (gap_before.contains(s.begin())) {
        if (s.touches(it->begin())) {
          s.end_ = it->begin();
          it->begin_ = s.begin();
        } else {
          spans.insert(it, s);
        }
        return s;
      }

      /* If the insertion begins within or immediately after this one, extend
       * it into the following gap, merging with the next piece if they become
       * contiguous */
      if (it->touches(s.begin())) {
        if (!s.contains(it->end())) {
          return {};
        }

        s.begin_ = it->end();

        if (next == end()) {
          if (!outside.contains(s.end()))
            s.end_ = outside.end();
          it->end_ = s.end_;
        } else if (s.touches(next->begin())) {
          s.end_ = next->begin();
          it->end_ = next->end();
          spans.erase(next);
        } else {
          it->end_ = s.end();
        }

        return s;
      }

      /* If we find the end of the insertion without encountering the beginning,
       * it must begin before the first piece, so extend that. */
      if (where == where::front && it->touches(s.end())) {
        if (!outside.contains(s.begin()))
          s.begin_ = outside.begin();
        s.end_ = front().begin();
        spans.front().begin_ = s.begin_;
        return s;
      }

      if (next != end())
        gap_before = {it->end(), next->begin()};
    }

    /* If we do not find the insertion point, it must come after the mesh */
    assert(where == where::back);
    if (s.touches(front().begin()))
      s.end_ = front().begin();

    if (s.touches(back().end())) {
      s.begin_ = back().end();
      spans.back().end_ = s.end_;
    } else {
      spans.push_back(s);
    }
    return s;
  }

  void remove(span s) {

  }

private:
  std::vector<span> spans;
};

}}

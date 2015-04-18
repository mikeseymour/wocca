#include "wocca/magic/basic.h"

#include <type_traits>

namespace {
using namespace wocca::magic;

struct unwrapped {};
struct wrapped {typedef unwrapped result;};

static_assert(std::is_same<unwrapped, self<unwrapped>::result>(), "");
static_assert(std::is_same<unwrapped, self<wrapped>::result>(), "");

}

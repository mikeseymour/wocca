#include "wocca/magic/basic.h"
#include "test/magic.h"

using namespace wocca::magic;

TEST_CASE("wocca/magic/basic") {
    struct unwrapped {};
    struct wrapped {typedef unwrapped result;};

    CHECK_META(unwrapped, self<unwrapped>);
    CHECK_META(unwrapped, self<wrapped>);

    CHECK(typeid(self<wrapped>).name() == typeid(self<unwrapped>).name());
}

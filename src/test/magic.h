#ifndef WOCCA_TEST_MAGIC_H
#define WOCCA_TEST_MAGIC_H

#include "test/catch.hpp"

#define CHECK_META(RESULT, ...) CHECK(typeid(typename __VA_ARGS__::result).name() == typeid(RESULT).name())

#endif

#include "wocca/tags/parser.h"
#include "test/catch.hpp"

using namespace wocca::tags;

static_assert(parser<2,3,5>::index(1) == -1);
static_assert(parser<2,3,5>::index(2) == 0);
static_assert(parser<2,3,5>::index(3) == 1);
static_assert(parser<2,3,5>::index(4) == -1);
static_assert(parser<2,3,5>::index(5) == 2);
static_assert(parser<2,3,5>::index(6) == -1);

static_assert(parser<2,3,5,6>::index(1) == -1);
static_assert(parser<2,3,5,6>::index(2) == 0);
static_assert(parser<2,3,5,6>::index(3) == 1);
static_assert(parser<2,3,5,6>::index(4) == -1);
static_assert(parser<2,3,5,6>::index(5) == 2);
static_assert(parser<2,3,5,6>::index(6) == 3);
static_assert(parser<2,3,5,6>::index(7) == -1);

struct reader {
	int cursor = 0;
	tag operator()() {
		switch (cursor++) {
		case 0: return {7,"Second"};
		case 1: return {3,"First"};
		case 2: return {8,"Third"};
		case 3: return {999,"Bogus"};
		default: return {};
		}
	}
};

TEST_CASE("tags/parser") {
	reader r;
	parser<7,3,8> p(r);
	CHECK(p.at<3>() == "First");
	CHECK(p.at<7>() == "Second");
	CHECK(p.at<8>() == "Third");
	// p.at<999>(); // compile error
}
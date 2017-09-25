#include "wocca/sound/network.h"
#include "test/catch.hpp"

using namespace wocca::sound;

TEST_CASE("wocca/sound/network", "Sound processing network"){
    static int order;

    struct source : outputs<2> {
        void process() {CHECK(order == 0); ++order;}
    };
    struct mid1 : inputs<1>, outputs<1> {
        void process() {CHECK((order == 1 || order == 2)); ++order;}
    };
    struct mid2 : inputs<1>, outputs<1> {
        void process() {CHECK((order == 1 || order == 2)); ++order;}
    };
    struct sink : inputs<2> {
        void process() {CHECK(order == 3); ++order;}
    };

    network<
        connect<output<source,0>, input<mid1>>,
        connect<output<source,1>, input<mid2>>,
        connect<input<sink,0>, output<mid1>>,
        connect<input<sink,1>, output<mid2>>
    > network;

    order = 0;
    network.process();
    CHECK(order == 4);
}


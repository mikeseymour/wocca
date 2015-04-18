#ifndef WOCCA_SOUND_NETWORK_H
#define WOCCA_SOUND_NETWORK_H

#include "wocca/magic/graph.h"

namespace wocca {
namespace sound {

template <unsigned> struct inputs {};
template <unsigned> struct outputs {};

template <typename, unsigned=0> struct input;
template <typename, unsigned=0> struct output;
template <typename InOrOut, typename OutOrIn> struct connect;

template <typename T1, unsigned I1, typename T2, unsigned I2>
struct connect<input<T1,I1>, output<T2,I2>> : connect<output<T2,I2>, input<T1,I1>> {};

template <typename T1, unsigned I1, typename T2, unsigned I2>
struct connect<output<T1,I1>, input<T2,I2>> {
    typedef magic::graph::edge<T1, T2> edge_type;
};

template <typename... Connections> class network {
public:
    void process() {magic::tuple::visit(components, process_helper());}

    typename magic::graph::sort<typename Connections::edge_type...>::result components;

    // C++14: use a generic lambda [](auto && c){c.process();}
    struct process_helper {template <typename Component> void operator()(Component && c) {c.process();}};
};

}}
#endif

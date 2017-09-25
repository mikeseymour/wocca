#pragma once

#include "wocca/magic/graph.h"

namespace wocca {
namespace sound {

template <unsigned> struct inputs {};
template <unsigned> struct outputs {};

template <class, unsigned=0> struct input;
template <class, unsigned=0> struct output;
template <class InOrOut, class OutOrIn> struct connect;

template <class T1, unsigned I1, class T2, unsigned I2>
struct connect<input<T1,I1>, output<T2,I2>> {
    using edge = magic::graph::edge<T2, T1>;
};

template <class T1, unsigned I1, class T2, unsigned I2>
struct connect<output<T1,I1>, input<T2,I2>> {
    using edge = magic::graph::edge<T1, T2>;
};

template <typename... Connections> class network {
public:
    void process() {components.visit([](auto && c){c.process();});}

private:
    using edges = magic::tuple<typename Connections::edge...>;
    magic::graph::sort<edges> components;
};

}}


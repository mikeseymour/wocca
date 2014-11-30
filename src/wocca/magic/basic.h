#ifndef WOCCA_MAGIC_BASIC_H
#define WOCCA_MAGIC_BASIC_H

// Basic building blocks for template metaprograms

namespace wocca {
namespace magic {

// Helper to extract a wrapped metafunction result
template <typename T> T meta_result(...);
template <typename T> typename T::result meta_result(typename T::result*);

// Wrap a type as a metafunction result.
// In general, metafunctions results will be a nested type called "result",
// often defined by ultimately inheriting from some specialisation of "meta".
template <typename T> struct meta {typedef T result;};

template <typename T> using self = meta<decltype(meta_result<T>(0))>;

}}
#endif

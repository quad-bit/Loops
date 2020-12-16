#pragma once

#include <functional>
#include "RenderingWrapper.h"

//template <typename T, typename... Rest>
//inline void HashCombine(std::size_t &seed, T const &v, Rest &&... rest) {
//    std::hash<T> hasher;
//    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//    (int[]) {  0, (HashCombine(seed, std::forward<Rest>(rest)), 0)... };
//}

inline void HashCombine(std::size_t& seed) { }

template <typename T, typename... Rest>
inline void HashCombine(std::size_t& seed, const T& v, Rest... rest) 
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    HashCombine(seed, rest...);
}


bool operator==(const VertexInputAttributeInfo & lhs, const VertexInputAttributeInfo & rhs)
{
    if (lhs.binding == rhs.binding && lhs.format == rhs.format &&
        lhs.location == rhs.location && lhs.offset == rhs.offset)
        return true;
    else
        return false;
}

bool operator==(const VertexInputBindingInfo & lhs, const VertexInputBindingInfo & rhs)
{
    if (lhs.binding == rhs.binding && lhs.inputRate == rhs.inputRate &&
        lhs.stride == rhs.stride)
        return true;
    else
        return false;
}


namespace std
{
    template<> struct hash<VertexInputAttributeInfo>
    {
        std::size_t operator()(VertexInputAttributeInfo const& s) const noexcept
        {
            std::size_t h1 = std::hash<std::uint32_t>{}(s.binding);
            std::size_t h2 = std::hash<std::uint32_t>{}(s.location);
            std::size_t h3 = std::hash<std::uint32_t>{}(s.offset);
            std::size_t h4 = std::hash<std::uint32_t>{}((uint32_t)s.format);

            std::size_t seed = 0UL;
            HashCombine(seed, h1, h2, h3, h4);

            return seed; // or use boost::hash_combine
        }
    };

    template<> struct hash<VertexInputBindingInfo>
    {
        std::size_t operator()(VertexInputBindingInfo const& s) const noexcept
        {
            std::size_t h1 = std::hash<std::uint32_t>{}(s.binding);
            std::size_t h2 = std::hash<std::uint32_t>{}(s.stride);
            std::size_t h3 = std::hash<std::uint32_t>{}((uint32_t)s.inputRate);

            std::size_t seed = 0UL;
            HashCombine(seed, h1, h2, h3);

            return seed; // or use boost::hash_combine
        }
    };
}


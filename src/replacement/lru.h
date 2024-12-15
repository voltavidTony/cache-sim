/// @file lru.h
/// @brief Declaration of the least recently used replacement policy

#pragma once

#include "replacement_policy.h"

/// @brief The LRU replacement policy
class LRU : public ReplacementPolicy {
public:

    /// @brief Construct a new LRU replacement policy
    /// @param cache The parent cache
    /// @param num_sets The number of sets in the cache
    /// @param assoc The associativity of the chace
    LRU(CacheABC& cache, uint32_t num_sets, uint32_t assoc);
    ~LRU();

    /// @brief Determine which line of a range of lines to replace
    /// @param set_idx The index of the set to choose from
    /// @return The chosen line's index within the set (0 to assoc-1)
    uint32_t getVictim(uint32_t set_idx);

    /// @brief Notify the replacement policy that a line was just accessed
    /// @param set_idx The index of the set containing the line
    /// @param way_idx The index of the way containing the line (0 to assoc-1)
    void touch(uint32_t set_idx, uint32_t way_idx);

    /// @brief Print out the replacer's internal state
    /// @param set_idx The index of the set
    void printState(uint32_t set_idx);

private:

    /// @brief Line age, in set accesses since last line access
    uint32_t** age;
};
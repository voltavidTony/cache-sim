/// @file cache.h
/// @brief Declaration of the Cache class

#pragma once

#include "cache_abc.h"

/// @brief An L1 Cache with coherence protocol and replacement policy
class Cache : public CacheABC {
public:

    /// @brief Construct a new cache
    /// @param memory_bus The parent memory bus
    /// @param flushed A reference to the memory bus' flushed flag
    /// @param cache_id The ID of this cache
    /// @param config The configuration of the parent memory bus
    Cache(MemoryBus& memory_bus, bool& flushed, uint32_t cache_id, cache_config& config);
    ~Cache();

    /// @brief Issue a PrRd message to this cache
    /// @param addr The address accessed
    void receivePrRd(addr_t addr);
    /// @brief Issue a PrWr message to this cache
    /// @param addr The address accessed
    void receivePrWr(addr_t addr);

    /// @brief Issue a BusRd message to neighboring caches
    /// @param bus_msg The specific bus message
    /// @return True if the 'COPIES-EXIST' line was asserted
    bool issueBusMsg(bus_msg_e bus_msg);
    /// @brief Issue a bus message to this cache
    /// @param bus_msg The specific bus message
    /// @param addr The address accessed
    /// @return True if the bus transaction resulted in a hit
    bool receiveBusMsg(bus_msg_e bus_msg, addr_t addr);

    /// @brief Get the state of a line in the cache
    /// @param set_idx The index of the set containing the line
    /// @param way_idx The index of the way containing the line (0 to assoc-1)
    /// @return The state of the cache line
    state_e getLineState(uint32_t set_idx, uint32_t way_idx);

    /// @brief Print simulation run statistics in CSV format (headerless)
    /// @note Does not produce output if the cache is unused
    void printStats();

private:

    /// @brief Parent memory bus
    MemoryBus& memory_bus;
    /// @brief Reference to memory bus flag indicating if a line was flushed
    bool& flushed;
    /// @brief Config from the parent memory bus
    cache_config& config;

    /// @brief Coherence protocol used by this cache
    CoherenceProtocol* coherence_protocol;
    /// @brief Replacement policy used by this cache
    ReplacementPolicy* replacement_policy;
    /// @brief Cache lines contained in this cache
    cache_line* lines;

    /// @brief ID of this cache
    uint32_t cache_id;
    /// @brief The number of sets in the cache
    uint32_t num_sets;
    /// @brief Number of bits that come before the line offset field
    uint32_t line_offset;
    /// @brief Number of bits that come before the tag field
    uint32_t tag_offset;

    /// @brief Cache runtime statistics
    size_t statistics[N_STATISTICS] = { 0 };

    /// @brief The address being accessed by the current processor read or write
    /// @note Remembering the currently accessed address only works because each memory
    /// access is atomic, i.e. all resulting bus messages will finish before the next memory access
    addr_t curr_access_addr;

    /// @brief Update the correct state transition statistic
    /// @param before The state the line was in before
    /// @param after The new state of the line
    void stateChangeStatistic(state_e before, state_e after);

    /// @brief Initialize a line in the cache, performing a writeback if necessary
    /// @param addr The address that requires caching
    /// @return A pointer to the newly initialized cache line
    /// @note The line's state will be 'Invalid'
    cache_line* allocate(addr_t addr);
    /// @brief Locate a line in the cache
    /// @param addr The address being accessed
    /// @return A pointer to the line if found, else nullptr
    cache_line* findLine(addr_t addr);
};

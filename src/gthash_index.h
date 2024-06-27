#ifndef GRG_GTHASH_INDEX_H
#define GRG_GTHASH_INDEX_H

#include <vector>
#include <unordered_map>
#include <cstdint>
#include <algorithm>
#include <cassert>
#include <list>

#include "grgl/common.h"
#include "grgl/grg.h"
#include "grgl/grgnode.h"
#include "grgl/mutation.h"
#include "util.h"
#include "lean_bk_tree.h"
#include "similarity/mmh3.h"

namespace grgl {

using GtHashT = uint32_t;
using GenotypeHash = std::vector<GtHashT>;
using MutationList = std::vector<Mutation>;

void dumpHash(const GenotypeHash& hash);

inline GenotypeHash bitwiseIntersect(std::list<GenotypeHash>& gtHashList) {
    release_assert(gtHashList.size() > 1);
    GenotypeHash result = gtHashList.front();
    for (const auto& gtHash : gtHashList) {
        release_assert(result.size() == gtHash.size());
        for (size_t bucket = 0; bucket < gtHash.size(); bucket++) {
            result[bucket] &= gtHash[bucket];
        }
    }
    return std::move(result);
}

inline size_t bitwiseHamming(const GenotypeHash& hash1, const GenotypeHash& hash2) {
    size_t dist = 0;
    release_assert(hash1.size() == hash2.size());
    for (size_t i = 0; i < hash1.size(); i++) {
        static_assert(sizeof(GtHashT) == 4,
                      "Optimization for counting set bits assumes 32-bit ints");
        uint32_t xorValue = hash1[i] ^ hash2[i];
        // Fun times: https://graphics.stanford.edu/~seander/bithacks.htm
        uint32_t numBits = xorValue - ((xorValue >> 1U) & 0x55555555U);
        numBits = (numBits & 0x33333333U) + ((numBits >> 2U) & 0x33333333U);
        numBits = ((numBits + (numBits >> 4U) & 0xF0F0F0FU) * 0x1010101U) >> 24U;
        dist += numBits;
    }
    return dist;
}

using NodeToGtHash = std::vector<GenotypeHash>;

/**
 * Index a dataset based on their genotype hashes (hash of mutations).
 */
class GenotypeHashIndex {
public:
    explicit GenotypeHashIndex(std::function<size_t(const NodeID&, const NodeID&)> distFunc)
        : m_bkTree(std::move(distFunc)) {
    }

    virtual ~GenotypeHashIndex() = default;

    GenotypeHashIndex(GenotypeHashIndex&) = delete;
    GenotypeHashIndex(GenotypeHashIndex&&) = default;
    GenotypeHashIndex& operator=(GenotypeHashIndex&) = delete;
    GenotypeHashIndex& operator=(GenotypeHashIndex&&) = default;

    /**
     * Add a new (hash, node) pair to the index.
     */
    void add(NodeID nodeId);

    /**
     * Remove a (hash, node) pair from the index.
     */
    void remove(NodeID nodeId);

    /**
     * Find the nearest neighbor to targetHash (distance=D) and then find all other neighbors
     * at the same distance D and return the list.
     */
    NodeIDList getMostSimilarNodes(NodeID nodeId, bool collectAll);

    void emitStats() const {
        std::cout << " -- Index Stats --" << std::endl;
        std::cout << "  -> Comparisons: " << m_comparisons << std::endl;
    }
private:
    LeanBKTree<NodeID> m_bkTree;
    // Keep track of how many comparisons we do.
    size_t m_comparisons{};
};

}

#endif /* GRG_GTHASH_INDEX_H */

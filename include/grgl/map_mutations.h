/* Genotype Representation Graph Library (GRGL)
 * Copyright (C) 2024 April Wei
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * should have received a copy of the GNU General Public License
 * with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef MAP_MUTATIONS_H
#define MAP_MUTATIONS_H

#include "grgl/common.h"
#include "grgl/mut_iterator.h"

#include <atomic>
#include <memory>
#include <iosfwd>

namespace grgl {

class MutableGRG;
using MutableGRGPtr = std::shared_ptr<MutableGRG>;


struct MutationMappingStats {
    std::atomic<size_t> totalMutations{0};
    std::atomic<size_t> emptyMutations{0};
    std::atomic<size_t> mutationsWithOneSample{0};
    std::atomic<size_t> mutationsWithNoCandidates{0};
    std::atomic<size_t> reusedNodes{0};
    std::atomic<size_t> reusedNodeCoverage{0};
    std::atomic<size_t> reusedExactly{0};
    std::atomic<size_t> singletonSampleEdges{0}; // Direct edge from Mutation to sample
    std::atomic<size_t> newTreeNodes{0};
    std::atomic<size_t> samplesProcessed{0};
    std::atomic<size_t> numCandidates{0};
    std::atomic<size_t> reuseSizeBiggerThanHistMax{0};
    std::atomic<size_t> numWithSingletons{0};
    std::atomic<size_t> maxSingletons{0};
    std::atomic<size_t> reusedMutNodes{0};
    std::vector<size_t> reuseSizeHist;

    MutationMappingStats() = default;

    // copy constructor
    MutationMappingStats(const MutationMappingStats& other) : 
        totalMutations(other.totalMutations.load()), 
        emptyMutations(other.emptyMutations.load()), 
        mutationsWithOneSample(other.mutationsWithOneSample.load()),
        mutationsWithNoCandidates(other.mutationsWithNoCandidates.load()),
        reusedNodes(other.reusedNodes.load()),
        reusedNodeCoverage(other.reusedNodeCoverage.load()),
        reusedExactly(other.reusedExactly.load()),
        singletonSampleEdges(other.singletonSampleEdges.load()),
        newTreeNodes(other.newTreeNodes.load()),
        samplesProcessed(other.samplesProcessed.load()),
        numCandidates(other.numCandidates.load()),
        reuseSizeBiggerThanHistMax(other.reuseSizeBiggerThanHistMax.load()),
        numWithSingletons(other.numWithSingletons.load()),
        maxSingletons(other.maxSingletons.load()),
        reusedMutNodes(other.reusedMutNodes.load()),
        reuseSizeHist(other.reuseSizeHist) {}

    // copy assignment
    MutationMappingStats& operator=(const MutationMappingStats& other) {
        if (this == &other) return *this; // handle self-assignment

        totalMutations.store(other.totalMutations.load());
        emptyMutations.store(other.emptyMutations.load());
        mutationsWithOneSample.store(other.mutationsWithOneSample.load());
        mutationsWithNoCandidates.store(other.mutationsWithNoCandidates.load());
        reusedNodes.store(other.reusedNodes.load());
        reusedNodeCoverage.store(other.reusedNodeCoverage.load());
        reusedExactly.store(other.reusedExactly.load());
        singletonSampleEdges.store(other.singletonSampleEdges.load());
        newTreeNodes.store(other.newTreeNodes.load());
        samplesProcessed.store(other.samplesProcessed.load());
        numCandidates.store(other.numCandidates.load());
        reuseSizeBiggerThanHistMax.store(other.reuseSizeBiggerThanHistMax.load());
        numWithSingletons.store(other.numWithSingletons.load());
        maxSingletons.store(other.maxSingletons.load());
        reusedMutNodes.store(other.reusedMutNodes.load());
        reuseSizeHist = other.reuseSizeHist;

        return *this;
    }

    void print(std::ostream& outStream) const {
        outStream << "mutations: " << this->totalMutations.load() << std::endl;
        outStream << "candidates: " << this->numCandidates.load() << std::endl;
        outStream << "emptyMutations: " << this->emptyMutations.load() << std::endl;
        outStream << "mutationsWithOneSample: " << this->mutationsWithOneSample.load() << std::endl;
        outStream << "singletonSampleEdges: " << this->singletonSampleEdges.load() << std::endl;
        outStream << "samplesProcessed: " << this->samplesProcessed.load() << std::endl;
        outStream << "reusedNodes: " << this->reusedNodes.load() << std::endl;
        outStream << "reusedExactly: " << this->reusedExactly.load() << std::endl;
        outStream << "reusedNodeCoverage: " << this->reusedNodeCoverage.load() << std::endl;
        outStream << "reusedMutNodes: " << this->reusedMutNodes.load() << std::endl;
        outStream << "reuseSizeBiggerThanHistMax: " << this->reuseSizeBiggerThanHistMax.load() << std::endl;
        outStream << "numWithSingletons: " << this->numWithSingletons.load() << std::endl;
        outStream << "maxSingletons: " << this->maxSingletons.load() << std::endl;
        outStream << "avgSingletons: " << (double)this->singletonSampleEdges.load() / (double)this->numWithSingletons.load()
                  << std::endl;
    }
};


class HaplotypeIndex;

MutationMappingStats mapMutations(const MutableGRGPtr& grg, MutationIterator& mutations);

}; // namespace grgl

#endif /* MAP_MUTATIONS_H */

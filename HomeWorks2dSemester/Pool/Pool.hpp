#ifndef MEMORY_POOL_HPP
#define MEMORY_POOL_HPP

#include <vector>
#include <list>
#include <mutex>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <atomic>
#include <chrono>
#include <iostream>

template <typename T, typename Alloc = std::allocator<T>>
class MemoryPool {
public:
    using valueType = T;
    using pointer = Pointer;
    using sizeType = std::size_t;
    using allocatorType = Alloc;

public:
    explicit MemoryPool(sizeType initialSize, sizeType blockSizeInBytes = 1024, const Alloc& alloc = Alloc());
    ~MemoryPool();

    void expandPool(sizeType additionalBlocks, sizeType blockSize = 0);
    template <typename... Args>
    pointer allocate(sizeType blockSize = 0, Args&&... vals);
    void deallocate(pointer block);
    void defragment();
    void printStats() const;

private:
    using allocatorTraits = std::allocator_traits<Alloc>;
    using pointer = typename Alloc::pointer;

    struct BlockNode {
        pointer blockPtr;
        size_t blockSize;
        
        bool operator==(const BlockNode& rhs) const {
            return blockPtr == rhs.blockPtr && blockSize == rhs.blockSize;
        }
    };

    using freeBlocksContainer = std::vector<BlockNode, typename allocatorTraits::template rebind_alloc<BlockNode>>;
    using usingBlocksContainer = std::list<BlockNode, typename allocatorTraits::template rebind_alloc<BlockNode>>;

    const std::size_t defaultBlockSize_;
    std::size_t poolSize_;
    freeBlocksContainer freeBlocks_;
    usingBlocksContainer usingBlocks_;
    std::atomic<std::size_t> currInd;
    Alloc alloc_;
    mutable std::mutex mutex_;

    struct PoolStats {
        std::size_t totalAllocations{0};
        std::size_t totalDeallocations{0};
        std::size_t peakUsage{0};
        std::size_t totalBytesAllocated{0};
        std::chrono::steady_clock::time_point startTime;
    };

    BlockNode findSuitableNode(sizeType requiredSize);
    void updateStats();
    void cleanup() noexcept;
};

#endif // MEMORY_POOL_HPP

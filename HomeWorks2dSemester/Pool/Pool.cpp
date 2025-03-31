#include "Pool.hpp"

template <typename T, typename Alloc>
MemoryPool<T, Alloc>::MemoryPool(sizeType initialSize, sizeType blockSizeInBytes, const Alloc& alloc)
    : defaultBlockSize_(blockSizeInBytes),
      poolSize_(initialSize),
      currInd(0),
      alloc_(alloc),
      freeBlocks_(alloc),
      usingBlocks_(alloc),
      stats{.startTime = std::chrono::steady_clock::now()} {
    expandPool(initialSize, defaultBlockSize_);
}

template <typename T, typename Alloc>
MemoryPool<T, Alloc>::~MemoryPool() {
    std::lock_guard<std::mutex> lock(mutex_);
    cleanup();
}

template <typename T, typename Alloc>
void MemoryPool<T, Alloc>::expandPool(sizeType additionalBlocks, sizeType blockSize) {
    std::lock_guard<std::mutex> lock(mutex_);
    blockSize = blockSize ? blockSize : defaultBlockSize_;

    try {
        for (sizeType i = 0; i < additionalBlocks; ++i) {
            pointer block = allocatorTraits::allocate(alloc_, (blockSize + sizeof(T) - 1) / sizeof(T));
            freeBlocks_.push_back({block, blockSize});
            stats.totalBytesAllocated += blockSize;
        }

        poolSize_ += additionalBlocks;
    } catch (...) {
        cleanup();
        throw;
    }
}

template <typename T, typename Alloc>
template <typename... Args>
typename MemoryPool<T, Alloc>::pointer MemoryPool<T, Alloc>::allocate(sizeType blockSize, Args&&... args) {
    std::lock_guard<std::mutex> lock(mutex_);

    blockSize = blockSize ? blockSize : defaultBlockSize_;
    BlockNode blockNode = findSuitableNode(blockSize);

    if (blockNode.blockPtr == nullptr) {
        expandPool(1, blockSize);
        blockNode = freeBlocks_.back();
        freeBlocks_.pop_back();
    } 
    else {
        freeBlocks_.erase(std::remove(freeBlocks_.begin(), freeBlocks_.end(), blockNode), freeBlocks_.end());
    }

    try {
        allocatorTraits::construct(alloc_, blockNode.blockPtr, std::forward<Args>(args)...);
        usingBlocks_.push_back(blockNode);
        updateStats();

        return blockNode.blockPtr;
    } catch (...) {
        freeBlocks_.push_back(blockNode);
        throw;
    }
}

template <typename T, typename Alloc>
void MemoryPool<T, Alloc>::deallocate(pointer block) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = std::find_if(usingBlocks_.begin(), usingBlocks_.end(), [block](const BlockNode& node) {
        return node.blockPtr == block;
    });

    if (it == usingBlocks_.end()) {
        throw std::invalid_argument("Invalid block pointer");
    }

    allocatorTraits::destroy(alloc_, block);
    BlockNode blockNode = *it;
    usingBlocks_.erase(it);
    freeBlocks_.push_back(blockNode);
    stats.totalDeallocations++;
}

template <typename T, typename Alloc>
void MemoryPool<T, Alloc>::defragment() {
    std::lock_guard<std::mutex> lock(mutex_);
    freeBlocks_.shrink_to_fit();
    std::sort(freeBlocks_.begin(), freeBlocks_.end(), [](const BlockNode& a, const BlockNode& b) {
        return a.blockPtr < b.blockPtr;
    });
}

template <typename T, typename Alloc>
void MemoryPool<T, Alloc>::printStats() const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::steady_clock::now() - stats.startTime).count();

    std::cout << "Memory Pool Statistics:\n"
              << "Total capacity: " << poolSize_ << " blocks\n"
              << "Free blocks: " << freeBlocks_.size() << "\n"
              << "Used blocks: " << usingBlocks_.size() << "\n"
              << "Total allocations: " << stats.totalAllocations << "\n"
              << "Total deallocations: " << stats.totalDeallocations << "\n"
              << "Peak usage: " << stats.peakUsage << " blocks\n"
              << "Total bytes allocated: " << stats.totalBytesAllocated << "\n"
              << "Uptime: " << duration << " seconds\n";
}

template <typename T, typename Alloc>
typename MemoryPool<T, Alloc>::BlockNode MemoryPool<T, Alloc>::findSuitableNode(sizeType requiredSize) {
    for (const auto& blockNode : freeBlocks_) {
        if (blockNode.blockSize >= requiredSize) {
            return blockNode;
        }
    }

    return {nullptr, 0};
}

template <typename T, typename Alloc>
void MemoryPool<T, Alloc>::updateStats() {
    ++stats.totalAllocations;
    stats.peakUsage = std::max(stats.peakUsage, usingBlocks_.size());
}

template <typename T, typename Alloc>
void MemoryPool<T, Alloc>::cleanup() noexcept {
    for (const auto& blockNode : freeBlocks_) {
        allocatorTraits::deallocate(alloc_, blockNode.blockPtr, (blockNode.blockSize + sizeof(T) - 1) / sizeof(T));
    }
    
    for (const auto& node : usingBlocks_) {
        allocatorTraits::destroy(alloc_, node.blockPtr);
        allocatorTraits::deallocate(alloc_, node.blockPtr, (node.blockSize + sizeof(T) - 1) / sizeof(T));
    }
}

#include "PoolAllocator.hpp"

template <typename T, typename Alloc>
PoolAllocator<T, Alloc>::PoolAllocator(Pool<T>& pool) noexcept
    : pool_(pool) {}

template <typename T, typename Alloc>
template <typename U>
PoolAllocator<T, Alloc>::PoolAllocator(const PoolAllocator<U, Alloc>& other) noexcept
    : pool_(other.pool_) {}

template <typename T, typename Alloc>
typename PoolAllocator<T, Alloc>::pointer PoolAllocator<T, Alloc>::allocate(size_type n, const void* hint) {
    if (n == 0) {
        return nullptr;
    }

    pointer block = pool_.allocate(n * sizeof(T));
    return block;
}

template <typename T, typename Alloc>
void PoolAllocator<T, Alloc>::deallocate(pointer p, size_type n) noexcept {
    if (p) {
        pool_.deallocate(p);
    }
}

template <typename T, typename Alloc>
template <typename U, typename... Args>
void PoolAllocator<T, Alloc>::construct(U* p, Args&&... args) noexcept {
    new (p) U(std::forward<Args>(args)...);
}

template <typename T, typename Alloc>
template <typename U>
void PoolAllocator<T, Alloc>::destroy(U* p) noexcept {
    p->~U();
}

template <typename T, typename Alloc>
bool PoolAllocator<T, Alloc>::operator==(const PoolAllocator& other) const noexcept {
    return &pool_ == &other.pool_;
}

template <typename T, typename Alloc>
bool PoolAllocator<T, Alloc>::operator!=(const PoolAllocator& other) const noexcept {
    return !(*this == other);
}

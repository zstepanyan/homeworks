#ifndef POOL_ALLOCATOR_HPP
#define POOL_ALLOCATOR_HPP

#include "Pool.hpp"
#include <memory>
#include <type_traits>

template <typename T, typename Alloc = std::allocator<T>>
class PoolAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    explicit PoolAllocator(Pool<T>& pool) noexcept;
    
    template <typename U>
    PoolAllocator(const PoolAllocator<U, Alloc>& other) noexcept;

    pointer allocate(size_type n, const void* hint = nullptr);

    void deallocate(pointer p, size_type n) noexcept;

    template <typename U, typename... Args>
    void construct(U* p, Args&&... args) noexcept;

    template <typename U>
    void destroy(U* p) noexcept;

    template <typename U>
    struct rebind {
        using other = PoolAllocator<U, Alloc>;
    };

    bool operator==(const PoolAllocator& other) const noexcept;
    bool operator!=(const PoolAllocator& other) const noexcept;

private:
    Pool<T>& pool_;
};

#endif // POOL_ALLOCATOR_HPP

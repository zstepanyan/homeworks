#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>


struct AllocationHeader {
    size_t size;        
};


template <typename T>
class Allocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = std::size_t;

    // Default constructor
    Allocator() noexcept = default;

    // Copy constructor
    template <typename U>
    Allocator(const AdvancedAllocator<U>&) noexcept {}

    // Allocate memory
    pointer allocate(size_type n) {
        size_type size = n * sizeof(T);
        size_type total_size = size + sizeof(AllocationHeader);

        // Allocate raw memory
        void* raw_mem = nullptr;
        try {
            raw_mem = ::operator new(total_size);
        } catch (const std::bad_alloc& e) {
            throw;
        }

        // Place header just before the allocated memory
        AllocationHeader* header = reinterpret_cast<AllocationHeader*>(raw_mem);
        header->size = size;

        // Return memory after the header
        return reinterpret_cast<pointer>(header + 1);
    }

    // Deallocate memory
    void deallocate(pointer p, size_type n) noexcept {
        if (!p) return;

        // Retrieve the header (before the memory block)
        AllocationHeader* header = reinterpret_cast<AllocationHeader*>(p) - 1;

        // Deallocate the raw memory
        ::operator delete(header);
    }

    // Get memory overhead (just the size of AllocationHeader in this case)
    size_type get_overhead() const noexcept {
        return sizeof(AllocationHeader);
    }

    // Rebind for allocator traits
    template <typename U>
    struct rebind {
        using other = AdvancedAllocator<U>;
    };
};


#endif // ALLOCATOR_HPP
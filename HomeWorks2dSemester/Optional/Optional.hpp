#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP

#include <type_traits>
#include <stdexcept>
#include <utility>

class nullopt_t {
public:
    constexpr nullopt_t() noexcept = default;
};

inline constexpr nullopt_t nullopt;

template <typename T>
class Optional {
public:
    using value_type = T;

    constexpr Optional() noexcept = default;
    constexpr Optional(nullopt_t) noexcept;
    Optional(T&& val);

    Optional(const Optional& rhs);
    Optional(Optional&& rhs) noexcept;

    Optional& operator=(const Optional& rhs);
    Optional& operator=(Optional&& rhs) noexcept;

    ~Optional();

    bool has_value() const noexcept;
    explicit operator bool() const noexcept;

    value_type& operator*();
    const value_type& operator*() const;

    value_type* operator->();
    const value_type* operator->() const;

    template <typename U>
    value_type value_or(U&& default_value) const&;

    template <typename U>
    value_type value_or(U&& default_value) &&;

private:
    union Storage {
        value_type value;
        Storage() noexcept {}
        ~Storage() {}
    };

    Storage storage;
    bool engaged = false;

    void destroy() noexcept;
};

template <typename T, typename... Args>
constexpr Optional<T> make_optional(Args&&... args);

#endif // OPTIONAL_HPP

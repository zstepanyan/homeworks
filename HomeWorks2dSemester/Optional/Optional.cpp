#include "Optional.hpp"

template <typename T>
void Optional<T>::destroy() noexcept {
    if (engaged) {
        storage.value.~value_type();
        engaged = false;
    }
}

template <typename T>
constexpr Optional<T>::Optional(nullopt_t) noexcept : engaged(false) {}

template <typename T>
Optional<T>::Optional(T&& val) : engaged(true) {
    new (&storage.value) value_type(std::move(val));
}

template <typename T>
Optional<T>::Optional(const Optional& rhs) : engaged(rhs.engaged) {
    if (rhs.engaged) {
        new (&storage.value) value_type(rhs.storage.value);
    }
}

template <typename T>
Optional<T>::Optional(Optional&& rhs) noexcept : engaged(rhs.engaged) {
    if (rhs.engaged) {
        new (&storage.value) value_type(std::move(rhs.storage.value));
        rhs.engaged = false;
    }
}

template <typename T>
Optional<T>& Optional<T>::operator=(const Optional& rhs) {
    if (this != &rhs) {
        Optional temp(rhs);
        destroy();
        engaged = temp.engaged;
        if (temp.engaged) {
            new (&storage.value) value_type(std::move(temp.storage.value));
            temp.engaged = false;
        }
    }
    return *this;
}

template <typename T>
Optional<T>& Optional<T>::operator=(Optional&& rhs) noexcept {
    if (this != &rhs) {
        destroy();
        engaged = rhs.engaged;
        if (rhs.engaged) {
            new (&storage.value) value_type(std::move(rhs.storage.value));
            rhs.engaged = false;
        }
    }
    return *this;
}

template <typename T>
Optional<T>::~Optional() { destroy(); }

template <typename T>
bool Optional<T>::has_value() const noexcept { return engaged; }

template <typename T>
Optional<T>::operator bool() const noexcept { return engaged; }

template <typename T>
T& Optional<T>::operator*() {
    if (!engaged) throw std::logic_error("No value");
    return storage.value;
}

template <typename T>
const T& Optional<T>::operator*() const {
    if (!engaged) throw std::logic_error("No value");
    return storage.value;
}

template <typename T>
T* Optional<T>::operator->() {
    if (!engaged) throw std::logic_error("No value");
    return &storage.value;
}

template <typename T>
const T* Optional<T>::operator->() const {
    if (!engaged) throw std::logic_error("No value");
    return &storage.value;
}

template <typename T>
template <typename U>
T Optional<T>::value_or(U&& default_value) const& {
    return engaged ? storage.value : static_cast<T>(std::forward<U>(default_value));
}

template <typename T>
template <typename U>
T Optional<T>::value_or(U&& default_value) && {
    return engaged ? std::move(storage.value) : static_cast<T>(std::forward<U>(default_value));
}

template <typename T, typename... Args>
constexpr Optional<T> make_optional(Args&&... args) {
    return Optional<T>(T(std::forward<Args>(args)...));
}

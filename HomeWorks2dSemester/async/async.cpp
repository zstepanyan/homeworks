#include <iostream>
#include <thread>
#include <future>
#include <utility>
#include <functional>

template <typename Func, typename... Args>
auto my_async(Func&& f, Args&&... args) -> std::future<decltype(f(args...))> {
    using return_type = decltype(f(args...));

    std::promise<return_type> prom;
    std::future<return_type> fut = prom.get_future();

    std::thread([prom = std::move(prom), f = std::forward<Func>(f)](Args... args) mutable {
        try {
            if constexpr (std::is_void_v<return_type>) {
                f(args...);
                prom.set_value();
            } 
            else {
                prom.set_value(f(args...));
            }
        } catch (...) {
            prom.set_exception(std::current_exception());
        }
    }, std::forward<Args>(args)...).detach();

    return fut;
}

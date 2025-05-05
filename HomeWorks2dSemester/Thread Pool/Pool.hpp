#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <mutex>
#include <thread>
#include <queue>
#include <functional>
#include <future>
#include <condition_variable>
#include <vector>
#include <stdexcept>
#include <type_traits>

class thread_pool {
public:
    explicit thread_pool(std::size_t threads);
    ~thread_pool();

    template <typename Func, typename... Args>
    auto enqueue(Func&& func, Args&&... args) -> std::future<typename std::result_of<Func(Args...)>::type>;

    std::size_t hardware_capability() const;

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    const std::size_t cores_;

    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_;
};

// Constructor
thread_pool::thread_pool(std::size_t threads)
    : cores_(threads), stop_(false) {
    for (std::size_t i = 0; i < threads; ++i) {
        workers_.emplace_back([this] {
            while (true) {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex_);
                    this->condition_.wait(lock, [this] {
                        return this->stop_ || !this->tasks_.empty();
                    });

                    if (this->stop_ && this->tasks_.empty()){
                        return;
                    }
                    
                    task = std::move(this->tasks_.front());
                    this->tasks_.pop();
                }

                task();
            }
        });
    }
}

// Enqueue
template <typename Func, typename... Args>
auto thread_pool::enqueue(Func&& func, Args&&... args)
    -> std::future<typename std::result_of<Func(Args...)>::type> {

    using return_type = typename std::result_of<Func(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        if (stop_)
            throw std::runtime_error("enqueue on stopped thread_pool");

        tasks_.emplace([task]() { (*task)(); });
    }

    condition_.notify_one();
    return res;
}

// Destructor
thread_pool::~thread_pool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        stop_ = true;
    }
    
    condition_.notify_all();
    for (std::thread& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

std::size_t thread_pool::hardware_capability() const {
    return cores_;
}


#endif // THREAD_POOL_HPP

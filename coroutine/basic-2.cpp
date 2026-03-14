#include <coroutine>
#include <iostream>
#include <thread>


class Task1Driver {
public:
    struct promise_type {
        Task1Driver get_return_object() {
            return Task1Driver{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never initial_suspend() { return {}; }

        // retuning suspend_always requires user to destroy the coroutine object manually
        // returning suspend_never implies the coroutine object is destroyed automatically
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
    
    explicit Task1Driver(std::coroutine_handle<promise_type> h) : h(h) {}
    ~Task1Driver() { if (h) h.destroy(); }
    void resume() { h.resume(); }

private:
    std::coroutine_handle<promise_type> h;
};

struct IOReadAwaitable {
    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<> handle) {
        std::cout << "thread: " << std::this_thread::get_id() << ", " << "IOReadAwaitable.await_suspend" << std::endl;
        std::thread thread([this, handle]() {
            std::cout << "thread: " << std::this_thread::get_id() << ", " << "IOReadAwaitable.await_suspend IO thread starts" << std::endl;
            int work_time = 2;
            std::this_thread::sleep_for(std::chrono::seconds(work_time));
            bytes_ = 654321;
            std::cout << "thread: " << std::this_thread::get_id() << ", " << "IOReadAwaitable.await_suspend IO thread ends after " << work_time << " seconds, and reads " << bytes_ << " bytes" << std::endl;
            
            handle.resume();
        });
        thread.detach();
    }
    int await_resume() {
        std::cout << "thread: " << std::this_thread::get_id() << ", " << "IOReadAwaitable.await_resume" << std::endl;
        return bytes_;
    }

    int bytes_;
};

Task1Driver RunTask1() {
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "coroutine task1 starts, and co_awaits IOReadAwaitable to finish" << std::endl;
    int bytes = co_await IOReadAwaitable{};
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "coroutine task1 is resumed and reads " << bytes << " bytes via IOReadAwaitable" << std::endl;
    int work_time = 1;
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "coroutine task1 starts working on its own something for " << work_time << " seconds" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(work_time));
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "coroutine task1 finishes its own something after " << work_time << " seconds" << std::endl;
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "coroutine task1 end" << std::endl;
}

int main() {
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "main starts task1 " << std::endl;
    Task1Driver t1 = RunTask1();

    int work_time  = 2;
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "main starts working on something else for " << work_time << " seconds" << std::endl;
    // If this work_time is equal to that in IOReadAwaitable,
    //  then at this point, coroutine task1 is resumed in another thread, 
    //  and the following outputs may be interleaved with those of task1.
    std::this_thread::sleep_for(std::chrono::seconds(work_time));
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "main finishes working on something else after " << work_time << " seconds" << std::endl;
    
    // Extra waiting time is to avoid that main thread ends and destroys coroutine objects before worker thread ends
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "main ends" << std::endl;
}
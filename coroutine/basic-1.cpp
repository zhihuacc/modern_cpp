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

        // retuning suspend_always implies it requires user to destroy the coroutine object manually
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

struct IOWriteAwaitable {
    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<>) {
        std::cout << "thread: " << std::this_thread::get_id() << ", " << "IOWriteAwaitable.await_suspend" << std::endl;
        std::thread thread([]() {
            int work_time = 1;
            std::cout << "thread: " << std::this_thread::get_id() << ", " << "IOWriteAwaitable.await_suspend IO thread starts" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(work_time));
            std::cout << "thread: " << std::this_thread::get_id() << ", " << "IOWriteAwaitable.await_suspend IO thread ends after " << work_time << " seconds" << std::endl;
        });
        thread.detach();
    }
    void await_resume() {
        std::cout << "thread: " << std::this_thread::get_id() << ", " << "IOWriteAwaitable.await_resume" << std::endl;
    }
};

Task1Driver RunTask1() {
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "coroutine task1 starts, and co_awaits IOWriteAwaitable to finish" << std::endl;
    co_await IOWriteAwaitable{};
    int work_time = 2;
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "coroutine task1 starts doing its own something for " << work_time << " seconds" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(work_time));
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "coroutine task1 finishes its own something after " << work_time << " seconds" << std::endl;
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "coroutine task1 end" << std::endl;
}

int main() {
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "main starts task1 " << std::endl;
    Task1Driver t1 = RunTask1();

    int work_time = 3;
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "main starts working on something else for " << work_time << " seconds" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(work_time));
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "main finishes working on something else after " << work_time << " seconds, and resumes task1" << std::endl;
    t1.resume();

    work_time = 1;
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "main starts working on something else for " << work_time << " seconds" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(work_time));
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "main finishes working on something else after " << work_time << " seconds" << std::endl;
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "main end" << std::endl;
}
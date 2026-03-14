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

        // final_suspend() is called in any case, even when exceptions are thrown.
        std::suspend_always final_suspend() noexcept {
            std::cout << "thread: " << std::this_thread::get_id() << ", " << "Task1Driver.final_suspend" << std::endl;
            return {};
        }
        void return_void() {}
        // It's called if RunTask1() does not catch the exception
        void unhandled_exception() {
            exception_ = std::current_exception();
            if (exception_) {
                try {
                    std::rethrow_exception(exception_);
                } catch (const std::exception& e) {
                    std::cout << "thread: " << std::this_thread::get_id() << ", " << "Task1Driver.unhandled_exception: " << e.what() << std::endl;
                } catch (...) {
                    std::cout << "thread: " << std::this_thread::get_id() << ", " << "Task1Driver.unhandled_exception: unknown exception" << std::endl;
                }
            } else {
                std::cout << "thread: " << std::this_thread::get_id() << ", " << "Task1Driver.unhandled_exception: no exception" << std::endl;
            }
        }

        std::exception_ptr exception_;
    };
    
    explicit Task1Driver(std::coroutine_handle<promise_type> h) : h(h) {}
    ~Task1Driver() { if (h) h.destroy(); }
    void resume() { 
        promise_type& promise = h.promise();
        if (promise.exception_) std::rethrow_exception(promise.exception_);
        h.resume();
    }

private:
    std::coroutine_handle<promise_type> h;
    std::exception_ptr exception_;
};

struct IOReadAwaitable {
    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<> handle) {
        std::cout << "thread: " << std::this_thread::get_id() << ", " << "IOReadAwaitable.await_suspend" << std::endl;
        std::thread thread([this, handle]() {
            (void)handle;

            std::cout << "thread: " << std::this_thread::get_id() << ", " << "IOReadAwaitable.await_suspend IO thread starts" << std::endl;
            int work_time = 1;
            std::this_thread::sleep_for(std::chrono::seconds(work_time));
            // after this sleeping, the awaitable object has been destructed due to the exception thrown in await_suspend().
            // So the write to bytes_ is not safe
            bytes_ = 654321;
            std::cout << "thread: " << std::this_thread::get_id() << ", " << "IOReadAwaitable.await_suspend IO thread ends after " << work_time << " seconds, and reads " << bytes_ << " bytes" << std::endl;
            

            // This exception cannot be caught by the coroutine task1.
            // It means this exception will not be caught by try-catch block, and will terminate the process directly
            //  and hence no chance for unhandled_exception() to be called.
            // std::cout << "thread: " << std::this_thread::get_id() << ", " << "IOReadAwaitable worker thread throws an exception" << std::endl;
            // throw std::runtime_error("IOReadAwaitable worker thread's exception");
        });
        thread.detach();

        // This exception can be caught by the coroutine task1.
        // The exception unwinding will destruct the awaitable object.
        std::cout << "thread: " << std::this_thread::get_id() << ", " << "IOReadAwaitable.await_suspend throws an exception" << std::endl;  
        throw std::runtime_error("IOReadAwaitable.await_suspend's exception");
    }
    int await_resume() {
        std::cout << "thread: " << std::this_thread::get_id() << ", " << "IOReadAwaitable.await_resume" << std::endl;
        return bytes_;
    }

    int bytes_;
};

Task1Driver RunTask1() {
    // If the try-catch is enabled, the unhandled_exception() will not be called
    // try {
        std::cout << "thread: " << std::this_thread::get_id() << ", " << "coroutine task1 starts, and co_awaits IOReadAwaitable to finish" << std::endl;
        int bytes = co_await IOReadAwaitable{};
        std::cout << "thread: " << std::this_thread::get_id() << ", " << "coroutine task1 is resumed and reads " << bytes << " bytes via IOReadAwaitable" << std::endl;
        int work_time = 2;
        std::cout << "thread: " << std::this_thread::get_id() << ", " << "coroutine task1 starts working on its own something for " << work_time << " seconds" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(work_time));
        std::cout << "thread: " << std::this_thread::get_id() << ", " << "coroutine task1 finishes its own something after " << work_time << " seconds" << std::endl;
        std::cout << "thread: " << std::this_thread::get_id() << ", " << "coroutine task1 end" << std::endl;
    // } catch (const std::exception& e) {
    //     std::cout << "thread: " << std::this_thread::get_id() << ", " << "coroutine task1 catches an exception: " << e.what() << std::endl;
    // }
}

int main() {
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "main starts task1 " << std::endl;
    Task1Driver t1 = RunTask1();

   
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "thread: " << std::this_thread::get_id() << ", " << "main ends" << std::endl;
}
#ifndef QUEUE_TS_H
#define QUEUE_TS_H

#include <memory> // shared_ptr, unique_ptr
#include <mutex>  // mutex, lock_guard, unique_lock
#include <condition_variable>

template<typename T>
class queue_ts {
    private:
        struct node {
                std::shared_ptr<T>      data;
                std::unique_ptr<node>   next;  // each node can only be pointed by its parent

                node() = default;
                node(const T &value): data(std::make_shared<T>(value)) {}
        };

        std::unique_ptr<node>       head;
        std::mutex                  head_mutex;

        node                       *tail;
        std::mutex                  tail_mutex;

        std::condition_variable     data_cv;

        node *get_tail();
        // Pop from head without locking
        std::shared_ptr<T> pop_head();

    public:

        queue_ts();
        ~queue_ts();

        queue_ts(const queue_ts &) = delete;
        queue_ts &operator=(const queue_ts&) = delete;
        // Thread-safe push to tail
        void push(const T &value);
        // Thread-safe pop from head without blocking
        std::shared_ptr<T> try_pop();
        // Thread-safe pop from head
        std::shared_ptr<T> wait_pop();
        bool empty();
};

template<typename T>
queue_ts<T>::queue_ts(): head(std::make_unique<node>()), tail(head.get()) {}

template<typename T>
queue_ts<T>::~queue_ts() {
    
    std::unique_lock<std::mutex> lock(head_mutex);
    for (; head;) {
        head = std::move(head->next);
    }
}

template<typename T>
void queue_ts<T>::push(const T &value) {
    
    // Do expensive mem-alloc outside critical region
    std::shared_ptr<T> new_data = std::make_shared<T>(value);
    std::unique_ptr<node> new_tail = std::make_unique<node>();

    {
        // Lock tail to modify it
        std::lock_guard<std::mutex> lock(tail_mutex);
        tail->data = new_data;
        tail->next = std::move(new_tail);  //WARN: new_tail is set to nullptr by move()
        tail = tail->next.get();  //WARN: should not be: tail = new_tail.get(), since new_tail is set to nullptr by the above move
    }
    data_cv.notify_one();
}

// Extra keyword typename prior to queue_ts<T>::node is necessary
template<typename T>
typename queue_ts<T>::node *queue_ts<T>::get_tail() {

    std::lock_guard<std::mutex> lock(tail_mutex);
    return tail;
}

template<typename T>
std::shared_ptr<T> queue_ts<T>::pop_head() {

    std::shared_ptr<T> data = head->data;
    head = std::move(head->next);
    return data;
}

template<typename T>
std::shared_ptr<T> queue_ts<T>::try_pop() {

    std::lock_guard<std::mutex> head_lock(head_mutex);
    // Call function to get tail pointer to reduce holding time of tail_m
    if (head.get() == get_tail()) {
        // It's empty
        return std::make_shared<T>();
    }

    return pop_head();
}

template<typename T>
std::shared_ptr<T> queue_ts<T>::wait_pop() {

    std::unique_lock<std::mutex> head_lock(head_mutex);
    data_cv.wait(head_lock, [&]() {return head.get() != get_tail();});
    
    return pop_head();
}

template<typename T>
bool queue_ts<T>::empty() {

    std::lock_guard<std::mutex> lock(head_mutex);
    return head.get() == get_tail();
}

#endif
#ifndef _LIST_TS_H
#define _LIST_TS_H

#include <memory> // shared_ptr, unique_ptr
#include <mutex>  // mutex, lock_guard, unique_lock
#include <functional> // std::function


// Thread-safe singly-linked list
template <typename T>
class list_ts {

    private:
        struct node_ts {
            mutable std::mutex                  m;
            T                           data;
            std::unique_ptr<node_ts>    next;  // each node can only be pointed by its parent

            node_ts() = default;
            node_ts(const T &data): data(data) {}
            ~node_ts() = default;
        };
        node_ts head;
    
    public:
        list_ts() = default;
        list_ts(const list_ts &) = delete;   // Not allow to copy
        list_ts& operator=(const list_ts &) = delete; // Not allow to assign
        ~list_ts();

        void push_front(const T &data);
        
        // Read-only to the list. If need modify the list, use update_if()
        template<typename F>
        void for_each(F func) const;

        template<typename F>
        bool find_first_if(F p, T &data);

        template<typename F>
        int update_if(F p, const T &data, bool first_only = false);

        template<typename F>
        void remove_if(F p, bool first_only=false);

};


template <typename T>
list_ts<T>::~list_ts() {
    // WARN: Explicitly remove nodes. Otherwise, nodes will be destructed implicitly 
    //   by recursively calling destructor for each node 
    //   on stack, which causes stack overflow when the list is too long.
    remove_if([](auto i) {return true;});
}

template<typename T>
template<typename F>
void list_ts<T>::for_each(F func) const {

    std::unique_lock<std::mutex> curr_locker(head.m);
    for (node_ts *next = head.next.get(); next != nullptr; next = next->next.get()) {
        // Lock curr -> Lock next -> Unlock curr
        std::unique_lock<std::mutex> next_locker(next->m);
        curr_locker.unlock();

        func(next->data);

        curr_locker = std::move(next_locker);
    }
}

template <typename T>
void list_ts<T>::push_front(const T &data) {

    std::unique_ptr<node_ts> new_node = std::make_unique<node_ts>(data);

    std::lock_guard<std::mutex> locker(head.m);
    new_node->next = std::move(head.next);  // next is unique_ptr, so need std::move
    head.next = std::move(new_node);
}


template<typename T>
template<typename F>
bool list_ts<T>::find_first_if(F p, T &data) {

    std::unique_lock<std::mutex> curr_locker(head.m);
    for (node_ts *next = head.next.get(); next != nullptr; next = next->next.get()) {
        // Lock curr -> Lock next -> Unlock curr
        std::unique_lock<std::mutex> next_locker(next->m);
        curr_locker.unlock();

        if (p(next->data)) {

            data = next->data;
            return true;
        } 

        curr_locker = std::move(next_locker);
    }

    return false;
}

template<typename T>
template<typename F>
int list_ts<T>::update_if(F p, const T &data, bool first_only) {

    int updated = 0;
    // Try to find and update existing node
    std::unique_lock<std::mutex> curr_locker(head.m);
    for (node_ts *next = head.next.get(); next != nullptr; next = next->next.get()) {
        // Lock curr -> Lock next -> Unlock curr
        std::unique_lock<std::mutex> next_locker(next->m);
        curr_locker.unlock();

        if (p(next->data)) {
            // Found and update
            next->data = data;
            updated++;

            if (first_only)
                return updated;
        } 
        curr_locker = std::move(next_locker);
    }

    return updated;
}

template<typename T>
template<typename F>
void list_ts<T>::remove_if(F p, bool first_only) {
    
    std::unique_lock<std::mutex> curr_locker(head.m);
    // WARN: can't use next = next->next.get()
    for (node_ts *curr = &head, *next = head.next.get(); next != nullptr; next = curr->next.get()) {
        // Lock curr -> Lock next
        std::unique_lock<std::mutex> next_locker(next->m);
        
        if (p(next->data)) {

            // WARN: this statement is Necessory! It may segfault without this in case of concurrent remove_if.
            //  This may be bcas curr->next = std::move(next->next) would first destruct curr->next which 
            //  whould destroy curr->next.m. So calling next_locker.unlock would unlock a destroyed mutex which cause segfault.
            //  This statement avoids destruct curr->next before unlock current->next.m.
            std::unique_ptr<node_ts> old = std::move(curr->next);
            // WARN: this move() will set next->next = nullptr,
            //  so the iter expression in for-loop should NOT be next = next->next.get(),
            //  instead, use curr->next.get()
            curr->next = std::move(next->next); 
            // WARN: Necessory to unlock next.m even though next will be deleted, becas 
            next_locker.unlock();

            // WARN: Need make sure no threads are holding or waiting a mutex before destroying the mutex.
            //  Here the node pointed by next is held by current thread only, becas
            //  other threads are being blocked by curr->m and haven't had a chance to touch next->m
            //  so it's ok for current thread to destroy it after unlocking it.

            if (first_only) {
                return;
            }
        } else {
            // Unlock curr while holding next
            curr_locker.unlock();
            curr = next;
            curr_locker = std::move(next_locker);
        }
    }
}

#endif
/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** MessageBuffer
*/

#ifndef MESSAGEBUFFER_HPP_
#define MESSAGEBUFFER_HPP_

#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <queue>
#include <utility>

template<typename T>
class MessageBuffer {
private:
    static constexpr size_t MAX_SIZE = 10;
    std::queue<T> buffer_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;

public:
    bool push(const T& item) {
        std::scoped_lock lock{mutex_};
        if (buffer_.size() >= MAX_SIZE) {
            return false;
        }
        buffer_.push(item);
        cv_.notify_one();
        return true;
    }

    bool push(T&& item) {
        std::scoped_lock lock{mutex_};
        if (buffer_.size() >= MAX_SIZE) {
            return false;
        }
        buffer_.push(std::move(item));
        cv_.notify_one();
        return true;
    }

    bool tryPop(T& item) {
        std::scoped_lock lock{mutex_};
        if (buffer_.empty()) {
            return false;
        }
        item = buffer_.front();
        buffer_.pop();
        return true;
    }

    T pop() {
        std::unique_lock lock{mutex_};
        cv_.wait(lock, [this] { return !buffer_.empty(); });
        T item = buffer_.front();
        buffer_.pop();
        return item;
    }

    bool empty() const {
        std::scoped_lock lock{mutex_};
        return buffer_.empty();
    }

    size_t size() const {
        std::scoped_lock lock{mutex_};
        return buffer_.size();
    }

    // Put item back at front (for failed sends)
    bool pushFront(const T& item) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (buffer_.size() >= MAX_SIZE) {
            return false;
        }

        std::queue<T> temp;
        temp.push(item);
        while (!buffer_.empty()) {
            temp.push(buffer_.front());
            buffer_.pop();
        }

        buffer_ = std::move(temp);
        cv_.notify_one();
        return true;
    }
};

#endif /* !MESSAGEBUFFER_HPP_ */

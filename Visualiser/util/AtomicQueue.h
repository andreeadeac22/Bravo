#ifndef ATOMICQUEUE_H
#define ATOMICQUEUE_H

#include <mutex>
#include <condition_variable>
#include <queue>

template<class T>
class AtomicQueue
{
public:
    AtomicQueue() {}

    void push(T val) {
        mutex.lock();
        queue.push(val);
        mutex.unlock();

        cond.notify_one();
    }

    bool pop(T& val) {
        std::unique_lock<std::mutex> lk(mutex);
        cond.wait(lk);

        bool gotData = !queue.empty();

        if (gotData) {
            val = queue.front();
            queue.pop();
        }

        lk.unlock();

        return gotData;
    }

    bool pop_timeout(T& val, uint32_t timeout) {
        std::unique_lock<std::mutex> lk(mutex);
        cond.wait_for(lk, std::chrono::milliseconds(timeout));

        bool gotData = !queue.empty();

        if (gotData) {
            val = queue.front();
            queue.pop();
        }

        lk.unlock();

        return gotData;
    }

    void notifyAll() {
        cond.notify_all();
    }


private:
    std::queue<T> queue;

    mutable std::condition_variable cond;
    mutable std::mutex mutex;

};

#endif // ATOMICQUEUE_H

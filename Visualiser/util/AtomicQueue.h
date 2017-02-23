#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>


template<class T>
/**
 * @brief The AtomicQueue class provides a thread safe queue
 * interface.
 *  Pop operations are blocking; threads will wait until
 * data is available.
 */
class AtomicQueue
{
public:
    AtomicQueue() {}

    /**
     * @brief Push data to the queue
     * @param val
     */
    void push(T val) {
        mutex.lock();
        queue.push(val);
        mutex.unlock();

        cond.notify_one();
    }

    /**
     * @brief Pop data off the queue. Will block
     * until some data is available, or until the thread
     * is manually woken by a call to notifyAll()
     * @param val   Reference to value to store; use this to get pop'd value
     * @return  Whether data was pop'd, or the thread interrupted.
     */
    bool pop(T& val) {
        std::unique_lock<std::mutex> lk(mutex);

        if (queue.empty()) {
            cond.wait(lk);
        }

        bool gotData = !queue.empty();

        if (gotData) {
            val = queue.front();
            queue.pop();
        }

        lk.unlock();

        return gotData;
    }

    /**
     * @brief Pop with a timeout. Will block until either
     * (1) data is available
     * (2) the timeout expires
     * (3) the thread is interrupted.
     * @param val   Reference to value to store; use this to get pop'd value
     * @param timeout   Maximum time to block for until data is availale.
     * A timeout of 0 means the method will return immediately if no data
     * is available.
     * @return  Whether data was pop'd, or the thread interrupted/timed out
     */
    bool pop_timeout(T& val, uint32_t timeout) {
        std::unique_lock<std::mutex> lk(mutex);

        if (queue.empty()) {
            //No timeout means immediately exit
            if (timeout == 0U) {
                lk.unlock();
                return false;
            }

            cond.wait_for(lk, std::chrono::milliseconds(timeout));
        }

        bool gotData = !queue.empty();

        if (gotData) {
            val = queue.front();
            queue.pop();
        }

        lk.unlock();

        return gotData;
    }

    /**
     * @brief Wake all waiting threads, will cause
     * any threads waiting for data to return without data
     * (thus unblocking threads)
     */
    void notifyAll() {
        cond.notify_all();
    }


private:
    std::queue<T> queue;

    mutable std::condition_variable cond;
    mutable std::mutex mutex;

};

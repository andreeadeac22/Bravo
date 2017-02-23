#pragma once

#include <thread>
#include <vector>
#include <mutex>

#include "AtomicQueue.h"

#include "util/log.h"

template <class T>
/**
 * @brief AsyncJobHandler is an abstract class providing a base
 * to turn a class which handles small work units (jobs) into an
 * asynchronously operating class.
 *  Work units are defined by the programmer, as a template
 * of this class.
 *  Allows the programmer to set worker threads running in the background
 * to process jobs. The super class must implement the virtual
 * "process" method, to process each work unit.
 *  An AsyncJobHandler object can process jobs serially by calling
 * processNJobs(maxjobs) or asynchronous by threads spawned by calling
 * setThreadCount(N)
 */
class AsyncJobHandler {
    struct ThreadStateHandler {
        bool running;
    };

public:
    /**
     * @brief Initialise AsyncJobHandler
     * @param threadcount   Number of threads to start initially (default 0)
     */
    AsyncJobHandler(int threadcount = 0)
        : m_jobQueue(), m_threads(), m_threadStates()
    {
        setThreadCount(threadcount);
    }

    ~AsyncJobHandler() {
        //Has the effect of deleting all threads
        setThreadCount(0);
    }

    /**
     * @brief Set the number of worker threads to process these jobs.
     * Setting this to less than the current value will cause some threads
     * to be shut down.
     * @param threadcount   Number of threads
     */
    void setThreadCount(int threadcount);

    int getThreadCount() {
        return m_threads.size();
    }

    /**
     * @brief Trigger the processing of UP TO jobcount jobs, or however many jobs
     * are left if that is less.
     * @param jobcount  Max number of jobs to process.
     * @return  Number of jobs processed
     */
    int processNJobs(int jobcount);

    /**
     * Attempt to stop all threads
     */
    void stop();

    /**
     * @brief Enqueue a job for execution
     */
    void enqueueJob(T job) {
        m_jobQueue.push(job);
    }

    /**
     * @brief Process a job stored as type T in argument 0
     */
    virtual void process(T) = 0;

private:
    AtomicQueue<T> m_jobQueue;

    std::vector<std::thread*> m_threads;

    //Stores the state of the thread, allowing interaction
    std::vector<ThreadStateHandler*> m_threadStates;

    mutable std::mutex m_threadManMutex;

    void run_thread(ThreadStateHandler* threadState);

};

template <class T>
void AsyncJobHandler<T>::setThreadCount(int threadcount)
{
    std::unique_lock<std::mutex> lk(m_threadManMutex);

    if (threadcount == m_threads.size()) {
        return;
    } else if (threadcount > m_threads.size()) {
        //Spawn a number of threads
        int spawnCount = threadcount - m_threads.size();

        for (int i = 0; i < spawnCount; i++) {
            m_threadStates.push_back(new ThreadStateHandler { true });
            m_threads.push_back(new std::thread(&AsyncJobHandler<T>::run_thread, this, m_threadStates.back()));
        }
    } else {
        //Kill a number of threads
        int killCount = m_threads.size() - threadcount;

        if (killCount < 0)
            killCount = 0;

        for (int i = 0; i < killCount; i++) {
            //Signal a thread to stop
            m_threadStates.back()->running = false;

            //Free the thread by detaching it (will be auto cleaned up)
            m_threads.back()->detach();
            delete m_threads.back();

            //Remove the thread reference
            m_threadStates.pop_back();
            m_threads.pop_back();
        }
    }
}

template <class T>
int AsyncJobHandler<T>::processNJobs(int jobcount)
{
    int i;

    for (i = 0; i < jobcount; i++) {
        T val;

        if (m_jobQueue.pop_timeout(val, 0U)) {
            process(val);
        } else {
            //No data, exit
            break;
        }
    }

    return i;
}

template <class T>
void AsyncJobHandler<T>::stop()
{
    //Setting thread count to 0 has effect of asking threads to stop and detaching
    setThreadCount(0);

    //Notifying waiting threads prevents threads being stuck in lock
    m_jobQueue.notifyAll();
}

template <class T>
void AsyncJobHandler<T>::run_thread(ThreadStateHandler* threadState)
{
    log_info << "AsyncJobHandler [" << this << "] worker thread starting." << std::endl;

    T val;

    while (threadState->running && m_jobQueue.pop(val)) {
        process(val);
    }

    log_info << "AsyncJobHandler [" << this << "] worker thread exiting." << std::endl;

    //Must delete thread state on exit
    delete threadState;
}

#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <functional>

#include "AtomicQueue.h"

#include "util/log.h"

template <class Worker, class Job>
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
    template <class... Args>
    AsyncJobHandler(int workerCount, Args... args);

    ~AsyncJobHandler() {
        stop();
    }

    /**
     * Attempt to stop all threads
     */
    void stop();

    /**
     * @brief Enqueue a job for execution
     */
    void enqueueJob(Job job) {
        jobQueue.push(job);
    }

private:
    AtomicQueue<Job> jobQueue;

    std::vector<std::thread*> threads;

    //Stores the state of the thread, allowing interaction
    std::vector<ThreadStateHandler*> threadStates;

    template <class... Args>
    void spawnWorker(Args... args);

    static void run_thread(AtomicQueue<Job> *jobQueue, ThreadStateHandler* state, Worker *worker);

};

template <class Worker, class Job>
template <class... Args>
AsyncJobHandler<Worker, Job>::AsyncJobHandler(int workerCount, Args... args)
{
    for (int i = 0; i < workerCount; i++) {
        spawnWorker(args...);
    }
}

template <class Worker, class Job>
void AsyncJobHandler<Worker, Job>::stop()
{
    for (int i = 0; i < threads.size(); i++) {
        //Signal a thread to stop
        threadStates[i]->running = false;

        //Free the thread by detaching it (will be auto cleaned up)
        threads[i]->detach();
        delete threads[i];
    }
}

template <class Worker, class Job>
template <class... Args>
void AsyncJobHandler<Worker, Job>::spawnWorker(Args... args)
{
    Worker* worker = new Worker(args...);
    ThreadStateHandler* threadState = new ThreadStateHandler { true };
    threadStates.push_back(threadState);
    threads.push_back(new std::thread(AsyncJobHandler::run_thread, &jobQueue, threadState, worker));
}

template <class Worker, class Job>
void AsyncJobHandler<Worker, Job>::run_thread(AtomicQueue<Job> *jobQueue, ThreadStateHandler *state, Worker *worker)
{
    log_info << "AsyncJobHandler [" << worker << "] worker thread starting." << std::endl;

    Job job;

    while (state->running && jobQueue->pop(job)) {
        worker->process(job);
    }

    log_info << "AsyncJobHandler [" << worker << "] worker thread exiting." << std::endl;

    //Must delete thread state on exit
    delete state;
    delete worker;
}

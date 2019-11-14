//
// Created by shuru on 10/27/2019.
//

#include "JobQueue.h"
#include "JobSystem.h"


JobQueue::JobQueue()
{
    jobRingBuffer = new Job* [MAX_JOB_COUNT];
    memset(jobRingBuffer, 0, sizeof(Job*) * MAX_JOB_COUNT);
    front = 0;
    back = 0;
}

JobQueue::~JobQueue()
{
    delete[] jobRingBuffer;
}

bool JobQueue::Pop(Job** job)
{
    int64_t b = back.load(boost::memory_order_relaxed) - 1;
    back.store(b, boost::memory_order_relaxed);
    boost::atomic_thread_fence(boost::memory_order_seq_cst);
    int64_t f = front.load(boost::memory_order_relaxed);

    bool result = false;
    if (f <= b)
    {
        *job = jobRingBuffer[b % MAX_JOB_COUNT];
        result = true;
        // if no more than one item left in the queue
        if (f == b)
        {
            if (!front.compare_exchange_strong(f, f + 1,
                                               boost::memory_order_seq_cst,
                                               boost::memory_order_relaxed))
            {
                // failed race
                result = false;
            }
            back.store(b + 1, boost::memory_order_relaxed);
        }
    }
    else
    {
        // empty queue
        back.store(b + 1, boost::memory_order_relaxed);
        result = false;
    }
    return result;
}

bool JobQueue::Push(Job* job)
{
    int64_t b = back.load(boost::memory_order_relaxed);
    int64_t f = front.load(boost::memory_order_acquire);

    if (b - f > MAX_JOB_COUNT - 1)
    {
        // Full queue
        return false;
    }

    jobRingBuffer[b % MAX_JOB_COUNT] = job;
    boost::atomic_thread_fence(boost::memory_order_release);
    back.store(b + 1, boost::memory_order_relaxed);  //store
    return true;
}

bool JobQueue::Steal(Job** job)
{
    int64_t f = front.load(boost::memory_order_acquire); // load
    boost::atomic_thread_fence(boost::memory_order_seq_cst);
    int64_t b = back.load(boost::memory_order_acquire); // load

    if (f >= b)
    {
        return false;
    }

    *job = jobRingBuffer[f % MAX_JOB_COUNT];

    if (!front.compare_exchange_strong(f, f + 1,
                                       boost::memory_order_seq_cst,
                                       boost::memory_order_relaxed))
    {
        // failed race
        return false;
    }
    return true;
}

bool JobQueue::Empty()
{
    return front == back;
}

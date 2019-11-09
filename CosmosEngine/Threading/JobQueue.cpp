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

Job* JobQueue::Pop()
{
    int64_t b = back.load(boost::memory_order_acquire);
    --b;
    back.store(b, boost::memory_order_release);
    int64_t f = front.load(boost::memory_order_acquire);

    if (f <= b)
    {
        Job* job = jobRingBuffer[b % MAX_JOB_COUNT];

        // if there's still more than one item left in the queue
        if (f != b)
        {
            return job;
        }

        // if this is the last item in the queue
        // we test if we won the race against a concurrent steal operation
        if (!front.compare_exchange_weak(f, f + 1, boost::memory_order_acq_rel))
        {
            // we lost, job stolen and return nullptr
            job = nullptr;
        }

        // no matter we won (the last job stolen) or lost (the last job poped)
        // the queue is empty, so we set back = top
        back.store(f + 1, boost::memory_order_release);
        return job;
    }
    else
    {
        back.store(f, boost::memory_order_release);
        return nullptr;
    }
}

void JobQueue::Push(Job* job)
{
    int64_t b = back.load(boost::memory_order_acquire);

    jobRingBuffer[b % MAX_JOB_COUNT] = job; // store

    ++b;

    back.store(b, boost::memory_order_release);  //store
}

Job* JobQueue::Steal()
{
    int64_t f = front.load(boost::memory_order_acquire); // load
    int64_t b = back.load(boost::memory_order_acquire); // load

    if (f == b)
    {
        return nullptr;
    }
    else
    {
        Job* job = jobRingBuffer[f % MAX_JOB_COUNT];
        if (!front.compare_exchange_weak(f, f + 1, boost::memory_order_acq_rel))
        {
            // a concurrent steal or pop operation removed an element from the deque in the meantime.
            return nullptr;
        }
        return job;
    }
}

bool JobQueue::Empty()
{
    return front == back;
}

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
    bool b_overflow = false;
    int b = back.load(boost::memory_order_acquire);
    if (b < 0)
    {
        b = MAX_JOB_COUNT - 1;
        b_overflow = true;
    }
    back.store(b, boost::memory_order_release);
    int f = front.load(boost::memory_order_acquire);

    if (f <= b || (f != b && b_overflow))
    {
        Job* job = jobRingBuffer[b];

        // if there's still more than one item left in the queue
        if (f != b)
        {
            return job;
        }

        // if this is the last item in the queue
        int f_desired = ((f + 1) == MAX_JOB_COUNT ? 0 : (f + 1));
        // we test if we won the race against a concurrent steal operation
        if (!front.compare_exchange_weak(f, f_desired, boost::memory_order_acq_rel))
        {
            // we lost, job stolen and return nullptr
            job = nullptr;
        }

        // no matter we won (the last job stolen) or lost (the last job poped)
        // the queue is empty, so we set back = top
        back.store(f_desired, boost::memory_order_release);
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
    int b = back.load(boost::memory_order_acquire);

    jobRingBuffer[b] = job; // store

    ++b;
    if (b == MAX_JOB_COUNT) b = 0;
    back.store(b, boost::memory_order_release);  //store
}

Job* JobQueue::Steal()
{
    int f = front.load(boost::memory_order_acquire); // load
    int b = back.load(boost::memory_order_acquire); // load

    if (f == b)
    {
        return nullptr;
    }
    else
    {
        Job* job = jobRingBuffer[f];
        int f_desired = ((f + 1) == MAX_JOB_COUNT ? 0 : (f + 1));
        if (!front.compare_exchange_weak(f, f_desired, boost::memory_order_acq_rel))
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

//
// Created by shuru on 10/27/2019.
//

#ifndef GAMEENGINE_JOBQUEUE_H
#define GAMEENGINE_JOBQUEUE_H

#include <boost/atomic.hpp>

struct Job;

struct JobQueue
{
    Job** jobRingBuffer;
    boost::atomic_int64_t front;
    boost::atomic_int64_t back;

    JobQueue();

    ~JobQueue();

    bool Pop(Job** job);

    bool Push(Job* job);

    bool Steal(Job** job);

    bool Empty();
};

#endif //GAMEENGINE_JOBQUEUE_H

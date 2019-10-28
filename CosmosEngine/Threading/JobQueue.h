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
    boost::atomic<int> front;
    boost::atomic<int> back;

    JobQueue();

    ~JobQueue();

    Job* Pop();

    void Push(Job* job);

    Job* Steal();

    bool Empty();
};

#endif //GAMEENGINE_JOBQUEUE_H

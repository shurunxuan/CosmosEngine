//
// Created by shuru on 10/27/2019.
//

#ifndef GAMEENGINE_JOBSYSTEM_H
#define GAMEENGINE_JOBSYSTEM_H

#include "../Export.h"
#include "JobQueue.h"

#include <boost/function.hpp>
#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include <boost/container/vector.hpp>
#include <boost/container/map.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

struct Job;

typedef boost::function<void(Job*)> JobFunction;

constexpr uint64_t MAX_JOB_COUNT = (1u << (sizeof(uint16_t) * 8 + 2)) /*- 1*/;

struct ENGINE_API Job
{
public:
    Job();

    JobFunction function;
    Job* parent;
    boost::atomic<int32_t> unfinishedJobs; // atomic
    char data[128];
};

class ENGINE_API JobSystem
{
public:
    JobSystem();

    ~JobSystem();

    void StartUp();

    void Shutdown();

    Job* CreateJob(JobFunction function);

    Job* CreateJobAsChild(Job* parent, JobFunction function);

    Job* GetJob();

    void Run(Job* job);

    void Execute(Job* job);

    void Wait(Job* job);

    void Finish(Job* job);

    boost::thread::id GetRandomThreadID();

private:
    unsigned int concurrentThreadsSupported;
    boost::container::map<boost::thread::id, JobQueue*> queueMap;
    boost::container::vector<JobQueue*> queueVec;
    boost::container::vector<boost::thread> workerThreads;
    boost::thread::id mainThreadID;
    boost::condition_variable queueNotEmpty;
    boost::mutex queueEmptyMutex;

    boost::random::random_device* rd;  //Will be used to obtain a seed for the random number engine
    boost::random::mt19937* gen; //Standard mersenne_twister_engine seeded with rd()
    boost::random::uniform_int_distribution<>* dis;

    boost::atomic<bool> active;

    Job* jobRingBuffer;
    boost::atomic<uint32_t> allocatedJobs;

    Job* AllocateJob();
};

ENGINE_EXTERNAL_VAR JobSystem* jobSystem;

#endif //GAMEENGINE_JOBSYSTEM_H

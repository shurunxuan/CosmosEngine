//
// Created by shuru on 10/27/2019.
//

#include "JobSystem.h"

#include "../Logging/Logging.h"

Job::Job()
{
    parent = nullptr;
    unfinishedJobs = 0;
    memset(data, 0, sizeof(data));
}

JobSystem::JobSystem()
{
    concurrentThreadsSupported = boost::thread::hardware_concurrency();
    if (concurrentThreadsSupported < 4) concurrentThreadsSupported = 4;

    workerThreads.reserve(concurrentThreadsSupported);

    rd = new boost::random::random_device();
    gen = new boost::random::mt19937((*rd)());
    dis = new boost::random::uniform_int_distribution<>(0, concurrentThreadsSupported - 1);

    mainThreadID = boost::this_thread::get_id();

    jobRingBuffer = new Job[MAX_JOB_COUNT];
    allocatedJobs = 0;

    active = false;
}

JobSystem::~JobSystem()
{
    active = false;

    delete[] jobRingBuffer;

    for (auto& i : queueVec)
    {
        delete i;
    }

    delete rd;
    delete gen;
    delete dis;
}

void JobSystem::StartUp()
{
    queueVec.reserve(concurrentThreadsSupported);
    for (unsigned int i = 0; i < concurrentThreadsSupported; ++i)
    {
        queueVec.push_back(new JobQueue);
    }

    auto mainThreadID = boost::this_thread::get_id();

    queueMap[mainThreadID] = queueVec[0];

    active = true;

    boost::function<void(JobQueue*)> workerThreadFunction = [this](JobQueue* assignedQueue)
    {
        const auto threadID = boost::this_thread::get_id();

        queueMap[threadID] = assignedQueue;

        while (active)
        {
            Job* job = GetJob();
            if (job)
            {
                Execute(job);
            }

            boost::unique_lock<boost::mutex> l(queueEmptyMutex);
            queueNotEmpty.wait(l, [this]()
            {
                bool allEmpty = true;
                for (auto queue : queueVec)
                {
                    allEmpty &= queue->Empty();
                }

                return !(allEmpty && active);
            });
        }

    };

    for (unsigned int id = 1; id < concurrentThreadsSupported; ++id)
    {
        boost::function<void()> func = boost::bind(workerThreadFunction, queueVec[id]);
        workerThreads.emplace_back(func);
    }

    LOG_INFO << "Job System start up completed with " << workerThreads.size() << " additional worker threads";
}

void JobSystem::Shutdown()
{
    active = false;
    queueNotEmpty.notify_all();
    for (auto& thread : workerThreads)
    {
        thread.join();
    }

    LOG_INFO << "Job System Shutdown completed";
}

Job* JobSystem::CreateJob(JobFunction function)
{
    Job* job = AllocateJob();
    job->function = std::move(function);
    job->parent = nullptr;
    job->unfinishedJobs = 1;

    return job;
}

Job* JobSystem::CreateJobAsChild(Job* parent, JobFunction function)
{
    ++parent->unfinishedJobs;

    Job* job = AllocateJob();
    job->function = std::move(function);
    job->parent = parent;
    job->unfinishedJobs = 1;

    return job;
}

Job* JobSystem::GetJob()
{
    auto threadID = boost::this_thread::get_id();
    JobQueue* threadQueue = queueMap[threadID];

    if (threadQueue != nullptr)
    {
        Job* job = threadQueue->Pop();
        if (job != nullptr) return job;
    }

    boost::thread::id otherThreadID = GetRandomThreadID();

    if (threadID == otherThreadID)
    {
        // don't try to steal from ourselves
        boost::this_thread::yield();
        return nullptr;
    }

    JobQueue* stealQueue = queueMap[otherThreadID];
    if (stealQueue == nullptr)
    {
        boost::this_thread::yield();
        return nullptr;
    }
    Job* stolenJob = stealQueue->Steal();
    if (stolenJob == nullptr)
    {
        // we couldn't steal a job from the other queue either, so we just yield our time slice for now
        boost::this_thread::yield();
        return nullptr;
    }

    return stolenJob;
}

void JobSystem::Run(Job* job)
{
    JobQueue* threadQueue = queueMap[boost::this_thread::get_id()];
    while (threadQueue == nullptr) threadQueue = queueMap[boost::this_thread::get_id()];
    //WorkStealingQueue* threadQueue = queueMap[GetRandomThreadID()];
    //while (threadQueue == nullptr) threadQueue = queueMap[GetRandomThreadID()];
    threadQueue->Push(job);
    queueNotEmpty.notify_all();
}

void JobSystem::Execute(Job* job)
{
    (job->function)(job);
    Finish(job);
}

void JobSystem::Wait(Job* job)
{
    while (job->unfinishedJobs.load() > 0)
    {
        Job* job1 = GetJob();
        if (job1)
        {
            Execute(job1);
        }
    }
}

void JobSystem::Finish(Job* job)
{
    int32_t unfinishedJobs = --job->unfinishedJobs;

    if ((unfinishedJobs == 0) && (job->parent))
    {
        Finish(job->parent);
    }
}

boost::thread::id JobSystem::GetRandomThreadID()
{
    size_t randomIndex = (*dis)(*gen);

    if (randomIndex == concurrentThreadsSupported - 1)
        return mainThreadID;

    while (randomIndex >= workerThreads.size())
    {
        randomIndex = (*dis)(*gen);
    }
    return workerThreads[randomIndex].get_id();
}

Job* JobSystem::AllocateJob()
{
    const uint32_t index = allocatedJobs++;
    return &jobRingBuffer[index % MAX_JOB_COUNT];
}

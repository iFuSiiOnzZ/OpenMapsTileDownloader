#pragma once
#include <list>
#include <pthread.h>

#include "./Task.h"
#include "./Utilities/Mutex.h"
#include "./Utilities/ContitionVariable.h"

class CPool
{
    private:
        std::list<CTask *>  m_pTasks;
        pthread_t              *m_pThreadsHandle;

        unsigned int        m_NumOfTreads;
        bool                m_ThreadRun;

        CMutex              m_Mutex;
        CConditionVarible   m_CondVar;
        CConditionVarible   m_CondVarTaskFinished;

    public:
        CPool  (void);
        ~CPool (void);

        void                Init            (unsigned int l_NumThreads = 0);
        void                AddTask         (CTask *l_pTask);

        void                ThreadStop      (void);
        void                WaitForWorkers  (void);

        void                MainThread     (void);
        static void        *ThreadStart    (void *l_pParam);
};
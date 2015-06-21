#include <malloc.h>
#include "./Pool.h"

CPool::CPool()
{
    m_pThreadsHandle = 0;
    m_ThreadRun = true;
}

CPool::~CPool(void)
{
    if(m_pThreadsHandle == 0) return;

    m_ThreadRun = false;
    m_pTasks.clear();

    m_CondVar.WakeAll();
    m_CondVarTaskFinished.WakeAll();

    for(int i = 0; i < m_NumOfTreads; ++i)
    {
        pthread_join(*(m_pThreadsHandle + i), 0);
    }

    free(m_pThreadsHandle);
    m_pThreadsHandle = 0;
}

void CPool::Init(unsigned int l_NumThreads)
{
    if(l_NumThreads == 0) return;

    m_NumOfTreads = l_NumThreads;
    m_pThreadsHandle = (pthread_t *) malloc (m_NumOfTreads * sizeof(pthread_t));

    for(unsigned int i = 0; i < m_NumOfTreads; ++i)
    {
         if(pthread_create((m_pThreadsHandle + i), 0, &CPool::ThreadStart, (void *) this) < 0)
         {
             printf("ERROR: Thread could not be created\n");
         }
    }
}

void CPool::AddTask(CTask *l_pTask)
{
    m_Mutex.Lock();
        m_pTasks.push_back(l_pTask);
    m_Mutex.UnLock();

    m_CondVar.Wake();
}

void CPool::ThreadStop(void)
{
    m_ThreadRun = false;
}

void *CPool::ThreadStart(void *l_pParam)
{
    CPool *refToThis = (CPool *) l_pParam;
    refToThis->MainThread();

    return 0;
}

void CPool::MainThread(void)
{
    while(m_ThreadRun)
    {
        CTask *l_pTask = 0;

        m_Mutex.Lock();
            while(m_pTasks.empty() && m_ThreadRun) m_CondVar.Sleep(m_Mutex.m_CriticalSection);
            if(!m_ThreadRun){ m_Mutex.UnLock(); return; }

             l_pTask = m_pTasks.front();
             m_pTasks.pop_front();
        m_Mutex.UnLock();

        CTask *l_pDependencyTask = l_pTask->GetDependency();
        if(l_pDependencyTask != 0) l_pDependencyTask->Execute();

        l_pTask->Execute();
        m_CondVarTaskFinished.Wake();
    }

    pthread_exit(0);
}

void CPool::WaitForWorkers(void)
{
    m_Mutex.Lock();
        while(!m_pTasks.empty()) m_CondVarTaskFinished.Sleep(m_Mutex.m_CriticalSection);
    m_Mutex.UnLock();
}
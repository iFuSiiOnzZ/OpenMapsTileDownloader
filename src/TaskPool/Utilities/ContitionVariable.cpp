#include "./ContitionVariable.h"

CConditionVarible::CConditionVarible(unsigned int l_NumThreads)
{
    pthread_cond_init(&m_ConditionVariable, 0);
}

CConditionVarible::~CConditionVarible()
{
    pthread_cond_destroy(&m_ConditionVariable);
}

void CConditionVarible::Sleep(pthread_mutex_t &l_CriticalSenction)
{
    pthread_cond_wait(&m_ConditionVariable, &l_CriticalSenction);
}

void CConditionVarible::Wake(void)
{
    pthread_cond_signal(&m_ConditionVariable);
}

void CConditionVarible::WakeAll(void)
{
    pthread_cond_broadcast(&m_ConditionVariable);
}
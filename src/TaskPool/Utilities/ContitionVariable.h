#pragma once
#include <pthread.h>

class CConditionVarible
{
    public:
        CConditionVarible  (unsigned int l_NumThreads);
        ~CConditionVarible (void);

        void Sleep   (pthread_mutex_t &l_CriticalSenction);

        void Wake    (void);
        void WakeAll (void);

        pthread_cond_t   m_ConditionVariable;
};

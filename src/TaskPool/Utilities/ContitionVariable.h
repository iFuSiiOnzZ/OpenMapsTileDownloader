#pragma once
#include <pthread.h>

class CConditionVarible
{
    public:
        CConditionVarible  (void);
        ~CConditionVarible (void);

        void Sleep   (pthread_mutex_t &l_CriticalSenction);

        void Wake    (void);
        void WakeAll (void);

        pthread_cond_t   m_ConditionVariable;
};

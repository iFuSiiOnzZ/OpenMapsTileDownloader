#pragma once
#include <pthread.h>

class CMutex
{
    public:
        CMutex  (void);
        ~CMutex (void);

        void Lock   (void);
        void UnLock (void);

        pthread_mutex_t    m_CriticalSection;
};

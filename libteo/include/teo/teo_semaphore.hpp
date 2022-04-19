#ifndef TEO_TEO_SEMAPHORE_HPP
#define TEO_TEO_SEMAPHORE_HPP

// Source: https://sodocumentation.net/cplusplus/topic/9785/semaphore

#include <mutex>
#include <condition_variable>

namespace teo
{
    class Semaphore
    {
    public:
        Semaphore(int count_ = 0)
            : count(count_)
        {
        }

        inline void notify()
        {
            std::unique_lock<std::mutex> lock(mtx);
            count++;
            //notify the waiting thread
            cv.notify_one();
        }
        inline void wait()
        {
            std::unique_lock<std::mutex> lock(mtx);
            while (count == 0)
            {
                //wait on the mutex until notify is called
                cv.wait(lock);
            }
            count--;
        }

    private:
        std::mutex mtx;
        std::condition_variable cv;
        int count;
    };
}

#endif
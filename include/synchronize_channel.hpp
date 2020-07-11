#ifndef SYNCHRONIZE_CHANNEL_HPP_
#define SYNCHRONIZE_CHANNEL_HPP_

#include <mutex>
#include <condition_variable>

class SynchronizeChannel
{
public:
    void wait()
    {
        std::unique_lock<std::mutex> lock(mutex_);

        cv_.wait(lock);
    }

    void notify_all()
    {
        std::unique_lock<std::mutex> lock(mutex_);

        cv_.notify_all();
    }
private:
    std::mutex mutex_;
    std::condition_variable cv_;
};

#endif // SYNCHRONIZE_CHANNEL_HPP_
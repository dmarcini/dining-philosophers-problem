#ifndef FORK_HPP_
#define FORK_HPP_

#include <mutex>
#include <string>

#include "synchronize_channel.hpp"

class Fork
{
public:
    enum class Status
    {
        Clean,
        Dirty
    };

    Fork(uint owner_id);

    Fork(const Fork &fork);
    Fork(Fork &&fork);

    Fork& operator=(const Fork&) = delete;
    Fork& operator=(Fork&&) = delete;

    ~Fork();

    void request(uint requester_id);
    void free();

    std::mutex& mutex();
    std::string status() const;
private:
    uint next_id();

    uint id_ {next_id()};
    Status status_ {Status::Dirty};

    uint owner_id_;
    SynchronizeChannel synchronize_channel_;

    std::mutex mutex_;
};

#endif // FORK_HPP_
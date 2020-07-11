#include "fork.hpp"

#include <mutex>
#include <string>
#include <utility>


Fork::Fork(uint owner_id)
    : owner_id_(owner_id) {}


Fork::Fork(const Fork &fork)
    : id_(fork.id_), status_(fork.status_), owner_id_(fork.owner_id_) {}


Fork::Fork(Fork &&fork)
    : id_(fork.id_), status_(fork.status_), owner_id_(fork.owner_id_) {}


Fork::~Fork()
{
    free();
}


void Fork::request(uint requester_id)
{
    while (owner_id_ != requester_id) {
        if (status_ == Status::Dirty) {
            std::lock_guard<std::mutex> lock(mutex_);

            status_ = Status::Clean;
            owner_id_ = requester_id;
        }
        else {
            synchronize_channel_.wait();
        }
    }
}


void Fork::free()
{
    status_ = Status::Dirty;

    synchronize_channel_.notify_all();
}


std::mutex& Fork::mutex()
{
    return mutex_;
}


std::string Fork::status() const
{
    switch (status_) {
        case Status::Clean:
            return "FORK:" + std::to_string(id_ + 1) + " <= PHILOSOPHER:"
                   + std::to_string(owner_id_ + 1) + " | Clean";
        case Status::Dirty:
            return "FORK:" + std::to_string(id_ + 1) + " <= PHILOSOPHER:"
                   + std::to_string(owner_id_ + 1) + " | Dirty";
        default:
            return "";
    }
}


uint Fork::next_id()
{
    static uint id {0};

    return id++;
}

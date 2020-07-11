#ifndef TABLE_SETUP_HPP_
#define TABLE_SETUP_HPP_

#include <atomic>

#include "synchronize_channel.hpp"

struct TableSetup
{
    SynchronizeChannel synchronize_channel;
    std::atomic<bool> dinner_finished {false};
};

#endif // TABLE_SETUP_HPP_
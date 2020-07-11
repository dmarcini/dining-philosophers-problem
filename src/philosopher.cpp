#include "philosopher.hpp"

#include <thread>
#include <mutex>
#include <string>
#include <utility>
#include <random>

#include "fork.hpp"
#include "table_setup.hpp"
#include "visualisation.hpp"


Philosopher::Philosopher(Fork &left_fork, Fork &right_fork,
                         TableSetup &table_setup, Visualisation &visualisation)
    : left_fork_(left_fork), right_fork_(right_fork), table_setup_(table_setup),
      visualisation_(visualisation), thread_(&Philosopher::start, this) {}


Philosopher::Philosopher(const Philosopher &philosopher)
    : left_fork_(philosopher.left_fork_), right_fork_(philosopher.right_fork_),
      table_setup_(philosopher.table_setup_),
      visualisation_(philosopher.visualisation_) {}


Philosopher::Philosopher(Philosopher &&philosopher)
    : left_fork_(philosopher.left_fork_), right_fork_(philosopher.right_fork_),
      table_setup_(philosopher.table_setup_),
      visualisation_(philosopher.visualisation_) {}


Philosopher::~Philosopher()
{
    if (thread_.joinable()) {
        thread_.detach();
    }
}


std::string Philosopher::id() const
{
    return "PHILOSOPHER:" + std::to_string(id_ + 1);
}


std::string Philosopher::status() const
{
    switch (status_) {
        case Status::Eating:
            return "Eating                ";
        case Status::Requesting:
            return "Waiting for forks     ";
        case Status::Thinking:
            return "Thinking              ";
        case Status::Finished:
            return "Finished              ";
        default:
            return "                      ";
    }
}


std::string Philosopher::progress() const
{
    int current {progress_};
    int space {25 - current - 1};
    std::string bar {"["};

    while (current--) {
        bar += "#";
    }

    while (space--) {
        bar += " ";
    }

    return bar += "]";
}


void Philosopher::start()
{
    table_setup_.synchronize_channel.wait();

    do {
        think();
        eat();
    } while(!table_setup_.dinner_finished);

    change_status(Status::Finished);

    free_forks();
}


void Philosopher::think()
{
    change_status(Status::Thinking);

    wait();
}


void Philosopher::eat()
{
    request_forks();

    std::lock(left_fork_.mutex(), right_fork_.mutex());

    std::lock_guard<std::mutex> left_fork_lock(left_fork_.mutex(),
                                               std::adopt_lock);
    std::lock_guard<std::mutex> right_fork_lock(right_fork_.mutex(),
                                                std::adopt_lock);

    change_status(Status::Eating);
    wait();
    free_forks();
}


void Philosopher::request_forks()
{
    change_status(Status::Requesting);

    left_fork_.request(id_);
    right_fork_.request(id_);
}


void Philosopher::free_forks()
{
    left_fork_.free();
    right_fork_.free();
}


void Philosopher::wait()
{
    static auto random_time = []() -> int {
        constexpr int min_time {50};
        constexpr int interval {151};

        return (rand() % interval) + min_time;
    };

    for(progress_ = 0; progress_ < 25; ++progress_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(random_time()));

        visualisation_.update_table_info(id_, id(), status(), finished());
        visualisation_.update_progress(id_, progress());
    }
}


void Philosopher::change_status(Philosopher::Status new_status)
{
    status_ = new_status;

    visualisation_.update_table_info(id_, id(), status(), finished());
}


bool Philosopher::finished() const
{
    return status_ == Status::Finished;
}


uint Philosopher::next_id()
{
    static uint id {0};

    return id++;
}

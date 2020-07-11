#ifndef PHILOSOPHER_HPP_
#define PHILOSOPHER_HPP_

#include <thread>
#include <chrono>

#include "fork.hpp"
#include "table_setup.hpp"
#include "visualisation.hpp"

class Philosopher
{
public:
    enum class Status
    {
        Thinking,
        Requesting,
        Eating,
        Finished
    };

    Philosopher(Fork &left_fork, Fork &right_fork,
                TableSetup &table_setup, Visualisation &visualisation);

    Philosopher(const Philosopher &philosopher);
    Philosopher(Philosopher &&philosopher);

    Philosopher& operator=(const Philosopher&) = delete;
    Philosopher& operator=(Philosopher&&) = delete;

    ~Philosopher();

    bool finished() const;

    std::string id() const;
    std::string progress() const ;
    std::string status() const;
private:
    void start();

    void think();
    void eat();
    void request_forks();
    void free_forks();
    void wait();
    void change_status(Status new_status);

    uint next_id();

    uint id_ {next_id()};
    Status status_ {Status::Thinking};
    int progress_ {0};

    Fork &left_fork_;
    Fork &right_fork_;
    TableSetup &table_setup_;
    Visualisation &visualisation_;

    std::thread thread_;
};

#endif // PHILOSOPHER_HPP_
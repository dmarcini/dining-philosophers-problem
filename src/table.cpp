#include "table.hpp"


Table::Table(size_t philosophers_num)
    : visualisation_(table_setup_), thread_(&Table::update_resources, this)
{
    forks_.reserve(philosophers_num);
    philosophers_.reserve(philosophers_num);

    for(size_t i {0}; i < philosophers_num; ++i) {
        forks_.emplace_back(i < philosophers_num - 1 ? i : 0);
    }

    for(size_t i {0}; i < philosophers_num; ++i) {
        auto &left_fork {forks_[i]};
        auto &right_fork {forks_[(i < philosophers_num - 1 ? i + 1 : 0)]};

        philosophers_.emplace_back(left_fork, right_fork,
                                   table_setup_, visualisation_);
    }

    std::vector<std::string> ids;
    std::vector<std::string> threads_status;
    std::vector<std::string> resources_status;
    std::vector<std::string> progress;
    std::vector<bool> finished;

    for(const auto &philosopher : philosophers_) {
        ids.push_back(philosopher.id());
        threads_status.push_back(philosopher.status());
        progress.push_back(philosopher.progress());
        finished.push_back(philosopher.finished());
    }

    for(const auto &fork : forks_) {
        resources_status.push_back(fork.status());
    }

    visualisation_.init_table_info(ids, threads_status, resources_status,
                                   progress, finished);
}


Table::~Table()
{
    if(thread_.joinable()) {
        thread_.detach();
    }
}


void Table::start()
{
    table_setup_.synchronize_channel.notify_all();
    visualisation_.start();
}


void Table::update_resources()
{
    table_setup_.synchronize_channel.wait();

    do {
        for(size_t i {0}; i < philosophers_.size(); ++i) {
            visualisation_.update_resources(i, forks_[i].status());
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    } while(!table_setup_.dinner_finished);
}

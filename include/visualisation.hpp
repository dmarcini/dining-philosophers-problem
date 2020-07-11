#ifndef VISUALISATION_HPP_
#define VISUALISATION_HPP_

#include <ncurses.h>
#include <vector>
#include <thread>

#include "table_setup.hpp"

class Visualisation
{
public:
    using StrVec = std::vector<std::string>;

    Visualisation(TableSetup &table_setup);

    Visualisation(const Visualisation&) = default;
    Visualisation(Visualisation&&) = default;

    Visualisation& operator=(const Visualisation&) = delete;
    Visualisation& operator=(Visualisation&&) = delete;

    ~Visualisation();

    void init_table_info(const StrVec &ids, const StrVec &threads_status,
                         const StrVec &resources_status, const StrVec &progress,
                         const std::vector<bool> &finished);

    void start();

    void update_table_info(uint id, std::string name, std::string status,
                           bool finished);
    void update_progress(uint id, std::string progress);
    void update_resources(uint id, std::string status);
private:
    class TableSkeleton
    {
    public:
        void print(size_t threads_num);

        void print_row(size_t threads_num);
        void print_column(size_t threads_num);

        size_t& id_width() { return id_width_; };
        size_t id_width() const { return id_width_; };

        size_t& status_width() { return status_width_; };
        size_t status_width() const { return status_width_; };

        size_t& progress_width() { return progress_width_; };
        size_t progress_width() const { return progress_width_; };

        size_t& resources_width() { return resources_width_; };
        size_t resources_width() const { return resources_width_; };
    private:
        size_t id_width_ {};
        size_t status_width_ {}; 
        size_t progress_width_ {};
        size_t resources_width_ {};
    };

    void update_table_content();

    void thread_descr_color(uint id);
    void resource_descr_color(uint id);

    TableSetup &table_setup_;

    TableSkeleton table_skeleton_;

    StrVec ids_;
    StrVec threads_status_;
    StrVec threads_progress_;
    StrVec resources_status_;
    std::vector<bool> finished_;

    std::string program_status_ {"Press SPACE to end"};
};

#endif // VISUALISATION_HPP_
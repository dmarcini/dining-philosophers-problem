#ifndef TABLE_HPP_
#define TABLE_HPP_

#include <vector>
#include <thread>

#include "fork.hpp"
#include "philosopher.hpp"
#include "table_setup.hpp"
#include "visualisation.hpp"

class Table
{
public:
    Table(size_t num_philosophers);

    Table(const Table&) = delete;
    Table(Table&&) = delete;

    Table& operator=(const Table&) = delete;
    Table& operator=(Table&&) = delete;

    ~Table();

    void start();
    void update_resources();
private:
    TableSetup table_setup_;
    Visualisation visualisation_;

    std::vector<Fork> forks_;
    std::vector<Philosopher> philosophers_;

    std::thread thread_;
};

#endif // TABLE_HPP_
#include "visualisation.hpp"


Visualisation::Visualisation(TableSetup &table_setup)
    : table_setup_(table_setup) {}


Visualisation::~Visualisation()
{
    endwin();
}


void Visualisation::TableSkeleton::print(const size_t threads_num)
{    
    print_row(threads_num);
    print_column(threads_num);
}


void Visualisation::TableSkeleton::print_row(const size_t threads_num)
{
    auto cols {id_width_ + status_width_ + progress_width_ + resources_width_};

    for (size_t i {0}; i < cols + 9; ++i) {
        mvprintw(1, i, "-");
        mvprintw(threads_num + 2, i, "-");
    }
}


void Visualisation::TableSkeleton::print_column(const size_t threads_num)
{
    mvprintw(0, 5, "THREADS");
    mvprintw(0, 24, "ACTIONS");
    mvprintw(0, 50, "PROGRESS");
    mvprintw(0, 80, "RESOURCES");

    for (size_t i {0}; i < threads_num + 3; ++i) {
        auto total_width {id_width_ + 3};

        mvprintw(i, 0, "|");
        mvprintw(i, total_width, "|");
        mvprintw(i, total_width += status_width_, "|");
        mvprintw(i, total_width += progress_width_ + 3, "|");
        mvprintw(i, total_width + resources_width_ + 3, "|");
    }
}


void Visualisation::init_table_info(const StrVec &ids,
                                    const StrVec &threads_status,
                                    const StrVec &resources_status,
                                    const StrVec &threads_progress,
                                    const std::vector<bool> &finished)
{
    ids_ = ids;
    threads_status_ = threads_status;
    threads_progress_ = threads_progress;
    resources_status_ = resources_status;
    finished_ = finished;

    // initialize ncurses
    initscr();
    attron(A_BOLD);
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, true);

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);

    auto &ts = table_skeleton_;

    ts.id_width() = ids_[0].size();
    ts.status_width() = threads_status_[0].size(); 
    ts.progress_width() = threads_progress_[0].size();
    ts.resources_width() = resources_status_[1].size();

    ts.print(ids_.size());
}


void Visualisation::start()
{
    bool program_finished {false};

    while (!program_finished) {
        timeout(0);

        int option {getch()};

        if (option == ' ') {
            table_setup_.dinner_finished = true;
            program_status_ = "Finishing...       ";
        }

        if (table_setup_.dinner_finished) {
            bool all_finished {true};

            for (auto finished : finished_) {
                if (!finished) {
                    all_finished = false;
                    break;
                }
            }

            if (all_finished) {
                program_finished = true;
                program_status_ = "Finished!";
            }
        }

        update_table_content();

        if (program_finished) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}


void Visualisation::update_table_content()
{
    for (size_t i {0}; i < ids_.size(); ++i) {
        auto row {2};
        auto col {i + 2};
        // keep information about 1 thread and 1 resource
        std::string row_content {};

        row_content = ids_[i];
        attron(COLOR_PAIR(3));
        mvprintw(col, row, row_content.c_str());
        row = row_content.length() + 5;

        thread_descr_color(i);

        row_content = threads_status_[i] + threads_progress_[i];
        mvprintw(col, row, row_content.c_str());
        row += row_content.length() + 3;

        resource_descr_color(i);

        row_content = resources_status_[i];
        mvprintw(col, row, resources_status_[i].c_str());

        attroff(COLOR_PAIR(1));
        attroff(COLOR_PAIR(2));
        attroff(COLOR_PAIR(3));
        attroff(COLOR_PAIR(4));
        attroff(COLOR_PAIR(5));
        attroff(COLOR_PAIR(6));
    }

    mvprintw(ids_.size() + 5, 1, program_status_.c_str());
    mvprintw(ids_.size() + 6, 1, " ");

    table_skeleton_.print_column(ids_.size());

    refresh();
}


void Visualisation::thread_descr_color(uint id)
{
    if (threads_status_[id].find("Eating") != std::string::npos) {
        attron(COLOR_PAIR(1));
    } else if (threads_status_[id].find("Thinking") != std::string::npos) {
        attron(COLOR_PAIR(2));
    } else if (threads_status_[id].find("Waiting") != std::string::npos) {
        attron(COLOR_PAIR(4));
    } else {
        attron(COLOR_PAIR(6));
    }
}


void Visualisation::resource_descr_color(uint id)
{
    if (resources_status_[id].find("Dirty") != std::string::npos) {
        attron(COLOR_PAIR(5));
    } else {
        attron(COLOR_PAIR(6));
    }
}


void Visualisation::update_table_info(uint id, std::string name,
                                      std::string status,
                                      bool finished)
{
    ids_[id] = name;
    threads_status_[id] = status;
    finished_[id] = finished;
}


void Visualisation::update_progress(uint id, std::string progress)
{
    threads_progress_[id] = progress;
}


void Visualisation::update_resources(uint id, std::string status)
{
    resources_status_[id] = status;
}

#include <cstdlib>
#include <ctime>

#include "table.hpp"

int main(int argc, char* argv[])
{
    std::srand(std::time(nullptr));

    size_t philosophers_num {5};

    if(argc == 2)
    {
        try
        {
            philosophers_num = std::stoi(argv[1]);
        }
        catch(...) {} 

        if (philosophers_num < 3) {
            philosophers_num = 3;
        }
    }

    Table(philosophers_num).start();

    return 0;
}
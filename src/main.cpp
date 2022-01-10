#include "server_async.h"
#include <iostream>


int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Count of parameters is incorrect! Please enter the PORT and BULK_SIZE" << std::endl;
        return 1;
    }
    const size_t port = std::stoi(argv[1]);
    const size_t bulk_size = std::stoi(argv[2]);
    AsyncServer server(port, bulk_size);

    return 0;
}


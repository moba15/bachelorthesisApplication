//
// Created by mor on 26.08.24.
//

#pragma once


#include "../shared/shared.h"
#include <semaphore>
#include <iostream>
namespace clientside {
    class client {
    public:
        client() = default;
        ~client();
        void start();
        void stop();
    private :
        shared::shared_memory *sharedMemory;
        bool stopped;
        sem_t  *mutex_sem, *count_sem;
        int fd_shm;


        void parse_command(std::string command);
        void printHelpPage() {
            std::cout << "-----Help------" << std::endl;
            std::cout << "stop -> stops the client" << std::endl;
            std::cout << "insert <key> <value> -> inserts (key,value). Key and value can only be one word (without spaces)" << std::endl;
            std::cout << "insert <key> -> removes key entry. Key can only be one word (without spaces)" << std::endl;
            std::cout << "read <key> -> reads key entry. Key can only be one word (without spaces)" << std::endl;
        }




    };
}

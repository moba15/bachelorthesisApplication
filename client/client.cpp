//
// Created by mor on 26.08.24.
//

#include <iostream>
#include <semaphore>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <thread>
#include <sstream>
#include "client.h"
#include "../shared/shared.h"
#include <vector>
#include <numeric>

clientside::client::~client() {
    munmap(sharedMemory, sizeof(struct shared::shared_memory));
    close(fd_shm);


   // free(sharedMemory);
}



void clientside::client::start() {
    stopped = false;

    //init shm

    mutex_sem = sem_open(SEM_MUTEX_NAME,0, 0, 0);
    if(mutex_sem == SEM_FAILED) {
        //TODO Use exceptions
        std::exit(1);
    }
    count_sem = sem_open(SEM_COUNT_NAME,  0,0,0);
    if(count_sem == SEM_FAILED) {
        //TODO Use exceptions
        std::exit(1);
    }

    fd_shm = shm_open(SHARED_MEMORY_NAME, O_RDWR , 0660 );
    if(fd_shm == -1) {
        //TODO Use exceptions
        std::exit(1);
    }
    if(ftruncate(fd_shm, sizeof(struct shared::shared_memory)) ==  -1) {
        //TODO
        std::exit(-1);
    }
    sharedMemory = static_cast<shared::shared_memory *>(mmap(NULL, sizeof(struct shared::shared_memory), PROT_READ | PROT_WRITE,
                                                             MAP_SHARED, fd_shm, 0));
    if(sharedMemory == MAP_FAILED) {
        //TODO
        std::exit(-1);
    }
    std::cout << "Type help to see the help page" << std::endl;
    std::string command;
    while(!stopped) {
        getline(std::cin, command);
        parse_command(command);
    }
}

void clientside::client::parse_command(std::string command) {
    if(command == "stop") {
        this->stopped = true;
        return;
    }
    std::vector<std::string> split{};
    std::istringstream is{command};
    std::string tmp;
    while(getline(is,tmp, ' ')) {
        split.push_back(tmp);
    }

    if(split.size() == 2 && split[0] == "remove" ) {
        /*std::string toRemove = std::reduce(split.begin() +1, split.end(), std::string{},[] (std::string c, std::string c1) {
            return  c + c1 + " ";
        });*/
        std::string toRemove = split[1];
        //toRemove.erase(toRemove.end()-1, toRemove.end());
        std::cout << "Send: remove with key: " + toRemove << std::endl;
        if(sem_wait(mutex_sem) == -1) {
            //TODO
            std::exit(1);
        }
        shared::command c{};
        c.answered = false;
        c.cmd = 1;
        std::copy(toRemove.begin(), toRemove.end(), c.key);
        auto myIndex = sharedMemory->number_command_index;
        sharedMemory->buf[myIndex] = c;

        sharedMemory->number_command_index = (sharedMemory->number_command_index +1)%MAX_BUFFER_SIZE;

        sem_post(mutex_sem);
        sem_post(count_sem);
        while(!sharedMemory->buf[myIndex].answered) {

        }
        std::cout << "  Removed: " << static_cast<unsigned int>(sharedMemory->buf[myIndex].result[0]) << std::endl;



    } else if(split.size() == 3 && split[0] == "insert") {
        std::string key = split[1];
        std::string value = split[2];

        std::cout << "Send: inserting (key,value): (" << key << "," << value << ")"  << std::endl;
        shared::command c{};
        c.answered = false;
        c.cmd = 2;
        std::copy(key.begin(), key.end(), c.key);
        std::copy(value.begin(), value.end(), c.value);
        sharedMemory->buf[sharedMemory->number_command_index] = c;

        sharedMemory->number_command_index = (sharedMemory->number_command_index +1)%MAX_BUFFER_SIZE;
        sem_post(mutex_sem);
        sem_post(count_sem);
    } else if(split.size() == 2 && split[0] == "read") {
        std::string key = split[1];


        std::cout << "Send: read key: " << key << std::endl;
        shared::command c{};
        c.answered = false;
        c.cmd = 3;
        std::copy(key.begin(), key.end(), c.key);
        auto myIndex = sharedMemory->number_command_index;
        sharedMemory->buf[myIndex] = c;

        sharedMemory->number_command_index = (sharedMemory->number_command_index +1)%MAX_BUFFER_SIZE;
        sem_post(mutex_sem);
        sem_post(count_sem);

        while(!sharedMemory->buf[myIndex].answered) {

        }
        std::cout << "  Read: " << sharedMemory->buf[myIndex].result << std::endl;
    } else if(split[0] == "help") {
        printHelpPage();
    }



}


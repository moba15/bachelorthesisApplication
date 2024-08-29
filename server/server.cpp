//
// Created by mor on 26.08.24.
//

#include <iostream>
#include <semaphore>
#include "server.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <thread>
#define exclusive true
void printHelpPage() {
    std::cout << "-----Help------" << std::endl;
    std::cout << "stop -> stops the server" << std::endl;
    std::cout << "size -> prints the current size of the hashmap" << std::endl;
}
serverside::server::server(size_t n) : sizeOfHashMap{n}, map(n) {}
using namespace std::chrono_literals;
void serverside::server::start() {
    std::cout << "Starting server: Buckets:" << sizeOfHashMap << std::endl;
    std::cout << "Please stop the server by typing stop in order to avoid errors" << std::endl;
    char n = -1;
    std::cout << "Type help to see the help page" << std::endl;

    stopped = false;
    // readingInputFromClients();
    std::thread t{&server::readingInputFromClients, this};
    std::string command;
    while(!this->stopped) {
        getline( std::cin, command);
        if(command == "stop") {
            this->stopped = true;
        } else if(command == "size") {
            std::cout << "HasMap size: " << map.size() << std::endl;
        } else if(command == "help") {
            printHelpPage();
        }
    }
    t.join();


}

void serverside::server::stop() {
    std::cout << "Stopping serverside" << std::endl;
}

void serverside::server::readingInputFromClients() {
    //std::cout << "Reading" << std::endl;
    sem_t *mutex_sem, *count_sem;
    int fd_shm;

    mutex_sem = sem_open(SEM_MUTEX_NAME,  O_CREAT, 0660, 0);
    if(mutex_sem == SEM_FAILED) {
        std::cout << "There went something wrong with creating/mapping to the shm" << std::endl;
        std::exit(1);
    }
    count_sem = sem_open(SEM_COUNT_NAME,  O_CREAT, 0660, 0);
    if(count_sem == SEM_FAILED) {
        std::cout << "There went something wrong with creating/mapping to the shm" << std::endl;
        std::exit(1);
    }
    if(exclusive) {
        fd_shm = shm_open(SHARED_MEMORY_NAME, O_RDWR | O_CREAT | O_EXCL , 0660 );
    } else {
        fd_shm = shm_open(SHARED_MEMORY_NAME, O_RDWR | O_CREAT  , 0660 );
    }

    if(fd_shm == -1) {
        std::cout << "There went something wrong with creating/mapping to the shm" << std::endl;
        std::exit(1);
    }
    if(ftruncate(fd_shm, sizeof(struct shared::shared_memory)) ==  -1) {
        std::cout << "There went something wrong with creating/mapping to the shm" << std::endl;
        std::exit(-1);
    }
    sharedMemory = static_cast<shared::shared_memory *>(mmap(NULL, sizeof(struct shared::shared_memory), PROT_READ | PROT_WRITE,
                                                     MAP_SHARED, fd_shm, 0));
    if(sharedMemory == MAP_FAILED) {
        std::cout << "There went something wrong with creating/mapping to the shm" << std::endl;
        std::exit(-1);
    }
    sharedMemory->current_command_index = 0;

    if(sem_post(mutex_sem) == -1) {
        std::exit(1);
    }
    timespec ts{};
    ts.tv_sec = 1;

   while(!stopped) {
       //TODO SEM_MAX_VALUE

       if(sem_timedwait(count_sem, &ts) == -1) {
           if(errno == EINTR || errno == ETIMEDOUT ) {
               continue;
           }
           std::cout << "EXIT" << std::endl;
           std::exit(1);
       }
       //Has new content inside
       auto currentIndex = sharedMemory->current_command_index;
       shared::command command = sharedMemory->buf[currentIndex];
       sharedMemory->current_command_index = (sharedMemory->current_command_index+1)%MAX_BUFFER_SIZE;
       if(command.cmd == 1) {
           //remove
           auto result = map.remove(command.key);
           std::cout << "Removing entry with key: '" << command.key << "' Result:" << result << std::endl;
           sharedMemory->buf[currentIndex].result[0] = static_cast<char>(result);
           sharedMemory->buf[currentIndex].answered = true;

       } else if(command.cmd == 2) {
           //insert
           map.insert(command.key, command.value);
           std::cout << "Inserting entry with (key, value): (" << command.key << ", " << command.value << ")" << std::endl;
       } else if(command.cmd == 3) {
           //read
           auto result = map.read(command.key);
           std::cout << "Reading entry with key: '" << command.key << "' Result:" << result << std::endl;
           std::copy(result.begin(), result.end(), sharedMemory->buf[currentIndex].result);
           sharedMemory->buf[currentIndex].answered = true;
       }


   }
    munmap(sharedMemory, sizeof(struct shared::shared_memory));
    close(fd_shm);

    sem_unlink(SEM_MUTEX_NAME);
    sem_unlink(SEM_COUNT_NAME);
    shm_unlink(SHARED_MEMORY_NAME);
}



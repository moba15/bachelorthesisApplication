//
// Created by mor on 26.08.24.
//

#pragma once


#include <cstddef>
#include "hashmap/hashmap.h"
#include "../shared/shared.h"

namespace serverside {
    class server {
    public:
        explicit server(size_t n);
        void start();
        void stop();

    private:
        size_t sizeOfHashMap;
        int shm_fd{-1};
        void *ptr_to_shm;
        hashmap2<std::string ,std::string> map{0};
        shared::shared_memory* sharedMemory;
        bool stopped;




        void readingInputFromClients();

    };
}




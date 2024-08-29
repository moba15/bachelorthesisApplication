//
// Created by mor on 26.08.24.
//

#pragma once
#define SEM_MUTEX_NAME "/sem-mutex-bachelor"
#define SEM_COUNT_NAME "/sem-count-bachelor"
#define SHARED_MEMORY_NAME "/shm-bachelor"
#define MAX_BUFFER_SIZE 1024
#define MAX_KEY_STRING_SIZE 64
namespace shared {
    struct command {
        unsigned char cmd;
        char key[MAX_KEY_STRING_SIZE];
        char value[MAX_KEY_STRING_SIZE];
        bool answered;
        char result[MAX_KEY_STRING_SIZE];
    };
    struct shared_memory {
        command buf[MAX_BUFFER_SIZE]{};
        int current_command_index = 0;
        int number_command_index = 0;

    };
}
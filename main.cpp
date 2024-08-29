#include <iostream>
#include "server/hashmap/hashmap.h"
#include "server/server.h"
#include "client/client.h"

int main(int argc, char* argv[]) {
    std::string path{argv[0]};
    if(argc < 2) {
        std::cout << "Please use " << path <<  " server <numberOfBuckets> or " << path << " client" << std::endl;
        return -1;
    }
    std::string s{argv[1]};
    if(s == "client") {
        std::cout << "Starting client..." << std::endl;
        clientside::client c{};
        c.start();
    } else if(s == "server" ) {
        if(argc < 3) {
            std::cout << "Please provide a number of buckets: " << path << " server <numberOfBuckets>" << std::endl;
            return 1;
        }

        unsigned long n = std::strtol(argv[2], nullptr, 10);
        if(n <= 0 || n==LONG_MAX || n==LONG_MIN) {
            std::cout << "Please use a valid number" << std::endl;
            return -1;
        }
        serverside::server server{n};
        server.start();




    } else {
        std::cout << "Please use " << path <<  "server <numberOfBuckets> or " << path << " client" << std::endl;
        return -1;
    }
    return 0;
}

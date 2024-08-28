//
// Created by mor on 27.08.24.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <thread>
#include "server/hashmap/hashmap.h"
using namespace std::chrono_literals;
TEST_CASE("simple") {
    SUBCASE("insert 1 bucket") {
        serverside::hashmap2<std::string, std::string> map{1};
        map.insert("a", "b");
        map.insert("c", "d");
        map.insert("e", "f");
        CHECK_EQ(map.read("a"), "b");
        CHECK_EQ(map.read("e"), "f");
        CHECK_EQ(map.read("c"), "d");
        CHECK_EQ(map.read("d"), std::string());
    }

    SUBCASE("insert 100 bucket") {
        serverside::hashmap2<std::string, std::string> map{100};
        std::string a{"a"};
        map.insert(a, "b");
        map.insert("c", "d");
        map.insert("e", "f");
        CHECK_EQ(map.read(a), "b");
        CHECK_EQ(map.read("e"), "f");
        CHECK_EQ(map.read("c"), "d");
        CHECK_EQ(map.read("d"), std::string());
    }

    SUBCASE("remove end 1 bucket") {
        serverside::hashmap2<std::string, std::string> map{1};
        std::string a{"a"};
        map.insert(a, "b");
        map.insert("c", "d");
        map.insert("e", "f");
        CHECK_EQ(map.read(a), "b");
        CHECK_EQ(map.read("e"), "f");
        CHECK_EQ(map.read("c"), "d");


        CHECK_EQ(map.remove("e"), true);
        CHECK_EQ(map.remove("e"), false);
        CHECK_EQ(map.read("e"), std::string ());

        CHECK_EQ(map.read("a"), "b");
        CHECK_EQ(map.read("c"), "d");

    }

    SUBCASE("remove begin 1 bucket") {
        serverside::hashmap2<std::string, std::string> map{1};
        std::string a{"a"};
        map.insert(a, "b");
        map.insert("c", "d");
        map.insert("e", "f");
        CHECK_EQ(map.read(a), "b");
        CHECK_EQ(map.read("e"), "f");
        CHECK_EQ(map.read("c"), "d");


        CHECK_EQ(map.remove("a"), true);
        CHECK_EQ(map.remove("a"), false);
        CHECK_EQ(map.read("a"), std::string ());

        CHECK_EQ(map.read("e"), "f");
        CHECK_EQ(map.read("c"), "d");

    }

    SUBCASE("remove middle 1 bucket") {
        serverside::hashmap2<std::string, std::string> map{1};
        std::string a{"a"};
        map.insert(a, "b");
        map.insert("c", "d");
        map.insert("e", "f");
        CHECK_EQ(map.read(a), "b");
        CHECK_EQ(map.read("e"), "f");
        CHECK_EQ(map.read("c"), "d");


        CHECK_EQ(map.remove("c"), true);
        CHECK_EQ(map.remove("c"), false);
        CHECK_EQ(map.read("c"), std::string ());

        CHECK_EQ(map.read("e"), "f");
        CHECK_EQ(map.read("a"), "b");

    }

    SUBCASE("remove 100 buckets") {
        serverside::hashmap2<std::string, std::string> map{100};
        std::string a{"a"};
        map.insert(a, "b");
        map.insert("c", "d");
        map.insert("e", "f");
        CHECK_EQ(map.read(a), "b");
        CHECK_EQ(map.read("e"), "f");
        CHECK_EQ(map.read("c"), "d");


        CHECK_EQ(map.remove("c"), true);
        CHECK_EQ(map.remove("c"), false);
        CHECK_EQ(map.read("c"), std::string ());

        CHECK_EQ(map.read("e"), "f");
        CHECK_EQ(map.read("a"), "b");

    }
}


TEST_CASE("single thread") {
    SUBCASE("insert, read, remove 10000 buckets: 1" ) {
        serverside::hashmap2<size_t , size_t > map{1};
        for(size_t i = 0; i<10000; i++) {
            map.insert(i*2, i);
        }

        for(size_t i = 0; i<10000; i++) {
            CHECK_EQ(map.read(2*i), i);
            CHECK_EQ(map.read(2*i+1), 0);
        }


        for(size_t i = 0; i<10000; i++) {
            if(i % 4 == 0) {
                CHECK_EQ(map.remove(i), true);
            } else if(i %2 != 0) {
                CHECK_EQ(map.remove(i), false);
            }

        }

        for(size_t i = 0; i<10000; i++) {
            if(i%4 == 0) {
                CHECK_EQ(map.read(i), 0);
            } else if(i%2 == 0) {
                CHECK_EQ(map.read(i), i/2);
            }

        }


    }

    SUBCASE("insert, read, remove 10000 buckets: 500" ) {
        serverside::hashmap2<size_t , size_t > map{500};
        for(size_t i = 0; i<10000; i++) {
            map.insert(i*2, i);
        }

        for(size_t i = 0; i<10000; i++) {
            CHECK_EQ(map.read(2*i), i);
            CHECK_EQ(map.read(2*i+1), 0);
        }


        for(size_t i = 0; i<10000; i++) {
            if(i % 4 == 0) {
                CHECK_EQ(map.remove(i), true);
            } else if(i %2 != 0) {
                CHECK_EQ(map.remove(i), false);
            }

        }

        for(size_t i = 0; i<10000; i++) {
            if(i%4 == 0) {
                CHECK_EQ(map.read(i), 0);
            } else if(i%2 == 0) {
                CHECK_EQ(map.read(i), i/2);
            }

        }


    }
}

TEST_CASE("multiple thread thread") {
   SUBCASE("2 threads insert, read, remove 10000 buckets: 1" ) {
        serverside::hashmap2<size_t , size_t > map{1};
        std::thread t1{[&] () {
            std::this_thread::sleep_for(100ms);
            for(size_t i = 0; i<10000; i++) {
                map.insert(i*2, i);
            }

            for(size_t i = 0; i<10000; i++) {
                if(2*i % 4 != 0) {
                    CHECK_EQ(map.read(2*i), i);
                }


            }


            for(size_t i = 0; i<10000; i++) {
                if(i % 4 == 0) {
                    CHECK_EQ(map.remove(i), true);
                } else if(i %2 != 0 && i%3 != 0) {
                    CHECK_EQ(map.remove(i), false);
                }

            }

            for(size_t i = 0; i<10000; i++) {
                if(i%4 == 0) {
                    CHECK_EQ(map.read(i), 0);
                } else if(i%2 == 0) {
                    CHECK_EQ(map.read(i), i/2);
                }

            }
        }};
        std::thread t2{[&] () {
            for(size_t i = 0; i<10000; i++) {
                if((i*3) % 2 != 0) {
                    map.insert(i*3, i);
                }
            }

            for(size_t i = 0; i<10000; i++) {
                if((i*3) % 2 != 0) {
                    CHECK_EQ(map.read(3*i), i);
                }


            }





        }};
        t1.join();
       for(size_t i = 0; i<10000; i++) {
           if(i%4 == 0 && i%3 != 0) {
               CHECK_EQ(map.read(i), 0);
           } else if(i%2 == 0 && i%3 != 0) {
               // CHECK_EQ(map.read(i), i/2);
           }

       }
        t2.join();


    }
    SUBCASE("2 threads insert 10000 buckets: 1" ) {
        serverside::hashmap2<size_t , size_t > map{1};
        std::thread t1{[&] () {
            std::this_thread::sleep_for(100ms);
            for(size_t i = 0; i<10000; i++) {
                map.insert(i*2, i);
                map.read(i*3);
            }


        }};
        std::thread t2{[&] () {
            for(size_t i = 0; i<10000; i++) {
                if((i*3) % 2 != 0) {
                    map.insert(i*3, i);
                }

                //map.read(i*2);
            }


        }};

        t1.join();
        t2.join();
        for(size_t i = 0; i<10000; i++) {
            CHECK_EQ(map.read(2*i), i);
            if((i*3) % 2 != 0) {
                CHECK_EQ(map.read(3*i), i);
            }

        }


    }
}




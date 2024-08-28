//
// Created by mor on 23.08.24.
//
#pragma once

#include <cstddef>
#include <array>
#include <vector>
#include <list>
#include  <algorithm>
#include <string>
#include <memory>
#include <shared_mutex>
#include <mutex>
#include <unordered_map>
namespace serverside {
    template<class K, class V>
    class hashmap2 {
    public:
        hashmap2() = delete;
        explicit hashmap2(std::size_t n) : numberOfBuckets{n}, buckets{n}, mutexes{n}, u{n}{}

        void insert(K key, V value) {
            auto index = hashBucket(key);
            std::unique_lock lck{mutexes[index]};

            auto& bucket = buckets[index];
            if(bucket == nullptr) {
               auto t = std::make_unique<std::vector<std::pair<K , V>>>();
              buckets[index] = std::move(t);

            }
            auto& bucket2 = buckets[index];
            auto found = std::find_if(bucket2->begin(), bucket2->end(), [&key] (std::pair<K, V> p) {
                return key == p.first;
            });
            if(found == bucket2->end()) {
                //Not found
                std::pair p{key, value};
                bucket2->push_back(p);
                s++;
            } else {
                //found
                bucket2->at(static_cast< unsigned  long>(std::distance(bucket2->begin(), found))).second = value;
            }


        }
        bool remove(K key) {
            auto index = hashBucket(key);
            std::unique_lock lck{mutexes[index]};
            auto& bucket = buckets[index] ;
            if(bucket == nullptr) {
                return false;
            }
            auto found = std::find_if(bucket->begin(), bucket->end(), [&key] (auto p) {
                return p.first == key;
            });

            if(found == bucket->end()) {
                //Not found
                return false;
            } else {
                bucket->erase(std::remove_if(bucket->begin(), bucket->end(), [&key] (auto p) {
                    return key == p.first;
                }));
                s--;
                return true;
            }

        }
        V read(K key) {
            auto index = hashBucket(key);
            std::shared_lock lck{mutexes[index]};
            auto& bucket = buckets[index];

            if(bucket == nullptr) {
                return V{};
            }
            auto found = std::find_if(bucket->begin(), bucket->end(), [&key] (auto p) {
                return p.first == key;
            });
            if(found == bucket->end()) {
                //Not found
                return V{};
            } else {
               return bucket->at(static_cast<unsigned  long>(std::distance(bucket->begin(), found))).second;
            }

        }
        size_t size() {
            return s;
        }

    private:
        std::unordered_map<std::string, std::string> u;
        size_t numberOfBuckets;
        size_t s = 0;
        std::vector<std::unique_ptr<std::vector<std::pair<K, V>>>> buckets;
        std::hash<K> hash{};
        std::vector<std::shared_mutex> mutexes;

        std::size_t hashBucket(K key) {
            return hash(key) % numberOfBuckets;
        }

    };






    /*class HashMap {
    public:
        HashMap() = delete;
        explicit HashMap(std::size_t n) :numberOfBuckets{n},buckets{n}, mutexes{n}, u{n}{}

        void insert(std::string_view key, int value) {
            auto index = hashBucket(key);
            std::unique_lock lck{mutexes[index]};

            auto& bucket = buckets[index];
            if(bucket == nullptr) {
               auto t = std::make_unique<std::vector<std::pair<std::string_view , int>>>();
              (void)buckets.insert(buckets.begin() + hashBucket(key), std::move(t));

            }
            auto& bucket2 = buckets[index];
            auto found = std::find_if(bucket2->begin(), bucket2->end(), [&key] (std::pair<std::string_view, int> p) {
                return key == p.first;
            });
            if(found == bucket2->end()) {
                //Not found
                std::pair p{key, value};
                bucket2->push_back(p);
            } else {
                //found
                auto p = bucket2->at(static_cast< unsigned  long>(std::distance(bucket2->begin(), found)));
                bucket2->at(static_cast< unsigned  long>(std::distance(bucket2->begin(), found))).second = value;
            }


        }
        bool remove(std::string_view key) {
            auto index = hashBucket(key);
            std::unique_lock lck{mutexes[index]};
            auto& bucket = buckets[index] ;
            if(bucket == nullptr) {
                return false;
            }
            auto found = std::find_if(bucket->begin(), bucket->end(), [&key] (std::pair<std::string_view, int> p) {
                return p.first == key;
            });

            if(found == bucket->end()) {
                //Not found
                return false;
            } else {
                bucket->erase(std::remove_if(bucket->begin(), bucket->end(), [&key] (auto p) {
                    return key == p.first;
                }));

                return true;
            }


        }
        int read(std::string_view key) {
            auto index = hashBucket(key);
            std::shared_lock lck{mutexes[index]};
            auto& bucket = buckets[index] ;

            if(bucket == nullptr) {
                return NULL;
            }
            auto found = std::find_if(bucket->begin(), bucket->end(), [&key] (std::pair<std::string_view, int> p) {
                return p.first == key;
            });
            if(found == bucket->end()) {
                //Not found
                return NULL;
            } else {
               return bucket->at(static_cast<unsigned  long>(std::distance(bucket->begin(), found))).second;
            }

        }



    private:
        std::unordered_map<std::string, std::string> u;
        size_t numberOfBuckets;
        std::vector<std::unique_ptr<std::vector<std::pair<std::string_view, int>>>> buckets;
        std::hash<std::string_view> hash{};
        std::vector<std::shared_mutex> mutexes;

        std::size_t hashBucket(std::string_view key) {
            return hash(key) % numberOfBuckets;
        }

    };*/






}




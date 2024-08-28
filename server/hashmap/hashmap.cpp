

#include "hashmap.h"


/*template<class K, class V>
serverside::HashMap<K, V>::HashMap(std::size_t n) : numberOfBuckets(n),buckets(n){

}

template<class K, class V>
void serverside::HashMap<K, V>::insert(size_t key, int value) {
    std::vector<std::pair<K, V>> bucket = buckets[hashBucket(key)];
    auto found = std::find_if(bucket.begin(), bucket.end(), [&key] (std::pair<size_t, int> p) {
        return key == p.first;
    });
    if(found == bucket.end()) {
        //Not found
        std::pair p{key, value};
        bucket.push_back(p);

    } else {
        //found
        bucket.at(static_cast< unsigned  long>(std::distance(bucket.begin(), found))).second = value;
    }


}


template<class K, class V>
bool serverside::HashMap<K, V>::remove(size_t key) {
    return false;
}


template<class K, class V>
size_t serverside::HashMap<K, V>::read(size_t key) {
    return 0;
}

template<class K, class V>
std::size_t serverside::HashMap<K, V>::hashBucket(size_t key) {
    return key % numberOfBuckets;
}*/
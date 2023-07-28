#include "hash_pair.h"
#include <hashtable.h>

using namespace std;

template<class T1, class T2> size_t hash_pair::operator()(const pair<T1, T2> &p) const {
    auto hash1 = hash<T1> {}(p.first);
    auto hash2 = hash<T2> {}(p.second);

    if (hash1 != hash2) {
        return hash1 ^ hash2;
    }

    // If hash1 == hash2, their XOR is zero.
    return hash1;
}

template size_t hash_pair::operator()<int, int>(const pair<int, int> &p) const; // IMPORTANT OR IT WONT COMPILE

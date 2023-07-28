#ifndef HASH_PAIR_H
#define HASH_PAIR_H

#include <utility>

using namespace std;

struct hash_pair {
    template<class T1, class T2> size_t operator()(const pair<T1, T2> &p) const;
};

#endif
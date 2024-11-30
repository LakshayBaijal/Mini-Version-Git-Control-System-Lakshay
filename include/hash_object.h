
#ifndef HASH_OBJECT_H
#define HASH_OBJECT_H

#include <string>
using namespace std;

class HashObject {
public:
    static string hash_object(const string& filepath, bool write);
};

#endif 

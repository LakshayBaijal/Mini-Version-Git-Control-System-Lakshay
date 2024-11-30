
#ifndef COMMIT_H
#define COMMIT_H

#include <string>
using namespace std;

class Commit {
public:
    static void commit(const string& message = "No commit message");
};

#endif 

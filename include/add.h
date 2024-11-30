#ifndef ADD_H
#define ADD_H

#include <vector>
#include <string>
using namespace std;
class Add {
public:
    static void add(const vector<string>& paths);

private:
    static void addFile(const string& filepath);

    static void addAll(const string& directory);

    static void updateIndex(const string& mode, const string& type, const string& sha1, const string& path);

    static vector<tuple<string, string, string, string>> readIndex();

    static void writeIndex(const vector<tuple<string, string, string, string>>& entries);
};

#endif

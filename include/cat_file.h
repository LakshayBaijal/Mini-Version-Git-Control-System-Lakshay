#ifndef CAT_FILE_H
#define CAT_FILE_H

#include <string>
using namespace std;

class CatFile {
public:
    static void catFile(const string& flag, const string& sha1);

private:
    static string getObjectPath(const string& sha1);

    static string readFileContent(const string& path);

    static void printContent(const string& path);

    static void printSize(const string& path);

    static void printType(const string& path);

    static void parseTree(const string& body);
};

#endif

#ifndef SHA1_H
#define SHA1_H

#include <string>
using namespace std;

class SHA1 {
public:
    SHA1();
    void update(const string& s);
    void update(const char* data, size_t len);
    string final();
    static string hash(const string& s);

private:
    unsigned int h0, h1, h2, h3, h4;
    string buffer;
    unsigned long long bitlen;

    void transform();
};

#endif 
